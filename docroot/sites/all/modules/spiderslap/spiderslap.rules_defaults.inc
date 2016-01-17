<?php
/**
 * @file
 * Install, update and uninstall functions for the Spider Slap module.
 */

/**
 * Implements hook_default_rules_configuration().
 *
 * Creates a rule using rules module that blocks the IP of a bot
 * that triggers a watchdog message 'now/bugoff' and the sends
 * an email to [site:mail]. Email can be changed in Rules UI.
 */

function spiderslap_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = 'Spider Slap';
  $rule->active = TRUE;
  $rule->event('watchdog')
    ->condition(rules_condition('data_is', array('data:select' => 'log-entry:message', 'op' => 'contains', 'value' => 'now/bugoff')))
    ->condition(rules_condition('path_has_alias', array('source' => 'now/bugoff', 'value' => TRUE))->negate())
    ->action('php_eval', array('code' => 'if ($log_entry["message"] == "now/bugoff") { system_block_ip_action();}'))
    ->action('mail', array('to' => '[site:mail]', 'subject' => 'Bad Spider IP Blocked',
    'message' => '[site:name] has IP blocked a spider that ignored robots.txt. The Watchdog log entry says: /now/bugoff
    [site:current-date]. Offender IP is: [spider_ip:bot_ip]'));

  $configs['rules_spider_slap'] = $rule;
  return $configs;
}