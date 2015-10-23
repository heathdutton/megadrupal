<?php

/**
 * @file
 * Flo2Cash library.
 */

/**
 * Verify a transaction against F2C server.
 *
 * @TODO Rewrite using drupal_http_request() instead of cURL.
 */
function uc_flo2cash_verify_transaction($input) {
  if (!isset($input['transaction_id'])) {
    watchdog('uc_flo2cash', 'Transaction process callback requested without transaction ID.', array(), WATCHDOG_ERROR);
    return FALSE;
  }

  $postdata = file_get_contents('php://input');
  $postdata = str_replace("txn_id", "transaction_id", $postdata);
  $postdata = str_replace("txn_status", "transaction_status", $postdata);
  $postdata = str_replace("receipt_no", "receipt_id", $postdata);
  // watchdog('uc_flo2cash', 'process:  raw POST: '.$postdata, WATCHDOG_NOTICE);
  $postdata .= '&cmd=_xverify-transaction';
  // Submit the verification request.
  $ch = curl_init(variable_get('uc_flo2cash_web2pay_verify_url', 'FIXME'));
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $postdata);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  // watchdog('uc_flo2cash', 'process:  postdata: '.$postdata, WATCHDOG_NOTICE);
  $response = curl_exec($ch);
  curl_close($ch);

  if (isset($_POST['txn_id']) && !empty($_POST['txn_id'])) {
    $transaction_id = $_POST['txn_id'];
  }

/*
  // rename the first three fields
  $data = array(
    'verifier' => $input['verifier'],
    'transaction_id' => $input['txn_id'],
    'transaction_status' => $input['txn_status'],
    'response_text' => $input['response_text'],
    'receipt_id' => $input['receipt_no'],
  );
  foreach ($input as $k => $v) {
    $data[$k] = $v;
  }
  error_log(__LINE__ . ':' . print_r($data,1));
  $url = variable_get('uc_flo2cash_web2pay_verify_url', 'https://secure.flo2cash.co.nz/web2pay/MNSHandler.aspx');
  $data['cmd'] = '_xverify-transaction';
  $data = http_build_query($data, '', '&');
  $request = drupal_http_request($url, array(), 'POST', $data);
  error_log(__LINE__ . ':' . var_export($data, 'data'));
  error_log(__LINE__ . ':' . $request->data);

  if (isset($_POST['txn_id']) && !empty($_POST['txn_id'])) {
    $transaction_id = $_POST['txn_id'];
  }

  if ($request->data != 'VERIFIED') {
    watchdog('uc_flo2cash', 'Payment verification failed. Transaction ID was @txn_id, response was: <pre>@response</pre>.', array('@txn_id' => $_POST['txn_id'], '@response' => $request->data), WATCHDOG_ERROR);
  }
*/
  if ($response == 'VERIFIED') {
    return TRUE;
  }
  else {
    watchdog('uc_flo2cash', 'Payment verification failed. Transaction ID was @txn_id, response was @response.', array('@txn_id' => $_POST['txn_id'], '@response' => $response), WATCHDOG_ERROR);
    return FALSE;
  }
}
