<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 22/11/15
 * Time: 8:54 PM
 */

namespace Drupal\adapter_oop_design_pattern\Module\Native\PaymentProvider3;

use Drupal\adapter_oop_design_pattern\Module\Log\Log;

class CreditHandler {

  public function doCredit($card_num, $cvv, $expiry) {
    // Payment handling code is omitted.
  }

}
