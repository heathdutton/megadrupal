<?php

/**
 * Settings form for Traced.
 * @param $form
 * @return mixed
 */
function traced_admin_settings_form($form) {
  $form['traced_enabled'] = array(
    '#title' => 'Enable Tracy debugger',
    '#description' => 'Enables the Tracy debugger for users with permission to view it.',
    '#type' => 'checkbox',
    '#default_value' => variable_get('traced_enabled', FALSE)
  );

  $form['traced_watchdog_active'] = array(
    '#title' => 'Enable watchdog logging',
    '#description' => 'Logs watchdog messages to the console via Tracy.',
    '#type' => 'checkbox',
    '#default_value' => variable_get('traced_watchdog_active', FALSE)
  );

  $form['traced_strict_mode'] = array(
    '#title' => 'Strict Mode',
    '#description' => 'Sets the strict mode in Tracy.',
    '#type' => 'checkbox',
    '#default_value' => variable_get('traced_strict_mode', FALSE)
  );

  $form['traced_mode'] = array(
    '#title' => 'Debug Mode',
    '#description' => 'Sets the debug mode in Tracy.',
    '#type' => 'select',
    '#options' => array(
      '' => '- Select -',
      'DEVELOPMENT' => 'Development',
      'PRODUCTION' => 'Production',
    ),
    '#default_value' => variable_get('traced_mode', 'DEVELOPMENT')
  );

  return system_settings_form($form);
}