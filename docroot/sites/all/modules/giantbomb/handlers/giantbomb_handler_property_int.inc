<?php
/**
 * @file
 * Giant Bomb data property handler.
 */

class GiantBombIntProperty extends GiantBombBaseProperty {
  function value() {
    return check_plain($this->value);
  }

  function validate() {
    return is_numeric($this->value);
  }
}