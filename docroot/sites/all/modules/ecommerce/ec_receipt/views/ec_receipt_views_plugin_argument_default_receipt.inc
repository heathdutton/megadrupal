<?php

/**
 * @file
 * Implement determining receipt id from URL.
 */

class ec_receipt_views_plugin_argument_default_receipt extends views_plugin_argument_default {
  function get_argument() {
    $receipt = menu_get_object('ec_receipt', 2);
    if (!empty($receipt)) {
      return $receipt->erid;
    }
  }
}
