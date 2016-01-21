<?php

/**
 * @file
 * Rules include file - event & action definitions
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_subscription_rules_event_info() {
  module_load_include('inc', 'entity', 'entity.rules');

  $variables = array_merge(
    entity_rules_events_variables('commerce_order', t('Order', array(), array('context' => 'a drupal commerce order')), TRUE, TRUE),
    entity_rules_events_variables('commerce_payment_transaction', t('Last completed transaction'), TRUE)
  );

  $items = array(
    'subscription_initial_payment' => array(
      'label' => 'Initial subscription payment',
      'group' => t('Subscriptions'),
      'variables' => $variables,
    ),
    'subscription_payment' => array(
      'label' => 'Subscription payment',
      'group' => t('Subscriptions'),
      'variables' => $variables,
    ),
    'subscription_setup' => array(
      'label' => 'Subscription setup',
      'group' => t('Subscriptions'),
      'variables' => $variables,
    ),
    'subscription_inital_payment_failure' => array(
      'label' => 'Initial subscription payment failed',
      'group' => t('Subscriptions'),
      'variables' => $variables,
    ),
    'subscription_payment_failure' => array(
      'label' => 'Subscription payment failed',
      'group' => t('Subscriptions'),
      'variables' => $variables,
    ),
    'subscription_cancelled' => array(
      'label' => 'Subscription cancelled',
      'group' => t('Subscriptions'),
      'variables' => $variables,
    ),
    'subscription_expired' => array(
      'label' => 'Subscription expired',
      'group' => t('Subscriptions'),
      'variables' => $variables,
    ),
  );

  return $items;
}

