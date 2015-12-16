<?php
/**
 * @file
 * Defines default example rule for Ogone.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_firstdata_connect_default_rules_configuration() {
  // Reaction rule triggered on cron run that captures pending transactions.
  $rule = rules_reaction_rule();
  $rule->label = t('Capture transactions on cron run.');
  $rule->tags = array('Commerce Order');
  $rule->active = TRUE;
  $rule
    ->event('cron')
    ->action('batch_capture', array(
      'date' => 'now',
    ));

  $rules['capture_transactions_on_cron_run'] = $rule;

  return $rules;
}
