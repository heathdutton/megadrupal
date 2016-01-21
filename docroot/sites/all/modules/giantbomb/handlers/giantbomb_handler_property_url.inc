<?php
/**
 * @file
 * Giant Bomb data property handler.
 */

class GiantBombURLProperty extends GiantBombBaseProperty {
  function validate() {
    return valid_url($this->value, TRUE);
  }

  function value() {
    return check_plain($this->value);
  }
}