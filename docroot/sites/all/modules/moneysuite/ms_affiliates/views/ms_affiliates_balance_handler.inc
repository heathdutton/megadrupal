<?php

/**
 * @file
 */

class ms_affiliates_balance_handler extends views_handler_field {
  function render($values) {
    $account = user_load($values->uid);
    return ms_core_format_money($account->af_balance);
  }
}
