<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 22/11/15
 * Time: 8:44 PM
 */

namespace Drupal\adapter_oop_design_pattern\Module\Native\PaymentProvider1;

use Drupal\adapter_oop_design_pattern\Module\Log\Log;

class PaymentComponent {

  public static function process($credit_card_num, $credit_card_expiry, $credit_card_cvv) {
    // Payment handling code is omitted.
  }

}
