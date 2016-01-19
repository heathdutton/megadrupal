<?php
/**
 * @file
 * Provides a stream wrapper class for the Bassets server.
 */

/**
 * The BassetsStreamWrapper class extends the DrupalLocalStreamWrapper
 * and deliver files from a menu path and not from the local file system.
 */
class BassetsStreamWrapper extends DrupalLocalStreamWrapper {

  /**
   * The content of the file.
   *
   * @var String
   */
  protected $streamContent = NULL;


  /**
   * The pointer to the next read or write within the content variable.
   *
   * @var Integer
   */
  protected $streamPointer;

  /**
   * Overrides getExternalUrl().
   *
   * Builds the path for our menu callback to deliver files.
   */
  public function getExternalUrl() {
    $path = str_replace('\\', '/', $this->getTarget());
    return $GLOBALS['base_url'] . '/' . self::getDirectoryPath() . '/' . drupal_encode_path($path);
  }

  /**
   * {@inheritDoc}
   */
  public function getDirectoryPath() {
    return BASSETS_DELIVER_PATH;
  }

  /**
   * Implements chmod().
   *
   * Returns a TRUE result since this is a read-only stream wrapper.
   */
  public function chmod($mode) {
    return TRUE;
  }

  /**
   * {@inheritDoc}
   */
  public function realpath() {
    return $this->getExternalUrl();
  }

  /**
   * {@inheritDoc}
   */
  public function stream_open($uri, $mode, $options, &$opened_url) {
    $this->uri = $uri;
    $allowed_modes = array('r', 'rb');
    if (!in_array($mode, $allowed_modes)) {
      return FALSE;
    }

    // Attempt to fetch the URL's data using drupal_http_request().
    if (!$this->getStreamContent()) {
      return FALSE;
    }

    // Reset the stream pointer since this is an open.
    $this->streamPointer = 0;
    return TRUE;
  }

  /**
   * {@inheritDoc}
   */
  public function stream_lock($operation) {
    return TRUE;
  }

  /**
   * {@inheritDoc}
   */
  public function stream_read($count) {
    if (is_string($this->streamContent)) {
      $remaining_chars = strlen($this->streamContent) - $this->streamPointer;
      $number_to_read = min($count, $remaining_chars);
      if ($remaining_chars > 0) {
        $buffer = substr($this->streamContent, $this->streamPointer, $number_to_read);
        $this->streamPointer += $number_to_read;
        return $buffer;
      }
    }
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function stream_write($data) {
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function stream_eof() {
    return $this->streamPointer == strlen($this->streamContent);
  }

  /**
   * {@inheritDoc}
   */
  public function stream_seek($offset, $whence) {
    if (strlen($this->streamContent) >= $offset) {
      $this->streamPointer = $offset;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function stream_flush() {
    return TRUE;
  }

  /**
   * {@inheritDoc}
   */
  public function stream_tell() {
    return $this->streamPointer;
  }

  /**
   * {@inheritDoc}
   */
  public function stream_stat() {
    $stat = array();
    $request = drupal_http_request($this->getExternalUrl(), array('method' => 'HEAD'));
    if (empty($request->error)) {
      if (isset($request->headers['content-length'])) {
        $stat['size'] = $request->headers['content-length'];
      }
      else {
        $size = strlen($this->getStreamContent());
        // If the HEAD request does not return a Content-Length header, fall
        // back to performing a full request of the file to determine its file
        // size.
        if ($size) {
          $stat['size'] = $size;
        }
      }
    }

    return !empty($stat) ? $this->getStat($stat) : FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function stream_close() {
    $this->streamPointer = 0;
    $this->streamContent = NULL;
    return TRUE;
  }

  /**
   * {@inheritDoc}
   */
  public function rename($from_uri, $to_uri) {
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function unlink($uri) {
    // We return FALSE rather than TRUE so that managed file records can be
    // deleted.
    return TRUE;
  }

  /**
   * {@inheritDoc}
   */
  public function mkdir($uri, $mode, $options) {
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function rmdir($uri, $options) {
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function url_stat($uri, $flags) {
    $this->uri = $uri;
    if ($flags & STREAM_URL_STAT_QUIET) {
      return @$this->stream_stat();
    }
    else {
      return $this->stream_stat();
    }
  }

  /**
   * {@inheritDoc}
   */
  public function dir_opendir($uri, $options) {
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function dir_readdir() {
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function dir_rewinddir() {
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function dir_closedir() {
    return FALSE;
  }

  /**
   * {@inheritDoc}
   */
  public function dirname($uri = NULL) {
    list($scheme, $target) = explode('://', $uri, 2);
    $dirname = dirname($target);

    if ($dirname == '.') {
      $dirname = '';
    }

    return $scheme . '://' . $dirname;
  }

  /**
   * {@inheritDoc}
   */
  public function getLocalPath($uri = NULL) {
    return $this->getExternalUrl();
  }

  /**
   * Helper function to return a full array for stat functions.
   */
  protected function getStat(array $stat = array()) {
    $defaults = array(
      'dev' => 0,      // device number.
      'ino' => 0,      // inode number.
      'mode' => 0100000 | 0444, // inode protectio.
      'nlink' => 0,    // number of links.
      'uid' => 0,      // userid of owner.
      'gid' => 0,      // groupid of owner.
      'rdev' => -1,    // device type, if inode device *.
      'size' => 0,     // size in bytes.
      'atime' => 0,    // time of last access (Unix timestamp).
      'mtime' => 0,    // time of last modification (Unix timestamp).
      'ctime' => 0,    // time of last inode change (Unix timestamp).
      'blksize' => -1, // blocksize of filesystem IO.
      'blocks' => -1,  // number of blocks allocated.
    );

    $return = array();
    foreach (array_keys($defaults) as $index => $key) {
      if (!isset($stat[$key])) {
        $return[$index] = $defaults[$key];
        $return[$key] = $defaults[$key];
      }
      else {
        $return[$index] = $stat[$key];
        $return[$key] = $stat[$key];
      }
    }

    return $return;
  }

  /**
   * Fetch the content of the file using drupal_http_request().
   */
  protected function getStreamContent() {
    if (!isset($this->streamContent)) {
      $this->streamContent = NULL;

      $request = drupal_http_request($this->getExternalUrl());
      if (empty($request->error) && !empty($request->data)) {
        $this->streamContent = $request->data;
      }
    }

    return $this->streamContent;
  }
}
