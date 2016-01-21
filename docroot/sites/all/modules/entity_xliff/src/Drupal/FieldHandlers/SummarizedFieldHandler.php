<?php

/**
 * @file
 * Defines an entity xliff field handler for use by text fields that may also
 * provide a distinct summary.
 */

namespace EntityXliff\Drupal\FieldHandlers;

use EntityXliff\Drupal\Interfaces\FieldHandlerInterface;


class SummarizedFieldHandler implements FieldHandlerInterface {

  /**
   * {@inheritdoc}
   */
  public function getValue(\EntityMetadataWrapper $wrapper) {
    $response = array();

    $value = $wrapper->value();
    $info = $wrapper->info();

    // Check for value text.
    if (isset($value['value']) && !empty($value['value'])) {
      $response['value'] = array(
        '#label' => $info['label'] . ' (value)',
        '#text' => $value['value'],
      );
    }

    // Check for summary text.
    if (isset($value['summary']) && !empty($value['summary'])) {
      $response['summary'] = array(
        '#label' => $info['label'] . ' (summary)',
        '#text' => $value['summary'],
      );
    }

    return $response;
  }

  /**
   * {@inheritdoc}
   */
  public function setValue(\EntityMetadataWrapper $wrapper, $value) {
    $newValue = $wrapper->value();

    if (isset($value['value'])) {
      $newValue['value'] = html_entity_decode($value['value']);
    }
    if (isset($value['summary'])) {
      $newValue['summary'] = html_entity_decode($value['summary']);
    }

    $wrapper->set($newValue);
  }

}
