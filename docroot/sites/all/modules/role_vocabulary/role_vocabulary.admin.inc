<?php

/**
 * Settings form.
 */
function role_vocabulary_admin_settings() {
  foreach (taxonomy_get_vocabularies() as $vocabulary) {
    $options[$vocabulary->vid] = $vocabulary->name;
  }
  $form['role_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Role Vocabulary'),
    '#options' => $options,
    '#default_value' => variable_get('role_vocabulary', 0),
    '#description' => t('The vocabulary whose terms will be generated and sycn\'d from the <a href="!url">user roles</a>. <strong>Note:</strong> Choosing the wrong vocabulary could result in loss of terms when roles changes are made.', array(
      '!url' => url('admin/people/permissions/roles'),
    )),
  );
  return system_settings_form($form);
}

/**
 * Sync form.
 */
function role_vocabulary_admin_sync() {
  $form['sync'] = array(
    '#type' => 'submit',
    '#value' => t('Sync Roles and Terms'),
  );
  return $form;
}

/**
 * Sync form - submit callback.
 */
function role_vocabulary_admin_sync_submit() {
  role_vocabulary_sync();
  drupal_set_message("Your Role Vocabulary has been sync'd.");
}
