<?php
/**
 * @file
 * commerce_file_bundle.rules_defaults.inc
 *
 * Default rules configuration for Commerce Files.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_file_bundle_default_rules_configuration() {

  // Issue licenses based on line item files.
  $rule = rules_reaction_rule();
  $rule->label = t('Issue licenses on order update (bundles friendly)');
  $rule->active = TRUE;
  // Trigger after others.
  $rule->weight = 10;
  $rule->event('commerce_order_update')
    ->condition('data_is', array(
      'data:select' => 'commerce-order:state',
      'op' => '==',
      'value' => 'completed',
    ))
    ->condition(rules_condition('data_is', array(
      'data:select' => 'commerce-order-unchanged:state',
      'op' => '==',
      'value' => 'completed',
    ))->negate())
    ->action('commerce_file_bundle_license_issue_order', array(
      'order:select' => 'commerce-order',
      'license-status' => 'active',
    ));

  $rules['commerce_file_bundle_license_issue_order'] = $rule;

  // Revoke licenses for line item files on a canceled order.
  $rule = rules_reaction_rule();
  $rule->label = t('Revoke licenses on order update, (bundles friendly)');
  $rule->active = TRUE;
  // Trigger after others.
  $rule->weight = 10;
  $rule->event('commerce_order_update')
    ->condition('data_is', array(
      'data:select' => 'commerce-order:state',
      'op' => '==',
      'value' => 'canceled',
    ))
    ->condition(rules_condition('data_is', array(
      'data:select' => 'commerce-order-unchanged:state',
      'op' => '==',
      'value' => 'canceled',
    ))->negate())
    ->action('commerce_file_bundle_license_revoke_order', array(
      'order:select' => 'commerce-order',
    ));

  $rules['commerce_file_bundle_license_revoke_order'] = $rule;

  return $rules;
}
