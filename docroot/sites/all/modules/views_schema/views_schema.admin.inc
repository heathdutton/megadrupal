<?php

/*
 * @file
 *   Functions to administer Views Schema module
 */

function views_schema_admin_settings() {
  drupal_set_title('Modules for Views Schema');
  $form = array();

  $activated = variable_get('views_schema_enabled_modules', array());

  $mod = array();
  $tableCount = array();
  foreach (drupal_get_schema() as $table) {
    $mod[$table['module']] = $table['module'];
    if (isset($tableCount[$table['module']])) {
      $tableCount[$table['module']]++;
    }
    else {
      $tableCount[$table['module']] = 1;
    }
  }

  $modules = array();
  foreach ($mod as $key => $value) {
    $modules[$key] = $value . ' ' . t('(@count tables)', array('@count' => $tableCount[$key]));
  }

  $form['views_schema_enabled_modules'] = array(
    '#type' => 'checkboxes',
    '#options' => $modules,
    '#title' => t('Select modules'),
    '#default_value' => $activated,
  );

  $form['#submit'][] = 'views_schema_admin_settings_submit';

  return system_settings_form($form);
}

function views_schema_admin_settings_submit($form, &$form_state) {
  views_invalidate_cache();
  drupal_set_message(t('Views cache refreshed to activate changes.'), 'info');
}



