<?php

/**
 * @file
 * Classes to work with Yandex.Disk API via WebDAV protocol.
 */

/**
 * Yandex.Disk API WebDAV class.
 *
 * Here are only methods described in API docs. You probably want to use
 * YandexDiskApiWebdavHelper for extended functionality.
 *
 * @link http://api.yandex.com/disk/doc/dg/concepts/about.xml
 */
class YandexDiskApiWebdav {

  /**
   * Base path of a URI of the service api callback.
   */
  const SCHEMA_HOST = 'https://webdav.yandex.com';

  /**
   * Indicates that overwriting is allowed in copy/move methods.
   */
  const OVERWRITE_ALLOW = 'T';

  /**
   * Indicates that overwriting is not allowed in copy/move methods.
   */
  const OVERWRITE_DENY = 'F';

  /**
   * Disk path to work with.
   *
   * The $path must start with a slash.
   *
   * @var string
   */
  protected $path = '/';

  /**
   * Optional query parameters of the path to use with certain requests.
   *
   * @var array
   */
  protected $pathQuery;

  /**
   * Options that will be used in call of drupal_http_request().
   *
   * @var array
   */
  protected $options;

  /**
   * Authorization header value.
   *
   * @var string
   */
  protected $authHeader;

  /**
   * Static field containing a result of last called drupal_http_request().
   *
   * @var object
   */
  public static $lastResponse;

  /**
   * Class constructor.
   *
   * @param string $auth_string
   *   Authentication type and token in string of form 'Type token'. For
   *   example: 'Basic abcdefghijklmnopqrstuvwxyz123456' or 'OAuth ...'.
   */
  public function __construct($auth_string) {
    $this->authHeader = $auth_string;
    $this->resetOptions();
  }

  /**
   * Resets options to initial state after request execution.
   */
  protected function resetOptions() {
    $this->options = array(
      'timeout' => PHP_INT_MAX,
      'headers' => array(
        'Accept' => '*/*',
        'Authorization' => $this->authHeader,
      ),
    );
    $this->pathQuery = array();
  }

  /**
   * Sets the query parameters for the path.
   *
   * @param array $query
   *   The query parameter array to be set, e.g., $_GET.
   */
  public function setPathQuery(array $query) {
    $this->pathQuery = $query;
  }

  /**
   * Downloads a file.
   *
   * The start_byte and end_byte can be used for requesting a particular section
   * of the file. The response to this type of request contains the header
   * Content-Type: multipart/byteranges.
   *
   * @param string $path
   *   Path to the file.
   * @param int $start_byte
   *   (optional) An offset from the start of the file to get a file part.
   * @param int $end_byte
   *   (optional) An end byte is included in a file part that will be returned.
   *
   * @return $this
   *   Same object.
   */
  public function get($path, $start_byte = NULL, $end_byte = NULL) {
    $this->options['method'] = 'GET';
    $this->path = $path;

    if (isset($start_byte)) {
      $range = 'bytes=' . $start_byte;
      if (isset($end_byte)) {
        $range .= '-' . $end_byte;
      }

      $this->options['headers']['Range'] = $range;
    }

    return $this;
  }

  /**
   * Uploads a file.
   *
   * At the beginning and end of uploading the file, the service checks whether
   * the file exceeds the space available to the user on Disk. If there is not
   * enough space, the service returns a response with the code 507 Insufficient
   * Storage.
   * Support is provided for transferring compressed files (Content-Encoding:
   * gzip header) and chunked files (Transfer-Encoding: chunked).
   *
   * @param string $path
   *   Path to the file.
   * @param string $data
   *   Data to be saved to the file.
   * @param string $content_type
   *   (optional) Data type.
   *
   * @return $this
   *   Same object.
   */
  public function put($path, $data, $content_type = 'application/binary') {
    $this->options['method'] = 'PUT';
    $this->path = $path;

    $this->options['headers']['Content-Type'] = $content_type;

    if ($data !== '') {
      // Check for duplicate files.
      $this->options['headers']['Etag'] = md5($data);
      $this->options['headers']['Sha256'] = strtoupper(hash('sha256', $data));
      $this->options['headers']['Expect'] = '100-continue';

      // Compress data.
      if (extension_loaded('zlib') && strpos($content_type, 'text/') === 0) {
        $data_compressed = gzencode($data, 9, FORCE_GZIP);

        // Check if compressing worked.
        if (strlen($data_compressed) < strlen($data)) {
          $data = $data_compressed;
          $this->options['headers']['Content-Encoding'] = 'gzip';
        }
      }
    }

    $this->options['data'] = $data;

    return $this;
  }

  /**
   * Creates a directory.
   *
   * According to the protocol, only one directory can be created as the result
   * of a single request. If the application sends a request to create the
   * /a/b/c/ directory, but the /a/ directory does not contain a /b/ directory,
   * the service will not create the /b/ directory, and will respond with the
   * code 409 Conflict.
   *
   * @param string $path
   *   Path to the directory to create.
   *
   * @return $this
   *   Same object.
   */
  public function mkcol($path) {
    $this->options['method'] = 'MKCOL';
    $this->path = $path;

    return $this;
  }

  /**
   * Copies a file/directory.
   *
   * If parent directory where the file/directory should be copied to does not
   * exist, the service will respond with the code 409 Conflict.
   * If overwriting is not allowed and target exists, the service will respond
   * with the code 412 Precondition Failed.
   *
   * @param string $source
   *   Path to the file/directory to copy.
   * @param string $destination
   *   Path where the copy should be created.
   * @param string $overwrite
   *   (optional) Constant indicating whether overwriting is allowed or denied
   *   if target already exists. Default is to allow.
   *
   * @return $this
   *   Same object.
   */
  public function copy($source, $destination, $overwrite = self::OVERWRITE_ALLOW) {
    $this->options['method'] = 'COPY';
    $this->path = $source;

    $this->options['headers']['Destination'] = $destination;
    $this->options['headers']['Overwrite'] = $overwrite;

    return $this;
  }

  /**
   * Moves/renames a file/directory.
   *
   * If parent directory where the file/directory should be moved to does not
   * exist, the service will respond with the code 409 Conflict.
   * If overwriting is not allowed and target exists, the service will respond
   * with the code 412 Precondition Failed.
   *
   * @param string $source
   *   Path to the source file/directory.
   * @param string $destination
   *   New path for the file/directory.
   * @param string $overwrite
   *   (optional) Constant indicating whether overwriting is allowed or denied
   *   if target already exists. Default is to allow.
   *
   * @return $this
   *   Same object.
   */
  public function move($source, $destination, $overwrite = self::OVERWRITE_ALLOW) {
    $this->options['method'] = 'MOVE';
    $this->path = $source;

    $this->options['headers']['Destination'] = $destination;
    $this->options['headers']['Overwrite'] = $overwrite;

    return $this;
  }

  /**
   * Removes a file/directory.
   *
   * As specified in the protocol, removing a directory always removes all of
   * the files and directories that are in it.
   *
   * @param string $path
   *   Path to the file/directory to delete.
   *
   * @return $this
   *   Same object.
   */
  public function delete($path) {
    $this->options['method'] = 'DELETE';
    $this->path = $path;

    return $this;
  }

  /**
   * Gets file/directory properties.
   *
   * @param string $path
   *   Path to the file/directory.
   * @param int $depth
   *   (optional) Use 1 for directory to get a list of its contents properties.
   * @param string $data
   *   (optional) Data to send with the request.
   *
   * @return $this
   *   Same object.
   */
  public function propfind($path, $depth = 0, $data = NULL) {
    $this->options['method'] = 'PROPFIND';
    $this->path = $path;

    $this->options['headers']['Depth'] = $depth;

    $this->options['data'] = $data;

    return $this;
  }

  /**
   * Changes the properties of a file/directory.
   *
   * @param string $path
   *   Path to the file/directory.
   * @param string $data
   *   (optional) Data to send with the request.
   *
   * @return $this
   *   Same object.
   */
  public function proppatch($path, $data = NULL) {
    $this->options['method'] = 'PROPPATCH';
    $this->path = $path;

    $this->options['data'] = $data;

    return $this;
  }

  /**
   * Executes a request.
   *
   * @return bool
   *   Whether the request was executed successfully.
   *
   * @throws \YandexDiskException
   *   If service failed to response.
   */
  public function execute() {
    $url = self::SCHEMA_HOST . drupal_encode_path($this->path);
    if ($this->pathQuery) {
      $url .= '?' . drupal_http_build_query($this->pathQuery);
    }
    $response = drupal_http_request($url, $this->options);

    self::$lastResponse = $response;
    $code = (string) $response->code;

    if (!$code) {
      throw new YandexDiskException(t('No response from service.'));
    }

    // Check for success code.
    $return = ($code[0] == 2 || $this->options['method'] == 'PUT' && $code == 100);

    // Prepare instance for future requests.
    $this->resetOptions();

    return $return;
  }
}

/**
 * Yandex.Disk API WebDAV helper class.
 *
 * Each object of this class works with only one Yandex.Disk account.
 */
class YandexDiskApiWebdavHelper extends YandexDiskApiWebdav {

  /**
   * Yandex.Disk account name.
   *
   * @var string
   */
  protected $user;

  /**
   * Static cache for resources properties.
   *
   * @var array
   *   Multiple-leveled associative array with the following structure:
   *   - Key is an account name, value is an array:
   *     - Key is a resource path, value is an array of properties or NULL in
   *       case resource does not exist. Possible resource's properties:
   *       - d:resourcetype: Empty element.
   *       - d:collection: Empty element, exists only in catalogue's properties.
   *       - d:getlastmodified: Time string ('Mon, 08 Oct 2012 07:02:36 GMT').
   *       - d:getetag: ETag string (only for file).
   *       - d:getcontenttype: Content-type string (only for file).
   *       - d:getcontentlength: Filesize in bytes (only for file).
   *       - d:displayname: Name of file/catalogue.
   *       - d:creationdate: Time string ('2012-10-08T07:02:36Z').
   *       - public_url: Web accessible URL for the resource.
   */
  protected static $propertiesCache;

  /**
   * Creates a class instance by an account name, not by auth string.
   *
   * @param string $account_name
   *   Yandex.Disk account name.
   *
   * @return static
   *   Disk class instance.
   *
   * @throws \YandexDiskException
   *   If there is no valid access token for requested account.
   */
  public static function forAccount($account_name) {
    if ($auth_string = yandexdisk_auth_string($account_name)) {
      $disk = new self($auth_string);
      $disk->user = $account_name;
      return $disk;
    }
    else {
      throw new YandexDiskException(t('Access token missing for @account.', array('@account' => $account_name)));
    }
  }

  /**
   * Sets/removes a header option.
   *
   * @param string $name
   *   Header name.
   * @param mixed $value
   *   Header value to set. Set to NULL to remove header.
   */
  public function setHeader($name, $value) {
    if (isset($value)) {
      $this->options['headers'][$name] = $value;
    }
    else {
      unset($this->options['headers'][$name]);
    }
  }

  /**
   * Checks if operation is allowed for the user and if so executes the request.
   *
   * @return bool
   *   Whether the request was executed successfully.
   *
   * @throws \YandexDiskException
   *   If user is not allowed to perform this operation.
   */
  public function execute() {
    $op = strtolower($this->options['method']);
    $uri = 'yandexdisk://' . $this->getUser() . $this->path;
    if (!yandexdisk_access($op, $uri)) {
      throw new YandexDiskException(t('Access denied for current user to !op the @uri.', array('!op' => $op, '@uri' => $uri)));
    }

    // Clear the properties cache.
    if (!in_array($op, array('get', 'copy', 'propfind'))) {
      $cache = &$this->propertiesCache();
      unset($cache[$this->path]);
    }

    return parent::execute();
  }

  /**
   * Returns the account name of the current Disk instance.
   *
   * @return string
   *   Yandex.Disk account name.
   *
   * @throws \YandexDiskException
   *   If request to the service failed.
   */
  public function getUser() {
    if (!isset($this->user)) {
      // Because this method can be called inside of other request execution,
      // save original options into variables to restore them after this
      // operation.
      $original_options = $this->options;
      $original_path = $this->path;
      $original_path_query = $this->pathQuery;
      $this->resetOptions();

      // This method is available only for OAuth authentication, and not for
      // Basic one.
      $this->get('/')->setPathQuery(array('userinfo' => NULL));

      // Avoid access checking in self::execute(), use parent.
      if (parent::execute()) {
        list($this->user) = sscanf(self::$lastResponse->data, 'login:%s');
      }

      // Restore original options.
      $this->options = $original_options;
      $this->path = $original_path;
      $this->pathQuery = $original_path_query;

      if (!isset($this->user)) {
        throw new YandexDiskException(t('Cannot get the account name.'));
      }
    }

    return $this->user;
  }

  /**
   * Provides a static cache for resources properties of the current Disk.
   *
   * @return array
   *   An array of properties arrays for corresponding paths by reference.
   */
  protected function &propertiesCache() {
    if (!isset(self::$propertiesCache[$this->getUser()])) {
      self::$propertiesCache[$this->user] = array();
    }

    return self::$propertiesCache[$this->user];
  }

  /**
   * Retrieves resources properties from static cache or from the service.
   *
   * @param string $path
   *   Path of the resource.
   *
   * @return array|null
   *   Properties of the resource if one exists, NULL otherwise.
   *
   * @throws \YandexDiskException
   *   If status code of response from the service is non-standard.
   */
  public function getProperties($path) {
    $properties = &$this->propertiesCache();

    if (!array_key_exists($path, $properties)) {
      $this->propfind($path)->execute();

      switch (self::$lastResponse->code) {
        case 200:
          $this->setProperties($path, self::$lastResponse->data);
          break;

        case 404:
          $properties[$path] = NULL;
          break;

        default:
          throw new YandexDiskException();
      }
    }

    return $properties[$path];
  }

  /**
   * Parses resources properties from a service response and caches them.
   *
   * @param string $path
   *   Path of the resource.
   * @param string $raw_xml
   *   XML string as returned from a service.
   *
   * @return array
   *   Array of properties arrays for each path returned in XML.
   */
  public function setProperties($path, $raw_xml) {
    $properties = &$this->propertiesCache();
    $return = array();

    $xml = new DOMDocument();
    $xml->loadXML($raw_xml);

    foreach ($xml->getElementsByTagName('response') as $i => $response) {
      $raw_properties = $response->getElementsByTagName('prop')->item(0);

      // Build an item's path.
      if ($i) {
        $item_name = $raw_properties
          ->getElementsByTagName('displayname')->item(0)->nodeValue;
        $item_path = rtrim($path, '/') . '/' . $item_name;
      }
      else {
        $item_path = $path;
      }

      foreach ($raw_properties->getElementsByTagName('*') as $property) {
        $properties[$item_path][$property->tagName] = trim($property->nodeValue);
      }

      $return[$item_path] = $properties[$item_path];
    }

    return $return;
  }

  /**
   * Checks whether path exists on Disk.
   *
   * @param string $path
   *   Path to check.
   *
   * @return bool
   *   TRUE if path exists, FALSE otherwise.
   */
  public function pathExists($path) {
    return (bool) $this->getProperties($path);
  }

  /**
   * Checks if path on Disk is a regular file.
   *
   * @param string $path
   *   Path to check.
   *
   * @return bool
   *   TRUE if the path exists and is a file, FALSE otherwise.
   */
  public function isFile($path) {
    if ($properties = $this->getProperties($path)) {
      return !isset($properties['d:collection']);
    }

    return FALSE;
  }

  /**
   * Checks if path on Disk is a directory.
   *
   * @param string $path
   *   Path to check.
   *
   * @return bool
   *   TRUE if the path exists and is a directory, FALSE otherwise.
   */
  public function isDir($path) {
    if ($properties = $this->getProperties($path)) {
      return isset($properties['d:collection']);
    }

    return FALSE;
  }

  /**
   * Helper method to read from stream.
   *
   * @param string $path
   *   Path to the file.
   * @param int $offset
   *   An offset from the start of the file.
   * @param int $length
   *   A number of bytes to return.
   *
   * @return string
   *   Returns the extracted part of the file.
   *
   * @throws \YandexDiskException
   *   If there was a problem to read the file.
   *
   * @see \YandexDiskApiWebdav::get()
   */
  public function read($path, $offset, $length) {
    $this->get($path, $offset, $offset + $length - 1)->execute();

    switch (self::$lastResponse->code) {
      case 200:
      case 206:
        return self::$lastResponse->data;

      default:
        throw new YandexDiskException();
    }
  }

  /**
   * Helper method to write to stream.
   *
   * @param string $path
   *   Path to the file.
   * @param string $data
   *   Data to be saved to the file.
   * @param string $content_type
   *   (optional) Data type.
   *
   * @return true
   *   If the file was created.
   *
   * @throws \YandexDiskException
   *   If there was a problem to write the file.
   *
   * @see \YandexDiskApiWebdav::put()
   */
  public function write($path, $data, $content_type = 'application/binary') {
    $this->put($path, $data, $content_type)->execute();

    switch (self::$lastResponse->code) {
      case 100:
      case 201:
        return TRUE;

      default:
        throw new YandexDiskException();
    }
  }

  /**
   * Retrieves a directory contents.
   *
   * Set offset and amount parameters to get a paginated list of elements. It is
   * assumed that the items are arranged alphabetically, and any nested
   * directories are listed before the files. The response shows the $amount
   * number of items without the requested directory itself.
   *
   * @param string $path
   *   Path to the directory.
   * @param int $offset
   *   (optional) Number of items to skip.
   * @param int $amount
   *   (optional) Desired number of items to return.
   *
   * @return string[]
   *   Array with names of directories and files on success.
   *
   * @throws \YandexDiskException
   *   If there was a problem getting a directory contents, or if the $path is
   *   not a directory.
   */
  public function scanDir($path, $offset = 0, $amount = 0) {
    $this->propfind($path, 1);
    if ($amount) {
      $this->setPathQuery(array('offset' => $offset, 'amount' => $amount));
    }
    $this->execute();

    switch (self::$lastResponse->code) {
      case 200:
        $properties = $this->setProperties($path, self::$lastResponse->data);

        if (!$this->isDir($path)) {
          throw new YandexDiskException(t('Resource is not a directory.'));
        }

        $list = array();

        foreach ($properties as $item_path => $item) {
          // Skip the requested directory itself.
          if ($item_path != $path) {
            $list[] = $item['d:displayname'];
          }
        }

        return $list;

      default:
        throw new YandexDiskException();
    }
  }

  /**
   * Helper method to get an image preview.
   *
   * @param string $path
   *   Path to the image.
   * @param string|int $size
   *   There are several ways to set the preview size:
   *   - T-shirt size. Supported values:
   *     - 'XXXS': 50 pixels on each side (square).
   *     - 'XXS': 75 pixels on each side (square).
   *     - 'XS': 100 pixels on each side (square).
   *     - 'S': 150 pixels wide, preserves original aspect ratio.
   *     - 'M': 300 pixels wide, preserves original aspect ratio.
   *     - 'L': 500 pixels wide, preserves original aspect ratio.
   *     - 'XL': 800 pixels wide, preserves original aspect ratio.
   *     - 'XXL': 1024 pixels wide, preserves original aspect ratio.
   *     - 'XXXL': 1280 pixels wide, preserves original aspect ratio.
   *   - An integer. Yandex.Disk returns a preview with this width. If the
   *     specified width is more than 100 pixels, the preview preserves the
   *     aspect ratio of the original image. Otherwise, the preview is
   *     additionally modified: the largest possible square section is taken
   *     from the center of the image to scale to the specified width.
   *   - Exact dimensions, such as '128x256'. Yandex.Disk returns a preview with
   *     the specified dimensions. The largest possible section with the
   *     specified width/height ratio is taken from the center of the original
   *     image (in the example, the ratio is 128/256 or 1/2). Then this section
   *     of the image is scaled to the specified dimensions.
   *   - Exact width or height, such as '128x' or 'x256'. Yandex.Disk returns
   *     a preview with the specified width or height that preserves the aspect
   *     ratio of the original image.
   *
   * @return string
   *   Binary image data on success.
   *
   * @throws \YandexDiskException
   *   If there was a problem getting a preview.
   */
  public function imagePreview($path, $size) {
    $this->get($path)->setPathQuery(array('preview' => NULL, 'size' => $size));
    $this->execute();

    switch (self::$lastResponse->code) {
      case 200:
        return self::$lastResponse->data;

      default:
        throw new YandexDiskException();
    }
  }

  /**
   * Publishes file or directory.
   *
   * @param string $path
   *   Path to the file or directory.
   *
   * @return string
   *   Public URL on success.
   *
   * @throws \YandexDiskException
   *   If there was a problem publishing the resource.
   */
  public function publish($path) {
    $xml = new SimpleXMLElement('<propertyupdate/>');
    $xml['xmlns'] = 'DAV:';
    $xml->set->prop->public_url = 1;
    $xml->set->prop->public_url['xmlns'] = 'urn:yandex:disk:meta';

    $this->proppatch($path, $xml->asXML())->execute();

    switch (self::$lastResponse->code) {
      case 200:
        $properties = $this->setProperties($path, self::$lastResponse->data);
        return $properties[$path]['public_url'];

      default:
        throw new YandexDiskException();
    }
  }

  /**
   * Unpublishes file or directory.
   *
   * @param string $path
   *   Path to the file or directory.
   *
   * @return bool
   *   TRUE on success.
   *
   * @throws \YandexDiskException
   *   If there was a problem unpublishing the resource.
   */
  public function unpublish($path) {
    $xml = new SimpleXMLElement('<propertyupdate/>');
    $xml['xmlns'] = 'DAV:';
    $xml->remove->prop->public_url['xmlns'] = 'urn:yandex:disk:meta';

    $this->proppatch($path, $xml->asXML())->execute();

    switch (self::$lastResponse->code) {
      case 200:
        $properties = $this->setProperties($path, self::$lastResponse->data);
        return empty($properties[$path]['public_url']);

      default:
        throw new YandexDiskException();
    }
  }

  /**
   * Returns a public URL of the file or directory.
   *
   * @param string $path
   *   Path to the file or directory.
   *
   * @return string
   *   Public URL if there is one, or an empty string if resource is not
   *   published.
   *
   * @throws \YandexDiskException
   *   If there was a problem checking the public URL.
   */
  public function publicUrl($path) {
    // 'Public_url' is not available with other properties. Propfind it now.
    $xml = new SimpleXMLElement('<propfind/>');
    $xml['xmlns'] = 'DAV:';
    $xml->prop->public_url['xmlns'] = 'urn:yandex:disk:meta';

    $this->propfind($path, 0, $xml->asXML())->execute();

    switch (self::$lastResponse->code) {
      case 200:
        $properties = $this->setProperties($path, self::$lastResponse->data);
        return $properties[$path]['public_url'];

      default:
        throw new YandexDiskException();
    }
  }

  /**
   * Returns an amount of free and/or used space on Disk in bytes.
   *
   * @param string $type
   *   (optional) Type of space amount to return:
   *   - 'used'.
   *   - 'available'.
   *
   * @return string|string[]
   *   If $type specified, then a number is returned. Otherwise, an array of
   *   numbers.
   *
   * @throws \YandexDiskException
   *   If there was a problem checking an amount of Disk space.
   */
  public function quota($type = NULL) {
    $xml = new SimpleXMLElement('<propfind/>');
    $xml['xmlns'] = 'DAV:';
    $prop = $xml->addChild('prop');
    $prop->addChild('quota-available-bytes');
    $prop->addChild('quota-used-bytes');

    $this->propfind('/', 0, $xml->asXML())->execute();

    switch (self::$lastResponse->code) {
      case 200:
        $properties = $this->setProperties('/', self::$lastResponse->data);

        if ($type) {
          return $properties['/']['d:quota-' . $type . '-bytes'];
        }

        return $properties['/'];

      default:
        throw new YandexDiskException();
    }
  }
}

/**
 * Exception subclass to use in work with YandexDiskApiWebdav.
 */
class YandexDiskException extends Exception {

  /**
   * Result of a request made prior to exception was thrown.
   *
   * @var mixed
   */
  protected $response;

  /**
   * Constructs the exception.
   *
   * @param string $message
   *   (optional) The Exception message to throw. Overrides any message in
   *   service response.
   */
  public function __construct($message = NULL) {
    $response = YandexDiskApiWebdav::$lastResponse;
    $this->response = $response;

    // Get message from last service response if it isn't set explicitly.
    if (!isset($message) && isset($response)) {
      if (isset($response->error) && $response->error !== '') {
        $message = $response->error;
      }
      else {
        $message = $response->status_message;
      }

      $message = check_plain($message);
    }

    parent::__construct($message, (int) @$response->code);
  }

  /**
   * Returns last service response.
   *
   * @return mixed
   *   Last service response.
   */
  public function getServiceResponse() {
    return $this->response;
  }
}
