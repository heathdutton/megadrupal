<?php

/**
 * @file
 */

class ms_affiliates_paypal_handler extends views_handler_field {
  function render($values) {
    $account = user_load($values->uid);
    return $account->ms_affiliates_paypal_address;
  }
}
