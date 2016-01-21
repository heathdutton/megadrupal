<?php
/**
 * @file
 * API payment payulatam
 */

/**
 * Page response.
 */
function commerce_payulatam_response($order) {
  $param = commerce_payulatam_get_param($_GET);

  if ($param['transactionState'] == 6 && $param['polResponseCode'] == 5) {
    $message = 'Failed transaction';
  }
  elseif ($param['transactionState'] == 6 && $param['polResponseCode'] == 4) {
    $message = 'Transaction rejected';
  }
  elseif ($param['transactionState'] == 12 && $param['polResponseCode'] == 9994) {
    $message = 'Pending, Please check whether the debit was made in the Bank';
  }
  elseif ($param['transactionState'] == 4 && $param['polResponseCode'] == 1) {
    $message = 'Transaction approved';
  }
  else {
    $message = $param['mensaje'];
  }

  $rows = array(
    array(t('Transaction state'), $message),
    array(t('Transaction ID'), $param['transactionId']),
    array(t('Sale reference'), $param['reference_pol']),
    array(t('Transaction Reference'), $param['referenceCode']),
    array(t('Ammount'), number_format($param['TX_VALUE'])),
    array(t('Currency'), $param['currency']),
    array(t('Description'), $param['description']),
    array(t('Entity'), $param['lapPaymentMethod']),
  );

  if ($param['pseBank']) {
    $rows[] = array(t('CUS'), $param['cus']);
    $rows[] = array(t('Bank'), $param['pseBank']);
  }

  $content['table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
  );

  drupal_alter('commerce_payulatam_response', $content, $param);

  return $content;
}

/**
 * Page confirmation.
 */
function commerce_payulatam_confirmation($order) {
  $result = array(
    'status' => 1,
    'message' => t('Confirmation success'),
  );

  try {
    $param = commerce_payulatam_get_param($_POST);
    $transaction = commerce_payulatam_save_transation($param, $order);

    $payment = commerce_payment_method_instance_load($order->data['payment_method']);

    $default_status = array(
      COMMERCE_PAYMENT_STATUS_SUCCESS => 'pending',
      COMMERCE_PAYMENT_STATUS_FAILURE => 'canceled',
    );

    if (!isset($payment['settings']['py_assign_status']) || $payment['settings']['py_assign_status'] == 'M') {
      $status = $default_status[$transaction->status];

      if (isset($payment['settings']['py_status_' . $transaction->status])) {
        $status = $payment['settings']['py_status_' . $transaction->status];
      }

      commerce_order_status_update($order, $status);
      if ($transaction->status == COMMERCE_PAYMENT_STATUS_SUCCESS) {
        commerce_checkout_complete($order);
      }
    }
    elseif ($transaction->status == COMMERCE_PAYMENT_STATUS_SUCCESS) {
      commerce_payment_redirect_pane_next_page($order);
    }
    else {
      commerce_payment_redirect_pane_previous_page($order);
    }
    module_invoke_all('commerce_payulatam_confirmation', $order, $transaction);
  }
  catch (Exception $e) {
    watchdog('commerce_payulatam', $e->getMessage(), array(), WATCHDOG_NOTICE, current_path());
    $result['message'] = $e->getMessage();
  }

  return $result;
}

/**
 * Save transaction.
 */
function commerce_payulatam_save_transation($param, $order) {
  $exchange_money = $param['value'];
  $amount = commerce_currency_decimal_to_amount($exchange_money, $param['currency']);

  $result = db_insert('commerce_payulatam')
    ->fields(array(
      'order_id' => $order->order_id,
      'created' => REQUEST_TIME,
      'reference_payulatam' => $param['reference_pol'],
      'state_transaction' => $param['state_pol'],
      'value' => $amount,
      'response' => serialize($param),
    ))
    ->execute();

  $transaction = commerce_payment_transaction_new('payulatam', $order->order_id);

  $transaction->payload[REQUEST_TIME] = $param;

  $message = array();

  $transaction->remote_id = $param['transaction_id'];

  $transaction->remote_status = $param['response_message_pol'];

  switch ($param['state_pol']) {
    case 4:
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      break;

    default:
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
  }

  $transaction->instance_id = $param['extra1'];
  $transaction->amount = $amount;
  $transaction->currency_code = $param['currency'];
  $transaction->message = implode('<br />', $message);
  commerce_payment_transaction_save($transaction);

  return $transaction;
}

/**
 * Generate url response and confirmation.
 */
function commerce_payulatam_get_url($order_id) {
  $token = uniqid();

  $url_response = COMMERCE_PAYULATAM_URL_RESPONSE . '/' . $order_id . '/' . $token . '/' . commerce_payulatam_get_md5($order_id, $token, 'RESPONSE');

  $url_confirmation = COMMERCE_PAYULATAM_URL_CONFIRMATION . '/' . $order_id . '/' . $token . '/' . commerce_payulatam_get_md5($order_id, $token);

  return array(
    'response' => url($url_response, array(
      'absolute' => TRUE,
    )),
    'confirmation' => url($url_confirmation, array(
      'absolute' => TRUE,
    )),
  );
}

/**
 * Generate number reference unique.
 */
function commerce_payulatam_get_reference($alias, $order_id) {
  return $alias . $order_id;
}

/**
 * Digital signature of the transaction.
 */
function commerce_payulatam_get_firm($settings, $state_transation = NULL) {
  $params = array(
    $settings['py_api_key'],
    $settings['py_merchant_id'],
    $settings['sale_reference'],
    $settings['amount'],
    $settings['currency_code'],
  );

  if ($state_transation) {
    $params[] = $state_transation;
  }

  return md5(implode('~', $params));
}

/**
 * Encrypted key.
 */
function commerce_payulatam_get_md5($order_id, $token, $type = 'CONFIRMATION') {
  return md5($order_id . '~' . strtoupper($type) . '~' . $token);
}