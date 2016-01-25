<?php

/**
 * @file
 * Hooks to tell Rules about our plugins.
 */

/**
 * Implements hook_rules_plugin_info().
 */
function rules_list_conditions_rules_plugin_info() {
  return array(
    'any' => array(
      'label' => t('Any such item is in the list'),
      'class' => 'RulesListConditionsAny',
      'embeddable' => 'RulesConditionContainer',
      'component' => FALSE,
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesListConditionsUI',
        ),
      ),
    ),
    'all' => array(
      'label' => t('All such items are in the list'),
      'class' => 'RulesListConditionsAll',
      'embeddable' => 'RulesConditionContainer',
      'component' => FALSE,
      'extenders' => array(
        'RulesPluginUIInterface' => array(
          'class' => 'RulesListConditionsUI',
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_file_info().
 */
function rules_list_conditions_rules_file_info() {
  $p = 'includes/rules_list_conditions.';
  return array(
    $p . 'core',
    $p . 'plugins',
    $p . 'ui',
  );
}
