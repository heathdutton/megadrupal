<?php
/**
 * @file
 * Admin page.
 */

/**
 * Create the form.
 */
function ferank_admin_settings() {
  $form['type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Type of marker (analytic)'),
    '#collapsible' => TRUE,
  );
  $form['type']['ferank_type'] = array(
    '#type' => 'select',
    '#title' => t('Classic marker or marker conform to the cookie law ?'),
    '#description' => t('More info about the <em>tarteaucitron</em> marker: <a href="https://opt-out.ferank.eu/" target="_blank">tarteaucitron.js</a>'),
    '#options' => array('---', 'classique', 'tarteaucitron'),
    '#default_value' => variable_get('ferank_type', 'classique'),
  );
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role of users (analytic)'),
    '#collapsible' => TRUE,
    '#description' => t("Choose the users that should NOT be tracked by FERank. Note: admin's page are always ignored."),
  );
  $form['roles']['ferank_track_roles'] = array(
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#default_value' => variable_get('ferank_track_roles', array()),
  );

  $form['ads'] = array(
    '#type' => 'fieldset',
    '#title' => t('Editor ID (advertising)'),
    '#collapsible' => TRUE,
  );
  $form['ads']['ferank_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter your editor ID'),
    '#description' => t('Get it on <a href="https://www.ferank.fr/client/securite/" target="_blank">this page</a>'),
    '#default_value' => variable_get('ferank_id', ''),
  );
  return system_settings_form($form);
}
