<?php

/**
* @file
* Field handler for File fields.
*/

namespace Drupal\maps_import\Mapping\Target\Drupal\Field;

/**
 * Drupal Field class for field which type is "file".
 */
class FieldItemFile extends DefaultField {

  /**
   * @inheritdoc
   */
  public function sanitize($value) {
    $return = array();

    if (is_numeric($value) && $file = file_load((int) $value)) {
      $return = get_object_vars($file);
      $return['display'] = 1;
    }

    return $return;
  }

}
