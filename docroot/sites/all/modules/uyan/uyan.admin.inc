<?php

/**
 * @file
 * Administration page for the Uyan Comments
 */

/**
 * Return a system setting form for Uyan admin page.
 */
function uyan_admin_settings() {
  $form['uyan_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display uyan on these content types:'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('uyan_node_types', array()),
  );

  $form['uyan_appkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Uyan user id:'),
    '#maxlength' => 256,
    '#default_value' => variable_get('uyan_appkey', 0),
  );
  return system_settings_form($form);
}
