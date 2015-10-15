<?php

/**
 * @file
 * Contains \MediaNetXStreamWrapper.
 */

/**
 * Create an instance of MediaNetXStreamWrapper.
 *
 * @code
 *  $netx = new MediaNetXStreamWrapper('netx://a/[asset-id]/s/[asset-size]/n/[asset-file-name]');
 * @endcode
 */
class MediaNetXStreamWrapper extends MediaReadOnlyStreamWrapper {

  /**
   * {@inheritdoc}
   */
  protected $parameters = array(
    // Asset ID.
    'a' => NULL,
    // @todo Category ID.
    'c' => NULL,
    // File size.
    's' => NULL,
    // File name.
    'n' => NULL,
  );

  /**
   * The string buffer to write.
   *
   * @var string
   */
  protected $tmpFile = '';

  /**
   * Flag that data needs flush.
   *
   * @var bool
   */
  protected $needFlush = FALSE;

  /**
   * Flag that URI is image style.
   *
   * @var bool
   */
  protected $imageStyle = FALSE;

  /**
   * Selected category to write a file.
   *
   * @var int
   */
  protected $categoryId = 0;

  /**
   * The file name to import.
   *
   * @var string
   */
  protected $fileName = '';

  /**
   * {@inheritdoc}
   *
   * Adds own mime types for provided content.
   */
  public static function getMimeType($uri, $mapping = NULL) {
    $mime_type = DrupalLocalStreamWrapper::getMimeType($uri, $mapping);
    $mime_parts = explode('/', $mime_type);
    switch ($mime_parts[0]) {
      case 'image':
        return 'image/netx';

      case 'video':
        return 'video/netx';

      case 'audio':
        return 'audio/netx';

    }
    return $mime_type;
  }

  /**
   * {@inheritdoc}
   */
  protected function getTarget($uri = NULL) {
    // @todo The usage is not clear, probably needs to implement and use.
    return FALSE;
  }

  /**
   * Returns a local image file path.
   *
   * Copies a file if needed.
   *
   * @return string|bool
   *   A local path, FALSE if error.
   */
  public function getLocalImagePath() {
    $local_file = $this->getAssetFilename();
    if (!file_exists($local_file)) {
      $dirname = drupal_dirname($local_file);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

      $repository = new NetXRepository();
      try {
        $asset_image = $repository->getImageFileContent($this->getAssetID());
      }
      catch (Exception $e) {
        // @todo add some kind of logging here.
        watchdog('netx', $e->getMessage(), array(), WATCHDOG_CRITICAL);
        return FALSE;
      }

      file_unmanaged_save_data($asset_image, $local_file, TRUE);
    }
    return $local_file;
  }

  /**
   * Returns a local preview file path.
   *
   * Copies a file if needed.
   *
   * @param string $path
   *   The remote path of the file preview.
   *
   * @return string
   *   A local path.
   */
  public function getLocalPreviewPath($path) {
    $local_file = file_default_scheme() . '://media-netx' . $path;
    if (!file_exists($local_file)) {
      $dirname = drupal_dirname($local_file);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

      $repository = new NetXRepository();
      // Copy preview image (jpg). Each asset has one.
      $asset_image = $repository->getFileContent($path);
      file_unmanaged_save_data($asset_image, $local_file, TRUE);
    }
    return $local_file;
  }

  /**
   * Calculates asset ID from uri.
   *
   * @return int
   *   Asset ID.
   */
  public function getAssetID() {
    $params = $this->get_parameters();
    return $params['a'];
  }

  /**
   * Builds local filename.
   *
   * @todo Implement scalable logic for sub-folders (use category from JSON).
   *
   * @return string
   *   A local file name.
   */
  protected function getAssetFilename() {
    $params = $this->get_parameters();
    $local_path = file_default_scheme() . '://media-netx/';
    return $local_path . $params['a'] . '/' . $params['n'];
  }

  /**
   * {@inheritdoc}
   */
  public function setUri($uri) {
    parent::setUri($uri);
    // Set canonical URI.
    if (($params = $this->get_parameters()) && isset($params['a']) && isset($params['n'])) {
      // Normalize only valid assets with file name.
      $this->uri = 'netx://' . implode('/', $this->get_parameters());
    }
  }

  /**
   * {@inheritdoc}
   */
  public function interpolateUrl() {
    if ($parameters = $this->get_parameters()) {
      // Redirect to NetX CDN via REST.
      $repo = new NetXRepository();
      return $repo->getRemoteOriginal(
        $parameters['a'],
        $parameters['n'],
        $this->getMimeType($parameters['n'])
      );
    }
  }

  /**
   * Checks if a particular path is a directory.
   *
   * @param string $path
   *   The path to check
   *
   * @return bool
   *   TRUE when target category not found.
   */
  protected function isDirectory($path) {
    // @todo Find a better way to extract last category name.
    $name = drupal_basename($path);
    if (empty($name)) {
      $repo = new NetXRepository();
      $name = $repo->getDefaultCategory();
    }
    $categories = netx_get_categories();
    return (bool) _netx_find_category($name, $categories['subCategories']);
  }

  /**
   * {@inheritdoc}
   */
  public function mkdir($uri, $mode, $options) {
    // @todo Implement properly.
    return TRUE;
  }

  // @codingStandardsIgnoreStart
  // @todo Code sniffer needs lowerCamel format.
  /**
   * {@inheritdoc}
   */
  public function url_stat($path, $flags) {
    // Sane default values.
    $defaults = array(
      'dev' => 0,
      'ino' => 0,
      'mode' => 0666,
      'nlink' => 0,
      'uid' => 0,
      'gid' => 0,
      'rdev' => 0,
      'size' => 0,
      'atime' => 0,
      'mtime' => 0,
      'ctime' => 0,
      'blksize' => 0,
      'blocks' => 0,
    );

    if ($path == 'netx://') {
      // Suppose directory read only to check online in self::chmod().
      $defaults['mode'] = 040111;
      return array_values($defaults) + $defaults;
    }

    // Try to retrieve asset metadata from URI.
    $this->setUri($path);
    $params = $this->get_parameters();
    if (isset($params['a'])) {
      if ($this->imageStyle) {
        // Image style cache does not exists.
        return FALSE;
      }
      $defaults['size'] = $params['s'];
    }
    elseif ($this->isDirectory($path)) {
      // Suppose directory read only to check online in self::chmod().
      $defaults['mode'] = 040111;
    }
    else {
      $uploads = &drupal_static('netx_file_presave');
      if (isset($this->context) || isset($uploads[$path])) {
        // File in progress of upload or in file_move() right after upload.
        // @todo Check file exists in DAM (could be slow).
        $defaults['mode'] = 0100666;
      }
      else {
        return FALSE;
      }
    }
    return array_values($defaults) + $defaults;
  }

  /**
   * {@inheritdoc}
   */
  protected function _parse_url($url) {
    $path = explode('://', $url);
    $parts = explode('/', $path[1]);
    // Image style used.
    if ($parts[0] == 'styles') {
      // Remove styles parts.
      unset($parts[0]);
      unset($parts[1]);
      unset($parts[2]);
      $this->imageStyle = TRUE;
    }
    // Remove random uri part.
    if (isset($parts[0]) && is_numeric($parts[0])) {
      array_shift($parts);
    }
    return parent::_parse_url('netx://' . implode('/', $parts));
  }

  /**
   * {@inheritdoc}
   */
  public function dirname($uri = NULL) {
    list($scheme, $target) = explode('://', $uri, 2);
    $dirname = dirname(trim($target, '\/'));

    if ($dirname == '.') {
      $dirname = '';
    }

    // Category would be validated in this::chmod().
    return $scheme . '://' . $dirname;
  }

  /**
   * {@inheritdoc}
   */
  public function stream_open($uri, $mode, $options, &$opened_url) {
    $this->uri = $uri;
    if ($mode === 'wb') {
      // Open for writing, check that category accessible.
      $this->fileName = drupal_basename($uri);
      $dir = file_uri_target($this->dirname($uri));
      $repo = new NetXRepository();
      if (empty($dir)) {
        $dir = $repo->getDefaultCategory();
      }
      $this->categoryId = $repo->getCategoryByPath($dir);
      return (bool) $this->categoryId;
    }
    // @todo Make sure we able to write and asset exists.
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function stream_flush() {
    // If we are not writing to stream just return TRUE.
    if (empty($this->tmpFile) && !$this->needFlush) {
      return TRUE;
    }

    // Extract file name to find mime type.
    $extension = array_pop(explode('.', $this->fileName));
    $mimetype = media_get_extension_mimetype($extension);

    $repo = new NetXRepository();
    $data = $repo->uploadFileData($this->tmpFile, $this->fileName, $mimetype);

    // Asset import started.
    $status = 0;
    $asset_id = 0;

    // Asset import takes a time, so loop import.
    $count = 4;
    $delta = 0;
    do {
      // Give a time to process data (2s) plus increase each failed attempt.
      usleep(2000000 + $delta * 1000000);
      if (empty($asset_id)) {
        $asset_id = $repo->importAsset($this->fileName, $this->categoryId);
      }
      if ($asset_id) {
        $status = $repo->getImportStatus($asset_id);
        // Log when extended debug enabled.
        if (variable_get('netx_log_requests', FALSE)) {
          watchdog('netx', 'Upload status for asset %asset is %status with %data', array(
            '%status' => $status,
            '%asset' => $asset_id,
            '%data' => $data,
          ), WATCHDOG_DEBUG);
        }
      }
      // Prevent looping.
      $count--;
      $delta++;
    }
    while ($count && ($status == 0 || $status == 2));

    if ($status != 1) {
      $message = 'Error importing @file - asset @asset status @status';
      $message_args = array(
        '@file' => $this->fileName,
        '@asset' => $asset_id,
        '@status' => $status,
      );
      watchdog('netx', $message, $message_args, WATCHDOG_ERROR);
      // Throw exception because no way to return FALSE from copy() to prevent
      // saving a wring file.
      // @todo Find away to report failed copy operation.
      throw new \Exception(format_string($message, $message_args));
    }

    // Save metadata to be populated in netx_file_presave().
    $uploads = &drupal_static('netx_file_presave');
    $uploads[$this->uri] = $asset_id;

    watchdog('netx', 'Imported asset %asset', array(
      '%asset' => $asset_id,
    ), WATCHDOG_INFO);
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function stream_close() {
    // No clean-up is needed.
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
  public function stream_read($count) {
    // @todo Allow read from remote file.
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function stream_write($data) {
    $this->tmpFile .= $data;
    $this->needFlush = TRUE;
    return strlen($data);
  }

  /**
   * {@inheritdoc}
   */
  public function stream_eof() {
    // @todo Implement properly.
    return parent::stream_eof();
  }

  /**
   * {@inheritdoc}
   */
  public function stream_seek($offset, $whence) {
    // @todo Implement properly.
    return parent::stream_seek($offset, $whence);
  }

  /**
   * {@inheritdoc}
   */
  public function stream_tell() {
    // @todo Implement properly.
    return parent::stream_tell();
  }

  /**
   * {@inheritdoc}
   */
  public function stream_stat() {
    // @todo Implement properly.
    return parent::stream_stat();
  }

  /**
   * {@inheritdoc}
   */
  public function chmod($mode) {
    // Check online access.
    $repo = new NetXRepository();

    list(, $dirname) = explode('://', $this->uri, 2);
    if (empty($dirname)) {
      // When no path provided use default category.
      $dirname = $repo->getDefaultCategory();
    }
    if ($repo->getCategoryByPath($dirname)) {
      // Path is category.
      return TRUE;
    }
    // No way to change permissions for files.
    return FALSE;
  }
  // @codingStandardsIgnoreEnd

  /**
   * {@inheritdoc}
   */
  public function getLocalPath($uri = NULL) {
    return FALSE;
  }
}
