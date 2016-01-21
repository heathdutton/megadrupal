<?php
/**
 * @file
 * Provides a base extension.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\extensions\Extension;

/**
 * Class BaseExtension
 *
 * @package Drupal\extensions\Base
 */
class Extension
  implements ExtensionInterface {

  /**
   * The extension name.
   *
   * @var string
   */
  protected $name;

  /**
   * Extension properties.
   *
   * Use this for storing properties on a base extension type, without having to
   * dynamically create properties, or extend the class.
   *
   * @var array
   */
  protected $properties = array();

  /**
   * Set the extension name.
   *
   * @param string $name
   *   A valid machine name.
   */
  public function setName($name) {

    $this->name = $name;
  }

  /**
   * Get the extension name.
   *
   * @return string
   *   The name.
   */
  public function getName() {

    return $this->name;
  }

  /**
   * Set all properties.
   *
   * @param array $args
   *   An array of properties.
   */
  public function setProperties($args) {

    $this->properties = $args;
  }

  /**
   * Get all properties.
   *
   * @return array
   *   An array of properties.
   */
  public function getProperties() {

    return $this->properties;
  }

  /**
   * Set an property value.
   *
   * @param string $key
   *   Name of the property.
   * @param mixed $value
   *   Value of the property.
   */
  public function setProperty($key, $value) {
    $this->properties[$key] = $value;
  }

  /**
   * Get an property value.
   *
   * @param string $key
   *   The property key.
   * @param mixed $default
   *   A default value.
   *
   * @return mixed
   *   The result.
   */
  public function getProperty($key, $default = FALSE) {
    if (isset($this->properties[$key])) {
      return $this->properties[$key];
    }

    return $default;
  }

  /**
   * Implements stdClass::__get().
   */
  public function __get($name) {
    return $this->getProperty($name, NULL);
  }

  /**
   * Implements stdClass::__set().
   */
  public function __set($name, $value) {
    $this->setProperty($name, $value);
  }

  /**
   * Implements stdClass::__isset().
   */
  public function __isset($name) {
    if (isset($this->properties[$name])) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Implements stdClass::__unset().
   */
  public function __unset($name) {
    if (isset($this->properties[$name])) {
      unset($this->properties[$name]);
    }
  }

  /**
   * Log an exception to the watchdog.
   *
   * @param \Exception $e
   *   The error object.
   * @param string|null $message
   *   An optional message.
   * @param string|null $module
   *   The module logging the error.
   */
  protected function logException(\Exception $e, $message = NULL, $module = NULL) {

    if (empty($module)) {
      $module = 'extensions';
    }

    watchdog_exception($module, $e, '%m. In %file, line %line. Message: %message', array(
      '%m' => rtrim($message, '.'),
      '%message' => $e->getMessage(),
      '%line' => $e->getLine(),
      '%file' => $e->getFile(),
    ));
  }

  /**
   * Log an error to the watchdog.
   *
   * @param string|null $message
   *   An optional message.
   * @param string|null $module
   *   The module logging the error.
   */
  protected function logError($message, $module = NULL) {

    if (empty($module)) {
      $module = 'extensions';
    }

    watchdog($module, '%m.', array(
      '%m' => rtrim($message, '.'),
    ));
  }
}
