<?php
/**
 * @file
 * RoutingInfo class.
 *
 * Helper class to hold some routing information about a menu path.
 */

/**
 * Class RoutingInfo.
 *
 * This class is a 'data holder' (simple POPO) used to deliver data between
 * functions.
 *
 * @private @property string $path Menu path.
 * @private @property string $callback Callback function name.
 * @private @property string $module Name of module defining this path.
 * @private @property string $file Name of file that contains the callback.
 * @private @property string $line Line number with callback function.
 */
class RoutingInfo {

  /* @var string $path Menu path. */
  private $path;
  /* @var string $callback Callback function name. */
  private $callback;
  /* @var string $module Name of module defining this path. */
  private $module;
  /* @var string $file Name of file that contains the callback. */
  private $file;
  /* @var string $line Line number with callback function. */
  private $line;

  /**
   * Path getter.
   *
   * @return string
   *   Path.
   */
  public function getPath() {
    return $this->path;
  }

  /**
   * Path setter.
   *
   * @param string $path
   *   Path.
   */
  public function setPath($path) {
    $this->path = $path;
  }

  /**
   * Callback getter.
   *
   * @return string
   *   Callback function name.
   */
  public function getCallback() {
    return $this->callback;
  }

  /**
   * Callback setter.
   *
   * @param string $callback
   *   Callback function name.
   */
  public function setCallback($callback) {
    $this->callback = $callback;
  }

  /**
   * Module name getter.
   *
   * @return string
   *   Module name.
   */
  public function getModule() {
    return $this->module;
  }

  /**
   * Module name setter.
   *
   * @param string $module
   *   Module name.
   */
  public function setModule($module) {
    $this->module = $module;
  }

  /**
   * File name getter.
   *
   * @return string
   *   File name.
   */
  public function getFile() {
    return $this->file;
  }

  /**
   * File name setter.
   *
   * @param string $file
   *   File name.
   */
  public function setFile($file) {
    $this->file = $file;
  }

  /**
   * Line number getter.
   *
   * @return string
   *   Line number.
   */
  public function getLine() {
    return $this->line;
  }

  /**
   * Line number setter.
   *
   * @param string $line
   *   Line number.
   */
  public function setLine($line) {
    $this->line = $line;
  }

}
