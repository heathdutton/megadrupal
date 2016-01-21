<?php
/**
 * @file
 * A default factory for creating extensions.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\extensions\Extension;

/**
 * Class ExtensionFactory
 * @package Drupal\extensions\Extension
 */
class ExtensionFactory
  implements ExtensionFactoryInterface {

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
  public function createExtension($name, $object_type = '\Drupal\extensions\Extension\Extension', $properties = array()) {
    if (class_exists($object_type)) {
      $object = $this->newExtension($object_type);

      $object->setName($name);
      $object->setProperties($properties);

      return $object;
    }
  }

  /**
   * Create a new extension object.
   *
   * @param string $object_type
   *   (optional) The object type. Defaults to the base extension type.
   *
   * @return \Drupal\extensions\Extension\ExtensionInterface
   *   An extension
   */
  protected function newExtension($object_type = '\Drupal\extensions\Extension\Extension') {

    $object = new $object_type();
    if ($object instanceof ExtensionInterface) {

      return $object;
    }

    return FALSE;
  }
}