<?php

/**
 * Implementation of hook_rules_action_info().
 */
function rules_set_site_variables_rules_action_info() {
  $defaults = array(
   'parameter' => array(
      'variable' => array(
         'type' => 'list<text>',
         'label' => t('Variable'),
         'description' => t('The variable that should be changed.'),
         'options list' => 'rules_set_site_variables_var_list',
       ),
      'value' => array(
        'type' => 'text',
        'label' => t('Value'),
        'description' => t('The new value for the variable.'),
        'save' => TRUE,
      ),
    ),
    'group' => t('System'),
  );
  $items['rules_set_site_variables_change_variable'] = $defaults + array(
    'label' => t('Set Drupal Site variable'),
    'base' => 'rules_set_site_variables_change_variable',
  );

  return $items;
}

/**
 * Creates list of variables for choosing when configuring the rule.
 */
function rules_set_site_variables_var_list() {
  global $conf;
  foreach ($conf as $key => $value) {
    $values[$key] = $key;
  }
  return $values;
}
