<?php
/**
 * @file
 * Implements OTP's Three-participant payment transactions.
 *
 * Based on fiz3_controll.php in OTP's Webservice 4.0 pack.
 *
 * WS_CUSTOMERPAGE_CHAR_ENCODING, WEBSHOP_LIB_DIR needed for otpwebshop library.
 */

define('WS_CUSTOMERPAGE_CHAR_ENCODING', 'UTF-8');
define('WEBSHOP_LIB_DIR', libraries_get_path('otpwebshop') . '/lib');

define('COMMERCE_OTP_WEBSHOP_CUSTOMER_URL', 'https://www.otpbankdirekt.hu/webshop/do/webShopVasarlasInditas');
require_once drupal_get_path('module', 'commerce_otp') . '/CommerceOTPWebShopService.php';

/**
 * Process Three-participant payment.
 *
 * Based on process() function in fiz3_control.php
 *
 * @param array $data
 *   Configuration data.
 *
 * @return array
 *   WS response array or NULL.
 */
function _commerce_otp_payment_process(array $data) {
  ob_start();
  $service = new CommerceOTPWebShopService($data['posid']);
  $posid = $data['posid'];
  $transactionid = $data['transactionid'];
  $languagecode = $data['languagecode'];

  if (is_null($transactionid) || (trim($transactionid) == "")) {
    _commerce_otp_payment_process_backurl();
    return NULL;
  }

  // Set location with params where customer will redirected.
  $location = url(COMMERCE_OTP_WEBSHOP_CUSTOMER_URL, array(
      'query' => array(
        'posId' => $posid,
        'azonosito' => $transactionid,
        'nyelvkod' => $languagecode,
      ),
    )
  );

  header("Connection: close");
  header("Location: " . $location);
  header("Content-Length: " . ob_get_length());
  ob_end_flush();
  flush();

  // Set back URL params - required by OTP bank service.
  $back_url = $data['backurl'];
  if (!is_null($back_url) && trim($back_url) != '') {
    $back_url = url($back_url, array(
        'query' => array(
          'fizetesValasz' => 'true',
          'posId' => $posid,
          'tranzakcioAzonosito' => $transactionid,
        ),
      )
    );
  }

  $transaction_id = commerce_otp_get_payment_transaction($transactionid);
  $transaction = commerce_payment_transaction_load($transaction_id);
  $transaction->remote_status = COMMERCE_OTP_TR_STATUS_SENT;
  $transaction->message = commerce_otp_status_message(COMMERCE_OTP_TR_STATUS_SENT);
  $transaction->revision = TRUE;
  $transaction->log = 'sent to bank';
  commerce_payment_transaction_save($transaction);

  // Call WS payment transaction with params.
  $response = $service->fizetesiTranzakcio(
    $posid,
    $transactionid,
    $data['amount'],
    $data['exchange'],
    $data['languagecode'],
    isset($data['name_needed']) ? $data['name_needed'] : FALSE,
    isset($data['country_needed']) ? $data['country_needed'] : FALSE,
    isset($data['county_needed']) ? $data['county_needed'] : FALSE,
    isset($data['settlement_needed']) ? $data['settlement_needed'] : FALSE,
    isset($data['zipcode_needed']) ? $data['zipcode_needed'] : FALSE,
    isset($data['street_needed']) ? $data['street_needed'] : FALSE,
    isset($data['mailaddress_needed']) ? $data['mailaddress_needed'] : FALSE,
    isset($data['narration_needed']) ? $data['narration_needed'] : FALSE,
    isset($data['consumerreceipt_needed']) ? $data['consumerreceipt_needed'] : FALSE,
    isset($data['consumerregistration_needed']) ? $data['consumerregistration_needed'] : FALSE,
    isset($data['consumerregistrationid']) ? $data['consumerregistrationid'] : NULL,
    isset($data['shopcomment']) ? $data['shopcomment'] : '',
    $back_url,
    isset($data['cardpocketid']) ? $data['cardpocketid'] : NULL,
    isset($data['twostaged']) ? $data['twostaged'] : FALSE
  );

  if ($response) {
    watchdog('commerce_otp', "Three-participant payment process: $posid - $transactionid - " . implode($response->getMessages()));
  }
  else {
    watchdog('commerce_otp', "Three-participant payment process: $posid - $transactionid - No response");
  }

  return $response;
}

/**
 * Get payment transaction data.
 *
 * Based on processDirectedToBackUrl() function in fiz3_control.php
 *
 * @return WebShopFizetesAdatok|NULL
 *   transaction data
 */
function _commerce_otp_payment_process_backurl() {
  module_load_include('inc', 'commerce_otp', 'commerce_otp.admin');
  $transaction_data = NULL;
  $pos_id = $_REQUEST['posId'];
  $transaction_id = $_REQUEST['tranzakcioAzonosito'];
  if (!is_null($transaction_id) && (trim($transaction_id) != "") && commerce_otp_get_unprocessed_transactions($transaction_id)) {
    // Get payment transaction data in time interval.
    $service = new CommerceOTPWebShopService($pos_id);
    $response = $service->tranzakcioStatuszLekerdezes($pos_id, $transaction_id, 1, time() - 60 * 60 * 24, time() + 60 * 60 * 24);
    if ($response) {
      $answer = $response->getAnswer();
      if ($response->isSuccessful()
        && $response->getAnswer()
        && count($answer->getWebShopFizetesAdatok()) > 0) {
        // Successfully get transactions data.
        $payment_data = $answer->getWebShopFizetesAdatok();
        $transaction_data = current($payment_data);

        watchdog('commerce_otp', "Success on getting payment transaction data for: $pos_id - $transaction_id");
      }
    }
    else {
      watchdog('commerce_otp', "Fail on getting payment transaction data for: $pos_id - $transaction_id");
    }
  }
  return $transaction_data;
}
