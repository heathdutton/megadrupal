<?php
/**
 * @file
 *  The arg module
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

namespace Xu\Component\Arg;

/**
 * Defines a generic class for passing complex arguments
 */
class ArgHandler {

  /**
   * The internal arguments array
   */
  protected $args = array();

  /**
   * Constructor
   */
  public function __construct() {
    $this->args = array();
  }

  /**
   * Set an argument
   *
   * @param $key
   *  The key to use to set the data
   * @param $data
   *  The data
   *
   * @return \Xu\Component\Arg\ArgHandler
   */
  public function set($key, $data) {
    $this->args[$key] = $data;

    return $this;
  }

  /**
   * Unset a key
   *
   * @param $key
   *  The key to unset
   *
   * @return \Xu\Component\Arg\ArgHandler
   */
  public function delete($key) {
    if (isset($this->args[$key])) {
      unset($this->args[$key]);
    }

    return $this;
  }

  /**
   * Get an argument
   *
   * @param $key
   *  The key to retrieve data for
   * @param null $default [optional]
   *  A default value if $key is not set
   *
   * @return mixed
   *  The result of the get operation, the default value, or NULL.
   */
  public function get($key, $default = NULL) {
    if (isset($this->args[$key])) {
      return $this->args[$key];
    }
    elseif (isset($default) && !is_null($default)) {
      return $default;
    }

    return NULL;
  }

  /**
   * Wipe the arguments
   */
  public function wipe() {
    $this->args = array();
  }

  /**
   * Overides __get()
   */
  function __get($name) {
    $this->get($name);
  }

  /**
   * Overides __set()
   */
  function __set($name, $value) {
    $this->set($name, $value);
  }

  /**
   * Overides __isset()
   */
  function __isset($name) {
    if (isset($this->args[$name])) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Overides __unset()
   */
  function __unset($name) {
    $this->delete($name);
  }

}
