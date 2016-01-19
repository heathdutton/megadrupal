<?php

/**
 * @file
 * Defines an entity xliff field handler for integers, used because the Entity
 * API performs very strict type validation on values.
 */

namespace EntityXliff\Drupal\FieldHandlers;

use EntityXliff\Drupal\Interfaces\FieldHandlerInterface;


class IntegerFieldHandler implements FieldHandlerInterface {

  /**
   * {@inheritdoc}
   */
  public function getValue(\EntityMetadataWrapper $wrapper) {
    return $wrapper->value();
  }

  /**
   * {@inheritdoc}
   */
  public function setValue(\EntityMetadataWrapper $wrapper, $value) {
    $wrapper->set(intval($value));
  }

}
