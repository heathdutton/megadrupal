<?php
/**
 * @file
 * Opac users registrations related function or form.
 */

/**
 * Form constructor for library registration form.
 *
 * @see opac_users_library_register_form_submit()
 * @ingroup forms
 */
function opac_users_library_register_form($form, &$form_state) {

  // Set step number if it is the first.
  if (empty($form_state['storage']['step'])) {
    $form_state['storage']['step'] = 0;
  }

  // Form depends on the step.
  // First one ask for the library user would like register.
  // The second is the registration form itself.
  switch ($form_state['storage']['step']) {
    case 0:
      $form = _opac_users_register_handler_step_0($form, $form_state);
      break;

    case 1:
      $form = _opac_users_register_handler_step_1($form, $form_state);
      break;

  }

  return $form;
}

/**
 * Form constructor for library registration form step 0.
 *
 * @see opac_users_library_register_form_submit()
 * @ingroup forms
 */
function _opac_users_register_handler_step_0($form, &$form_state) {
  module_load_include('inc', 'opac', "includes/opac.db");

  // Get all available servers.
  $servers = opac_server_load_multiple(FALSE, array('serv_enabled' => 1));

  // Build ILS server options.
  $server_options = array('none' => t('None'));
  foreach ($servers as $id => $server) {
    $server_options[$id] = $server->serv_name;
  }

  $form['serv_id'] = array(
    '#type' => 'select',
    '#title' => t('Library'),
    '#default_value' => 'none',
    '#options' => $server_options,
    '#description' => t('Choose the library you want to register.'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue registration'),
  );

  return $form;
}

/**
 * Form constructor for library registration form step 1.
 *
 * @see opac_users_library_register_form_submit()
 * @ingroup forms
 */
function _opac_users_register_handler_step_1($form, &$form_state) {
  module_load_include('inc', 'opac', "includes/opac.db");

  global $user;
  $form = array();

  // Create an instance of the connector.
  $server = opac_server_load($form_state['storage']['serv_id']);
  $connector = opac_get_instance($server);

  if (method_exists($connector, 'patronRegistrationForm')) {
    $connector_form = $connector->patronRegistrationForm($user);
    $form += $connector_form;
  }

  return $form;
}

/**
 * Form submission handler for opac_users_library_register_form().
 */
function opac_users_library_register_form_submit($form, &$form_state) {
  switch ($form_state['storage']['step']) {
    case 0:
      $form_state['rebuild'] = TRUE;
      $form_state['storage']['serv_id'] = $form_state['values']['serv_id'];
      $form_state['storage']['step'] = 1;
      break;

    case 1:
  }
}
