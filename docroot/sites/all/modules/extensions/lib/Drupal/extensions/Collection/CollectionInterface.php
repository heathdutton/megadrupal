<?php
/**
 * @file
 * Provides an interface describing a Collection.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\extensions\Collection;

/**
 * Class CollectionInterface
 * @package Drupal\extensions\Collection
 */
use Drupal\extensions\Extension\ExtensionFactoryInterface;

interface CollectionInterface {

  /**
   * Wrapper to set initialise a extension with blank values.
   *
   * @param string $extension_name
   *   The extension machine name.
   * @param string|null $human_name
   *   A human readable name.
   */
  public function registerExtension($extension_name, $human_name = NULL);

  /**
   * Set arguments for a extension.
   *
   * @param string $extension_name
   *   The extension machine name.
   * @param string|array $factory
   *   The factory function name. If this is provided as an array, the first
   *   key will be a class name, and the second the method to be called on that
   *   class.
   */
  public function extensionFactory($extension_name, $factory);

  /**
   * Set a class handler for a extension.
   *
   * @param string $extension_name
   *   The extension machine name.
   * @param null $handler
   *   The class to use for the handler.
   */
  public function extensionClass($extension_name, $handler = NULL);

  /**
   * Create a new extension factory.
   *
   * @param string $factory_type
   *   (optional) A factory object type. Defaults to the default factory.
   *
   * @return \Drupal\extensions\Extension\ExtensionFactoryInterface
   *   An Extension factory object.
   */
  public function createExtensionFactory($factory_type = '\Drupal\extensions\Extension\ExtensionFactory');

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
  public function createExtension($key, ExtensionFactoryInterface $factory_object, $class, $properties = array());

  /**
   * Set default properties for an extension.
   *
   * @param string $extension_name
   *   The extension machine name.
   * @param null $properties
   *   The class to use for the handler.
   */
  public function extensionProperties($extension_name, $properties);

  /**
   * Create and load an extension.
   *
   * @param string $key
   *   The extension to load.
   *
   * @return \Drupal\extensions\Extension\ExtensionInterface
   *   An extension.
   */
  public function loadExtension($key);

  /**
   * Get all extensions as loaded objects.
   *
   * @todo cache this a bit
   *
   * @return array
   *   An array of extension parameters.
   */
  public function getExtensions();

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
  public function getExtension($name);
}