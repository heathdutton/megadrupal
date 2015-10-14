<?php

/**
 * @file
 * Defines the actions of adding money to ones account.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_account_balance_rules_action_info() {
  $currency = commerce_currency_load();

  return array(
    'commerce_account_balance_deposit_to_account' => array(
      'label' => t("Deposit money to a user's account"),
      'parameter' => array(
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
          'optional' => FALSE,
          'allow null' => FALSE,
        ),
        'amount' => array(
          'label' => t('Amount in @currency', array('@currency' => $currency['symbol'])),
          'type' => 'decimal',
          'allow null' => FALSE,
          'optional' => FALSE,
          'description' => 'A decimal amount of money to add to the balance',
        ),
        'commerce_order' => array(
          'label' => t('Order'),
          'description' => 'The order this transaction belongs to (optional)',
          'type' => 'commerce_order',
          'optional' => TRUE,
          'allow null' => TRUE,
        ),
      ),
      'group' => t('Commerce Account Balance'),
    ),
  );
}

/**
 * Action callback for depositing money to ones account.
 */
function commerce_account_balance_deposit_to_account($user, $amount_decimal, $order) {
  $amount = round($amount_decimal * 100);
  $uid = $user->uid;

  $currency = commerce_currency_load();
  $formatted_amount = commerce_currency_format($amount, $currency['code']);

  $transaction = entity_create('commerce_account_balance_transaction', array(
      'type' => 'deposit',
      'action' => t('Deposit of @amount to your account', array('@amount' => $formatted_amount)),
      'uid' => $uid,
      'time' => REQUEST_TIME,
      'amount' => $amount,
      'order_id' => $order ? $order->order_id : NULL,
    ));

  $transaction_wrapper = entity_metadata_wrapper('commerce_account_balance_transaction', $transaction);
  $transaction_wrapper->save();
}
