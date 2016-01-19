<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 22/11/15
 * Time: 8:47 PM
 */

namespace Drupal\adapter_oop_design_pattern\Module\Native\PaymentProvider2;

use Drupal\adapter_oop_design_pattern\Module\Log\Log;

class CreditCard {

  public $card_number;
  public $expiry;
  public $cvv;

  function __construct($card_number, $expiry, $cvv) {
    $this->card_number = $card_number;
    $this->expiry = $expiry;
    $this->cvv = $cvv;
  }

}
