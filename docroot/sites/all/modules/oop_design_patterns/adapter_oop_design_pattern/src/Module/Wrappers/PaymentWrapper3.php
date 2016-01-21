<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 22/11/15
 * Time: 8:40 PM
 */

namespace Drupal\adapter_oop_design_pattern\Module\Wrappers;

use Drupal\adapter_oop_design_pattern\Module\Log\Log;
use Drupal\adapter_oop_design_pattern\Module\Native\PaymentProvider3\CreditHandler;

class PaymentWrapper3 implements PaymentWrapperInterface {

  function doPayment($card_number, $expiry, $cvv) {
    Log::write('<b>Run doPayment() in PaymentWrapper3</b>');

    Log::write('Create an instance of CreditHandler');
    $credit_handler = new CreditHandler();
    Log::write('Run doCredit in CreditHandler');
    $credit_handler->doCredit($card_number, $cvv, $expiry);
  }

}
