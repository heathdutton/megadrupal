<?php

/**
 * Drupal system stream wrapper abstract class.
 */
abstract class SystemStreamWrapper extends DrupalLocalStreamWrapper {

  /**
   * Get the module, theme, or profile name of the current URI.
   */
  protected function getSystemName($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }
    list($scheme, $target) = explode('://', $uri, 2);
    $pos = strpos($target, '/');
    return $pos === FALSE ? $target : substr($target, 0, $pos);
  }

  protected function getTarget($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    list($scheme, $target) = explode('://', $uri, 2);

    // Remove erroneous leading or trailing, forward-slashes and backslashes.
    $target = trim($target, '\/');

    // Remove the module/theme/profile name form the file path.
    $target = substr($target, strlen($this->getSystemName()));

    // Trim again.
    $target = trim($target, '\/');
    return $target;
  }

  /**
   * Gets the path that the wrapper is responsible for.
   *
   * @return
   *   String specifying the path.
   */
  //abstract public function getDirectoryPath();

  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a system file.
   */
  public function getExternalUrl() {
    $dir = $this->getDirectoryPath();
    if (empty($dir)) {
      return FALSE;
    }

    $path = str_replace('\\', '/', $this->getTarget());
    return $GLOBALS['base_url'] . '/' . $dir . '/' . drupal_encode_path($path);
  }

  /**
   * DrupalStreamWrapperInterface requires that these methods be implemented,
   * but none of them apply to a read-only stream wrapper. On failure they
   * are expected to return FALSE.
   */

  public function stream_write($data) {
    return FALSE;
  }
  public function unlink($uri) {
    // Although the remote file itself can't be deleted, return TRUE so that
    // file_delete() can remove the file record from the database.
    return TRUE;
  }
  public function rename($from_uri, $to_uri) {
    return FALSE;
  }
  public function mkdir($uri, $mode, $options) {
    return FALSE;
  }
  public function rmdir($uri, $options) {
    return FALSE;
  }
  public function chmod($mode) {
    return FALSE;
  }
  public function dirname($uri = NULL) {
    return FALSE;
  }
}

class ModuleSystemStreamWrapper extends SystemStreamWrapper {

  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    return drupal_get_path('module', $this->getSystemName());
  }
}

/**
 * Stream wrapper for theme files using theme://.
 */
class ThemeSystemStreamWrapper extends SystemStreamWrapper {

  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    return drupal_get_path('theme', $this->getSystemName());
  }
}

/**
 * Stream wrapper for profile files using profile://.
 */
class ProfileSystemStreamWrapper extends SystemStreamWrapper {

  /**
   * Override SystemSteamWrapper::getSystemName() to support profile://current
   */
  protected function getSystemName($uri = NULL) {
    $name = parent::getSystemName($uri);
    if ($name == 'current') {
      return drupal_get_profile();
    }
    else {
      return $name;
    }
  }

  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    // We cannot use drupal_get_path() here as it actually doesn't work if
    // $type is 'profile'.
    // @see http://drupal.org/node/1006714
    return 'profiles/' . $this->getSystemName();
  }
}

/**
 * Stream wrapper for library files using library://.
 */
class LibrarySystemStreamWrapper extends SystemStreamWrapper {

  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    return libraries_get_path($this->getSystemName());
  }
}
