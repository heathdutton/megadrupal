<?php

/**
 * @file
 * Forms for depositing money to the account.
 */

/**
 * Form callback for the 'deposit money' form.
 *
 * @see hook_line_item_rebase_unit_price()
 */
function commerce_account_balance_deposit_form($form, &$form_state, $account) {
  $line_item = commerce_line_item_new('deposit');
  $form_state['line_item'] = $line_item;
  $form_state['order'] = commerce_order_new();
  $form_state['account'] = $account;

  field_attach_form('commerce_line_item', $line_item, $form, $form_state);

  // The standard commerce unit price widget will have to be hidden.
  $form['commerce_unit_price'][LANGUAGE_NONE][0]['amount']['#type'] = 'hidden';
  $form['commerce_unit_price'][LANGUAGE_NONE][0]['amount']['#value'] = 0;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Deposit cash', array()),
    '#weight' => 40,
  );
  return $form;
}

/**
 * Submit callback for the deposit widget.
 */
function commerce_account_balance_deposit_form_submit($form, &$form_state) {
  $line_item = $form_state['line_item'];
  $order = $form_state['order'];
  $account = $form_state['account'];

  field_attach_submit('commerce_line_item', $line_item, $form, $form_state);

  // Use the official hook to recalculate the unit price.
  commerce_line_item_rebase_unit_price($line_item);

  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $line_item_wrapper->save();
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  $order_wrapper->uid = $account->uid;
  $order_wrapper->mail = $account->mail;
  $order_wrapper->status = 'checkout_checkout';
  $order_wrapper->created = REQUEST_TIME;
  $order_wrapper->changed = REQUEST_TIME;
  $order_wrapper->commerce_line_items = array($line_item_wrapper->line_item_id->value());

  commerce_order_calculate_total($order);
  $order_wrapper->save();

  $form_state['redirect'] = 'checkout/' . $order_wrapper->order_id->value();
}

/**
 * Form for purchasing an item and executing a rule action set.
 */
function commerce_account_balance_purchase_item_form($form, &$form_state, $amount, $balance, $label, $entity, $rules_config) {
  $currency = commerce_currency_load();
  $amount_formatted = commerce_currency_format($amount, $currency['code']);
  $balance_new = commerce_currency_format($balance - $amount, $currency['code']);
  $form['information'] = array(
    '#markup' => t('You are about to purchase %label for %amount. After
      this purchase you will have %balance_new left on your balance.', array(
        '%label' => $label,
        '%amount' => $amount_formatted,
        '%balance_new' => $balance_new,
      )),
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Purchase'),
    '#weight' => 40,
  );
  $form['actions']['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#weight' => 41,
    '#name' => 'cancel',
  );
  return $form;
}

/**
 * Callback when the user purchases something.
 */
function commerce_account_balance_purchase_item_form_submit($form, &$form_state) {

  $amount = $form_state['build_info']['args'][0];
  $label = $form_state['build_info']['args'][2];
  $entity = $form_state['build_info']['args'][3];
  $rules_config = $form_state['build_info']['args'][4];

  global $user;
  $uid = $user->uid;

  $parameters = $rules_config->componentVariables();
  $parameter_keys = array_keys($parameters);
  $first_parameter = array_shift($parameter_keys);
  if ($first_parameter == "user") {
    $rule_executed = rules_invoke_component($rules_config->name, $user, $entity);
  }
  else {
    $rule_executed = rules_invoke_component($rules_config->name, $entity, $user);
  }

  if ($rule_executed === FALSE) {
    drupal_set_message(t('Your purchase could not be carried out. Please contact a system administrator.'), 'error');
  }
  else {
    $currency = commerce_currency_load();
    $amount_formatted = commerce_currency_format($amount, $currency['code']);

    $transaction = entity_create('commerce_account_balance_transaction', array(
        'type' => 'purchase',
        'action' => t('Purchase of @label for @amount', array('@label' => $label, '@amount' => $amount_formatted)),
        'uid' => $uid,
        'time' => REQUEST_TIME,
        'amount' => -$amount,
        'order_id' => NULL,
      ));

    $transaction_wrapper = entity_metadata_wrapper('commerce_account_balance_transaction', $transaction);
    $transaction_wrapper->save();
  }
}

/**
 * Form for purchasing an item and executing a rule action set.
 */
function commerce_account_balance_insufficient_balance_form($form, &$form_state, $amount, $balance, $label, $entity, $rules_config) {
  $currency = commerce_currency_load();
  $balance_formatted = commerce_currency_format($balance, $currency['code']);
  $markup = t("Your account balance of <strong>@balance</strong> is not enough
    to purchase this.", array('@balance' => $balance_formatted));
  $form['information'] = array(
    '#markup' => $markup,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Deposit Cash'),
    '#weight' => 39,
  );
  $form['actions']['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#weight' => 40,
    '#name' => 'cancel',
  );
  return $form;
}

/**
 * Callback when the user purchases something.
 */
function commerce_account_balance_insufficient_balance_form_submit($form, &$form_state) {
  $account = $GLOBALS['user'];
  $form_state['redirect'] = 'user/' . $account->uid . '/balance';
}
