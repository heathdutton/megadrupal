<?php
/**
 * @file
 * Default Rules configurations for uc_coupon_workflow.
 */

/**
 * Implements hook_default_rules_configuration().
 */

function uc_coupon_workflow_default_rules_configuration() {
  
  // An example of issuing a coupon on user registration.
  $rule = rules_reaction_rule();
  $rule->label = t('Assign coupon #1 when a new user registers (template).');
  $rule->active = FALSE;
  $rule->event('user_insert')
    ->condition(rules_condition('uc_coupon_workflow_suspended')->negate())
    ->action('uc_coupon_workflow_assign', array(
      'account:select' => 'account',
      'cid' => 1
      ))
    ->action('uc_coupon_workflow_email', array(
      'coupon:select' => 'coupon',
      'from' => '[site:mail]',
      'addresses' => '[account:mail]',
      'subject' => t('Coupon at [site:name]'),
      'message' => t("[account:name],\n\nThank you for registering at [site:name].\n\nYou may use the following coupon code(s) during checkout:\n\n[coupon:codes]\n\nPlease ensure you are logged in to your account, or you will not be able to use the coupon.\n\n\n-- The [site:name] team"),
      'format' => filter_default_format(),
    ));

  $configs['uc_coupon_workflow_register'] = $rule;
 
  return $configs;
}
