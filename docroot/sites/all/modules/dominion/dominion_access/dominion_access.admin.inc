<?php

/**
 * Admin settings form.
 */
function dominion_access_admin_settings($form, &$form_id) {
  $options = db_select('role', 'r')
    ->fields('r', array('rid', 'name'))
    ->orderBy('r.rid', 'asc')
    ->execute()
    ->fetchAllKeyed();
  $form[DOMINION_ACCESS_VAR_ROLES] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available roles'),
    '#options' => $options,
    '#default_value' => variable_get(DOMINION_ACCESS_VAR_ROLES, array(1 => 1, 2 => 2)),
    '#required' => TRUE,
    '#description' => t('Roles to choose from when setting access restrictions to a subsite. The subsite is available for all users when no roles have been selected.'),
  );

  $form[DOMINION_ACCESS_VAR_EXCLUDE] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude pages'),
    '#default_value' => variable_get(DOMINION_ACCESS_VAR_EXCLUDE, DOMINION_ACCESS_VAR_EXCLUDE_DEFAULT),
    '#description' => t('Pages which are excluded from the access restriction. Use the internal path, one page per line. Use &lt;front&gt; for the frontpage.'),
  );
  
  $form[DOMINION_ACCESS_VAR_REDIRECT] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect'),
    '#default_value' => variable_get(DOMINION_ACCESS_VAR_REDIRECT, ''),
    '#description' => t('Redirect unauthorized users to this page when they visit the subsite. Leave empty to show the default "403 Access denied" page or use &lt;login&gt; to display a loginbox.'),
  );
  
  $options = db_select('system', 's')
    ->fields('s', array('name'))
    ->condition('s.type', 'theme')
    ->condition('s.status', 1)
    ->orderBy('s.name', 'asc')
    ->execute()
    ->fetchCol();
  $options = drupal_map_assoc($options);
  $form[DOMINION_ACCESS_VAR_THEME] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#default_value' => variable_get(DOMINION_ACCESS_VAR_THEME, ''),
    '#options' => $options,
    '#description' => t('The to use when unauthorized users are visiting this subsite.'),
  );

  return system_settings_form($form);
}
