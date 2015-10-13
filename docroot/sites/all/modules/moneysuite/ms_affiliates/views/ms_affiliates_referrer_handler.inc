<?php

/**
 * @file
 */

class ms_affiliates_referrer_handler extends views_handler_field {
  function render($values) {
    $account = user_load($values->uid);
    $affiliate = user_load($account->affiliate_aid);
    return $affiliate->name;
  }
}
