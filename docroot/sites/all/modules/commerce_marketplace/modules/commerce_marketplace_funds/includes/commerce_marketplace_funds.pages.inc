<?php

/**
 * @file
 * Contains callbacks for user pages of commerce_marketplace_funds.
 */

function commerce_marketplace_funds_account_autocomplete_callback($type = "", $name = "") {
  $matches = array();
  if ($type && $name) {
    $info = entity_get_info('cmp_funds_account');
    if (in_array($type, array_keys($info['bundles']))) {
      $owner_info = entity_get_info($type);
      if ($type == 'user') {
        $owner_info['entity keys']['label'] = 'name';
      }
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', $type);
      $query->propertyCondition($owner_info['entity keys']['label'], $name, 'STARTS_WITH');
      $results = $query->execute();
      if (isset($results[$type]) && !empty($results[$type])) {
        foreach(array_keys($results[$type]) as $entity_id) {
          $entity = $owner_info['load hook']($entity_id);
          $account = commerce_marketplace_funds_account_load_by_owner($type, $entity_id);
          if (commerce_marketplace_funds_account_can_receive_transactions($account)) {
            $owner_name = $entity->{$owner_info['entity keys']['label']};
            $matches["$owner_name ($account->account_id)"] = $owner_name;
          }
        }
      }
    }
  }
  drupal_json_output($matches);
}

/**
 * Page callback for viewing Funds Accounts.
 */
function commerce_marketplace_funds_account_view($owner_type, $owner, $view_mode = 'full') {
  $owner_info = entity_get_info($owner_type);
  $account = commerce_marketplace_funds_account_load_by_owner($owner_type, $owner->{$owner_info['entity keys']['id']});
  $account->content = array();

  // Build fields content.
  field_attach_prepare_view('cmp_funds_account', array($account->account_id => $account), $view_mode);
  entity_prepare_view('cmp_funds_account', array($account->account_id => $account));

  $account->content += field_attach_view('cmp_funds_account', $account, $view_mode);
  return $account->content;
}


/**
 * Page callback for viewing Funds Transactions.
 */
function commerce_marketplace_funds_transaction_view($transaction, $view_mode = 'full') {
  $transaction->content = array();
  field_attach_prepare_view('cmp_funds_transaction', array($transaction->transaction_id => $transaction), $view_mode);
  entity_prepare_view('cmp_funds_transaction', array($transaction->transaction_id => $transaction));

  if ($transaction->from_account) {
    $originator = commerce_marketplace_funds_account_load($transaction->from_account);
    $originator_owner = commerce_marketplace_funds_get_account_owner($originator);
    $info = entity_get_info($originator->owner_type);
    $label = '';
    if (isset($info['entity keys']['label'])) {
      $label = check_plain($originator_owner->{$info['entity keys']['label']});
    }
    elseif (isset($info['label callback'])) {
      $label = check_plain($info['label callback']($originator_owner));
    }
    $path = entity_uri($originator->owner_type, $originator_owner);
    $transaction->content['originator'] = array(
      '#type' => 'markup',
      '#weight' => -2,
      '#markup' => '<strong>' . t('From') . '</strong>: ' . l($label, $path['path']) . '<br />',
    );
  }

  if ($transaction->to_account) {
    $destination = commerce_marketplace_funds_account_load($transaction->to_account);
    $destination_owner = commerce_marketplace_funds_get_account_owner($destination);
    $info = entity_get_info($destination->owner_type);
    $label = '';
    if (isset($info['entity keys']['label'])) {
      $label = check_plain($destination_owner->{$info['entity keys']['label']});
    }
    elseif (isset($info['label callback'])) {
      $label = check_plain($info['label callback']($destination_owner));
    }
    $path = entity_uri($destination->owner_type, $destination_owner);
    $transaction->content['destination'] = array(
      '#type' => 'markup',
      '#weight' => -1,
      '#markup' => '<strong>' . t('To') . '</strong>: ' . l($label, $path['path']) . '<br />',
    );
  }

  $transaction->content += field_attach_view('cmp_funds_transaction', $transaction, $view_mode);

  return $transaction->content;
}

/**
 * Page callback for viewing account transactions.
 */
function commerce_marketplace_funds_account_transactions_list($owner_type, $owner) {
  // @TODO: replace this with a view.';
  $header = array(
    'transaction_id' => array('data' => '#', 'field' => 't.transaction_id'),
    'created' => array('data' => t('Date'), 'field' => 't.created', 'sort' => 'desc'),
    'type' => array('data' => t('Type'), 'field' => 't.type'),
    'from' => array('data' => t('From')),
    'to' => array('data' => t('To')),
    'order_id' => array('data' => t('Order')),
    'amount' => array('data' => t('Amount')),
    'fee' => array('data' => t('Fee')),
    'status' => array('data' => t('Status')),
  );

  $ids = entity_extract_ids($owner_type, $owner);
  $account = commerce_marketplace_funds_account_load_by_owner($owner_type, $ids[0]);
  $rows = array();
  $query = db_select('commerce_marketplace_funds_transaction', 't');

  $condition = db_or()->condition('t.to_account', $account->account_id)->condition('t.from_account', $account->account_id);
  $query->condition($condition);
  $query->fields('t', array('transaction_id', 'created', 'type', 'from_account', 'to_account', 'order_id', 'status'));
  $query->orderBy('t.created', 'DESC');
  $query->extend('PagerDefault')->limit(20);
  $query->extend('TableSort')->orderByHeader($header);
  $results = $query->execute()->fetchAll();

//  $transaction_entity_info = entity_get_info('cmp_funds_transaction');

  foreach ($results as $result) {
    $from_link = $to_link = $order_link = '-';
    if ($result->from_account) {
      $from_account = commerce_marketplace_funds_account_load($result->from_account);
      $from_account_owner = commerce_marketplace_funds_get_account_owner($from_account);
      $from_account_name = entity_label($from_account->owner_type, $from_account_owner);
      $uri = entity_uri($from_account->owner_type, $from_account_owner);
      $from_link = l(check_plain($from_account_name), $uri['path']);
    }
    if ($result->to_account) {
      $to_account = commerce_marketplace_funds_account_load($result->to_account);
      $to_account_owner = commerce_marketplace_funds_get_account_owner($to_account);
      $to_account_name = entity_label($to_account->owner_type, $to_account_owner);
      $uri = entity_uri($to_account->owner_type, $to_account_owner);
      $to_link = l(check_plain($to_account_name), $uri['path']);
    }
    if ($result->order_id) {
      $order = commerce_order_load($result->order_id);
      if($order) {
        if ($order->type == 'cmp_funds_deposit_order') {
          $order_link = l($order->order_id, 'user/' . $order->uid . '/orders/' . $order->order_id);
        }
        elseif ($order->type == 'commerce_store_order') {
          $owner_path = entity_uri($owner_type, $owner);
          $order_link = l($order->order_id, $owner_path['path'] . '/sales/' . $order->order_id);
        }
      }
    }
    $transaction = commerce_marketplace_funds_transaction_load($result->transaction_id);
    $amount = field_get_items('cmp_funds_transaction', $transaction, 'amount');
    $fee = commerce_marketplace_funds_get_fee($amount[0]);

    if ($transaction->status == FUNDS_TRANSACTION_PENDING) {
      $status = t('Pending');
    }
    elseif ($transaction->status == FUNDS_TRANSACTION_SUCCESS) {
      $status = t('Successful');
    }
    else {
      $status = t('Failure');
    }

//    $type = $transaction_entity_info['bundles'][$result->type]['label'];

    $rows[$result->transaction_id] = array(
      'data' => array(
        $transaction->transaction_id,
        format_date($transaction->created, 'short'),
        $result->type,
        $from_link,
        $to_link,
        $order_link,
        commerce_currency_format($amount[0]['amount'], $amount[0]['currency_code']),
        commerce_currency_format($fee['amount'], $fee['currency_code']),
        $status,
      ),
    );
  }
  $build['transactions'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );
  $build['pager_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Returns fee component of a price field.
 */
function commerce_marketplace_funds_get_fee($price) {
  foreach ($price['data']['components'] as $component) {
    if ($component['name'] == 'cmp_funds_fee') {
      return $component['price'];
    }
  }
  return array(
    'amount' => 0,
    'currency_code' => $price['currency_code'],
    'data' => array(),
  );
}
