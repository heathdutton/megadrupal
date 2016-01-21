<?php
/**
 * @file
 * Giant Bomb data property handler.
 */

class GiantBombTextProperty extends GiantBombBaseProperty {
  function value() {
    return check_plain($this->value);
  }

  function validate() {
    return TRUE;
  }
}