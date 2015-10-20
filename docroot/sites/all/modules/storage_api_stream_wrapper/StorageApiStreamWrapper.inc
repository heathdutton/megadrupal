<?php

/**
 * @file
 * Drupal stream wrapper implementation for  Storage API Stream Wrapper.
 *
 * Implements DrupalStreamWrapperInterface to provide an Storage API wrapper with
 * the storage-api:// prefix.
 */

class StorageApiStreamWrapper implements DrupalStreamWrapperInterface {

  protected $uri;
  protected $mode;
  protected $temp_uri;
  protected $fp;

  /**
   * {@inheritdoc}
   */
  public function chmod($mode) {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function dirname($uri = NULL) {
    $scheme = file_uri_scheme($uri);
    $target = file_uri_target($uri);

    $dirname = dirname($target);
    if ($dirname == '.') {
      $dirname = '';
    }

    return $scheme . '://' . $dirname;
  }

  /**
   * {@inheritdoc}
   */
  public function getExternalUrl() {
    try {
      $url = storage_stream_wrapper_load($this->uri)->serveURL(TRUE);
    }
    catch (StorageException $e) {
      // Storage was not found. Maybe it's an image derivative that hasn't
      // been created yet.
      try {
        $url = storage_stream_wrapper_create_derivative_image($this->uri)->serveURL(TRUE);
      }
      catch (StorageException $e) {
        watchdog('storage_api_stream_wrapper', 'Exception in @function. (uri: @uri) (message: @message)', array('@uri' => $this->uri, '@function' => __FUNCTION__, '@message' => $e->getMessage()));
        //@see file_create_url()
        $url = FALSE;
      }
    }

    return $url;
  }

  /**
   * {@inheritdoc}
   */
  public static function getMimeType($uri, $mapping = NULL) {
    // This function may be called too early, before the file referenced
    // by $uri exists in its final destination.
    //
    // @see plupload_element_validate() for an example of this. It builds
    // the $uri for the supposed destination of the file that is being uploaded.
    // This $uri may coincide with the final destination, or even worst, it may
    // reference an already existing file (ie, if uploading foo.pdf and a
    // file by that name already exists. The new foo.pdf will be renamed to
    // foo_0.pdf, but that hasn't happened yet!).
    //
    // What Drupal core does, in DrupalLocalStreamWrapper::getMimeType() is to
    // map the mime type from the extension of the file in $uri. It actually
    // doesn't try to access the file.
    // So we can safely do the same here.
    return DrupalLocalStreamWrapper::getMimeType($uri, $mapping);
  }

  /**
   * {@inheritdoc}
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * {@inheritdoc}
   *
   * Return the actual path of a uri if using local file system container type.
   */
  public function realpath() {
    try {
      $storage = storage_stream_wrapper_load($this->uri);
      $container = $storage->selectContainerInstance();
      if ($container && get_class($container) == 'StorageFS') {
        return $storage->getUri();
      }
    }
    catch (StorageException $e) {
    }

    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * {@inheritdoc}
   */
  public function dir_closedir() {
    watchdog('storage_api_stream_wrapper', 'Operation not implemented @op()', array('@op' => __FUNCTION__), WATCHDOG_ERROR);
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function dir_opendir($uri, $options) {
    watchdog('storage_api_stream_wrapper', 'Operation not implemented @op()', array('@op' => __FUNCTION__), WATCHDOG_ERROR);
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function dir_readdir() {
    watchdog('storage_api_stream_wrapper', 'Operation not implemented @op()', array('@op' => __FUNCTION__), WATCHDOG_ERROR);
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function dir_rewinddir() {
    watchdog('storage_api_stream_wrapper', 'Operation not implemented @op()', array('@op' => __FUNCTION__), WATCHDOG_ERROR);
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function mkdir($uri, $mode, $options) {
    watchdog('storage_api_stream_wrapper', 'Operation not implemented @op()', array('@op' => __FUNCTION__), WATCHDOG_ERROR);
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function rename($from_uri, $to_uri) {
    watchdog('storage_api_stream_wrapper', 'Operation not implemented @op(@from, @to)', array('@op' => __FUNCTION__, '@from' => $from, '@to' => $to), WATCHDOG_ERROR);
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function rmdir($uri, $options) {
    watchdog('storage_api_stream_wrapper', 'Operation not implemented @op()', array('@op' => __FUNCTION__), WATCHDOG_ERROR);
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function stream_close() {
    fclose($this->fp);

    if ($this->mode == 'w') {
      $options = array(
        'source_uri' => $this->temp_uri,
        'filename' => file_uri_target($this->uri),
      );

      try {
        $storage = storage_stream_wrapper_selector()->storageAdd($options);

        db_insert('storage_stream_wrapper')
          ->fields(array(
            'storage_id' => $storage->storage_id,
            'uri' => $this->uri,
          ))
          ->execute();
      }
      catch (StorageException $e) {
        watchdog('storage_api_stream_wrapper', 'Exception in @function. (uri: @uri) (message: @message)', array('@uri' => $this->uri, '@function' => __FUNCTION__, '@message' => $e->getMessage()));
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function stream_eof() {
    return feof($this->fp);
  }

  /**
   * {@inheritdoc}
   */
  public function stream_flush() {
    return fflush($this->fp);
  }

  /**
   * {@inheritdoc}
   */
  public function stream_lock($operation) {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function stream_open($uri, $mode, $options, &$opened_url) {
    $this->uri = $uri;
    $this->mode = $mode[0];

    try {
      switch ($this->mode) {
        case 'w':
          $this->temp_uri = new StorageTempURI();

          if (!$this->fp = fopen($this->temp_uri, 'wb')) {
            throw new StorageException();
          }

          break;

        case 'r':
          $this->temp_uri = storage_stream_wrapper_load($uri)->getUri();

          if (!$this->fp = fopen($this->temp_uri, 'rb')) {
            throw new StorageException();
          }

          break;
      }
    }
    catch (StorageException $e) {
      watchdog('storage_api_stream_wrapper', 'Exception in @function. (uri: @uri) (message: @message)', array('@uri' => $this->uri, '@function' => __FUNCTION__, '@message' => $e->getMessage()));
      return FALSE;
    }

    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function stream_read($count) {
    return fread($this->fp, $count);
  }

  /**
   * {@inheritdoc}
   */
  public function stream_seek($offset, $whence) {
    // fseek() returns 0 on success and -1 on a failure.
    // stream_seek()   1 on success and  0 on a failure.
    return !fseek($this->fp, $offset, $whence);
  }

  /**
   * {@inheritdoc}
   */
  public function stream_stat() {
    return fstat($this->fp);
  }

  /**
   * {@inheritdoc}
   */
  public function stream_tell() {
    return ftell($this->fp);
  }

  /**
   * {@inheritdoc}
   */
  public function stream_write($data) {
    return fwrite($this->fp, $data);
  }

  /**
   * {@inheritdoc}
   */
  public function unlink($uri) {
    try {
      storage_stream_wrapper_load($uri)->remove();
    }
    catch (StorageException $e) {
      watchdog('storage_api_stream_wrapper', 'Exception in @function. (uri: @uri) (message: @message)', array('@uri' => $uri, '@function' => __FUNCTION__, '@message' => $e->getMessage()));
      return FALSE;
    }

    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function url_stat($uri, $flags) {
    // Initialize stat.
    $stat     = array();
    $stat[0]  = $stat['dev'] = 0;
    $stat[1]  = $stat['ino'] = 0;
    $stat[2]  = $stat['mode'] = 0;
    $stat[3]  = $stat['nlink'] = 0;
    $stat[4]  = $stat['uid'] = 0;
    $stat[5]  = $stat['gid'] = 0;
    $stat[6]  = $stat['rdev'] = 0;
    $stat[7]  = $stat['size'] = 0;
    $stat[8]  = $stat['atime'] = 0;
    $stat[9]  = $stat['mtime'] = 0;
    $stat[10] = $stat['ctime'] = 0;
    $stat[11] = $stat['blksize'] = -1;
    $stat[12] = $stat['blocks'] = -1;

    // If uri is a directory, set the mode and return.
    // Here we resort to heuristics to guess if the uri
    // is a directory. It'll fail for directories with a dot.
    // We don't have an alternative since not all containers
    // support directories - namely S3.
    $pathinfo = pathinfo($uri);
    if ($pathinfo['basename'] == $pathinfo['filename']) {
      $stat[2] = $stat['mode'] = 0040777;
      return $stat;
    }

    try {
      $storage = storage_stream_wrapper_load($uri);
    }
    catch (StorageException $e) {
      if (!($flags & STREAM_URL_STAT_QUIET)) {
        watchdog('storage_api_stream_wrapper', 'Exception in @function. (uri: @uri) (message: @message)', array('@uri' => $uri, '@function' => __FUNCTION__, '@message' => $e->getMessage()));
      }
      return FALSE;
    }

    $stat[2]  = $stat['mode'] = 0100666;
    $stat[7]  = $stat['size'] = $storage->size;
    $stat[8]  = $stat['atime'] = $storage->created;
    $stat[9]  = $stat['mtime'] = $storage->created;
    $stat[10] = $stat['ctime'] = $storage->created;

    return $stat;
  }
}

