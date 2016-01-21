<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 22/11/15
 * Time: 8:40 PM
 */

namespace Drupal\adapter_oop_design_pattern\Module\Wrappers;


use Drupal\adapter_oop_design_pattern\Module\Log\Log;
use Drupal\adapter_oop_design_pattern\Module\Native\PaymentProvider1\PaymentComponent;

class PaymentWrapper1 implements PaymentWrapperInterface {

  function doPayment($card_number, $expiry, $cvv) {
    Log::write('<b>Run doPayment() in PaymentWrapper1</b>');

    Log::write('Run process() in PaymentComponent');
    PaymentComponent::process($card_number, $expiry, $cvv);
  }

}
