<?php

/**
 * @file
 * Administrative page callbacks for the Taxonomy Role Access module.
 */

/**
 * Builds Taxonomy Role Access settings form.
 */
function taxonomy_tools_role_access_admin_form($form, &$form_state) {
  $form = array();
  $options = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    $options[$vocabulary->machine_name] = $vocabulary->name;
  }
  // List all vocabularies.
  $form['taxonomy_tools_role_access_vocab_config'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Vocabularies'),
    '#description' => t('Select vocabularies which will use Taxonomy Role Access.'),
    '#default_value' => array_filter(variable_get('taxonomy_tools_role_access_vocab_config', array())),
  );
  $options = array();
  $roles = user_roles();
  // List all user roles.
  $form['taxonomy_tools_role_access_role_config'] = array(
    '#type' => 'checkboxes',
    '#options' => $roles,
    '#title' => t('User roles'),
    '#description' => t('Select user roles which will be controlled by Taxonomy Role Access.'),
    '#default_value' => variable_get('taxonomy_tools_role_access_role_config', array()),
  );
  return system_settings_form($form);
}
