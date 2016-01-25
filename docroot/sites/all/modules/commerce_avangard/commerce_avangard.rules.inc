<?php

/**
 * @file
 * Lists module's rules' actions.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_avangard_rules_action_info() {
  $actions = array();

  $actions['commerce_avangard_refund_order'] = array(
    'label' => t('Refunds order paid by card'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'skip save' => TRUE,
      ),
    ),
    'group' => t('Avangard API'),
    'callbacks' => array(
      'execute' => 'commerce_avangard_refund_order_action',
    ),
  );

  return $actions;
}

/**
 * Implements rules order refund action.
 *
 * @param object $order
 * @param array $settings
 * @param RulesState $state
 * @param RulesPlugin $element
 */
function commerce_avangard_refund_order_action($order, $settings, RulesState $state, RulesPlugin $element) {
  commerce_avangard_order_full_refund($order);
}
