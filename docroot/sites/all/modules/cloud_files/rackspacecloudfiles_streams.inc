<?php
/**
 * @file
 * Implement DrupalStreamWrapperInterface to provide an rcf:// streamwrapper.
 */

class RackspaceCloudFilesStreamWrapper implements DrupalStreamWrapperInterface {
  /**
   * Instance URI (stream).
   *
   * These streams will be referenced as 'rcf://target'
   *
   * @var String
   */
  protected $uri;

  /**
   * A generic resource handle.
   */
  public $handle = NULL;

  /**
   * Pointer to where we are in a directory read.
   */
  protected $directoryPointer = 0;

  /**
   * List of files in a given directory.
   */
  protected $directoryList;

  /**
   * The pointer to the next read or write
   */
  protected $streamPointer = 0;

  /**
   * A buffer for reading/wrting.
   */
  protected $streamData = NULL;

  /**
   * Object cache
   *
   * @var CFResponse
   */
  protected $object = NULL;

  /**
   * Container cache
   */
  protected $container = NULL;

  /**
   * Data is not written to rackspace in stream_write to minimize requests
   * to rackspace. instead, data is written to the $streamData property.
   * This $write property is flagged as true, and in stream_flush, all the
   * data is written to rackspace at once.
   *
   * @var <type> Boolean
   */
  protected $write = FALSE;

  /**
   * Implements setUri().
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Implements getUri().
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * Returns the local writable target of the resource within the stream.
   *
   * @param string $uri
   *   Optional URI.
   *
   * @return string
   *   Returns a string representing a location suitable for writing of a file,
   *   or FALSE if unable to write to the file such as with read-only streams.
   */
  protected function getTarget($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    list($scheme, $target) = explode('://', $uri, 2);

    // Remove erroneous leading or trailing, forward-slashes and backslashes.
    return trim($target, '\/');
  }

  /**
   * Base implementation of getMimeType().
   */
  public static function getMimeType($uri, $mapping = NULL) {
    if (!isset($mapping)) {
      // The default file map, defined in file.mimetypes.inc is quite big.
      // We only load it when necessary.
      include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';
      $mapping = file_mimetype_mapping();
    }

    $extension = '';
    $file_parts = explode('.', basename($uri));

    // Remove the first part: a full filename should not match an extension.
    array_shift($file_parts);

    /* Iterate over the file parts, trying to find a match.
     * For my.awesome.image.jpeg, we try:
     *    - jpeg
     *    - image.jpeg, and
     *    - awesome.image.jpeg
     */
    while ($additional_part = array_pop($file_parts)) {
      $extension = strtolower($additional_part . ($extension ? '.' . $extension : ''));
      if (isset($mapping['extensions'][$extension])) {
        return $mapping['mimetypes'][$mapping['extensions'][$extension]];
      }
    }

    return 'application/octet-stream';
  }

  /**
   * Overrides getExternalUrl().
   */
  public function getExternalUrl() {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing getExternalUrl (%uri)", array('%uri' => $this->uri), WATCHDOG_DEBUG);
    }

    $target = file_uri_target($this->uri);

    // Get fileinfo from the database.
    $fileinfo = $this->get_fileinfo($this->uri);

    // If file does not exist in the database (its probably new).
    if (!$fileinfo) {

      /* 
       * Attempt to get file from Rackspace.
       */

      if (!isset($this->container)) {
        $this->getContainer();
      }

      try{
        $this->object = $this->container->dataObject($target);

        // File does exist!  add file info to the database.
        $this->add_fileinfo($this->uri, $this->object->getContentType(), $this->object->getContentLength());
      }
      catch (Exception $e) {
        /*
         * Image styles support.
         * Image is generated when its first requested, so serve from the
         * private file system. This will save the file to rackspace and it'll
         * exist next and wont error into this.
         */
        $path = explode('/', $this->getLocalPath());
        if ($path[0] == 'styles') {
          array_shift($path);
          return url('system/files/styles/' . implode('/', $path), array('absolute' => TRUE));
        }
        else {
          // File does not exist in rackspace.
          watchdog('cloud_files', 'File not found: @target', array('@target' => $target), WATCHDOG_ERROR);
        }
      }
    }

    $url = 'http://' . variable_get('rackspace_cloud_cdn_domain') . '/' . $target;
    return $url;
  }

  /**
   * We have no concept of chmod, so just return TRUE.
   */
  public function chmod($mode) {
    return TRUE;
  }

  /**
   * Implements realpath().
   */
  public function realpath() {
    return 'rcf://' . $this->getLocalPath();
  }

  /**
   * Returns the canonical absolute path of the URI, if possible.
   *
   * @param string $uri
   *   (optional) The stream wrapper URI to be converted to a canonical
   *   absolute path. This may point to a directory or another type of file.
   *
   * @return string
   *   Returns the target of the URI.
   */
  protected function getLocalPath($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }
    return $this->getTarget($uri);
  }

  /**
   * Opens a stream, as for fopen(), file_get_contents(), file_put_contents().
   *
   * @param string $uri
   *   A string containing the URI to the file to open.
   * @param string $mode
   *   The file mode ("r", "wb" etc.).
   * @param $options
   *   A bit mask of STREAM_USE_PATH and STREAM_REPORT_ERRORS.
   * @param string &$opened_path
   *   A string containing the path actually opened.
   *
   * @return bool
   *   Returns TRUE if file was opened successfully. (Always returns TRUE).
   *
   * @see http://php.net/manual/en/streamwrapper.stream-open.php
   */
  public function stream_open($uri, $mode, $options, &$opened_path) {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing stream_open (%uri)", array('%uri' => $uri), WATCHDOG_DEBUG);
    }

    // Reset data.
    $this->stream_pointer = 0;
    $this->stream_data = NULL;

    $this->uri = $uri;
    $target = file_uri_target($this->uri);

    if (!isset($this->container)) {
      $this->getContainer();
    }

    try{
      // This will load the object if it exists, errors if it doesn't exist.
      $this->object = $this->container->dataObject($target);
    }
    catch (Exception $e) {
      // Say stream is open even if this doesnt exist (to write to).
    }

    // TODO check $mode, $options and act accordingly.
    return TRUE;
  }

  /**
   * Support for flock().
   *
   * @param $operation
   *   One of the following:
   *   - LOCK_SH to acquire a shared lock (reader).
   *   - LOCK_EX to acquire an exclusive lock (writer).
   *   - LOCK_UN to release a lock (shared or exclusive).
   *   - LOCK_NB if you don't want flock() to block while locking (not
   *     supported on Windows).
   *
   * @return bool
   *   Always returns TRUE at the present time. (no support)
   *
   * @see http://php.net/manual/en/streamwrapper.stream-lock.php
   */
  public function stream_lock($operation) {
    // Rackspace Cloud Files does not have locking.
    return FALSE;
  }

  /**
   * Support for fread(), file_get_contents() etc.
   *
   * @param int $count
   *   Maximum number of bytes to be read.
   *
   * @return string
   *   The string that was read, or FALSE in case of an error.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-read.php
   */
  public function stream_read($count) {
    // If the data is empty, get the data from the object.
    // This stores all the data in the $streamData property, only request once.
    if (!$this->stream_data) {
      // If the object is empty, get the object from Rackspace.
      if (!$this->object) {
        if (!isset($this->container)) {
          $this->getContainer();
        }
        $this->object = $this->container->dataObject($target);
      }
      $this->stream_data = $this->object->getContent();
    }
    $data = substr($this->stream_data, $this->stream_pointer, $count);
    $this->stream_pointer += $count;
    return $data;
  }

  /**
   * Support for fwrite(), file_put_contents() etc.
   *
   * @param string $data
   *   The string to be written.
   *
   * @return int
   *   The number of bytes written (integer).
   *
   * @see http://php.net/manual/en/streamwrapper.stream-write.php
   */
  public function stream_write($data) {
    // Flag write when flushed.
    $this->write = TRUE;
    $this->stream_data .= $data;

    $byteswritten = strlen($data);

    $this->stream_pointer += $byteswritten;

    return $byteswritten;
  }

  /**
   * Support for feof().
   *
   * @return bool
   *   TRUE if end-of-file has been reached.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-eof.php
   */
  public function stream_eof() {
    // If the data is empty, get the data from the object.
    // This stores all the data in the $streamData property, only request once.
    if (!$this->stream_data) {
      // If the object is empty, get the object from rackspace.
      if (!$this->object) {
        if (!isset($this->container)) {
          $this->getContainer();
        }
        $this->object = $this->container->dataObject(file_uri_target($this->uri));
      }
      $this->stream_data = $this->object->getContent();
    }
    return $this->stream_pointer >= strlen($this->stream_data);
  }

  /**
   * Support for fseek().
   *
   * @param int $offset
   *   The byte offset to got to.
   * @param $whence
   *   SEEK_SET, SEEK_CUR, or SEEK_END.
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-seek.php
   */
  public function stream_seek($offset, $whence) {
    switch ($whence) {
      case SEEK_SET:
        if (strlen($this->stream_data) >= $offset && $offset >= 0) {
          $this->stream_pointer = $offset;
          return TRUE;
        }
        return FALSE;

      case SEEK_CUR:
        if ($offset >= 0) {
          $this->stream_pointer += $offset;
          return TRUE;
        }
        return FALSE;

      case SEEK_END:
        if (strlen($this->stream_data) + $offset >= 0) {
          $this->stream_pointer = strlen($this->stream_data) + $offset;
          return TRUE;
        }
        return FALSE;

      default:
        return FALSE;
    }
    return FALSE;
  }

  /**
   * Support for fflush().
   *
   * @return bool
   *   TRUE if data was successfully stored (or there was no data to store).
   *
   * @see http://php.net/manual/en/streamwrapper.stream-flush.php
   */
  public function stream_flush() {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "flushing file, writing to Rackspace (stream_flush) %file", array('%file' => $this->uri), WATCHDOG_DEBUG);
    }

    if (!$this->object) {
      if (!$this->container) {
        $this->getContainer();
      }
      $target = file_uri_target($this->uri);

      try{
        // File may not exist yet.
        $this->object = $this->container->dataObject($target);
      }
      catch (Exception $e) {
        // Create new file if file does not exist.
        $this->object = $this->container->dataObject();
        $this->object->setName($target);
        // Let Drupal guess the mime type.
        $this->object->setContentType($this->getMimeType($this->uri));
      }
    }
    if ($this->write) {
      $this->object->setContent($this->stream_data);
      $this->object->update();

      $this->add_fileinfo($this->uri, $this->object->getContentType(), $this->object->getContentLength());
    }

    $this->stream_data = NULL;
    $this->stream_pointer = 0;
    return TRUE;
  }

  /**
   * Support for ftell().
   *
   * @return int
   *   The current offset in bytes from the beginning of file.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-tell.php
   */
  public function stream_tell() {
    return $this->stream_pointer;
  }

  /**
   * Support for fstat().
   *
   * @return array
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-stat.php
   */
  public function stream_stat() {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing stream_stat", NULL, WATCHDOG_DEBUG);
    }
    return array(
      'size' => strlen($this->stream_data),
    );
  }

  /**
   * Support for fclose().
   *
   * @return bool
   *   TRUE if stream was successfully closed.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-close.php
   */
  public function stream_close() {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing stream_close", NULL, WATCHDOG_DEBUG);
    }

    unset($this->object);
    $this->stream_data = NULL;
    $this->stream_pointer = 0;
    // Unassign the reference.
    unset($this->stream_data);
    return TRUE;
  }

  /**
   * Support for unlink().
   *
   * @param string $uri
   *   A string containing the uri to the resource to delete.
   *
   * @return bool
   *   TRUE if resource was successfully deleted.
   *
   * @see http://php.net/manual/en/streamwrapper.unlink.php
   */
  public function unlink($uri) {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing unlink", NULL, WATCHDOG_DEBUG);
    }

    $this->uri = $uri;

    $target = $this->getLocalPath($this->uri);

    if (!isset($this->container)) {
      $this->getContainer();
    }

    $deleted = FALSE;
    $doesnotexist = FALSE;
    try {
      // Dont put name here becuase it will fetch the object.
      $obj = $this->container->dataObject();
      // Delete by name, without fetching the object.
      $obj->setName($target);
      $deleted = $obj->delete();
    }
    catch (NoSuchObjectException $e) {
      $doesnotexist = TRUE;
    }

    if ($deleted || $doesnotexist) {
      // Remove file from database.
      $this->remove_fileinfo($this->uri);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Support for rename().
   *
   * @param string $from_uri,
   *   The uri to the file to rename.
   * @param string $to_uri
   *   The new uri for file.
   *
   * @return bool
   *   TRUE if file was successfully renamed.
   *
   * @see http://php.net/manual/en/streamwrapper.rename.php
   */
  public function rename($from_uri, $to_uri) {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing rename (%from, %to)", array("%from" => $from_uri, "%to" => $to_uri), WATCHDOG_DEBUG);
    }

    // TODO rename.
    return FALSE;
  }

  /**
   * Support for stat().
   * 
   * This important function goes back to the Unix way of doing things.
   * In this example almost the entire stat array is irrelevant, but the
   * mode is very important. It tells PHP whether we have a file or a
   * directory and what the permissions are. All that is packed up in a
   * bitmask. This is not normal PHP fodder.
   *
   * @param string $uri
   *   A string containing the URI to get information about.
   * @param string $flags
   *   A bit mask of STREAM_URL_STAT_LINK and STREAM_URL_STAT_QUIET.
   *
   * @return array
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   *
   * @see http://php.net/manual/en/streamwrapper.url-stat.php
   */
  public function url_stat($uri, $flags) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }
    $this->uri = $uri;

    $target  = $this->getTarget($this->uri);

    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "Checking file status (url_stat) %file", array('%file' => $uri), WATCHDOG_DEBUG);
    }

    // Default to fail.
    $stat = FALSE;
    $mode = 0;

    if (empty($target)) {
      $fileinfo = array('filemime' => 'application/directory', 'filesize' => 1);
    }
    else {
      $fileinfo = $this->get_fileinfo($this->uri);
    }

    // If file does not exist in the database (its probably new).
    // Try to get it from Rackspace.
    if (!$fileinfo) {
      if (!isset($this->object)) {
        if (!isset($this->container)) {
          $this->getContainer();
        }
      }

      // Check if the file exists.
      $objexists = TRUE;
      try{
        $this->object = $this->container->dataObject($target);
        $filemime = $this->object->getContentType();
        $filesize = $this->object->getContentLength();

        // File does exist!  add file info to the database.
        $this->add_fileinfo($this->uri, $filemime, $filesize);
      }
      catch (Exception $e) {
        $objexists = FALSE;
      }
    }
    else {
      // Fileinfo exists in the database.
      $objexists = TRUE;
      $filemime = $fileinfo['filemime'];
      $filesize = $fileinfo['filesize'];
    }

    if ($objexists) {
      // If the target is empty, its the root of the container (directory).
      if ($filemime == "application/directory") {
        // S_IFDIR means it's a directory.
        $mode = 0040000;
      }
      else {
        // S_IFREG, means it's a file.
        $mode = 0100000;
      }

      if ($mode) {
        $mode |= 0777;

        $stat = array(
          'dev' => 0,
          '0' => 0,
          'ino' => 0,
          '1' => 0,
          'mode' => $mode,
          '2' => $mode,
          'nlink' => 0,
          '3' => 0,
          'uid' => 0,
          '4' => 0,
          'gid' => 0,
          '5' => 0,
          'rdev' => 0,
          '6' => 0,
          'size' => $filesize,
          '7' => $filesize,
          'atime' => 0,
          '8' => 0,
          'mtime' => 0,
          '9' => 0,
          'ctime' => 0,
          '10' => 0,
          'blksize' => 0,
          '11' => 0,
          'blocks' => 0,
          '12' => 0,
        );
      }
    }

    return $stat;
  }

  /**
   * Gets the name of the directory from a given path.
   *
   * @param string $uri
   *   A URI.
   *
   * @return string
   *   A string containing the directory name.
   *
   * @see drupal_dirname()
   */
  public function dirname($uri = NULL) {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing dirname (%uri)", array("%uri" => $uri), WATCHDOG_DEBUG);
    }

    list($scheme, $target) = explode('://', $uri, 2);
    $target  = $this->getTarget($uri);
    $dirname = dirname($target);

    if ($dirname == '.') {
      $dirname = '';
    }

    return $scheme . '://' . $dirname;
  }

  /**
   * Support for mkdir().
   *
   * @param string $uri
   *   A string containing the URI to the directory to create.
   * @param $mode
   *   Permission flags - see mkdir().
   * @param $options
   *   A bit mask of STREAM_REPORT_ERRORS and STREAM_MKDIR_RECURSIVE.
   *
   * @return bool
   *   TRUE if directory was successfully created.
   *
   * @see http://php.net/manual/en/streamwrapper.mkdir.php
   */
  public function mkdir($uri, $mode, $options) {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing mkdir (%uri)", array("%uri" => $uri), WATCHDOG_DEBUG);
    }

    $this->uri = $uri;

    $target = file_uri_target($this->uri);

    // Only make directories if there are directories to make.
    if ($target) {
      // TODO check if it already exists?
      // TODO check options for recursive?
      $recursive = (bool) ($options & STREAM_MKDIR_RECURSIVE);

      if (!isset($this->container)) {
        $this->getContainer();
      }

      // Add each directory.
      $dirs = explode('/', $target);
      $build_path = "";
      foreach ($dirs as $dir) {
        if (!$build_path) {
          $build_path = $dir;
        }
        else {
          $build_path .= "/" . $dir;
        }

        $objexists = TRUE;
        // Check if this dir already exists.
        $fileinfo = $this->get_fileinfo('rcf://' . $build_path);
        if (!$fileinfo) {
          // Doesn't exist in db, check cloud files.
          try{
            $this->object = $this->container->dataObject($build_path);
            // File does exist!  add file info to the database.
            $this->add_fileinfo('rcf://' . $build_path, 'application/directory', 1);
          }
          catch (Exception $e) {
            // Not in cloud files either, create it.
            $objexists = FALSE;
          }
        }

        if (!$objexists) {
          $info = new stdClass();
          $info->subdir = $build_path;
          $obj = $this->container->dataObject($info);
          $obj->setContentType('application/directory');
          $obj->setContent('');
          $obj->update();

          // Add to database.
          $this->add_fileinfo("rcf://" . $build_path, "application/directory", 1);
        }
      }
    }

    return TRUE;
  }

  /**
   * Support for rmdir().
   *
   * @param string $uri
   *   A string containing the URI to the directory to delete.
   * @param $options
   *   A bit mask of STREAM_REPORT_ERRORS.
   *
   * @return bool
   *   TRUE if directory was successfully removed.
   *
   * @see http://php.net/manual/en/streamwrapper.rmdir.php
   */
  public function rmdir($uri, $options) {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing rmdir (%uri)", array("%uri" => $uri), WATCHDOG_DEBUG);
    }

    // TODO: delete directory and all files.
    return FALSE;
  }

  /**
   * Support for opendir().
   *
   * @param string $uri
   *   A string containing the URI to the directory to open.
   * @param $options
   *   Unknown (parameter is not documented in PHP Manual).
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-opendir.php
   */
  public function dir_opendir($uri, $options) {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing dir_opendir (%uri)", array("%uri" => $uri), WATCHDOG_DEBUG);
    }

    // TODO get list of items in directory.
    return FALSE;
  }

  /**
   * Support for readdir().
   *
   * @return mixed
   *   The next filename, or FALSE if there are no more files in the directory.
   */
  public function dir_readdir() {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing dir_readdir", NULL, WATCHDOG_DEBUG);
    }

    if ($this->directory_pointer < count($this->directory_list)) {
      $next = $this->directory_list[$this->directory_pointer];
      $this->directory_pointer++;
      return $next;
    }
    return FALSE;
  }

  /**
   * Support for rewinddir().
   *
   * @return bool
   *   TRUE on success.
   */
  public function dir_rewinddir() {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing dir_rewinddir", NULL, WATCHDOG_DEBUG);
    }

    $this->directory_pointer = 0;
  }

  /**
   * Support for closedir().
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-closedir.php
   */
  public function dir_closedir() {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing dir_closedir", NULL, WATCHDOG_DEBUG);
    }

    $this->directory_pointer = 0;
    unset($this->directory_list);
    return TRUE;
  }

  /**
   * Implements abstract public function getDirectoryPath().
   *
   * @return string
   *   Returns full URL to a Rackspace Cloud Files domain.
   */
  public function getDirectoryPath() {
    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "executing getDirectoryPath", NULL, WATCHDOG_DEBUG);
    }

    return "http://" . variable_get('rackspace_cloud_cdn_domain');
  }

  /**
   * Simple utility function for adding fileifo to database.
   * 
   * @param string $uri
   *   A string containing the URI of the file being added.
   * @param string $filemime
   *   A string containing the mimetype of the file being added.
   * @param int $uri
   *   The size of the file being added.
   *
   * @return bool
   *   FALSE if it failed.
   */
  public function add_fileinfo($uri, $filemime, $filesize) {
    // Add file info to database.
    $new_file = new stdClass();
    $new_file->uri = $uri;
    $new_file->filemime = $filemime;
    $new_file->filesize = $filesize;
    return drupal_write_record('cloud_files', $new_file);
  }

  /**
   * Simple utility function for getting fileifo from the database.
   * 
   * @param string $uri
   *   A string containing the URI of the file to get.
   *
   * @return array
   *   An array containing the file info (uri, filemime, and filesize).
   */
  public function get_fileinfo($uri) {
    $query = db_select('cloud_files', 'cf');
    $query
      ->condition('uri', $uri)
      ->fields('cf', array('uri', 'filemime', 'filesize'));
    $result = $query->execute();
    return $result->fetchAssoc();
  }

  /**
   * Simple utility function for deleting fileifo from the database.
   * 
   * @param string $uri
   *   A string containing the URI of the file to delete.
   */
  public function remove_fileinfo($uri) {
    return db_delete('cloud_files')
      ->condition('uri', $uri)
      ->execute();
  }

  /**
   * Simple utility function for getting Rackspace Cloud container.
   * 
   * @return Container
   *   Rackspace container instance.
   */
  public function getContainer() {
    libraries_load('php-opencloud');

    $client = $this->getClient();
    $object_store_service = $client->objectStoreService('cloudFiles', variable_get('rackspace_cloud_region'));

    if (variable_get('rackspace_cloud_debug')) {
      watchdog("cloud_files", "requesting container from Rackspace (%container)", array('%container' => variable_get('rackspace_cloud_container')), WATCHDOG_DEBUG);
    }

    $this->container = $object_store_service->getContainer(variable_get('rackspace_cloud_container'));
  }

  /**
   * Simple utility function for getting Rackspace Cloud client instance.
   * 
   * @return Rackspace
   *   Rackspace client instance.
   */
  public function getClient() {
    return _cloud_files_get_client();
  }
}
