<?php
/**
 * @file
 * Provides an interface defining an Extension Factory.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\extensions\Extension;

/**
 * Class ExtensionFactoryInterface
 * @package Drupal\extensions\Extension
 */
interface ExtensionFactoryInterface {

  /**
   * Create a new Extension object.
   *
   * @param string $name
   *   Machine name of the extension
   * @param string $object_type
   *   (optional) The object type. If not provided, defaults to the base
   *   extension type.
   * @param array $properties
   *   (optional) Any properties to assign on creation.
   *
   * @return \Drupal\extensions\Extension\ExtensionInterface
   *   An extension.
   */
  public function createExtension($name, $object_type = '\Drupal\extensions\Extension\Extension', $properties = array());
}
