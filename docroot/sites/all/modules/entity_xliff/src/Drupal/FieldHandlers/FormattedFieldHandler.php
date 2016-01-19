<?php

/**
 * @file
 * Defines an entity xliff field handler for use by fields with format.
 */

namespace EntityXliff\Drupal\FieldHandlers;

use EntityXliff\Drupal\Interfaces\FieldHandlerInterface;


class FormattedFieldHandler implements FieldHandlerInterface {

  /**
   * {@inheritdoc}
   */
  public function getValue(\EntityMetadataWrapper $wrapper) {
    $fieldValue = $wrapper->value();
    return $fieldValue['value'];
  }

  /**
   * {@inheritdoc}
   */
  public function setValue(\EntityMetadataWrapper $wrapper, $value) {
    $newValue = $wrapper->value();
    $newValue['value'] = html_entity_decode($value);
    $wrapper->set($newValue);
  }

}
