<?php

/**
 * @file ownCloudStreamWrapper.inc
 *
 * Implement DrupalStreamWrapperInterface to provide an owncloud://
 * stream wrapper.
 */
class ownCloudStreamWrapper implements DrupalStreamWrapperInterface {

  /**
   * Instance URI (stream).
   *
   * A stream is referenced as "owncloud://path".
   *
   * @var string
   */
  private $uri;

  /**
   * Name of the file.
   *
   * @var string
   */
  private $filename;

  /**
   * File pointer
   *
   * @var int
   */
  private $position;

  /**
   * Object cache
   *
   * @var string
   */
  private $file_content = null;

  /**
   * @var bool
   */
  private $file_exists = null;

  private function getFileName($uri = NULL) {
    if (!isset($this->filename)) {
      if (!isset($uri)) {
        $uri = $this->uri;
      }
      $file = owncloud_file_load_by_remote_id($uri);
      $this->filename = $file ? $file->filename : 'unknown';
    }
    return $this->filename;
  }

  private function getTarget($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }
    list($scheme, $target) = explode('://', $uri, 2);
    return trim($target, '\/');
  }

  private function getLocalPath($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }
    $path  = str_replace('owncloud://', '', $uri);
    $path = trim($path, '/');
    return $path;
  }

  public function stream_open($uri, $mode, $options, &$opened_url) {
    $this->uri = $uri;
    $this->position = 0;
    return TRUE;
  }

  public function stream_close() {
    $this->pointer = 0;
    return TRUE;
  }

  public function stream_lock($operation) {
    return lock_acquire('owncloud:' . $this->uri);
  }

  public function stream_read($count) {
    if ($this->_fileExists()) {
      $object = substr($this->_fileContent(), $this->position, $count);
      $this->position += $count;
      return $object;
    }
    return FALSE;
  }

  public function stream_write($data) {
    return FALSE;
  }

  public function stream_eof() {
    if ($this->_fileExists()) {
      return $this->position >= strlen($this->_fileContent());
    }
    return TRUE;
  }

  public function stream_seek($offset, $whence) {
    switch($whence) {
      case SEEK_SET:
        if ($this->_fileExists()) {
          $object = $this->_fileContent();
          if ($offset < strlen($object) && $offset >= 0) {
            $this->position = $offset;
            return TRUE;
          }
        }
        return FALSE;

      case SEEK_CUR:
        if ($offset >= 0) {
          $this->position += $offset;
          return TRUE;
        }
        return FALSE;

      case SEEK_END:
        if ($this->_fileExists()) {
          $object = $this->_fileContent();
          if (strlen($object) + $offset >= 0) {
            $this->position = strlen($object) + $offset;
            return TRUE;
          }
        }
        return FALSE;

      default:
        return FALSE;

    }
  }

  public function stream_flush() {
    return TRUE;
  }

  public function stream_tell() {
    return $this->position;
  }

  public function stream_stat() {
    return $this->_stat();
  }

  public function unlink($uri) {
    $this->uri = $uri;
    return FALSE;
  }

  public function rename($from_uri, $to_uri) {
    return FALSE;
  }

  public function mkdir($uri, $mode, $options) {
    $this->uri = $uri;
    return FALSE;
  }

  public function rmdir($uri, $options) {
    $this->uri = $uri;
    return FALSE;
  }

  public function url_stat($uri, $flags) {
    $this->uri = $uri;
    return $this->_stat($uri);
  }

  public function dir_opendir($uri, $options) {
    $this->uri = $uri;
    return FALSE;
  }

  public function dir_readdir() {
    return FALSE;
  }

  public function dir_rewinddir() {
    return FALSE;
  }

  public function dir_closedir() {
    return FALSE;
  }

  /**
   * Set the absolute stream resource URI.
   *
   * This allows you to set the URI. Generally is only called by the factory
   * method.
   *
   * @param $uri
   *   A string containing the URI that should be used for this instance.
   */
  function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Returns the stream resource URI.
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
    // image styles support
    $path = explode('/', $this->getLocalPath());
    $path[] = $this->getFileName();
    if ($path[0] == 'styles') {
      array_shift($path);
      return url('system/files/styles/' . implode('/', $path), array('absolute' => true));
    }
    return url('owncloud/file/' . implode('/', $path), array('absolute' => true));
  }

  /**
   * Returns the MIME type of the resource.
   *
   * @param $uri
   *   The URI, path, or filename.
   * @param $mapping
   *   An optional map of extensions to their mimetypes, in the form:
   *    - 'mimetypes': a list of mimetypes, keyed by an identifier,
   *    - 'extensions': the mapping itself, an associative array in which
   *      the key is the extension and the value is the mimetype identifier.
   *
   * @return string
   *   Returns a string containing the MIME type of the resource.
   */
  public static function getMimeType($uri, $mapping = NULL) {
    $file = owncloud_file_load_by_remote_id($uri);
    return DrupalLocalStreamWrapper::getMimeType($file->filename, $mapping);
  }

  public function chmod($mode) {
    return TRUE;
  }

  /**
   * Returns canonical, absolute path of the resource.
   *
   * Implementation placeholder. PHP's realpath() does not support stream
   * wrappers. We provide this as a default so that individual wrappers may
   * implement their own solutions.
   *
   * @return string
   *   Returns a string with absolute pathname on success (implemented
   *   by core wrappers), or FALSE on failure or if the registered
   *   wrapper does not provide an implementation.
   */
  public function realpath() {
    return 'owncloud://' . $this->getLocalPath();
  }

  /**
   * Gets the name of the directory from a given path.
   *
   * This method is usually accessed through drupal_dirname(), which wraps
   * around the normal PHP dirname() function, which does not support stream
   * wrappers.
   *
   * @param $uri
   *   An optional URI.
   * @return string
   *   A string containing the directory name, or FALSE if not applicable.
   * @see drupal_dirname()
   */
  public function dirname($uri = NULL) {
    list($scheme, $target) = explode('://', $uri, 2);
    $target  = $this->getTarget($uri);
    $dirname = dirname($target);

    if ($dirname == '.') {
      $dirname = '';
    }

    return $scheme . '://' . $dirname;
  }

  private function _stat($uri = null) {

    if ($uri == null) {
      $uri = $this->uri;
    }
    $mode = 0100000; // S_IFREG indicating file
    $mode |= 0777; // everything is writeable
    $stat = array(
      '0' => 0,
      '1' => 0,
      '2' => $mode,
      '3' => 0,
      '4' => 0,
      '5' => 0,
      '6' => 0,
      '7' => 0,
      '8' => 0,
      '9' => 0,
      '10' => 0,
      '11' => 0,
      '12' => 0,
      'dev' => 0,
      'ino' => 0,
      'mode' => $mode,
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

    $info = owncloud_request('fileinfo.php', array('id' => $this->getLocalPath()));
    if ($info) {
      // check if directory
      if ($info['type'] == 'dir') {
        $stat['mode'] = 0040000; // S_IFDIR indicating directory
        $stat['mode'] |= 0777;
        $stat['2'] = $stat['mode'];
      }
      else {
        if (isset($info['size'])) {
          $stat['size'] = $info['size'];
        }
        if (isset($info['mtime'])) {
          $stat['mtime'] = $info['mtime'];
        }
      }
      return $stat;
    }
    return FALSE;
  }

  /**
   * Lazy loads the current object
   */
  private function _fileContent() {
    if ($this->_fileExists()) {
      if (!$this->file_content) {
        $this->file_content = owncloud_request('download.php', array('id' => $this->getLocalPath()));
      }
      return $this->file_content;
    }
    return FALSE;
  }

  private function _fileExists() {
    if ($this->file_exists == null) {
      $this->file_exists = (bool) owncloud_request('fileinfo.php', array('id' => $this->getLocalPath()));
    }
    return $this->file_exists;
  }

}
