<?php

namespace Drupal\maps_import\Mapping\Target\Drupal\Field;

/**
 * @file
 * Custom management of the commerce_price field.
 */

class FieldItemLink extends DefaultField {

  /**
   * @inheritdoc
   */
  public function sanitize($values) {
    foreach ($values as &$value) {
      $value = array(
        'url' => (string) $value,
      );
    }

    return $values;
  }

}
