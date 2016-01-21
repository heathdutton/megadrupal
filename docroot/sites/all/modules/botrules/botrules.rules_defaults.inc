<?php

/**
 * @file
 * Default Rules configuration for Bot Rules (botrules) module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function botrules_default_rules_configuration() {
  $configs = array();
  $rule = rules_reaction_rule();
  $rule->label = t('New comment notification to IRC #test');
  $rule->active = FALSE;
  $rule->event('comment_insert');
  $rule->condition('botrules_condition_bot_on_channel', array('channel' => '#test'));
  $rule->action('botrules_action_message', array(
    'target' => '#test',
    'text' => "[site:name]: a new comment '[comment:title]' has been added to '[comment:node]', link: [comment:url]",
  ));
  $configs['botrules_new_comment_notification'] = $rule;
  return $configs;
}
