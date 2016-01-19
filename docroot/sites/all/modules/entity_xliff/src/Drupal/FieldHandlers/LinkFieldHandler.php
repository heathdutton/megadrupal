<?php

/**
 * @file
 * Defines an entity xliff field handler for use by link fields.
 */

namespace EntityXliff\Drupal\FieldHandlers;

use EntityXliff\Drupal\Interfaces\FieldHandlerInterface;


class LinkFieldHandler implements FieldHandlerInterface {

  /**
   * {@inheritdoc}
   */
  public function getValue(\EntityMetadataWrapper $wrapper) {
    $fieldValue = $wrapper->value();
    return $fieldValue['title'];
  }

  /**
   * {@inheritdoc}
   */
  public function setValue(\EntityMetadataWrapper $wrapper, $value) {
    $newValue = $wrapper->value();
    $newValue['title'] = html_entity_decode($value);
    $wrapper->set($newValue);
  }

}
