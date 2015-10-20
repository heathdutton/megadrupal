<?php
/**
 * @file
 * Commerce eWay Multi rules..
 */

/**
 * Implements hook_rules_event_info().
 *
 * @return events
 *   Array of rules events.
 */
function commerce_eway_multi_rules_event_info() {
  $events = array();

  $events['commerce_eway_multi_before_payment'] = array(
    'label' => t('Before Payment'),
    'group' => t('Commerce eWay Multi'),
    'variables' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('The current order'),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_eway_multi_rules_action_info() {
  $actions = array();

  $actions['commerce_eway_multi_change_customer_id'] = array(
    'label' => t('Change eWay Customer Id'),
    'parameter' => array(
      'new_customer_id' => array(
        'type' => 'text',
        'label' => t('New eWay Customer Id'),
      ),
    ),
    'group' => t('Commerce eWay Multi'),
    'callbacks' => array(
      'execute' => 'commerce_eway_multi_rules_change_customer_id',
    ),
  );

  return $actions;
}

/**
 * Action callback: switched the customer Id to be sent to eWay.
 *
 * @param int $new_customer_id
 *   Sets the new customer Id.
 */
function commerce_eway_multi_rules_change_customer_id($new_customer_id) {

  // Set the new Id.
  commerce_eway_multi_current_eway_id($new_customer_id);
}