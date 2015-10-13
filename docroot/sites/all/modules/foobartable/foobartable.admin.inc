<?php

/**
 * @file
 * Administration page callbacks for the foobartable module.
 */

function foobartable_admin_settings_form($form, &$form_state) {
  $settings = variable_get('foobartable_settings');

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#description' => t('These settings affect how FooBar Table is used in your Drupal installation.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['settings']['replace_core_theme'] = array(
    '#type' => 'radios',
    '#title' => t('Replace core table theming'),
    '#options' => array(
      1 => t("Replace core 'table' theming with 'foobartable' function"),
      0 => t("Do not replace core theming and use 'foobartable' theme when required"),
    ),
    '#default_value' => isset($settings['replace_core_theme']) ? $settings['replace_core_theme'] : 1,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for the foobartable Admin Settings Form.
 *
 * @see foobartable_admin_settings_form().
 */
function foobartable_admin_settings_form_submit(&$form, &$form_state) {
  $settings = array();

  foreach ($form_state['values'] as $key => &$value) {
    if (!in_array($key, array('submit', 'form_build_id', 'form_token', 'form_id', 'op'))) {
      $settings[$key] = $value;
    }
  }

  drupal_set_message(t('The configuration options have been saved.'));
  variable_set('foobartable_settings', $settings);

  drupal_theme_rebuild();
}
