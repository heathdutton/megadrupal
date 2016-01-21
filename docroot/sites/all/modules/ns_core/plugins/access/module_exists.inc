<?php

/**
 * @file
 * Plugin that checks if a module exists.
 */

$plugin = array(
  'title' => t("Module exists"),
  'description' => t('Control access by checking if a module exists.'),
  'callback' => 'ns_core_module_exists_access_check',
  'settings form' => 'ns_core_module_exists_access_settings',
  'summary' => 'ns_core_module_exists_access_summary',
  'defaults' => array('module' => ''),
  'all contexts' => TRUE,
);

/**
 * Settings form
 */
function ns_core_module_exists_access_settings($form, &$form_state, $conf) {
  $form['settings']['module'] = array(
    '#type' => 'textfield',
    '#title' => t('Module'),
    '#required' => TRUE,
    '#description' => t('Type the machine name of the module here.'),
    '#default_value' => $conf['module'],
  );
  return $form;
}

/**
 * Check for access
 */
function ns_core_module_exists_access_check($conf, $contexts) {
  return module_exists($conf['module']);
}

/**
 * Provide a summary description based upon the specified context
 */
function ns_core_module_exists_access_summary($conf, $context) {
  return t('Module %module is available', array($conf['module']));
}
