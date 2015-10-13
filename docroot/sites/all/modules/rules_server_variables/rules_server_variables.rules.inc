<?php

/**
 * @file
 *   Provides integration with Rules module.
 */

/**
 * Implements hook_rules_condition_info().
 */
function rules_server_variables_rules_condition_info() {
  $conditions = array();

  $conditions['rules_server_variables_compare'] = array(
    'label' => t('Server variable match'),
    'parameter' => array(
      'variable' => array(
        'type' => 'text',
        'label' => t('Server variable'),
        'description' => t('Enter the server variable (example "REMOTE_ADDR")'),
      ),
      'value' => array(
        'type' => 'text',
        'label' => t('Compare value'),
        'description' => t('Enter the string value to compare the server variable to.'),
      ),
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The operator used to compare the server variable.'),
        'default value' => 'equal_to',
        'options list' => 'rules_server_variables_operators_list',
        'restriction' => 'input',
      ),
    ),
    'callback' => 'rules_server_variables_compare',
    'group' => t('Server variables'),
  );

  return $conditions;
}
