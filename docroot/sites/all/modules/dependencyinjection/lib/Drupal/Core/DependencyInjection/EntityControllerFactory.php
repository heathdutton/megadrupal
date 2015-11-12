<?php

/**
 * @file
 *
 * @author Sylvain Lecoy <sylvain.lecoy@gmail.com>
 */

namespace Drupal\Core\DependencyInjection;

/**
 * Instantiates controller with drupal for registering the same instance in the
 * container.
 */
class EntityControllerFactory {

  /**
   * Wrapper around entity_get_controller.
   */
  public static function get($entity_type) {
    return entity_get_controller($entity_type);
  }

}
