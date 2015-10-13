<?php

/**
 * @file
 *   Admin forms for the Appbar module.
 */

/**
 * Settings form.
 */
function appbar_admin($form, &$form_state) {
  $form['appbar_css'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#description' => t('Select the style you would like to use for the appbar.'),
    '#required' => TRUE,
    '#default_value' => variable_get('appbar_css', 'default'),
    '#options' => _appbar_add_css(TRUE),
  );

  $form['#submit'][] = 'appbar_admin_submit';
  return system_settings_form($form);
}

/**
 * Submit function for the admin settings form.
 */
function appbar_admin_submit($form, &$form_state) {
  if ($form_state['values']['appbar_css'] != variable_get('appbar_css', 'default')) {
    drupal_clear_css_cache();
  }
}
