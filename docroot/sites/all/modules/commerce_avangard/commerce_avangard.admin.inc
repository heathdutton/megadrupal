<?php

/**
 * @file
 * Contains admin area menu callbacks and forms.
 */

/**
 * Form that asks user confirmation for order refund.
 *
 * @param array $form
 * @param array $form_state
 * @param object $order
 * @param object $transaction
 *
 * @return array
 */
function commerce_avangard_transaction_refund_form($form, &$form_state, $order, $transaction) {
  $form_state['order'] = $order;
  $form_state['transaction'] = $transaction;

  $transactions_balances = _commerce_avangard_get_order_transactions_balances($order, $transaction->remote_id);
  $amount_to_refund = $transactions_balances[$transaction->remote_id]['balance'];
  $form_state['amount'] = $amount_to_refund;

  $form['#submit'][] = 'commerce_avangard_transaction_refund_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to refund this transaction?'),
    "admin/commerce/orders/{$order->order_id}/payment",
    '<p>' . t('@amount paid via Avangard on @date.', array(
      '@amount' => commerce_currency_format($amount_to_refund, $transaction->currency_code),
      '@date' => format_date($transaction->created, 'short')
    )) . '</p>',
    t('Refund'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Implements hook_FORM_ID_form_submit().
 */
function commerce_avangard_transaction_refund_form_submit($form, &$form_state) {
  $result = commerce_avangard_transaction_refund($form_state['order'], $form_state['transaction'], $form_state['amount']);
  if ($result) {
    drupal_set_message(t('Transaction refunded successfully'));
    $form_state['redirect'] = "admin/commerce/orders/{$form_state['order']->order_id}/payment";
  }
}

/**
 * Displays list of transactions for the given date (uses today as default date).
 *
 * @param string $date
 *
 * @return string
 */
function commerce_avangard_date_operations($date = '') {
  if ($date === '') {
    $date = date('d.m.Y');
  }
  $response = _commerce_avangard_order_get_date_operations_list($date);
  if (isset($response['oper_info'])) {
    $bank_operations = isset($response['oper_info']['id']) ? array($response['oper_info']) : $response['oper_info'];

    // Here we suppose that all transactions are made in site's deafult currency.
    $currency = commerce_default_currency();
    $rows = array_map(function ($operation) use($currency) {
      $link = "/admin/commerce/orders/{$operation['order_number']}/payment";
      return array(
        $operation['id'],
        "<a href='{$link}'>{$operation['order_number']}</a>",
        $operation['status_date'],
        commerce_currency_format($operation['amount'], $currency),
        $operation['status_code'],
        $operation['status_desc'],
        $operation['ticket'],
        isset($operation['card_num']) ? "{$operation['card_num']} ({$operation['exp_mm']}/{$operation['exp_yy']})" : '',
      );
    }, $bank_operations);
    $header = array(
      t('Id'),
      t('Order Id'),
      t('Time'),
      t('Sum'),
      t('Status code'),
      t('Status'),
      t('Ticket'),
      t('Extra'),
    );
  }
  else {
    $rows = $header = array();
  }

  return theme('table', array(
    'rows' => $rows,
    'header' => $header,
    'caption' => $response['response_message'],
  ));
}
