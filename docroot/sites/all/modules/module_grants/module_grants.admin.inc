<?php

/**
 * @file
 * Admin configuration settings for Module Grants.
 */

/**
 * Menu callback for admin settings.
 */
function module_grants_admin_configure() {
  $access_modules = array();
  foreach (module_implements('node_grants') as $module) {
    $access_modules[] = $module;
  }

  $form['module_grants_multiple_modules'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configure behaviour when multiple content access modules are enabled'),
    '#description' => t('The following content access modules are currently enabled: %modules.',
      array('%modules' => empty($access_modules) ? 'none' : implode(',', $access_modules)))
  );
  $form['module_grants_multiple_modules']['module_grants_lenient'] = array(
    '#type' => 'checkbox',
    '#title' => t('Interpret absence of access grants as a "don\'t care", rather than a "deny access".'),
    '#default_value' => variable_get('module_grants_lenient', TRUE),
    '#description' => t('Only applies when two or more content access modules are enabled and one of the modules makes no statement about a node that is about to be accessed. If this box is checked, then a content access module saying nothing (via the node_access table) about the node in question will be deemed to be ok with the user having access to it. If not checked (i.e. "strict"), then a module saying nothing will be taken as a "deny access".')
  );
  $form['module_grants_multiple_modules']['module_grants_OR_modules'] = array(
    '#type' => 'checkbox',
    '#title' => t('OR rather than AND the access grants from multiple modules.'),
    '#default_value' => variable_get('module_grants_OR_modules', FALSE),
    '#description' => t('Normally you would <strong>not</strong> tick this box, except for testing purposes. To "OR" access grants coming from multiple modules means that content is being made accessible by one module when access has already been restricted by another. "OR" is core-behaviour and is undesirable in most cases. Bad cORe behaviour is the reason Module Grants was written in the first place.')
  );
  return system_settings_form($form);
}
