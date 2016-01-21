<?php

/**
 * @file
 * The EntityInfo Factory is a wrapper around Entity API.
 */

namespace Drupal\doctrine\Mapping;

/**
 * Reads entity info.
 *
 * @since 7.x-1.0
 * @author Sylvain Lecoy <sylvain.lecoy@gmail.com>
 */
class EntityInfoFactory {

  /**
   * @see entity_get_info()
   */
  public static function get() {
    return entity_get_info();
  }

}
