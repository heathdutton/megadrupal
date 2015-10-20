<?php

/**
* @file
* Field handler for Image fields.
*/

namespace Drupal\maps_import\Mapping\Target\Drupal\Field;

/**
 * Drupal Field class for field which type is "image".
 */
class FieldItemImage extends FieldItemFile {

  /**
   * @inheritdoc
   */
  public function sanitize($value) {
    if (is_numeric($value)) {
      $file = file_load((int) $value);
      
      return $file ? get_object_vars($file) : NULL;
    }
  }

}

