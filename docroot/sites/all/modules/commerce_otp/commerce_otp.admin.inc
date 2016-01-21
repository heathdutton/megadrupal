<?php
/**
 * @file
 * Administration code for Commerce OTP Credit card payment services.
 */

/**
 * OTP's transaction log lister page.
 */
function commerce_otp_transactions_list() {
  $table = 'commerce_payment_transaction_revision';
  $table_schema = drupal_get_schema($table);
  $header['order_id'] = array('data' => 'order_id', 'field' => 'order_id');
  foreach ($table_schema['fields'] as $name => $data) {
    $header[$name] = array('data' => $name, 'field' => $name);
  }
  unset($header['message_variables']);
  $header['transaction_id']['sort'] = 'desc';
  $fields = array_keys($table_schema['fields']);
  $query = db_select($table, 'tr');
  $query->join('commerce_payment_transaction', 't', 't.revision_id=tr.revision_id');
  $query->condition('t.payment_method', 'otp', '=');
  $query->fields('t', array('order_id'));
  $query->fields('tr', $fields);
  $table_sort = $query->extend('TableSort')->orderByHeader($header);
  $pager = $table_sort->extend('PagerDefault')->limit(50);
  $result = $pager->execute();
  foreach ($result as $res) {
    $res->message = format_string($res->message, unserialize($res->message_variables));
    unset($res->message_variables);
    $row = array();
    foreach ($res as $data) {
      $row[] = $data;
    }
    $rows[] = $row;
  }
  $output = '';
  if (isset($rows)) {
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('id' => 'sort-table'),
    ));
    $output .= theme('pager');
  }
  else {
    $output .= t('No transaction revisions.');
  }
  return $output;
}

/**
 * Check (ask and update status) not processed transactions from OTP bank.
 *
 * @see commerce_otp_cron()
 */
function commerce_otp_process_transactions() {
  $lastrun = variable_get('commerce_otp_cron_lastrun', 0);
  if ($lastrun >= REQUEST_TIME) {
    return;
  }
  $transactions = commerce_otp_get_unprocessed_transactions();
  if (empty($transactions)) {
    return;
  }

  module_load_include('inc', 'commerce_otp', 'commerce_otp.fiz3_control.php');
  foreach ($transactions as $transaction) {
    $data = unserialize($transaction->data);
    if (!$data['pos_id']) {
      continue;
    }
    $date = $transaction->changed ? $transaction->changed : $transaction->created;

    $service = new CommerceOTPWebShopService($data['pos_id']);
    $response = $service->tranzakcioStatuszLekerdezes($data['pos_id'], $transaction->remote_id, 1, $date - 60 * 60 * 24, $date + 60 * 60 * 24);
    if ($response) {
      $answer = $response->getAnswer();
      $order = commerce_order_load($transaction->oder_id);
      if ($response->isSuccessful() && $response->getAnswer() && count($answer->getWebShopFizetesAdatok()) > 0) {
        $transaction_data = current($answer->getWebShopFizetesAdatok());
        $response_code = $transaction_data->getPosValaszkod();

        $transaction = commerce_payment_transaction_load($transaction->transaction_id);
        $transaction->revision = TRUE;
        $transaction->log = 'cron update';

        if ($transaction_data->isSuccessful()) {
          commerce_otp_response_success($transaction, $response->posValaszkod, $order, TRUE);
        }
        elseif ($response_code == "VISSZAUTASITOTTFIZETES") {
          commerce_otp_response_cancel($transaction, $response->posValaszkod, $order, TRUE);
        }
        else {
          commerce_otp_response_error($transaction, $response->posValaszkod, $order, TRUE);
        }
      }
    }
  }
  variable_set('commerce_otp_cron_lastrun', REQUEST_TIME);
}

/**
 * Get OTP's unprocessed transactions.
 *
 * @param string $remote_transaction_id
 *   Remote transaction ID.
 *
 * @return array
 *   An associative array, or an empty array if there is no result.
 */
function commerce_otp_get_unprocessed_transactions($remote_transaction_id = NULL) {
  $query = db_select('commerce_payment_transaction', 't');
  $query->join('rules_config', 'rc', "CONCAT(t.payment_method, '|', rc.name)=t.instance_id");
  $query->fields('t');
  $query->condition('rc.active', 1, '=');
  $query->condition('t.remote_status', array(
    COMMERCE_OTP_TR_STATUS_PROCESSED,
    COMMERCE_OTP_TR_STATUS_SENT,
    COMMERCE_OTP_TR_STATUS_UNKNOWN_ERROR,
  ), 'IN');
  if ($remote_transaction_id) {
    $query->condition('t.remote_id', $remote_transaction_id, '=');
  }
  $result = $query->execute()->fetchAllAssoc('transaction_id');
  return $result;
}

/**
 * On Success response.
 *
 * Update transactions, redirect user to next page when order is defined.
 */
function commerce_otp_response_success($transaction, $response_code, $order, $cron = FALSE) {
  $transaction->remote_status = COMMERCE_OTP_TR_STATUS_SUCCESS;
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  $transaction->message = t('Payment completed successfully');
  commerce_payment_transaction_save($transaction);
  commerce_payment_redirect_pane_next_page($order);
  if (!$cron) {
    drupal_goto(commerce_checkout_order_uri($order));
  }
}

/**
 * On Cancelled response.
 *
 * Update transactions, redirect user to checkout page when order is defined.
 */
function commerce_otp_response_cancel($transaction, $response_code, $order, $cron = FALSE) {
  $error_message = t('Payment cancelled by customer');
  $transaction->remote_status = COMMERCE_OTP_TR_STATUS_CANCELLED;
  $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  $transaction->message = $error_message;
  commerce_payment_transaction_save($transaction);
  commerce_payment_redirect_pane_previous_page($order);
  if (!$cron) {
    drupal_set_message($error_message, 'warning');
    drupal_goto(commerce_checkout_order_uri($order));
  }
}

/**
 * On Error response.
 *
 * Update transactions, redirect user to checkout page when order is defined.
 */
function commerce_otp_response_error($transaction, $response_code, $order, $cron = FALSE) {
  $error_message = t('Payment failed');
  $transaction->remote_status = COMMERCE_OTP_TR_STATUS_FAILED;
  $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  $transaction->message = $error_message;
  $transaction->data['error_code'] = $response_code;
  commerce_payment_transaction_save($transaction);
  commerce_payment_redirect_pane_previous_page($order);
  if (!$cron) {
    drupal_set_message(t('Payment failed at the payment server. Please review your information and try again.'), 'error');
    drupal_set_message($error_message, 'warning');
    drupal_goto(commerce_checkout_order_uri($order));
  }
}
