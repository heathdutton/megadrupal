<?php
namespace Drupal\go1_base\TypedData\DataTypes;

class Integer extends Base {
  public function isEmpty() {
    if (!is_null($this->value)) {
      return $this->value === 0;
    }
  }

  public function validateInput(&$error = NULL) {
    if (!is_int($this->value)) {
      $error = 'Input is not an integer value.';
      return FALSE;
    }
    return parent::validateInput($error);
  }
}
