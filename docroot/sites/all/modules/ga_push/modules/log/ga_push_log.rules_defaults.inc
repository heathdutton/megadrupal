<?php
/**
 * @file
 * Rules default file for GA Push (log) module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function ga_push_log_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = 'Send google analytics exception';
  $rule->active = TRUE;
  $rule->event('watchdog')
    ->condition('data_is', array(
      'data:select' => 'log-entry:severity',
      'op' => 'IN',
      'value' => array(
        WATCHDOG_EMERGENCY,
        WATCHDOG_ALERT,
        WATCHDOG_CRITICAL,
      ),
    ))
    ->action('ga_push_action_push_exception', array(
      'exDescription:select' => 'log-entry:message',
      'exFatal' => TRUE,
      'method' => GA_PUSH_METHOD_DEFAULT,
    )
  );
  $configs['ga_push_log_exception'] = $rule;

  return $configs;
}
