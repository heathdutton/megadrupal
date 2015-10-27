<?php
/**
 * @file
 * Response page callback for Commerce OTP Credit card payment services.
 */

/**
 * Payment response/backUrl - bank redirect customer here from bank interface.
 */
function commerce_otp_response_page() {
  $remote_transaction_id = $_REQUEST['tranzakcioAzonosito'];
  module_load_include('inc', 'commerce_otp', 'commerce_otp.fiz3_control.php');
  $response = _commerce_otp_payment_process_backurl();

  $transaction_id = commerce_otp_get_payment_transaction($remote_transaction_id);
  $transaction = commerce_payment_transaction_load($transaction_id);
  $transaction->revision = TRUE;
  $transaction->log = 'response page';
  $order = commerce_order_load($transaction->order_id);

  if (is_null($response)) {
    $transaction->remote_status = COMMERCE_OTP_TR_STATUS_UNKNOWN_ERROR;
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    commerce_payment_transaction_save($transaction);
    commerce_payment_redirect_pane_previous_page($order);
    drupal_goto(commerce_checkout_order_uri($order));
  }
  else {
    $success_codes = array(
      '000', '001', '002', '003', '004', '005', '006', '007', '008', '009', '010',
    );
    module_load_include('inc', 'commerce_otp', 'commerce_otp.admin');
    if (in_array($response->posValaszkod, $success_codes)) {
      commerce_otp_response_success($transaction, $response->posValaszkod, $order);
    }
    elseif ($response->posValaszkod == "VISSZAUTASITOTTFIZETES") {
      commerce_otp_response_cancel($transaction, $response->posValaszkod, $order);
    }
    else {
      commerce_otp_response_error($transaction, $response->posValaszkod, $order);
    }
  }
}
