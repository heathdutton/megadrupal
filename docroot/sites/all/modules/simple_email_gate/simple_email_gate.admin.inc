<?php

/**
 * @file
 * Admin page callbacks for the simple_email_gate module.
 */

/**
 * Form builder; Configure simple_email_gate settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function simple_email_gate_admin_settings() {
  $options = array(
  0 => t('WhiteList'),
  1 => t('BlackList'),
  );
  $form['to'] = array(
    '#type' => 'fieldset',
    '#title' => t('To'),
    '#collapsible' => TRUE,
  );
  $form['to']['simple_email_gate_to_option'] = array(
    '#type' => 'radios',
    '#title' => t('Type'),
    '#options' => $options,
    '#default_value' => variable_get('simple_email_gate_to_option', 0),
  );
  $form['to']['simple_email_gate_to_list'] = array(
    '#type' => 'textarea',
    '#title' => 'List',
    '#default_value' => variable_get('simple_email_gate_to_list', ''),
    '#description' => t("Specify Email to addresses. Enter one path per line. The '*' character is a wildcard."),
  );

  
  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debugging/Development'),
    '#description' => t("Debug mode will notify if email is blocked."),
    '#collapsible' => TRUE,
  );
  $form['debug']['simple_email_gate_debug'] = array(
    '#title' => t('Enable debug mode.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('simple_email_gate_debug', 0),
  );
  return system_settings_form($form);
}
