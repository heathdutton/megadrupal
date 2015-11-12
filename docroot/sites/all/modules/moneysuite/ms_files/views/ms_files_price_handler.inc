<?php
/**
 * @file
 */

class ms_files_price_handler extends views_handler_field {
  function render($values) {
    $amount = $values->ms_files_profiles_amount;
    return ms_core_format_money($amount);
  }
}
