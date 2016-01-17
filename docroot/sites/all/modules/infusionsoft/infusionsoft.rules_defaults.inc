<?php
/**
 * @file
 */

/**
 * Implements hook_default_rules_configuration().
 */
function infusionsoft_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = 'Opt in User to Infusionsoft Email Service';
  $rule->active = FALSE;
  $rule->event('infusionsoft_contact_add')
    ->action('infusionsoft_opt_in', array('user' => 'An infusionsoft contact has been created.'));

  $configs['infusionsoft_opt_in_user_to_email_service'] = $rule;

  return $configs;
}
