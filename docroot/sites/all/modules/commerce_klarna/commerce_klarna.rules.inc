<?php
/**
 * @file
 * Rules hook for Klarna implementation.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_klarna_rules_event_info() {
  // Make entity_rules_events_variables() available.
  module_load_include('inc', 'entity', 'entity.rules');

  $events['commerce_klarna_pre_connect'] = array(
    'label' => t('Before initiating a transaction to Klarna'),
    'group' => t('Commerce Klarna'),
    'variables' => entity_rules_events_variables('commerce_order', t('Order', array(), array('context' => 'a drupal commerce order')), TRUE, TRUE),
  );

  return $events;
}
