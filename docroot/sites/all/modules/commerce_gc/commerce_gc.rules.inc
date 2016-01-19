<?php

/*
 * Implements hook_rules_action_info().
 */
function commerce_gc_rules_action_info() {
  $actions['commerce_gc_order_transaction_status_change'] = array(
    'label' => t('Change the status of transactions on an order that match a certain status'),
    'group' => t('Commerce Giftcard'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order')
      ),
      'statuses' => array(
        'type' => 'token',
        'label' => t('Statuses'),
        'description' => t('Choose statuses to look for.'),
        'options list' => 'commerce_gc_transaction_status_option_list',
        'multiple' => TRUE
      ),
      'target_status' => array(
        'type' => 'token',
        'label' => t('Target status'),
        'description' => t('Choose the new status.'),
        'options list' => 'commerce_gc_transaction_status_option_list'
      )
    )
  );

  return $actions;
}


/*
 * Implements hook_rules_condtion_info().
 */
function commerce_gc_rules_condition_info() {
  $conditions['commerce_gc_giftcard_minimum_balance'] = array(
    'label' => t('Determine whether a giftcard balance exceeds a minimum amount.'),
    'group' => t('Commerce Giftcard'),
    'parameter' => array(
      'commerce_coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Giftcard coupon'),
        'wrapped' => TRUE,
      ),
      'amount' => array(
        'type' => 'integer',
        'label' => t('Giftcard must be greater than this amount.'),
        'optional' => TRUE
      ),
    )
  );

  return $conditions;
}

/*
 * Rules action callback: set all transactions of a certain status connected to
 * an order to a target status.
 */
function commerce_gc_order_transaction_status_change($order, $statuses, $target_status) {
  if (!empty($order->data['giftcard_transaction_ids'])) {
    commerce_gc_transaction_change_status($order->data['giftcard_transaction_ids'], $statuses, $target_status);
  }
}

/*
 * Rules condition callback: Determine whether a giftcard balance is greater
 * than a certain amount.
 */
function commerce_gc_giftcard_minimum_balance(EntityDrupalWrapper $coupon_wrapper, $minimum = 0) {
  $amount = commerce_gc_giftcard_balance($coupon_wrapper->coupon_id->value());
  if ($amount > (int) $minimum) {
    return TRUE;
  }
  else {
    $code = $coupon_wrapper->code->value();
    $error = &drupal_static('commerce_coupon_error_' . strtolower($code));

    $name = $coupon_wrapper->commerce_gc_name->value() ? $coupon_wrapper->commerce_gc_name->value() : $code;
    $error = t('The giftcard %name does not have sufficient balance.', array('%name' => $name));
  }
}
