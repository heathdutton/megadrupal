<?php

/**
 * @file
 * The Schema Factory is a wrapper around Schema API.
 */

namespace Drupal\doctrine\Mapping;

/**
 * Reads the whole database schema.
 *
 * @since 7.x-1.0
 * @author Sylvain Lecoy <sylvain.lecoy@gmail.com>
 */
class SchemaFactory {

  /**
   * @see drupal_get_schema()
   */
  public static function get() {
    return drupal_get_schema();
  }

}
