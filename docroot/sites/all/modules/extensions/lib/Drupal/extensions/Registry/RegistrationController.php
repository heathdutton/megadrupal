<?php
/**
 * @file
 * Contains a class for controlling extension registration.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\extensions\Registry;

/**
 * Class RegistrationController
 * @package Drupal\sourcery\Filters
 */
class RegistrationController
  implements RegistrationControllerInterface {

  /**
   * Name of the collection for this registration.
   *
   * @var string
   */
  protected $collectionName;

  /**
   * Name of the extension for this registration.
   *
   * @var string
   */
  protected $extensionName;

  /**
   * The registry object.
   *
   * @var \Drupal\extensions\Registry\Registry
   */
  protected $register;

  /**
   * Constructor.
   *
   * @param \Drupal\extensions\Registry\Registry $register
   *   The Registry.
   */
  public function __construct(Registry $register) {
    $this->setRegister($register);
  }

  /**
   * Register a new extension.
   *
   * @param string $set_name
   *   Name of the set to add the extension to.
   * @param string $extension_name
   *   Machine name of the extension.
   * @param string|null $human_name
   *   A human readable name.
   *
   * @return \Drupal\extensions\Registry\RegistrationController
   *   The registration controller, for chaining.
   */
  public function register($set_name, $extension_name, $human_name = NULL) {

    $this->setCollectionName($set_name);
    $this->setExtensionName($extension_name);

    $this->getRegister()->registerExtension($set_name, $extension_name, $human_name);

    return $this;
  }

  /**
   * Set a factory function for creating this extension.
   *
   * @param string $factory_name
   *   A factory function.
   *
   * @return \Drupal\extensions\Registry\RegistrationController
   *   The registration controller, for chaining.
   */
  public function factory($factory_name) {

    $this->getRegister()
      ->getCollection($this->collectionName)
      ->extensionFactory($this->extensionName, $factory_name);

    return $this;
  }

  /**
   * Set the class handler for an extension.
   *
   * @param string $class_name
   *   A valid class name including namespace.
   *
   * @return \Drupal\extensions\Registry\RegistrationController
   *   The registration controller, for chaining.
   */
  public function className($class_name) {

    $this->getRegister()
      ->getCollection($this->collectionName)
      ->extensionClass($this->extensionName, $class_name);

    return $this;
  }

  /**
   * Set default properties on an extension.
   *
   * @param array $properties
   *   An array of properties.
   *
   * @return \Drupal\extensions\Registry\RegistrationController
   *   The registration controller, for chaining.
   */
  public function properties($properties = array()) {
    $this->getRegister()
      ->getCollection($this->collectionName)
      ->extensionProperties($this->extensionName, $properties);

    return $this;
  }

  /**
   * Set a filter register.
   *
   * @param \Drupal\extensions\Registry\Registry $register
   *   The filter register.
   *
   * @return \Drupal\extensions\Registry\RegistrationController
   *   The registration controller, for chaining.
   */
  protected function setRegister(Registry $register) {

    $this->register = $register;

    return $this;
  }

  /**
   * Get a filter register.
   *
   * @return \Drupal\extensions\Registry\Registry
   *   The filter register.
   */
  protected function getRegister() {

    return $this->register;
  }

  /**
   * Set a collection name for this registration.
   *
   * @param string $name
   *   Name of the Collection.
   *
   * @return \Drupal\extensions\Registry\RegistrationController
   *   The registration controller, for chaining.
   */
  public function setCollectionName($name) {

    $this->collectionName = $name;

    return $this;
  }

  /**
   * Set the Extension name for this registration.
   *
   * @param string $extension_name
   *   The extension name.
   *
   * @return \Drupal\extensions\Registry\RegistrationController
   *   The registration controller, for chaining.
   */
  public function setExtensionName($extension_name) {

    $this->extensionName = $extension_name;

    return $this;
  }

}
