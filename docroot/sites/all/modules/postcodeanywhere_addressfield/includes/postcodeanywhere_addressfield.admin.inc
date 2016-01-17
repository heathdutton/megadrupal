<?php

/**
 * Administration settings.
 */
function postcodeanywhere_addressfield_admin_settings($form, &$form_state) {
  // Debugging settings.
  $debug_log = variable_get('postcodeanywhere_addressfield_debug_log', FALSE);
  $debug = variable_get('postcodeanywhere_addressfield_debug', FALSE);
  $debug_activated = ((boolean) $debug || ($debug_log && user_access('access postcodeanywhere addressfield debug')));
  // Debugging the Postcodeanywhere service settings if it is activated.
  if ($debug_activated) {
    $postcodeanywhere_web_services = postcodeanywhere_addressfield_web_services_check();
  }
  $form['postcodeanywhere_addressfield_login'] = array(
    '#title' => t('Login'),
    '#type' => 'textfield',
    '#default_value' => variable_get('postcodeanywhere_addressfield_login', ''),
    '#description' => t('Login code you have from Postcodeanywhere service.'),
  );

  $form['postcodeanywhere_addressfield_license'] = array(
    '#title' => t('License'),
    '#type' => 'textfield',
    '#default_value' => variable_get('postcodeanywhere_addressfield_license', ''),
    '#description' => t('License code you have from Postcodeanywhere service.'),
    '#required' => TRUE,
  );

  $options = array(
    'button' => t('Use search address button.'),
    'autocomplete' => t('Use autocomplete search.'),
  );

  $form['postcodeanywhere_addressfield_widget_type'] = array(
    '#type' => 'select',
    '#title' => t('Type of the PostcodeAnywhere widget'),
    '#description' => t('The form elements & workflow to be used for the PostcodeAnywhere addressfield handler.'),
    '#options' => $options,
    '#empty_value' => '',
    '#default_value' => variable_get('postcodeanywhere_addressfield_widget_type', 'button'),
    '#required' => TRUE,
  );
  $form['postcodeanywhere_addressfield_hide_extra_fields'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide extra fields'),
    '#description' => t('Do not show fields as names or company on the postocode search form.'),
    '#default_value' => variable_get('postcodeanywhere_addressfield_hide_extra_fields', TRUE),
  );

  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debugging'),
    '#collapsible' => TRUE,
    '#collapsed' => !(variable_get('postcodeanywhere_addressfield_debug_log', 0) || variable_get('postcodeanywhere_addressfield_debug', 0)),
  );
  $form['debug']['postcodeanywhere_addressfield_debug_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log debug information to the system log'),
    '#default_value' => variable_get('postcodeanywhere_addressfield_debug_log', 0),
  );
  $form['debug']['postcodeanywhere_addressfield_debug'] = array(
    '#type' => 'radios',
    '#title' => t('Show debug information'),
    '#default_value' => variable_get('postcodeanywhere_addressfield_debug', 0),
    '#options' => array(
      0 => t('Never'),
      2 => t('In case of errors'),
      1 => t('Always'),
    ),
    '#description' => t('Debug information is only shown for PostcodeAnywhere requests and is visible for users having the permission <a href="!url">%link</a>.', array('%link' => t('Access the Postcodeanywhere Addressfield debug log'), '!url' => url('admin/people/permissions', array('fragment' => 'module-postcodeanywhere_addressfield')))),
  );

  return system_settings_form($form);
}
