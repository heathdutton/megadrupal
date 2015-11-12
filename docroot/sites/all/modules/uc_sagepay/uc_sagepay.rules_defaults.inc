<?php

/**
 * Implements hook_default_rules_configuration().
 */
function uc_sagepay_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = 'Sage Pay Direct admin transaction';
  $rule->active = TRUE;
  $rule->event('uc_sagepay_transaction')
    ->condition('uc_sagepay_condition_path_is_admin', array('path:select' => 'site:current-page:path'))
    ->action('data_set', array('data:select' => 'uc-sagepay-transaction:Apply3DSecure', 'value' => 2));

  $configs['uc_sagepay_admin_transaction'] = $rule;
  return $configs;
}
