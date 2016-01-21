<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 22/11/15
 * Time: 8:40 PM
 */

namespace Drupal\adapter_oop_design_pattern\Module\Wrappers;

use Drupal\adapter_oop_design_pattern\Module\Log\Log;
use Drupal\adapter_oop_design_pattern\Module\Native\PaymentProvider2\CreditCard;
use Drupal\adapter_oop_design_pattern\Module\Native\PaymentProvider2\Processor;

class PaymentWrapper2 implements PaymentWrapperInterface {

  /**
   * @param $card_number
   * @param $expiry
   * @param $cvv
   */
  function doPayment($card_number, $expiry, $cvv) {
    Log::write('<b>Run doPayment() in PaymentWrapper2</b>');

    Log::write('Create an instance of CreditCard');
    $card = new CreditCard($card_number, $expiry, $cvv);
    Log::write('Create an instance of Processor');
    $processor = new Processor();
    Log::write('Run pay() in Processor');
    $processor->pay($card);
  }

}
