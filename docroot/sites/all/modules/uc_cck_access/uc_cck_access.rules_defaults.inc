<?php

/**
 * @file
 * Default rules configurations.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function uc_cck_access_default_rules_configuration() {
  $configs = array();

  // Grant access when the order status changes to "Payment Received".
  $rule = rules_reaction_rule();
  $rule->label = t('Update UC CCK Access on full payment');
  $rule->active = TRUE;
  $rule->event('uc_order_status_update')
    ->condition('data_is', array(
      'data:select' => 'updated_order:order-status',
      'value' => 'payment_received'
    ))
    ->action('uc_cck_access_delay_access', array(
      'order:select' => 'order',
    ));
  $configs['uc_cck_access_grant_on_payment'] = $rule;

  return $configs;
}

