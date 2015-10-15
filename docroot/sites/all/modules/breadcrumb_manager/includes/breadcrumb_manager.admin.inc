<?php

/**
 * Breadcrumb Manager Settings form.
 */
function breadcrumb_manager_settings_form($form, &$form_state) {

  $form['breadcrumb_manager_home'] = array(
    '#type' => 'textfield',
    '#title' => 'Home label',
    '#default_value' => variable_get('breadcrumb_manager_home', 'Home'),
    '#required' => TRUE
  );

  $form['breadcrumb_manager_excluded_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Excluded paths'),
    '#default_value' => variable_get('breadcrumb_manager_excluded_paths', "search/*"),
    '#description' => t('Enter a list of path that will not be affected by Breadcrumb Manager. You can use "*" as wildcard.'),
    '#rows' => 5
  );

  $form['breadcrumb_manager_set_active_trail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set active trail'),
    '#default_value' => variable_get('breadcrumb_manager_set_active_trail', FALSE),
    '#description' => t('If checked, Breadcrumb Manager will set the active trail on the Main Menu')
  );

  $form['breadcrumb_manager_set_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set page title'),
    '#default_value' => variable_get('breadcrumb_manager_set_title', FALSE),
    '#description' => t('If checked, Breadcrumb Manager will set the current page title if does not exists.')
  );

  $form['breadcrumb_manager_titles_blacklist'] = array(
    '#type' => 'textarea',
    '#title' => t('Page titles blacklist'),
    '#default_value' => variable_get('breadcrumb_manager_titles_blacklist', ''),
    '#description' => t('Enter a list of titles (one title per row), for which the page title must be overwritten by the computed one.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="breadcrumb_manager_set_title"]' => array('checked' => FALSE),
      ),
    )
  );

  return system_settings_form($form);
}

