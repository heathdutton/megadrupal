<?php

/**
 * @file
 * Default rule configurations for Commerce User Profile Pane.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_user_profile_pane_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = t('Update the User with input from User Profile Pane.');
  $rule->active = TRUE;

  $rule->event('commerce_checkout_complete')
    ->action('commerce_user_profile_pane_user_update', array(
      'commerce_order:select' => 'commerce-order',
    ));

  // load the commerce_checkout_new_account to determine the weight due this
  // rule should execute after commerce_checkout_new_account had created the
  // user account.
  $new_account_rule = rules_config_load('commerce_checkout_new_account');
  $rule->weight = $new_account_rule->weight + 1;

  $rules['commerce_user_profile_pane_user_update'] = $rule;

  return $rules;
}
