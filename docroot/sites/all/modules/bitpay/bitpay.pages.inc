<?php

/**
 * Page callback for BitPay notification URL.
 */
function bitpay_callback() {
  bitpay_initialize();
  $invoice = bpVerifyNotification();
  if (variable_get('bitpay_debug', FALSE)) {
    watchdog('debug', 'Receiving BitPay invoice status update: %invoice.', array('%invoice' => print_r($invoice, TRUE)), WATCHDOG_DEBUG);
  }
  if (!is_array($invoice)) {
    watchdog('bitpay', 'Error while processing BitPay invoice update notification: %message.', array('%message' => strval($invoice)), WATCHDOG_ERROR);
    return MENU_ACCESS_DENIED;
  }
  // Update the record for this invoice.
  $record['btc_price'] = $invoice['btcPrice'];
  $record['status'] = $invoice['status'];
  bitpay_write_record($record, $invoice['id']);
  module_invoke_all('bitpay_invoice_update', $invoice);
}
