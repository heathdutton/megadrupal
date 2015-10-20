<?php

/**
 * @file
 * Relation Converter interface.
 */

namespace Drupal\maps_import\Converter\Child\Relation;

use Drupal\maps_import\Converter\Child\ChildInterface;

/**
 * Relation Converter interface.
 */
interface RelationInterface extends ChildInterface {

  /**
   * Defines the profile scope for relation converters.
   */
  const SCOPE_RELATION = 3;
  
}