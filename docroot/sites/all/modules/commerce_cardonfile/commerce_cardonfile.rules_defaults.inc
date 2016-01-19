<?php

/**
 * @file
 * Default rule configuration for Commerce Card on File.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_cardonfile_default_rules_configuration() {
  $rules = array();

  $rule = rules_reaction_rule();
  $rule->label = 'Update the order status after an unsuccessful card charge (soft decline)';
  $rule->weight = 10;
  $rule->requires = array(
    'rules',
    'commerce_cardonfile',
    'commerce_order',
  );
  $rule->event('commerce_cardonfile_charge_failed');
  $rule->condition('data_is', array(
    'data:select' => 'response:code',
    'op' => 'IN',
    'value' => array(
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE,
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE_INSUFFICIENT_FUNDS,
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE_LIMIT_EXCEEDED,
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE_CALL_ISSUER,
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE_TEMPORARY_HOLD,
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE_GENERIC_DECLINE,
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE_GATEWAY_ERROR,
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE_GATEWAY_UNAVAILABLE,
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE_GATEWAY_CONFIGURATION_ERROR,
    ),
  ));
  $rule->action('commerce_order_update_status', array(
    'commerce_order:select' => 'order',
    'order_status' => 'cardonfile_payment_failed_soft_decline',
  ));
  $rules['commerce_cardonfile_update_order_status_soft_decline'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = 'Update the order status after an unsuccessful card charge (hard decline)';
  $rule->weight = 10;
  $rule->requires = array(
    'rules',
    'commerce_cardonfile',
    'commerce_order',
  );
  $rule->event('commerce_cardonfile_charge_failed');
  $rule->condition('data_is', array(
    'data:select' => 'response:code',
    'op' => 'IN',
    'value' => array(
      COMMERCE_COF_PROCESS_CODE_INSUFFICIENT_DATA,
      COMMERCE_COF_PROCESS_CODE_CARD_NA,
      COMMERCE_COF_PROCESS_CODE_CARD_NOT_CHARGEABLE,
      COMMERCE_COF_PROCESS_CODE_METHOD_EMPTY,
      COMMERCE_COF_PROCESS_CODE_METHOD_NOT_CAPABLE,
      COMMERCE_COF_PROCESS_CODE_CARD_EXPIRED,
      COMMERCE_COF_PROCESS_CODE_METHOD_FAILURE_HARD_DECLINE,
    ),
  ));
  $rule->action('commerce_order_update_status', array(
    'commerce_order:select' => 'order',
    'order_status' => 'cardonfile_payment_error_hard_decline',
  ));
  $rules['commerce_cardonfile_update_order_status_hard_decline'] = $rule;

  return $rules;
}
