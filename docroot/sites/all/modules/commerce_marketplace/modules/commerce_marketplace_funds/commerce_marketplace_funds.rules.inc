<?php

/**
 * @file
 * Rules actions and conditions for working with funds accounts and transactions.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_marketplace_funds_rules_action_info() {
  $actions = array(
    'commerce_marketplace_funds_rules_create_transaction' => array(
      'label' => t('Create a new Funds Transaction'),
      'group' => t('Commerce Marketplace Funds'),
      'callbacks' => array(
        'validate' => 'commerce_marketplace_funds_rules_create_transaction_validate',
        'access' => 'commerce_marketplace_funds_rules_create_transaction_access',
      ),
      'parameter' => array(
        'transaction_type' => array(
          'type' => 'text',
          'options list' => 'commerce_marketplace_funds_transaction_types',
          'label' => t('Transaction Type'),
        ),
        'amount' => array(
          'type' => 'decimal',
          'label' => t('Transaction amount'),
        ),
        'destination' => array(
          'type' => 'cmp_funds_account',
          'label' => t('Destination account'),
          'optional' => TRUE,
        ),
        'status' => array(
          'type' => 'text',
          'label' => t('Transaction status'),
          'options list' => 'commerce_marketplace_funds_transaction_statuses',
        ),
        'originator' => array(
          'type' => 'cmp_funds_account',
          'label' => t('Originator account'),
          'optional' => TRUE,
        ),
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Order'),
          'optional' => TRUE,
        ),
        'notes' => array(
          'type' => 'text',
          'label' => t('Notes'),
          'optional' => TRUE,
        ),
      ),
      'provides' => array(
        'transaction' => array(
          'type' => 'cmp_funds_transaction',
          'label' => t('Transaction'),
        ),
      ),
    ),
  );

  $actions['commerce_marketplace_funds_rules_apply_transaction_fees'] = array(
    'label' => t('Apply fees to funds transaction'),
    'group' => t('Commerce Marketplace Funds'),
    'parameter' => array(
      'transaction' => array(
        'type' => 'cmp_funds_transaction',
        'label' => t('Transaction'),
      ),
      'fee_type' => array(
        'type' => 'text',
        'options list' => 'commerce_marketplace_funds_rules_fee_types',
        'label' => t('Fee type'),
      ),
      'fee' => array(
        'type' => 'decimal',
        'label' => t('Fee value'),
        'description' => t('If fee type is Percentage, enter a decimal number between 0 and 1, so 0.05 means 5%. If it\'s Flat rate, enter a positive decimal value. It\'s your responsibility to make sure that the flat rate is less than the transaction amount.')
      ),
    ),
  );

  $actions['commerce_marketplace_funds_rules_apply_transaction_to_accounts'] = array(
    'label' => t('Apply transaction to related accounts balance'),
    'group' => t('Commerce Marketplace Funds'),
    'parameter' => array(
      'transaction' => array(
        'type' => 'cmp_funds_transaction',
        'label' => t('Transaction'),
      ),
      'destination' => array(
        'type' => 'cmp_funds_account',
        'label' => t('Destination account'),
        'optional' => TRUE,
      ),
      'originator' => array(
        'type' => 'cmp_funds_account',
        'label' => t('Originator account'),
        'optional' => TRUE,
      ),
    ),
  );

  $actions['commerce_marketplace_funds_rules_account_load_by_owner'] = array(
    'label' => t('Load funds account using owner information'),
    'group' => t('Commerce Marketplace Funds'),
    'parameter' => array(
      'owner_type' => array(
        'type' => 'text',
        'options list' => 'commerce_marketplace_funds_account_types',
        'label' => t('Owner Type'),
      ),
      'owner_id' => array(
        'type' => 'integer',
        'label' => t('Owner ID'),
      ),
    ),
    'provides' => array(
      'funds_account' => array(
        'type' => 'cmp_funds_account',
        'label' => t('Funds Account'),
      ),
    ),
  );

  $actions['commerce_marketplace_funds_load_deposit_transaction_from_order'] = array(
    'label' => t('Load Deposit transaction from order'),
    'group' => t('Commerce Marketplace Funds'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => 'Order',
      ),
    ),
    'provides' => array(
      'funds_transaction' => array(
        'type' => 'cmp_funds_transaction',
        'label' => t('Funds transaction'),
      )
    )
  );

  return $actions;
}

/**
 * Returns a list of available fee types: Percentage and Flat rate.
 */
function commerce_marketplace_funds_rules_fee_types() {
  return array(
    'percent' => t('Percentage'),
    'flat' => t('Flat rate'),
  );
}

function commerce_marketplace_funds_rules_account_load_by_owner($owner_type, $owner_id) {
  return array('funds_account' => commerce_marketplace_funds_account_load_by_owner($owner_type, $owner_id));
}

/**
 * Access callback for creating a new funds transaction using Rules.
 */
function commerce_marketplace_funds_rules_create_transaction_access(RulesAbstractPlugin $element) {
  // @todo: Do we need to implement this?
  return TRUE;
}

/**
 * Makes sure that at least one of order and originator parameters are filled by the user.
 */
function commerce_marketplace_funds_rules_create_transaction_validate($element) {
  if (empty($element->settings['order:select']) && empty($element->settings['originator:select'])) {
    throw new RulesIntegrityException(t('Order and Originator account cannot be both empty.'), array(
      $element,
      'parameter',
      'originator'
    ));
  }
}

/**
 * Rules action callback for creating a new funds transaction.
 */
function commerce_marketplace_funds_rules_create_transaction($transaction_type, $amount, $destination, $status, $originator = NULL, $order = NULL, $notes = NULL) {
  // So, 1) Create transaction, 2) apply fees, 3) apply transaction to accounts, 4) save accounts
  $transaction = (object) array(
    'transaction_id' => '',
    'type' => $transaction_type,
    'from_account' => isset($originator->account_id) ? $originator->account_id : '',
    'to_account' => $destination->account_id,
    'order_id' => isset($order->order_id) ? $order->order_id : '',
    'status' => $status ? $status : FUNDS_TRANSACTION_PENDING,
  );
  $transaction->notes[LANGUAGE_NONE][0]['value'] = $notes;
  $transaction->amount[LANGUAGE_NONE][0]['amount'] = $amount;
  $transaction->amount[LANGUAGE_NONE][0]['currency_code'] = commerce_default_currency();

  return array('transaction' => $transaction);
}

/**
 * Apply fees to the transaction. Note that this function doesn't save the transaction.
 */
function commerce_marketplace_funds_rules_apply_transaction_fees($transaction, $fee_type, $fee_val) {
  if ($fee_type == 'percent') {
    $amount = $transaction->amount[LANGUAGE_NONE][0]['amount'];
    $fee = array(
      'amount' => -1 * $amount * $fee_val,
      'currency_code' => $transaction->amount[LANGUAGE_NONE][0]['currency_code'],
      'data' => array()
    );
    $transaction->amount[LANGUAGE_NONE][0]['amount'] += $fee['amount'];
    $transaction->amount[LANGUAGE_NONE][0]['data'] = commerce_price_component_add(
      $transaction->amount[LANGUAGE_NONE][0],
      'cmp_funds_fee',
      $fee,
      TRUE,
      TRUE
    );
  }
  elseif ($fee_type == 'flat') {
    $fee = array(
      'amount' => -1 * $fee_val,
      'currency_code' => $transaction->amount[LANGUAGE_NONE][0]['currency_code'],
      'data' => array()
    );
    $transaction->amount[LANGUAGE_NONE][0]['amount'] += $fee['amount'];
    $transaction->amount[LANGUAGE_NONE][0]['data'] = commerce_price_component_add(
      $transaction->amount[LANGUAGE_NONE][0],
      'cmp_funds_fee',
      $fee,
      TRUE,
      TRUE
    );
  }
}

/**
 * Apply transaction amount to transaction originator and destination accounts.
 * Note that this function doesn't save the accounts.
 */
function commerce_marketplace_funds_rules_apply_transaction_to_accounts($transaction, $destination = NULL, $originator = NULL) {
  $amount = $transaction->amount[LANGUAGE_NONE][0]['amount'];
  if ($originator) {
    $originator->balance[LANGUAGE_NONE][0]['amount'] -= $amount;
  }
  if ($destination) {
    if (empty($destination->balance)) {
      $destination->balance[LANGUAGE_NONE][0]['amount'] = 0;
      $destination->balance[LANGUAGE_NONE][0]['currency_code'] = $transaction->amount[LANGUAGE_NONE][0]['currency_code'];
    }
    $destination->balance[LANGUAGE_NONE][0]['amount'] += $amount;
    $destination->balance[LANGUAGE_NONE][0]['data'] = commerce_price_component_add(
      $destination->balance[LANGUAGE_NONE][0],
      'base_price',
      $destination->balance[LANGUAGE_NONE][0],
      TRUE
    );
  }
}

/**
 * Returns the deposit transaction associated with $order.
 */
function commerce_marketplace_funds_load_deposit_transaction_from_order($order) {
  $transaction_id = db_select('commerce_marketplace_funds_transaction', 't')
    ->condition('type', 'deposit')
    ->condition('order_id', $order->order_id)
    ->fields('t', array('transaction_id'))
    ->execute()->fetchField();
  if ($transaction_id && $transaction = commerce_marketplace_funds_transaction_load($transaction_id)) {
    return array('funds_transaction' => $transaction);
  }
  return array('funds_transaction' => NULL);
}


