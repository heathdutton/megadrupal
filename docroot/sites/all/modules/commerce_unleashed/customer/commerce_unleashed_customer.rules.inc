<?php
/**
 * @file
 *   commerce_unleashed_customer.rules.inc
 */

/**
 * Impliments hook_rules_action_info().
 */
function commerce_unleashed_customer_rules_action_info() {
  $actions = array();

  $actions['commerce_unleashed_customer_post'] = array(
    'label' => t('Post a customer to Unleashed'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
        'save' => TRUE,
      ),
    ),
    'group' => t('Commerce Unleashed'),
    'callbacks' => array(
      'execute' => 'commerce_unleashed_customer_post',
    ),
  );

  return $actions;
}
