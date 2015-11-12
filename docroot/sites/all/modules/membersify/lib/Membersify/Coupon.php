<?php

/**
 * Membersify_Coupon class.
 */
class Membersify_Coupon extends Membersify_Adjustment {
  protected static $object = 'coupon';

  public $code = '';
  public $max_uses = 0;
  public $used = 0;
  public $expiration = 0;
  public $minimum_order = 0.00;
}
