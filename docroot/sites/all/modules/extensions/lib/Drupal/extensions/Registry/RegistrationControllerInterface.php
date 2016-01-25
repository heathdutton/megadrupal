<?php
/**
 * @file
 * Provides an interface defining a Registration Controller.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\extensions\Registry;

/**
 * Class RegistrationControllerInterface
 * @package Drupal\extensions\Registry
 */
interface RegistrationControllerInterface {

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
  public function register($set_name, $extension_name, $human_name = NULL);

  /**
   * Set a factory function for creating this extension.
   *
   * @param string $factory_name
   *   A factory function.
   *
   * @return \Drupal\extensions\Registry\RegistrationController
   *   The registration controller, for chaining.
   */
  public function factory($factory_name);

  /**
   * Set the class handler for an extension.
   *
   * @param string $handler
   *   A valid class name including namespace.
   *
   * @return \Drupal\extensions\Registry\RegistrationController
   *   The registration controller, for chaining.
   */
  public function className($handler);
}