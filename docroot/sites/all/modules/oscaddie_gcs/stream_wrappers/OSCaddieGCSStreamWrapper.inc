<?php

/**
 * @file
 * Implements DrupalStreamWrapperInterface.
 */

class OSCaddieGCSStreamWrapper implements DrupalStreamWrapperInterface {

  /**
   * Google Cloud Store connection object.
   */
  protected $gcs = NULL;

  /**
   * Google Cloud Store client object.
   */
  protected $client = NULL;

  /**
   * Google Cloud Store credential object.
   */
  protected $cred = NULL;

  /**
   * Google Cloud Store API domain.
   */
  protected $domain = OSCADDIE_GCS_API_DOMAIN;

  /**
   * String Instance URI referenced as "gs://bucket/".
   */
  protected $uri;

  /**
   * String Google Cloud Storage client ID.
   */
  protected $clientId;

  /**
   * String Google Cloud Storage service account name.
   */
  protected $serviceAccountName;

  /**
   * String Google Cloud Store bucket name.
   */
  protected $bucket;

  /**
   * String Google Cloud Store folder name.
   */
  protected $folder;

  /**
   * String Google Cloud Store allowed key.
   */
  protected $key;

  /**
   * String Google Cloud Store allowed keyFilePath.
   */
  protected $keyFilePath;

  /**
   * String Google Cloud Store allowed pemFile.
   */
  protected $pemFile;

  /**
   * String Google Cloud Store allowed pemFid.
   */
  protected $pemFid;

  /**
   * Int A buffer for reading/wrting.
   */
  protected $object = NULL;

  /**
   * Int current read/write position.
   */
  protected $position = 0;

  /**
   * Int total size of the object.
   */
  protected $objectSize = 0;

  /**
   * String buffer.
   */
  protected $buffer = NULL;

  /**
   * Boolean Whether $buffer is active as a write buffer.
   */
  protected $bufferWrite = FALSE;

  /**
   * Int Buffer length.
   */
  protected $bufferLength = 0;

  /**
   * Bool HTTPS flag.
   */
  protected $https = FALSE;

  /**
   * Object constructor.
   */
  public function __construct() {
    if (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] !== FALSE) {
      $this->https = TRUE;
    }

    $this->clientId = variable_get('oscaddie_gcs_client_id');
    $this->serviceAccountName = variable_get('oscaddie_gcs_service_account_name');
    $this->bucket = variable_get('oscaddie_gcs_bucket_name');
    $this->folder = variable_get('oscaddie_gcs_folder');
    $this->key = variable_get('oscaddie_gcs_public_key');
    $this->keyFilePath = variable_get('oscaddie_gcs_public_key_file_path');
    $this->pemFile = variable_get('oscaddie_gcs_pem_file_path');
    $this->pemFid = variable_get('oscaddie_gcs_pem_fid');
    $this->getGCS();
  }

  /**
   * Sets the stream resource URI.
   *
   * URIs are formatted as "gs://bucket/key"
   *
   * @param string $uri
   *   The URI of the file.
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Returns the stream resource URI.
   *
   * URIs are formatted as "gs://bucket/key"
   *
   * @return string
   *   Returns the current URI of the instance.
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * Returns a web accessible URL for the resource.
   *
   * @return string
   *   Returns a string containing a web accessible URL for the resource.
   */
  public function getExternalUrl() {
    $this->callConstructor();
    // Image styles support
    // Delivers the first request to an image from the private file system
    // otherwise it returns an external URL to an image that has not been
    // created yet.
    $path = explode('/', $this->getLocalPath());
    if ($path[0] === 'styles') {
      $target = $this->getTarget($this->uri);
      if ($target) {
        array_shift($path);
        return url('system/files/styles/' . implode('/', $path), array('absolute' => TRUE));
      }
    }

    $url = '';
    $local_path = $this->getLocalPath();
    if ($local_path) {
      try {
        $url = $this->generateSignedURL($local_path);
      }
      catch (Exception $e) {
        watchdog_exception('oscaddie_gcs', $e, 'OSCaddieGCSStreamWrapper::getExternalUrl');
      }
    }
    return $url;
  }

  /**
   * Determine a file's media type.
   *
   * Uses Drupal's mimetype mappings. Returns 'application/octet-stream' if
   * no match is found.
   *
   * @return string
   *   Returns a string representing the file's MIME type
   */
  public static function getMimeType($uri, $mapping = NULL) {
    // Load the default file map.
    if (!isset($mapping)) {
      include_once DRUPAL_ROOT . '/includes/file.mimetypes.inc';
      $mapping = file_mimetype_mapping();
    }

    $extension = '';
    $file_parts = explode('.', basename($uri));

    // Remove the first part: a full filename should not match an extension.
    array_shift($file_parts);
    while ($additional_part = array_pop($file_parts)) {
      $extension = strtolower($additional_part . ($extension ? '.' . $extension : ''));
      if (isset($mapping['extensions'][$extension])) {
        return $mapping['mimetypes'][$mapping['extensions'][$extension]];
      }
    }

    return 'application/octet-stream';
  }

  /**
   * Changes permissions of the resource.
   *
   * This doesn't do anything for the moment so just returns TRUE;
   *
   * @param int $mode
   *   Integer value for the permissions. Consult PHP chmod() documentation
   *   for more information.
   *
   * @return bool
   *   Returns TRUE.
   */
  public function chmod($mode) {
    return TRUE;
  }

  /**
   * Returns canonical, absolute path of the resource.
   *
   * @return bool
   *   Returns FALSE as this wrapper does not provide an implementation.
   */
  public function realpath() {
    return $this->getLocalPath();
  }

  /**
   * Support for fopen(), file_get_contents(), file_put_contents() etc.
   *
   * @param string $uri
   *   A string containing the URI to the file to open.
   * @param string $mode
   *   The file mode ("r", "wb" etc.).
   * @param int $options
   *   A bit mask of STREAM_USE_PATH and STREAM_REPORT_ERRORS.
   * @param string $opened_path
   *   A string containing the path actually opened.
   *
   * @return bool
   *   Returns TRUE if file was opened successfully.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-open.php
   */
  public function stream_open($uri, $mode, $options, &$opened_path) {
    $this->callConstructor();
    $this->uri = $uri;

    // If this stream is being opened for writing, clear the object buffer.
    // Return true as we'll create the object on fflush call.
    if (strpbrk($mode, 'wax')) {
      $this->clearBuffer();
      $this->bufferWrite = TRUE;
      return TRUE;
    }

    $target = $this->getTarget($this->uri);
    if ($target) {
      try {
        $response = $this->gcs->objects->get($this->bucket, $this->folder . '/' . $target);
        if ($response) {
          $this->clearBuffer();
          $this->bufferWrite = FALSE;
          $this->objectSize = $response['size'];
          return TRUE;
        }
      }
      catch (Exception $e) {
        watchdog_exception('oscaddie_gcs', $e, 'OSCaddieGCSStreamWrapper::stream_open');
      }
    }

    return FALSE;
  }

  /**
   * Support for fclose().
   *
   * Clears the object buffer and returns TRUE
   *
   * @return bool
   *   TRUE
   *
   * @see http://php.net/manual/en/streamwrapper.stream-close.php
   */
  public function stream_close() {
    $this->clearBuffer();
    return TRUE;
  }

  /**
   * Support for flock().
   *
   * @param string $operation
   *   One of the following:
   *   - LOCK_SH to acquire a shared lock (reader).
   *   - LOCK_EX to acquire an exclusive lock (writer).
   *   - LOCK_UN to release a lock (shared or exclusive).
   *   - LOCK_NB if you don't want flock() to block while locking (not
   *     supported on Windows).
   *
   * @return bool
   *   returns TRUE if lock was successful
   *
   * @see http://php.net/manual/en/streamwrapper.stream-lock.php
   */
  public function stream_lock($operation) {
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
    if (!$this->object) {
      // If the object is empty, get the object from rackspace.
      $target = $this->getTarget($this->uri);
      if ($target) {
        try {
          $service = $this->gcs->objects->get($this->bucket, $this->folder . '/' . $target);
          if ($service) {
            $download_url = 'https://' . $this->domain . '/' . $this->bucket . '/' . $this->folder . '/' . $target;
            $request = new Google_Http_Request($download_url, 'GET');
            $signed_request = $this->client->getAuth()->sign($request);
            $response = $this->client->getIo()->makeRequest($signed_request);
            if ($response) {
              $this->object = $response->getResponseBody();
            }
          }
        }
        catch (Exception $e) {
          watchdog_exception('gooogle_cdn', $e, 'OSCaddieGCSStreamWrapper::stream_read');
        }
      }
    }
    $data = substr($this->object, $this->position, $count);
    $this->position += $count;
    return $data;
  }

  /**
   * Support for fwrite(), file_put_contents() etc.
   *
   * @param string $data
   *   The string to be written.
   *
   * @return int
   *   The number of bytes written.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-write.php
   */
  public function stream_write($data) {
    $data_length = strlen($data);
    $this->buffer .= $data;
    $this->bufferLength += $data_length;
    $this->position += $data_length;

    return $data_length;
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
    if (!$this->uri) {
      return TRUE;
    }
    return ($this->position >= $this->objectSize);
  }

  /**
   * Support for fseek().
   *
   * @param int $offset
   *   The byte offset to got to.
   * @param string $whence
   *   SEEK_SET, SEEK_CUR, or SEEK_END.
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.stream-seek.php
   */
  public function stream_seek($offset, $whence) {
    switch ($whence) {
      case SEEK_CUR:
        // Set position to current location plus $offset.
        $new_position = $this->position + $offset;
        break;

      case SEEK_END:
        // Set position to eof plus $offset.
        $new_position = $this->objectSize + $offset;
        break;

      case SEEK_SET:
      default:
        // Set position equal to $offset.
        $new_position = $offset;
        break;
    }

    $ret = ($new_position >= 0 && $new_position <= $this->objectSize);
    if ($ret) {
      $this->position = $new_position;
    }
    return $ret;
  }

  /**
   * Support for fflush(). Flush current cached stream data to storage.
   *
   * @return bool
   *   TRUE if data was successfully stored (or there was no data to store).
   *
   * @see http://php.net/manual/en/streamwrapper.stream-flush.php
   */
  public function stream_flush() {
    if ($this->bufferWrite) {
      try {
        $file = new Google_Service_Storage_StorageObject();
        $local_path = $this->getLocalPath($this->uri);
        $file->setName($this->folder . '/' . $local_path);
        $post_body = array(
          'mimeType' => OSCaddieGCSStreamWrapper::getMimeType($this->uri),
          'data' => $this->buffer,
        );
        $chunk_size_bytes = 1 * 1024 * 1024;
        $this->client->setDefer(TRUE);
        $request = $this->gcs->objects->insert($this->bucket, $file, $post_body);
        $media = new Google_Http_MediaFileUpload(
          $this->client,
          $request,
          OSCaddieGCSStreamWrapper::getMimeType($this->uri),
          NULL,
          TRUE,
          $chunk_size_bytes
        );
        $length = strlen($this->buffer);
        $media->setFileSize($length);

        // Send the content in chunks.
        $upload_result = FALSE;
        $pos = 0;
        $length = strlen($this->buffer);
        while (!$upload_result && $pos < $length) {
          $chunk = substr($this->buffer, $pos, $chunk_size_bytes);
          $upload_result = $media->nextChunk($chunk);
          $pos += $chunk_size_bytes;
        }
        if ($upload_result) {
          return TRUE;
        }
      }
      catch (Exception $e) {
        watchdog_exception('oscaddie_gcs', $e, 'OSCaddieGCSStreamWrapper::stream_flush');
      }
    }
    $this->clearBuffer();
    return FALSE;
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
    return $this->position;
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
    return $this->_stat();
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
    $this->callConstructor();
    $target = $this->getLocalPath($uri);
    if ($target) {
      try {
        $response = $this->gcs->objects->delete($this->bucket, $this->folder . '/' . $target);
        if ($response == NULL) {
          return TRUE;
        }
      }
      catch (Exceception $e) {
        watchdog_exception('oscaddie_gcs', $e, 'OSCaddieGCSStreamWrapper::unlink');
      }
    }
    return FALSE;
  }

  /**
   * Support for rename().
   *
   * If $to_uri exists, this file will be overwritten. This behavior is
   * identical to the PHP rename() function.
   *
   * @param string $from_uri
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
    $this->callConstructor();
    $from = $this->getLocalPath($from_uri);
    $to = $this->getLocalPath($to_uri);

    if ($from && $to) {
      try {
        $response = $this->gcs->objects->insert($this->bucket, $this->folder . '/' . $to);
        if ($response) {
          $deleted = $this->unlink($from_uri);
          if ($deleted) {
            return TRUE;
          }
        }
      }
      catch (Exception $e) {
        watchdog_exception('oscaddie_gcs', $e, 'OSCaddieGCSStreamWrapper::rename');
      }
    }
    return FALSE;
  }

  /**
   * Returns the local writable target of the resource within the stream.
   *
   * This function should be used in place of calls to realpath() or similar
   * functions when attempting to determine the location of a file. While
   * functions like realpath() may return the location of a read-only file, this
   * method may return a URI or path suitable for writing that is completely
   * separate from the URI used for reading.
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

    // Remove erroneous leading or trailing forward-slashes and backslashes.
    return trim($target, '\/');
  }

  /**
   * Support for mkdir().
   *
   * Google Cloud Storage is not a tree structure, so folder's
   * do not exist.
   *
   * @param string $uri
   *   A string containing the URI to the directory to create.
   * @param int $mode
   *   Permission flags - see mkdir().
   * @param int $options
   *   A bit mask of STREAM_REPORT_ERRORS and STREAM_MKDIR_RECURSIVE.
   *
   * @return bool
   *   TRUE if directory was successfully created.
   *
   * @see http://php.net/manual/en/streamwrapper.mkdir.php
   */
  public function mkdir($uri, $mode, $options) {
    // Folders do not exist.
    return TRUE;
  }

  /**
   * Support for rmdir().
   *
   * @param string $uri
   *   A string containing the URI to the directory to delete.
   * @param int $options
   *   A bit mask of STREAM_REPORT_ERRORS.
   *
   * @return bool
   *   TRUE if directory was successfully removed.
   * @see http://php.net/manual/en/streamwrapper.rmdir.php
   */
  public function rmdir($uri, $options) {
    return TRUE;
  }


  /**
   * Support for stat().
   *
   * @param string $uri
   *   A string containing the URI to get information about.
   * @param int $flags
   *   A bit mask of STREAM_URL_STAT_LINK and STREAM_URL_STAT_QUIET.
   *
   * @return array
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   *
   * @see http://php.net/manual/en/streamwrapper.url-stat.php
   */
  public function url_stat($uri, $flags) {
    $this->callConstructor();
    return $this->_stat($uri);
  }

  /**
   * Gets the name of the directory from a given path.
   *
   * This method is usually accessed through drupal_dirname(), which wraps
   * around the normal PHP dirname() function, which does not support stream
   * wrappers.
   *
   * @param string $uri
   *   An optional URI.
   *
   * @return string
   *   A string containing the directory name, or FALSE if not applicable.
   *
   * @see drupal_dirname()
   */
  public function dirname($uri = NULL) {
    list($scheme, $target) = explode('://', $uri, 2);
    $target  = $this->getTarget($uri);
    $dirname = dirname($target);

    if ($dirname === '.') {
      $dirname = '';
    }

    return $scheme . '://' . $dirname;
  }

  /**
   * Support for opendir().
   *
   * There are no directories in Google Cloud Storage.
   *
   * @param string $uri
   *   A string containing the URI to the directory to open.
   * @param array $options
   *   Unknown (parameter is not documented in PHP Manual).
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-opendir.php
   */
  public function dir_opendir($uri, $options) {
    return TRUE;
  }

  /**
   * Support for readdir().
   *
   * There are no directories in Google Cloud Storage.
   *
   * @return string|bool
   *   The next filename, or FALSE if there are no more files in the directory.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-readdir.php
   */
  public function dir_readdir() {
    return FALSE;
  }

  /**
   * Support for rewinddir().
   *
   * There are no directories in Google Cloud Storage.
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-rewinddir.php
   */
  public function dir_rewinddir() {
    return TRUE;
  }

  /**
   * Support for closedir().
   *
   * There are no directories in Google Cloud Storage.
   *
   * @return bool
   *   TRUE on success.
   *
   * @see http://php.net/manual/en/streamwrapper.dir-closedir.php
   */
  public function dir_closedir() {
    return TRUE;
  }

  /**
   * Return the local filesystem path.
   *
   * @param string $uri
   *   Optional URI, supplied when doing a move or rename.
   */
  protected function getLocalPath($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    $path  = str_replace('gs://', '', $uri);
    $path = trim($path, '/');
    return $path;
  }

  /**
   * Flush the object buffers.
   */
  protected function clearBuffer() {
    $this->position = 0;
    $this->objectSize = 0;
    $this->buffer = NULL;
    $this->bufferWrite = FALSE;
    $this->bufferLength = 0;
    $this->object = NULL;
  }

  /**
   * Get the Google Cloud Storage connection.
   */
  protected function getGCS() {
    if ($this->gcs == NULL) {
      if (!libraries_load('google-api-php-client')) {
        drupal_set_message(t('Unable to load the Google API PHP library. Please check you have installed the library correctly.') . 'error');
      }
      elseif (!class_exists('Google_Client')) {
        watchdog('osCaddie GCS', 'Google Client class not defined.');
      }
      elseif (empty($this->bucket)) {
        watchdog('osCaddie GCS', 'Bucket name not configured.');
      }
      else {
        try {
          $client = new Google_Client();
          // Replace this with your application name.
          $client->setApplicationName($this->serviceAccountName);
          // Replace this with the service you are using.
          $service = new Google_Service_Storage($client);
          // This file location should point to the private key file.
          $key = file_get_contents($this->keyFilePath);
          $cred = new Google_Auth_AssertionCredentials(
          // Replace this with the email address from the client.
            $this->serviceAccountName,
            // Replace this with the scopes you are requesting.
            array('https://www.googleapis.com/auth/devstorage.full_control'),
            $key
          );

          $client->setAssertionCredentials($cred);
          if ($client->getAuth()->isAccessTokenExpired()) {
            $client->getAuth()->refreshTokenWithAssertion($cred);
          }
          $token = $client->getAccessToken();
          $client->setAccessToken($token);
          // Store services for later use.
          $this->gcs = $service;
          $this->client = $client;
          $this->cred = $cred;
        }
        catch (Exception $e) {
          watchdog_exception('oscaddie_gcs', $e, 'OSCaddieGCSStreamWrapper::getGCS');
        }
      }
    }
  }

  /**
   * Creates a signed url for the content.
   *
   * @param string $file_path
   *   The entire filepath to the object.
   *
   * @return null|string
   *   A signed URL or NULL.
   */
  protected function generateSignedURL($file_path) {
    // This policy is defined in
    // https://developers.google.com/storage/docs/accesscontrol#Signed-URLs.
    $expiry = time() + variable_get('oscaddie_gcs_signed_url_expiry');
    $file_info = explode('/', $file_path);
    $file_name = rawurlencode(array_pop($file_info));
    $file_path = implode('/', $file_info) . '/' . $file_name;
    $string_policy = "GET\n\n\n" . $expiry . "\n/" . $this->bucket . '/' . $this->folder . '/' . $file_path;

    // This is a PEM version of the pks12 key generated by google.
    $fp = fopen($this->pemFile, 'r');
    $private_key = fread($fp, 8192);
    fclose($fp);
    $pkeyid = openssl_get_privatekey($private_key, OSCADDIE_GCS_PROVIDED_KEY);
    if (openssl_sign($string_policy, $signature, $pkeyid, 'sha256')) {
      $signature = urlencode(base64_encode($signature));
    }

    $url = NULL;
    if ($signature) {
      $secure = 'http://';
      if ($this->https) {
        $secure = 'https://';
      }
      $url = $secure . $this->bucket . '.' . $this->domain . '/' . $this->folder . '/' . $file_path . '?GoogleAccessId=' . $this->serviceAccountName . '&Expires=' . $expiry . '&Signature=' . $signature;
    }

    return $url;
  }

  /**
   * Get file status.
   *
   * @param string $uri
   *   The uri to the file.
   *
   * @return array
   *   An array with file status, or FALSE in case of an error - see fstat()
   *   for a description of this array.
   */
  protected function _stat($uri = NULL) {
    if ($this->client == NULL) {
      return FALSE;
    }
    if (!isset($uri)) {
      $uri = $this->uri;
    }
    $this->uri = $uri;
    $target = $this->getTarget($uri);
    if ($target) {
      try {
        // Determine type: file or folder.
        // If it has no extension, I assume a folder.
        $path_info = pathinfo($this->uri);
        if (!isset($path_info['extension'])) {
          $mode = 0040000;
          $size = 1;
          $mode |= 0777;
          $stat = $this->createFileMode($mode, $size);
          return $stat;
        }

        // Check to see if the file exists.
        $response = $this->gcs->objects->get($this->bucket, $this->folder . '/' . $target);
        if ($response && isset($response->size) && $response->size != NULL) {
          $mode = 0100000;
          $size = $response->size;
          $mode |= 0777;
          $stat = $this->createFileMode($mode, $size);
          return $stat;
        }
      }
      catch (Exception $e) {
      }
    }
    return FALSE;
  }

  /**
   * Creates the fstat array.
   *
   * @param int $mode
   *   The file mode.
   * @param int $size
   *   The file size.
   *
   * @return array
   *   An array with fstat info populated or empty.
   */
  protected function createFileMode($mode, $size) {
    $stat = array();

    $stat[0] = $stat['dev'] = 0;
    $stat[1] = $stat['ino'] = 0;
    $stat[2] = $stat['mode'] = $mode;
    $stat[3] = $stat['nlink'] = 0;
    $stat[4] = $stat['uid'] = 0;
    $stat[5] = $stat['gid'] = 0;
    $stat[6] = $stat['rdev'] = 0;
    $stat[7] = $stat['size'] = 0;
    $stat[8] = $stat['atime'] = 0;
    $stat[9] = $stat['mtime'] = 0;
    $stat[10] = $stat['ctime'] = 0;
    $stat[11] = $stat['blksize'] = 0;
    $stat[12] = $stat['blocks'] = 0;
    $stat[4] = $stat['uid'] = 0;
    $stat[7] = $stat['size'] = $size;
    $stat[8] = $stat['atime'] = 0;
    $stat[9] = $stat['mtime'] = 0;
    $stat[10] = $stat['ctime'] = 0;

    return $stat;
  }

  /**
   * Assert that the constructor has been called, call it if not.
   *
   * Due to PHP bug #40459, the constructor of this class isn't always called
   * for some of the methods. This private method calls the constructor if
   * it hasn't been called before.
   *
   * @see https://bugs.php.net/bug.php?id=40459
   */
  protected function callConstructor() {
    if ($this->gcs == NULL) {
      $this->__construct();
    }

    if (isset($this->client) && $this->client->getAuth()->isAccessTokenExpired()) {
      $this->client->getAuth()->refreshTokenWithAssertion($this->cred);
    }
  }
}
