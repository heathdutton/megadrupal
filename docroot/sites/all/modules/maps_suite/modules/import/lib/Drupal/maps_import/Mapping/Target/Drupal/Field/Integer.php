<?php

namespace Drupal\maps_import\Mapping\Target\Drupal\Field;

/**
 * @file
 * Custom management of an Integer field.
 */

class Integer extends DefaultField {

  /**
   * @inheritdoc
   */
  public function sanitize($values) {
    // Handle case of object properties that are integers ("$values" is not an
    // array in such case).
    if (!is_array($values)) {
      return $this->isInteger($values) ? (int) $values : NULL;
    }

    // Ensure the data is an integer or a boolean!
    $integers = array();

    foreach ($values as $value) {
      if ($this->isInteger($value)) {
        $integers[] = (int) $value;
      }
    }

    return $integers;
  }

  /**
   * Check whether the given value is an integer or not.
   *
   * @return bool
   */
  protected function isInteger($value) {
    return (is_int($value) || (is_numeric($value) && intval($value) == $value) || is_bool($value));
  }

}
