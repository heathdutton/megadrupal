<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 22/11/15
 * Time: 8:35 PM
 */

namespace Drupal\adapter_oop_design_pattern\Module\Wrappers;

interface PaymentWrapperInterface {

  function doPayment($card_number, $expiry, $cvv);

}
