<?php

/**
 * @file
 * Coupon..
 *
 * This file contains...
 */

class CommerceCouponType extends Entity {

  public $type;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'commerce_coupon_type');
  }

  /**
   * Returns whether the profile type is locked, thus may not be deleted or renamed.
   *
   * Profile types provided in code are automatically treated as locked, as well
   * as any fixed profile type.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}
