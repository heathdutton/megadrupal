<?php

/**
 * @file
 * commerce_notification.rules.inc
 */

/**
 * Implements hook_rules_condition_info().
 */
function commerce_notification_rules_condition_info() {
  $condition = array();

  $condition['set_date_condition_for_abandoned_cart_notification'] = array(
    'label' => t('Abandoned card notification comparison'),
    'parameter' => array(
      'account_user' => array(
        'type' => 'user',
        'label' => t('Current User'),
      ),
      'order' => array(
        'type' => 'commerce_order',
        'label' => 'Order of the current user',
      ),
      'order_count_comparison' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'options list' => 'commerce_notification_condition_value_operator_options',
      ),
      'order_count_value' => array(
        'type' => 'text',
        'label' => t('Order value'),
      ),
    ),
    'group' => t('Commerce Abandoned Cart Notification'),
    'callbacks' => array(
      'execute' => 'commerce_notification_compare',
    ),
  );

  return $condition;
}

/**
 * Creates option list.
 */
function commerce_notification_condition_value_operator_options() {
  return array(
    'less' => t('Is less than'),
    'less_equal' => t('Is less or equal to'),
    'equal' => t('Is equal to'),
    'greater_equal' => t('Is greater or equal to'),
    'greater' => t('Is greater than'),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_notification_rules_action_info() {
  $actions['save_abandoned_cart_notification'] = array(
    'label' => t('Save notification for users that abandoned their cart'),
    'parameter' => array(
      'account_user' => array(
        'type' => 'user',
        'label' => t('Current User'),
      ),
      'order' => array(
        'type' => 'commerce_order',
        'label' => 'Order of the current user',
      ),
    ),
    'group' => t('Commerce Abandoned Cart Notification'),
    'callbacks' => array(
      'execute' => 'commerce_notification_create',
    ),
  );

  return $actions;
}
