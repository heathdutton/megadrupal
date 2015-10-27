<?php

/**
 * @file
 * Contains page callbacks for administration of commerce_marketplace_funds.
 */


/**
 * Generates data structure used by theme_table for showing a list of funds account.
 */
function commerce_marketplace_funds_account_list_rows($type = '') {
  $q = new EntityFieldQuery();
  $q->entityCondition('entity_type', 'cmp_funds_account');
  if ($type) {
    $q->entityCondition('bundle', check_plain($type));
  }
  $results = $q->execute();
  $entities = array();
  if (!empty($results['cmp_funds_account'])) {
    $entities = entity_load('cmp_funds_account', array_keys($results['cmp_funds_account']));
  }
  if (!empty($entities)) {
    $rows = array();
    foreach ($entities as $entity) {
      $balance = field_get_items('cmp_funds_account', $entity, 'balance');
      $owner = commerce_marketplace_funds_get_account_owner($entity);
      $owner_path = entity_uri($entity->owner_type, $owner);
      $owner_name = entity_label($entity->owner_type, $owner);
      $account_path = commerce_marketplace_funds_account_uri($entity);
      $rows[] = array(
        'data' => array(
          'id' => l($entity->account_id, $account_path['path']),
          'owner' => l($owner_name, $owner_path['path']),
          'balance' => commerce_currency_format($balance[0]['amount'], $balance[0]['currency_code']),
        ),
      );
    }
    return $rows;
  }
  return array();
}


/**
 * Generates data structure used by theme_table for showing a list of funds transactions.
 */
function commerce_marketplace_funds_transaction_list_rows($type = '') {
  $q = new EntityFieldQuery();
  $q->entityCondition('entity_type', 'cmp_funds_transaction');
  if ($type) {
    $q->entityCondition('bundle', check_plain($type));
  }
  $results = $q->execute();
  $entities = array();
  if (!empty($results['cmp_funds_transaction'])) {
    $entities = entity_load('cmp_funds_transaction', array_keys($results['cmp_funds_transaction']));
  }
  if (!empty($entities)) {
    foreach ($entities as $entity) {
      $amount = field_get_items('cmp_funds_transaction', $entity, 'amount');
      if ($entity->from_account) {
        $from_account = commerce_marketplace_funds_account_load($entity->from_account);
        $from_account_owner = commerce_marketplace_funds_get_account_owner($from_account);
      }
      if ($entity->to_account) {
        $to_account = commerce_marketplace_funds_account_load($entity->to_account);
        $to_account_owner = commerce_marketplace_funds_get_account_owner($to_account);
      }
      if (isset($from_account_owner)) {
        $from_name = entity_label($from_account->owner_type, $from_account_owner);
        $from_path = entity_uri($from_account->owner_type, $from_account_owner);
      }
      if (isset($to_account_owner)) {
        $to_name = entity_label($to_account->owner_type, $to_account_owner);
        $to_path = entity_uri($to_account->owner_type, $to_account_owner);
      }
      
      $order = NULL;
      if ($entity->order_id) {
        $order = commerce_order_load($entity->order_id);
        $order_path = entity_uri('commerce_order', $order);
      }
      $transaction_path = commerce_marketplace_funds_transaction_uri($entity);
      $rows[] = array(
        'data' => array(
          'id' => l($entity->transaction_id, $transaction_path['path']),
          'created' => format_date($entity->created, 'short'),
          'from' => isset($from_name) ? l($from_name, $from_path['path']) : '-',
          'to' => isset($to_name) ? l($to_name, $to_path['path']) : '-',
          'order' => $order ? l($order->order_id, $order_path['path']) : '-',
          'amount' => commerce_currency_format($amount[0]['amount'], $amount[0]['currency_code']),
          'status' => $entity->status,
        ),
      );
    }
    return $rows;
  }
  return array();
}

/**
 * Returns a rendered list of Store Funds Accounts.
 */
function commerce_marketplace_funds_list_accounts($type) {
  $content = array();
  $rows = commerce_marketplace_funds_account_list_rows($type);
  if (!empty($rows)) {
    $content['accounts_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => array(t('ID'), t('Owner'), t('Balance')),
    );
  }
  else {
    $content[] = array(
      '#type' => 'item',
      '#markup' => t('No funds account entities currently exist.'),
    );
  }
  return $content;
}

/**
 * Returns a rendered list of Deposit Funds Transactions.
 */
function commerce_marketplace_funds_list_transactions($type) {
  $content = array();
  $rows = commerce_marketplace_funds_transaction_list_rows($type);
  if (!empty($rows)) {
    $content['transactions_table'] = array(
      '#theme' => 'table',
      '#rows' => $rows,
      '#header' => array(t('ID'), t('Created'), t('From'), t('To'), t('Order'), t('Amount'), t('Status')),
    );
  }
  else {
    $content[] = array(
      '#type' => 'item',
      '#markup' => t('No funds transaction entities currently exist.'),
    );
  }
  return $content;
}


/**
 * Page callback for creating new transfer transactions.
 */
function commerce_marketplace_funds_new_transfer_transaction_page($owner_type, $owner, $type) {
  $owner_info = entity_get_info($owner_type);
  $account = commerce_marketplace_funds_account_load_by_owner($owner_type, $owner->{$owner_info['entity keys']['id']});
  $transaction = (object) array(
    'transaction_id' => '',
    'type' => $type,
    'from_account' => $account->account_id,
    'to_account' => '',
    'order_id' => '',
    'status' => FUNDS_TRANSACTION_PENDING,
  );

  return drupal_get_form('commerce_marketplace_funds_new_transfer_transaction_form', $transaction, $account);
}

/**
 * New transfer transaction form callback.
 */
function commerce_marketplace_funds_new_transfer_transaction_form($form, &$form_state, $transaction, $account) {
  // here destination account owner should have 'receive funds transactions' permission.
  $form_state['account'] = $account;
  $form_state['transaction'] = $transaction;
  $form['transaction_id'] = array(
    '#type' => 'value',
    '#value' => $transaction->transaction_id,
  );
  $form['from_account'] = array(
    '#type' => 'value',
    '#value' => $account->account_id,
  );
  $form['status'] = array(
    '#type' => 'value',
    '#value' => FUNDS_TRANSACTION_SUCCESS,
  );
  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => '',
  );
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $transaction->type,
  );
  $form['to_account'] = array(
    '#title' => t('To'),
    '#description' => t('Expects input in owner_type/owner_name format. (e.g. user/admin or commerce_store/store 1)'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'funds/account/autocomplete',
    '#required' => TRUE,
    '#weight' => -1,
  );
  field_attach_form('cmp_funds_transaction', $transaction, $form, $form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $transaction->transaction_id ? t('Save') : t('Send'),
    '#weight' => 2,
  );
  $form['#validate'][] = 'commerce_marketplace_funds_new_transfer_transaction_form_validate';
  $form['#submit'][] = 'commerce_marketplace_funds_new_transfer_transaction_form_submit';
  return $form;
}

/**
 * Validate add transfer transaction form values.
 */
function commerce_marketplace_funds_new_transfer_transaction_form_validate($form, &$form_state) {
  form_state_values_clean($form_state);
  $transaction = (object) $form_state['values'];
  if (preg_match('/.*\((\d+)\)/', $transaction->to_account, $matches)) {
    $account = commerce_marketplace_funds_account_load($matches[1]);
    if ($account) {
      if (!commerce_marketplace_funds_account_can_receive_transactions($account)) {
        form_set_error('to_account', t('The selected account cannot receive transactions.'));
        return;
      }
      else {
        $transaction->to_account = $account->account_id;
        $form_state['values']['to_account'] = $account->account_id;
      }
    }
    else {
      form_set_error('to_account', t('The selected account does not exist.'));
      return;
    }
  }
  else {
    form_set_error('to_account', t('You have entered an invalid text in the account selection text field.'));
    return;
  }
  field_attach_form_validate('cmp_funds_transaction', $transaction, $form, $form_state);
  $amount = field_get_items('cmp_funds_transaction', $transaction, 'amount');
  $amount = $amount[0]['amount'];
  if ($amount <= 0) {
    form_set_error('amount', t('Transaction amount should be positive.'));
    return;
  }
  $originator = $form_state['account'];
  $originator_balance = field_get_items('cmp_funds_account', $originator, 'balance');
  if (!isset($originator_balance[0]['amount']) || $amount > $originator_balance[0]['amount']) {
    form_set_error('amount', t('The entered amount is more than your funds account\'s balance.'));
  }
  $form_state['transaction'] = $transaction;
}

/**
 * Save add transfer transaction form values.
 */
function commerce_marketplace_funds_new_transfer_transaction_form_submit($form, &$form_state) {
  // @todo: remove this code, fire a rules event instead.
  $transaction = $form_state['transaction'];
  field_attach_submit('cmp_funds_transaction', $transaction, $form, $form_state);
  $transaction = commerce_marketplace_funds_transaction_save($transaction);
  $transaction->status = $form_state['values']['status'];

  // Make changes to from_account and to_account balances.
  $originator = commerce_marketplace_funds_account_load($transaction->from_account);
  $destination = commerce_marketplace_funds_account_load($transaction->to_account);

  $amount = field_get_items('cmp_funds_transaction', $transaction, 'amount');
  $originator->balance[LANGUAGE_NONE][0]['amount'] -= $amount[0]['amount'];
  $destination->balance[LANGUAGE_NONE][0]['amount'] += $amount[0]['amount'];
  commerce_marketplace_funds_account_save($originator);
  commerce_marketplace_funds_account_save($destination);

  drupal_set_message(t('Transaction saved.'));
  $form_state['redirect'] = 'funds/transaction/' . $transaction->transaction_id;
}

/**
 * Page callback for creating new deposit transactions.
 */
function commerce_marketplace_funds_new_deposit_transaction_page($owner_type, $owner, $type) {
  $owner_info = entity_get_info($owner_type);
  $account = commerce_marketplace_funds_account_load_by_owner($owner_type, $owner->{$owner_info['entity keys']['id']});
  $transaction = (object) array(
    'transaction_id' => '',
    'type' => $type,
    'from_account' => '',
    'to_account' => $account->account_id,
    'order_id' => '',
    'status' => FUNDS_TRANSACTION_PENDING,
  );

  return drupal_get_form('commerce_marketplace_funds_new_deposit_transaction_form', $transaction, $account);
}

/**
 * New deposit transaction form callback.
 */
function commerce_marketplace_funds_new_deposit_transaction_form($form, &$form_state, $transaction, $account) {
  // here destination account owner should have 'receive funds transactions' permission.
  $form_state['account'] = $account;
  $form_state['transaction'] = $transaction;
  $form['transaction_id'] = array(
    '#type' => 'value',
    '#value' => $transaction->transaction_id,
  );
  $form['to_account'] = array(
    '#type' => 'value',
    '#value' => $account->account_id,
  );
  $form['status'] = array(
    '#type' => 'value',
    '#value' => $transaction->status,
  );
  $form['order_id'] = array(
    '#type' => 'value',
    '#value' => '',
  );
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $transaction->type,
  );
  $form['from_account'] = array(
    '#type' => 'value',
    '#value' => '',
  );
  field_attach_form('cmp_funds_transaction', $transaction, $form, $form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $transaction->transaction_id ? t('Save') : t('Send'),
    '#weight' => 2,
  );
  $form['#validate'][] = 'commerce_marketplace_funds_new_deposit_transaction_form_validate';
  $form['#submit'][] = 'commerce_marketplace_funds_new_deposit_transaction_form_submit';
  return $form;
}

/**
 * Validate add deposit transaction form values.
 */
function commerce_marketplace_funds_new_deposit_transaction_form_validate($form, &$form_state) {
  form_state_values_clean($form_state);
  $transaction = (object) $form_state['values'];

  field_attach_form_validate('cmp_funds_transaction', $transaction, $form, $form_state);
  $amount = field_get_items('cmp_funds_transaction', $transaction, 'amount');
  if ($amount <= 0) {
    form_set_error('amount', t('Amount should be greater than 0.'));
  }
  $form_state['transaction'] = $transaction;
}

/**
 * Save add deposit transaction form values.
 */
function commerce_marketplace_funds_new_deposit_transaction_form_submit($form, &$form_state) {
  global $user;
  $transaction = $form_state['transaction'];
  $amount = field_get_items('cmp_funds_transaction', $transaction, 'amount');

  $deposit = commerce_product_new('deposit');
  $deposit->uid = $user->uid;
  $deposit->sku = 'deposit_' . $user->uid . '_' . $amount[0]['amount'] . '_' . microtime();

  $deposit->commerce_price[LANGUAGE_NONE] = $amount;

  $deposit->title = 'Deposit ' . commerce_currency_format($amount[0]['amount'], commerce_default_currency());

  commerce_product_save($deposit);
  // @todo order type problem
  $deposit_order = ($user->uid) ? commerce_order_new($user->uid, 'checkout_checkout', 'cmp_funds_deposit_order') : commerce_cart_order_new();

  $line_item = commerce_product_line_item_new($deposit, 1, $deposit_order->order_id);

  commerce_line_item_save($line_item);

  $order_wrapper = entity_metadata_wrapper('commerce_order', $deposit_order);

  $order_wrapper->commerce_line_items[] = $line_item;

  $deposit_order->data['type'] = 'cmp_funds_deposit_order';
  commerce_order_save($deposit_order);

  $transaction->order_id = $deposit_order->order_id;
  $transaction->status = $form_state['values']['status'];
  field_attach_submit('cmp_funds_transaction', $transaction, $form, $form_state);
  commerce_marketplace_funds_transaction_save($transaction);

  $form_state['redirect'] = 'checkout/' . $deposit_order->order_id;
}

/**
 * Callback for editing transactions.
 * Each transaction type has its own creation form callback.
 * The form callback function names are using this pattern:
 *   commerce_marketplace_funds_new_TRANSACTION_TYPE_transaction_form
 */
function commerce_marketplace_funds_transaction_edit($transaction) {
  $account = NULL;
  if ($transaction->type == 'deposit') {
    $account = commerce_marketplace_funds_account_load($transaction->to_account);
  }
  elseif ($transaction->type == 'transfer') {
    $account = commerce_marketplace_funds_account_load($transaction->from_account);
  }
  elseif ($transaction->type == 'withdraw') {
    $account = commerce_marketplace_funds_account_load($transaction->from_account);
  }

  $function = 'commerce_marketplace_funds_new_' . $transaction->type . '_transaction_form';
  if ($transaction->type == 'withdraw') {
    module_load_include('inc', 'commerce_marketplace_funds_withdraw', 'includes/commerce_marketplace_funds_withdraw.transaction.admin');
    return drupal_get_form($function, NULL, NULL, $transaction, $account);
  }
  return drupal_get_form($function, $transaction, $account);
}
