<?php
/**
 * @file
 * Provides a class for managing a Collection of Extensions.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\extensions\Collection;

/**
 * Class Collection
 * @package Drupal\extensions\Registry
 */
use Drupal\extensions\Extension\ExtensionFactoryInterface;

class Collection
  implements CollectionInterface {

  public $name;

  protected $extensions = array();

  protected $loadedExtensions = array();

  /**
   * Constructor function.
   *
   * @param string $name
   *   Machine name of the extension set.
   */
  public function __construct($name) {
    $this->name = $name;
  }

  /**
   * Wrapper to set initialise a extension with blank values.
   *
   * @param string $extension_name
   *   The extension machine name.
   * @param string|null $human_name
   *   A human readable name.
   */
  public function registerExtension($extension_name, $human_name = NULL) {
    $this->extensions[$extension_name] = array();

    if (!empty($human_name)) {
      $this->extensions[$extension_name]['title'] = $human_name;
    }
  }

  /**
   * Set arguments for a extension.
   *
   * @param string $extension_name
   *   The extension machine name.
   * @param string $factory
   *   The factory function. If this is provided as an array, the first
   *   key will be a class name, and the second the method to be called on that
   *   class.
   */
  public function extensionFactory($extension_name, $factory) {
    if (isset($this->extensions[$extension_name])) {
      $this->extensions[$extension_name]['factory'] = $factory;
    }
  }

  /**
   * Set a class for an extension.
   *
   * @param string $extension_name
   *   The extension machine name.
   * @param null $class_name
   *   The class to use for the handler.
   */
  public function extensionClass($extension_name, $class_name = NULL) {
    if (isset($this->extensions[$extension_name])) {
      $this->extensions[$extension_name]['class'] = $class_name;
    }
  }

  /**
   * Set default properties for an extension.
   *
   * @param string $extension_name
   *   The extension machine name.
   * @param null $properties
   *   The class to use for the handler.
   */
  public function extensionProperties($extension_name, $properties) {
    if (isset($this->extensions[$extension_name])) {
      $this->extensions[$extension_name]['properties'] = $properties;
    }
  }

  /**
   * Get all loaded extension information.
   *
   * @return array
   *   An array of extension parameters.
   */
  public function getExtensionsSettings() {
    if (!empty($this->extensions)) {
      return $this->extensions;
    }

    return array();
  }

  /**
   * Get all extensions as loaded objects.
   *
   * @todo cache this a bit
   *
   * @return array
   *   An array of extension parameters.
   */
  public function getExtensions() {
    if (!empty($this->extensions)) {
      $extensions = array_keys($this->extensions);
      foreach ($extensions as $key) {
        $this->loadedExtensions[$key] = $this->loadExtension($key);
      }
    }

    return $this->loadedExtensions;
  }

  /**
   * Get all extensions as loaded objects.
   *
   * @todo cache this a bit
   *
   * @param string $name
   *   Name of the extension to load.
   *
   * @return object
   *   The extension
   */
  public function getExtension($name) {
    if (!empty($this->extensions)) {
      if (isset($this->extensions[$name])) {

        $this->loadedExtensions[$name] = $this->loadExtension($name);
      }
    }

    return $this->loadedExtensions[$name];
  }

  /**
   * Create and load an extension.
   *
   * @param string $key
   *   The extension to load.
   *
   * @return \Drupal\extensions\Extension\ExtensionInterface
   *   An extension.
   */
  public function loadExtension($key) {

    if (isset($this->extensions[$key])) {

      $class = '\Drupal\extensions\Extension\Extension';
      $factory_type = '\Drupal\extensions\Extension\ExtensionFactory';
      $properties = array();

      if (isset($this->extensions[$key]['factory'])) {
        $factory_type = $this->extensions[$key]['factory'];
      }

      if (isset($this->extensions[$key]['class'])) {
        $class = $this->extensions[$key]['class'];
      }

      if (isset($this->extensions[$key]['properties'])) {
        $properties = $this->extensions[$key]['properties'];
      }

      $factory_object = $this->createExtensionFactory($factory_type);
      return $this->createExtension($key, $factory_object, $class, $properties);
    }
  }

  /**
   * Create a new extension factory.
   *
   * @param string $factory_type
   *   (optional) A factory object type. Defaults to the default factory.
   *
   * @return \Drupal\extensions\Extension\ExtensionFactoryInterface
   *   An Extension factory object.
   */
  public function createExtensionFactory($factory_type = '\Drupal\extensions\Extension\ExtensionFactory') {

    $factory_object = new $factory_type();

    return $factory_object;
  }

  /**
   * Create a new extension using a Factory.
   *
   * @param string $key
   *   The extension to load.
   * @param ExtensionFactoryInterface $factory_object
   *   The factory object to use.
   * @param string $class
   *   The class name.
   * @param array $properties
   *   Properties to set on the extension.
   *
   * @return \Drupal\extensions\Extension\ExtensionInterface
   *   An extension.
   */
  public function createExtension($key, ExtensionFactoryInterface $factory_object, $class, $properties = array()) {

    return $factory_object->createExtension($key, $class, $properties);
  }
}
