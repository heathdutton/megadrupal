<?php

/**
 * @file
 * Contains Drupal\smartling\EntityConversionUtils.
 */

namespace Drupal\smartling\EntityConversionUtils;

/**
 * Provides an interface for a EntityConversionUtility.
 */
interface EntityConversionInterface {
  /*
   * Converts entity in neutral language to the entity in default language. So that this entity could be translated.
   */
  public function convert(&$entity, $entity_type);
}
