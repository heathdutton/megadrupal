<?php
/**
 * @file
 * Provides an interface describing an Extension.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\extensions\Extension;

/**
 * Class ExtensionInterface
 * @package Drupal\extensions\Extension
 */
interface ExtensionInterface {

  /**
   * Set the extension name.
   *
   * @param string $name
   *   A valid machine name.
   */
  public function setName($name);

  /**
   * Get the extension name.
   *
   * @return string
   *   The name.
   */
  public function getName();

  /**
   * Set all properties.
   *
   * @param array $args
   *   An array of properties.
   */
  public function setProperties($args);

  /**
   * Get all properties.
   *
   * @return array
   *   An array of properties.
   */
  public function getProperties();

  /**
   * Set an property value.
   *
   * @param string $key
   *   Name of the property.
   * @param mixed $value
   *   Value of the property.
   */
  public function setProperty($key, $value);

  /**
   * Get an property value.
   *
   * @param string $key
   *   The property key.
   * @param bool $default
   *   A default value.
   *
   * @return mixed|bool
   *   The result.
   */
  public function getProperty($key, $default = FALSE);

}
