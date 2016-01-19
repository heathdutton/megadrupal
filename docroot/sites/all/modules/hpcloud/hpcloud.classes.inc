<?php
/**
 * @file
 * Part of the hpcloud package.
 *
 * DO NOT ADD THIS FILE TO hpcloud.info! Because it has external dependencies,
 * and Drupal cannot handle this, the file must be manually loaded. This is done
 * in _hpcloud_bootstrap().
 *
 * Drupal's stream wrappers work in a non-standard way.
 * One major difference is that the scheme field is used
 * not for the scheme, but for the target (the 'host'). This means that we need
 * to provide both a base stream wrapper, and target-specific stream wrappers,
 * even though all are using the same underlying mechanism.
 *
 * Another MAJOR difference is that Drupal constructs "streams" itself, not
 * always passing through the actual stream wrapper layer. This means that "streams"
 * are really just plain old objects (at least some of the time). This has
 * seriously detrimental aspects for a real stream wrapper (which assumes,
 * for example, that it can query the context.)
 *
 * Consequently, the swift:// stream wrapper cannot be used directly, as it
 * assumes that it is operating in a real stream context, and frequently makes
 * use of context parameters and options (neither of which Drupal supports
 * consistently).
 *
 * The wrapper provided here is an adapter/proxy from Drupal's fake streams
 * to the real HPCloud stream wrapper.
 *
 * This file contains all of the stream wrapper classes needed to get access
 * to swift. Other utility classes are defined here, too.
 */

/**
 * Create a Drupal-friendly stream wrapper.
 *
 *
 * This is different from the HPCloud `swift://` stream wrapper. That is a
 * fully compliant PHP stream wrapper. This one interfaces with Drupal's partial
 * stream wrapper implementation, meaning that it can be constructed using
 * Drupal's non-standard construction method, and also does not have any
 * reliance on the PHP context system.
 *
 * Another major difference between this wrapper and the core HPCloud 
 * `swift://` stream wrapper is that this one fakes directory operations
 * (the other simply does not implement directory operations). This is
 * mandatory for Drupal, which (documentation notwithstanding) assumes
 * that the wrapper implementation has a concept of directories.
 *
 * Things we fake:
 *
 * - Directory operations
 * - The existence of a directory (all operations that check whether a
 *   directory exists return TRUE.)
 * - Modes. In some conditions we derive modes from container ACLs,
 *   while other times we just fake it.
 * - Paths. We jump through elaborate hoops to alter paths. Particularly
 *   with the getExternalUrl() function, we munge things to look differently
 *   depending on what we think is the state of the Object Store.
 *
 *
 */
class HPCloudDrupalStreamWrapper implements DrupalStreamWrapperInterface {

  /**
   * The URI presented to the wrapper.
   */
  protected $uri;


  /**
   * The context.
   *
   * Drupal does NOT reliably set this, which is TOTALLY lame.
   */
  public $context;

  /**
   * Object storage.
   */
  protected $store;
  /**
   * The container.
   *
   */
  protected $container;
  /**
   * The object.
   */
  protected $obj;

  /**
   * The handle to the underlying IO stream.
   */
  protected $handle;
  protected $dirlisting = array();
  protected $dirindex = 0;

  /**
   * The stat cache.
   */
  protected static $statCache = array();

  /**
   * Does this path look like a file path?
   *
   * This guesses whether the path is to a directory or a file.
   * Primarily, it guesses based on the presence of an extension. Because
   * the only piece of data we get is the path, and this path could
   * be remote, we can't do anything more sophisticated.
   *
   * @return boolean
   *   TRUE if the path looks like a path to a file, FALSE
   *   if it looks like a path to a directory.
   */
  public static function isFileish($path) {
    $ext = pathinfo($path, PATHINFO_EXTENSION);

    //watchdog('hpcloud', 'extension for %f is %e', array('%f' => $path, '%e' => $ext), WATCHDOG_NOTICE);

    return !empty($ext);
  }


  /**
   * Transform a Drupal URI into a Swift URL.
   *
   * We use the underlying swift:// handler to do file IO. Yet it takes a
   * different URI format than Drupal's core stream wrappers, so we have
   * to convert between the two.
   *
   * @param $uri
   *   The URI as a string.
   * @return string
   *   The Swift URI (swift://CONTAINER/OBJECT).
   */
  protected static function swiftUrl($uri) {
    $data = self::uriToData($uri);
    $scheme = \HPCloud\Storage\ObjectStorage\StreamWrapper::DEFAULT_SCHEME;
    $new_url = $scheme . '://' . $data['container'];
    if (!empty($data['object'])) {
      $new_url .= '/' . $data['object'];
    }

    return $new_url;
  }

  /**
   * Build a stream context for Swift.
   *
   * Drupal doesn't do stream contexts, pfffft. So we have
   * to generate ours internally.
   *
   * @param $extra
   *   An array of extra data to be merged into the context.
   * @return resource
   *   THe stream context.
   */
  public static function swiftContext($extra = array()) {
    $id = _hpcloud_login();
    //drupal_set_message('<pre class="ddump">' . print_r($tokens, TRUE) . '</pre>');
    $endpoint = self::swiftEndpoint($id->serviceCatalog());

    $extra['token'] = $id->token();
    $extra['swift_endpoint'] = $endpoint;
    $context = stream_context_create(array(
      'swift' => $extra,
    ));
    return $context;
  }

  /**
   * Get the swift endpoint.
   *
   * Given a service catalog, get the swift endpoint.
   *
   * @param $catalog
   *   The catalog array.
   * @return string
   *   The URL to Swift.
   */
  public static function swiftEndpoint($catalog) {

    foreach ($catalog as $entry) {
      if ($entry['type'] == \HPCloud\Storage\ObjectStorage::SERVICE_TYPE) {
        return $entry['endpoints'][0]['publicURL'];
      }
    }
  }

  /**
   * Get the MIME.
   *
   * This incurs a HUGE performance hit because it is static, yet requires
   * that a full object be constructed, which means two to three HTTP transactions.
   *
   * Static? Really? Don't we need to examine a file in most cases to figure out
   * what type of data it contains? (If not, then why don't we just have one big
   * extension map?).
   */
  public static function getMimeType($uri, $mapping = NULL) {
    $parts = self::uriToData($uri);

    // Try to get the remote resource and find the MIME.
    $wrapper = new HPCloudDrupalStreamWrapper();
    $wrapper->setUri($uri);
    $obj = $wrapper->object();

    if (!empty($obj)) {
      return $obj->contentType();
    }

    return self::guessMimeType($uri);
  }

  /**
   * Drupal extension to Streams.
   *
   * Unfortunately, this has to be treated as if it were a 
   * constructor, for it is the first point of entry for several
   * methods, and we can't really construct a stream without
   * knowing the URI.
   *
   * Equally unfortunate, stream_open() also has to be treated as a
   * constructor, and getMimeType is static, which means it has
   * to do out-of-band construction.
   */
  public function setUri($uri) {
    $this->uri = $uri;
    $data = self::uriToData($uri);
    if (empty($data['container'])) {
      watchdog('hpcloud', 'URI %uri has no container mapped.', array('%uri' => $uri));
      drupal_set_message('HPCloud is not configured correctly: The container name has not been mapped to this scheme.', 'error');
    }

    if (!$this->initializeObjectStorage()) {
      trigger_error('Failed to build object storage from the service catalog', E_USER_ERROR);
      return;
    }

    // XXX: Skip fetching the container and create it directly? Can we do this without risk?
    $this->container = $this->store->container($data['container']);
    if (!empty($data['object'])) {
      try {
        $this->obj = $this->container->remoteObject($data['object']);
      }
      catch (\HPCloud\Transport\FileNotFoundException $e) {
        // This is not a big deal -- just tracking for debugging purposes.
        //drupal_set_message('no such file:' . $data['object'], 'status');
        //drupal_set_message('<pre class="ddump">' . print_r(debug_backtrace(), TRUE) . '</pre>');
        watchdog('hpcloud', 'Could not find file named %f.', array('%f' => $data['object']));

      }
    }
  }

  // Inherit docs.
  public function getUri() {
    return $this->uri;
  }

  /**
   * Get the external URL.
   *
   * Pay attention. This gets complicated.
   *
   * If...
   *
   * - The container is private or the `private://` protocol is used, we generate an internal URL.
   * - The container is public, and the object exists, we return an external link to the object
   *   (which may be a CDN-cached URL).
   * - The container is public and the object looks like a URL, we return the URL to the
   *   object storage directory. This is done because internal ops use this. Be wary of this.
   * - The container is public and the object does not exist, we return an INTERNAL link that will
   *   hppefully kick off the generation of an object.
   * - The container is not the public scheme, but is a public container, serve out the EXTERNAL
   *   URL regardless.
   *
   * Why jump through all of these hoops? Because we want to balance the performance of CDN-cached
   * object storage with Drupal's passive method of generating certain files (like images).
   *
   * So, for example, say I upload a new image "foo.png". And assume that Drupal wants to apply
   * the 'thumbnail' style to this image.
   *
   * The original foo.png is saved on object storage, and then Drupal begins generating links
   * to the thumbnail image even though it does not yet exist.
   *
   * The first request for the thumbnail is, in Drupal's opinion, the ideal point for
   * generating this image. But wait! Drupal has to generate the image. So if we are
   * pointing users to the object store, Drupal doesn't have a chance to generate images.
   *
   * The result: Lots of 404s.
   *
   * How we work around this is by checking wether the object exists on object storage,
   * and then building a link to swift only if the object DOES exist. Otherwise, we build
   * a link to Drupal, and let it build the resource.
   *
   * Private files are similarly complex. They always need to be served out of Drupal. But
   * there are two ways files can be marked private:
   *
   * - via the `private://` handle
   * - via private containers
   *
   * So we have to handle both cases. The upshot, though, is that URLs to private resources
   * are always directed back through Drupal.
   *
   */
  public function getExternalUrl() {
    $data = self::uriToData($this->uri);

    // Private files are served through Drupal.
    if (!$this->container->acl()->isPublic() || $data['scheme'] == 'private') {
      // XXX: WHAT DO WE RETURN?
      return url('system/files/' . $data['object'], array('absolute' => TRUE));
    }

    // Public files that exist are served out of Object Storage directly.
    if (isset($this->obj)) {
      return $this->obj->url(TRUE);
    }


    // If we get here, then there are two possibilities:
    // 1. We are working with a directory-like path.
    // 2. We are working with a file that has not yet been created.
    $itLooksLikeAFileToMe = self::isFileish($data['object']);

    // Route public files through Drupal if it looks like they may not exist.
    if ($itLooksLikeAFileToMe && $data['scheme'] == 'public') {
      //watchdog('hpcloud', 'Giving local path for ' . $data['object']);
      return $this->getLastDitchPath($data['object']);
    }
    // Uh... I guess we give out a URL to the object store?
    else {
      return $this->container->url() . '/' . $data['object'];
    }
  }

  protected function getLastDitchPath($path) {

    // There are a few cases where an ObjectStorage race
    // condition can occur for CSS/JS files. (cf.
    // Swift's eventual consistency model.)
    static $exceptions = array(
      'css' => TRUE,
      'js' => TRUE,
    );
    $parts = explode('/', $path);

    if (isset($exceptions[$parts[0]])) {
      watchdog('hpcloud', 'Correcting eventual consistency error.');
      return $this->container->url() . '/' . $path;
    }

    // Otherwise we should be safe and point to Drupal. This is for
    // imagecache and other lazy generators.
    $basedir = variable_get('file_public_path', conf_path() . '/files');
    return $GLOBALS['base_url'] . '/' .  $basedir . '/' . $path;

  }

  /**
   * Get the object itself.
   *
   * @return object
   *   The \HPCloud\Storage\ObjectStorage\Object that represents the
   *   remote resource.
   */
  public function object() {
    return $this->obj;
  }
  /**
   * Get the container.
   *
   * @return object
   *   The \HPCloud\Storage\ObjectStorage\Container that represents
   *   the Swift container.
   */
  public function container() {
    return $this->container;
  }


  /**
   * Change permissions.
   *
   * Like many systems, ObjectStorage does not use UNIX modes. Calling
   * this will have no effect, but it must be here and return TRUE in
   * order for this stream wrapper to meet Drupal's expectations.
   */
  public function chmod($mode) {
    return TRUE;
  }

  /**
   * Get a directory-like URI from a base URI.
   *
   * This is the weirdest function. If $path is set, it is presumably
   * a URI. But we don't know anything about this URI, including where
   * it came from or whether we are responsible for handling it.
   *
   * If it's not set, then we use the present object's URI. Seems like
   * this is a misplaced utility function or static method.
   *
   * Finally, what we return isn't a dirname, but a URI with the last
   * item chopped off the path.
   */
  public function dirname($uri = NULL) {
    if ($uri == NULL) {
      $uri = $this->uri;
    }
    $data = self::uriToData($uri);

    // If path is empty or has no slashes, dirname returns '.'.
    $path = dirname($data['object']);
    if ($path == '.') {
      $path = '';
    }

    return $data['scheme'] . '://' . $path;
  }

  /**
   * Get the realname.
   *
   * Because some 3rd Party modules incorrectly use 
   * realpath to get the URI, we implement this returning
   * the URI.
   *
   * A responsible module should check the stream wrapper
   * for the STREAMWRAPPER_LOCAL bit and act accoringly. In
   * most cases, though, realpath isn't necessary at all.
   *
   * FIXME: Need to submit patches to those modules and then
   * remove this.
   */
  public function realpath() {
    //return self::swiftUrl($this->uri);
    return $this->uri;
    //return FALSE;
  }

  /**
   * Open the stream.
   */
  public function stream_open($uri, $mode, $options, &$opened_url) {
    if ($uri == $this->uri) {
      throw new Exception('This resource was already created!');
    }

    // Sometimes Drupal does not call the constructor functions.
    // See file.inc's file_unmanaged_copy(), which calls
    // copy() directly instead of stream_copy_stream().
    if (empty($this->uri)) {
      $this->uri = $uri;
    }

    $new_url = self::swiftUrl($uri);

    // We need to set a guessed MIME type.
    list($junk, $path) = explode('://', $uri);
    $mime = self::guessMimeType($path);
    $context = self::swiftContext(array('content_type' => $mime));

    // Suppress in order to generate our own error.
    $this->handle = @fopen($new_url, $mode, FALSE, $context);

    return !empty($this->handle);
  }
  /**
   * Close the stream.
   */
  public function stream_close() {
    unset(self::$statCache[$this->uri]);
    return fclose($this->handle);
  }

  /**
   * Lock the stream.
   *
   * There is no locking mechanism in ObjectStorage. But Drupal
   * expects this to return TRUE, so we oblige.
   */
  public function stream_lock($operation) {
    return TRUE;
  }
  /**
   * Read bytes from the stream.
   */
  public function stream_read($count) {
    // Work around the fact that Drupal doesn't
    // always check whether a handle is valid before
    // writing to it.
    if (empty($this->handle)) {
      return FALSE;
    }
    return fread($this->handle, $count);
  }
  /**
   * Write bytes to the stream.
   *
   * Note that this does not ever automatically flush.
   */
  public function stream_write($data) {
    return fwrite($this->handle, $data);
  }
  /**
   * Check for the EOF marker.
   *
   * Returns TRUE when the end of the stream has been reached.
   */
  public function stream_eof() {
    if (empty($this->handle)) {
      return FALSE;
    }
    return feof($this->handle);
  }
  /**
   * Seek to a location in the stream.
   */
  public function stream_seek($offset, $whence) {
    return fseek($this->handle, $offset, $whence);
  }

  /**
   * Write the data to the remote endpoint.
   */
  public function stream_flush() {
    if (isset(self::$statCache[$this->uri])) {
      unset(self::$statCache[$this->uri]);
    }
    $ret = fflush($this->handle);

    return $ret;
  }
  /**
   * Tell the position of the stream pointer.
   */
  public function stream_tell() {
    return ftell($this->handle);
  }
  /**
   * Stat the file-like object.
   *
   * Unlike url_stat this does not fake stats on
   * non-existing objects.
   */
  public function stream_stat() {
    //watchdog('hpcloud', 'fstatting %f', array('%f' => $uri), WATCHDOG_NOTICE);
    // Check the cache first.
    $cached = $this->checkStatCaches($this->uri);
    if (!empty($cached)) {
      return $cached;
    }

    $stats = fstat($this->handle);
    self::$statCache[$this->uri] = $stats;

    return $stats;
  }

  protected function checkStatCaches($uri, $container = NULL) {
    if (!empty(self::$statCache[$uri])) {
      // watchdog('hpcloud', 'Temp Cache Hit! ' . $uri);
      return self::$statCache[$uri];
    }

    //$parts = self::uriToData($uri);
    list($scheme, $path) = explode('://', $uri);
    //watchdog('hpcloud', 'Looking in pcache for ' . $path);

    // strip leading slash.
    if (strpos($path, '/') === 0) {
      $path = substr($path, 1);
    }

    if (empty($path)) {
      return;
    }

    if (empty($container)) {
      $container = $this->container;
    }

    // Check the persistent cache.
    $pcache = new HPCloudDrupalStatCache($container);
    if ($pcache->has($path)) {
      // watchdog('hpcloud', 'Persistent Cache Hit! ' . $path);
      return $pcache->stat($path);
    }
    //watchdog('hpcloud', 'Cache MISS! ' . $path , array(), WATCHDOG_WARNING);

    return NULL;
  }

  /**
   * Cast into a low-level IO stream.
   *
   * We cheat and actually copy the remote into a local
   * stream, returning the local stream.
   *
   * This is supposed to work for allowing filtering
   * through gzip functions.
   */
  /*
  public function stream_cast($cast_as) {
    $tmp = fopen('php://temp', 'wb+');
    rewind($this->handle);
    stream_copy_to_stream($this->handle, $tmp);
    rewind($tmp);
    return $tmp;
  }
   */
  /**
   * Delete the object at the given URI.
   */
  public function unlink($uri) {
    return unlink(self::swiftUrl($uri));
  }
  /**
   * Copy the object, then remove the old version.
   */
  public function rename($from_uri, $to_uri) {
    $new_from = self::swiftUrl($from_uri);
    $new_to   = self::swiftUrl($to_uri);
    return rename($new_from, $new_to);
  }
  /**
   * Fake a make a dir.
   *
   * ObjectStorage has pathy objects, but not directories.
   */
  public function mkdir($uri, $mode, $options) {
    return TRUE;
  }
  /**
   * Fake a rmdir.
   *
   * Object storage does not have directories, but allows
   * directory separators in object names. We fake it.
   */
  public function rmdir($uri, $options) {
    return TRUE;
  }
  /**
   * Return a stat for the URI.
   *
   * The job of this method is remarkably complex, since Drupal
   * calls it frequently, and since Drupal makes assumptions about
   * what sort of underlying system is being used.
   *
   * Specifically, Drupal often assumes that a stream wrapper
   * is mapping to a real file on the file system. It thus assumes
   * that there are directories, real paths, UNIX permissions, basenames,
   * file types, and so on.
   *
   * Many of these assumptions simply do not hold for Object Storage,
   * so we have to fake it.
   *
   * Important fakes:
   *
   * - Underlying device info is always fake. There are no inodes, block
   *   allocations, and so on.
   * - Permissions are faked -- sometimes they are set according to the
   *   parent container's URL, while other times they are just set to 777.
   * - Directory records are totally fake. Swift does not have directories,
   *   so we intercept requests that look like they are for directories and
   *   we create completely fake stats. See fakeStat().
   *
   * ALSO, Drupal is very liberal in calling stat() (actually, in calling the
   * kazillion functions that are actually backed by stat(), such as is_dir, 
   * file_exists, and so on).
   *
   * For that reason, we aggressively cache stat data -- to the point of possibly
   * returning stale data. This is generally irrelevant, except perhaps where a
   * container's permissions were changed.
   */
  public function url_stat($uri, $flags) {
    //watchdog('hpcloud', 'statting %f', array('%f' => $uri), WATCHDOG_NOTICE);
    //if (!empty(self::$statCache[$uri])) {
    //  return self::$statCache[$uri];
    //}

    // This will set the tokens, and authenticate to the server
    // only if needed.
    $id = _hpcloud_login();
    $token = $id->token();
    $endpoint = self::swiftEndpoint($id->serviceCatalog()); //ZZZ
    list($scheme, $path) = explode('://', $uri);
    $containerName = self::containerForScheme($scheme);
    $url = $endpoint . '/' . $containerName;
    //$parts = self::uriToData($uri);
    //drupal_set_message(print_r($parts, TRUE));
    //drupal_set_message("Container for $uri is $containerName.");

    $container = new \HPCloud\Storage\ObjectStorage\Container($containerName, $url, $token);

    // Check the cache first.
    try {
      $cached = $this->checkStatCaches($uri, $container);
      if (!empty($cached)) {
        return $cached;
      }
    }
    catch (Exception $e) {
      watchdog('hpcloud', 'Error fetching stats from cache: %err', array('%err' => $e->getMessage()));
    }
    // Need to ensure that the endpoint is in the
    // bootstrap config.
    $config = array('swift_endpoint' => $endpoint);
    \HPCloud\Bootstrap::setConfiguration($config);
    $new_url = self::swiftUrl($uri);
    if ($flags & STREAM_URL_STAT_QUIET) {
      $stats = @stat($new_url);
    }
    else {
      $stats = stat($new_url);
    }

    $data = self::uriToData($uri);
    if (empty($stats) && !self::isFileish($data['object'])) {
      //watchdog('hpcloud', 'Faking a directory for %f', array('%f' => $uri), WATCHDOG_NOTICE);
      $stats = HPCloudDrupalStatCache::fakeStat(TRUE);
    }

    // Don't cache non-existent files.
    if (!empty($stats)) {
      self::$statCache[$uri] = $stats;
    }

    return $stats;
  }
  /**
   * Get a prefix results list.
   */
  public function dir_opendir($uri, $options) {
    $swift = self::swiftUrl($uri);
    $context = self::swiftContext();

    // Directory-like pattern should end with /.
    if (strrpos($swift, '/') !== strlen($swift) - 1) {
      $swift .= '/';
    }
    // This is causing a segfault on PHP 5.3.2
    //$this->handle = opendir($swift, $context);

    // This is our workaround for the segfault.
    $dir = opendir($swift, $context);
    if (empty($dir)) {
      return FALSE;
    }
    while ($entry = readdir($dir)) {
      $this->dirlisting[] = $entry;
    }
    closedir($dir);

    return TRUE;
  }
  /**
   * Iterate through a list of objects.
   */
  public function dir_readdir() {
    //return readdir($this->handle);

    // This code is all a workaround for a bug in
    // PHP 5.3.2 which causes a segfault.

    if (count($this->dirlisting) <= $this->dirindex) {
      return FALSE;
    }
    $ret = $this->dirlisting[$this->dirindex];
    ++$this->dirindex;

    return $ret;
  }
  /**
   * Rewind a list of objects.
   */
  public function dir_rewinddir() {
    //return rewinddir($this->handle);
    // Workaround for the bug in PHP 5.3.2.
    $this->dirindex = 0;
  }
  /**
   * Close an object listing.
   */
  public function dir_closedir() {
    // return closedir($this->handle);
    // Workaround for bug in 5.3.2.

    $this->dirlisting = array();
    $this->dirindex = 0;
  }

  /**
   * Get directory path.
   *
   * While this is technically not part of any of the interfaces, it
   * appears to be called sometimes without a check to see whether it
   * exists.
   */
  public function getDirectoryPath() {
    $data = self::uriToData($this->uri);

    if ($data['scheme'] == 'public') {
      return variable_get('file_public_path', conf_path() . '/files');
    }
    elseif ($data['scheme'] == 'private') {
      return variable_get('file_private_path', '');
    }

    return '';

  }


  // ==========================================================================
  // Utilities
  // ==========================================================================
  /**
   * Rewrite the path from a Drupal URI to a Swift URI.
   *
   * This copies Drupa's path parsing code, and then builds a new
   * URL from it.
   *
   * @param $path
   *   The Drupal path (public://PATH).
   * @return
   *   The Swift path (swift://CONTAINER/OBJECT)
   */
  protected function rewritePath($path) {
    list($scheme, $path) = explode('://', $path, 2);

    // Technically this should be an rtrim for Swift, but we stay
    // with Drupal.
    $path = trim($path, '\/');

    $container_name = self::containerForScheme($scheme);

    // This indicates a higher-level issue.
    if (empty($container_name)) {
      drupal_set_message('A container has not been assigned to ' . $scheme, 'status');
      return;
    }

    $swift_scheme = \HPCloud\Storage\ObjectStorage\StreamWrapper::DEFAULT_SCHEME;

    return $swift_scheme . '://' . $container_anme . '/' . $path;
  }

  /**
   * Parse a URI and determine the container and object for this URI.
   *
   * Returns an array with the following keys:
   *
   * - container: The name of the container.
   * - object: The name of the object.
   */
  public static function uriToData($uri) {
    list($scheme, $path) = explode('://', $uri, 2);

    // Technically this should be an rtrim for Swift, but we stay
    // with Drupal.
    $path = trim($path, '\/');

    $container_name = self::containerForScheme($scheme);

    return array('container' => $container_name, 'object' => $path, 'scheme' => $scheme);
  }

  /**
   * Given a scheme name, get the associated container.
   */
  public static function containerForScheme($scheme) {
    $schemata = variable_get('hpcloud_enabled_schemes');

    if (!empty($schemata[$scheme])) {
      return $schemata[$scheme];
    }
  }

  /**
   * Given a filename, guess the content type.
   *
   * @param $fname
   *   String filename.
   * @return
   *   String MIME type (Content Type).
   */
  public static function guessMimeType($fname) {

    $ext = strtolower(pathinfo($fname, PATHINFO_EXTENSION));

    // SERIOUSLY? THIS is how you do it?
    include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';
    $map = file_mimetype_mapping();
    if (isset($map['extensions'][$ext])) {
      return $map['mimetypes'][$map['extensions'][$ext]];
    }

    // Go with the swift default, not the Drupal default (I think
    // they are the same anyway).
    return \HPCloud\Storage\ObjectStorage\Object::DEFAULT_CONTENT_TYPE;
  }

  /**
   * Initialize an ObjectStorage instance.
   *
   * This overrides the base class, using the Drupal variables
   * table in place of a stream context.
   *
   * @return boolean
   *   Boolean TRUE if the object was initialized, FALSE otherwise.
   */
  protected function initializeObjectStorage() {
    try {
      $id = _hpcloud_login();
    }
    catch (\HPCloud\Transport\AuthorizationException $e) {
      $msg = $e->getMessage();

      // Try logging in again.
      if (strpos('403', $msg) !== FALSE) {
        $id = _hpcloud_login(FALSE);
        if (empty($id)) {
          throw $e;
        }
      }
    }

    //$this->store = \HPCloud\Storage\ObjectStorage::newFromServiceCatalog($tokens['catalog'], $tokens['token']);
    $this->store = \HPCloud\Storage\ObjectStorage::newFromIdentity($id);

    try {
      // CDN!
      //$cdn = \HPCloud\Storage\CDN::newFromServiceCatalog($tokens['catalog'], $tokens['token']);
      $cdn = \HPCloud\Storage\CDN::newFromIdentity($id);
      if (!empty($cdn)) {
        $this->store->useCDN($cdn);
      }
    } catch (\HPCloud\Exception $e) {
      // Ignore an exception, but don't add CDN.
    }

    return !empty($this->store);
  }
}

/**
 * Provides a persistent stat cache.
 */
class HPCloudDrupalStatCache {

  const CACHE_KEY = 'hpcloud_stat_cache';

  protected $cache;
  protected $container;

  public function __construct($container) {
    $this->container = $container;
    $obj = cache_get(self::CACHE_KEY);

    if (empty($obj)) {
      $this->cache = $this->rebuild();
    }
    else {
      $this->cache = $obj->data;
    }
  }

  public function has($filename) {
    return isset($this->cache[$filename]);
  }

  public function stat($filename) {
    return $this->cache[$filename];
  }

  public function rebuild() {
    //drupal_set_message('Rebuilding for ' . $this->container->name() );
    watchdog('hpcloud', 'Rebuilding persistent cache for %container.', array('%container' => $this->container->name()));
    // FIXME: This assumes there will be < 10,000 objects.
    $objects = $this->container->objects();

    $cache = $this->baseCache();

    foreach ($objects as $object) {
      //watchdog('hpcloud', 'Adding entry for ' . $object->name());
      //drupal_set_message('Adding entry for ' . $object->name());
      $cache[$object->name()] = $this->generateStat($object);
    }

    // Testing at 3 minutes.
    $expires = REQUEST_TIME + 180;

    cache_set(self::CACHE_KEY, $cache, 'cache');

    return $cache;
  }

  protected function baseCache() {
    $cache = array(
      '' => self::fakeStat(TRUE),
      'css' => self::fakeStat(TRUE),
      'js' => self::fakeStat(TRUE),
      'field' => self::fakeStat(TRUE),
      'field/images' => self::fakeStat(TRUE),
      'styles' => self::fakeStat(TRUE),
      'ctools' => self::fakeStat(TRUE),
      'ctools/css' => self::fakeStat(TRUE),
      'ctools/js' => self::fakeStat(TRUE),
    );


    return $cache;
  }

  protected function generateStat($remote_object = NULL) {

    // File with 775
    $mode = 0100775;
    $time = $remote_object->lastModified();

    $values = array(
      'dev' => 0,
      'ino' => 0,
      'mode' => $mode,
      'nlink' => 0,
      'uid' => posix_getuid(),
      'gid' => posix_getgid(),
      'rdev' => 0,
      'size' => 0,
      'atime' => $time,
      'mtime' => $time,
      'ctime' => $time, 
      'blksize' => -1,
      'blocks' => -1,
    );

    $final = array_values($values) + $values;
    return $final;

  }

  /**
   * Fake stat data.
   *
   * Under certain conditions we have to return totally trumped-up
   * stats. This generates those.
   */
  public static function fakeStat($dir = FALSE) {

    // Set inode type to directory or file.
    $type = $dir ? 040000 : 0100000;
    // Fake world-readible
    $mode = $type + 0777;

    $values = array(
      'dev' => 0,
      'ino' => 0,
      'mode' => $mode,
      'nlink' => 0,
      'uid' => posix_getuid(),
      'gid' => posix_getgid(),
      'rdev' => 0,
      'size' => 0,
      'atime' => REQUEST_TIME,
      'mtime' => REQUEST_TIME,
      'ctime' => REQUEST_TIME,
      'blksize' => -1,
      'blocks' => -1,
    );

    $final = array_values($values) + $values;
    return $final;
  }


}
