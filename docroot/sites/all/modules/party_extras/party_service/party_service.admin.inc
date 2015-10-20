<?php
/**
 * @file
 * Admin forms for settings up party services.
 */

/**
 * Form callback for party services.
 */
function party_service_form($form, &$form_state, $service = NULL, $op = 'edit') {
  if ($op == 'add' && is_null($service)) {
    $service = entity_create('party_service', array());
  }

  $form_state['party_service'] = $form['#party_service'] = $service;

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => !empty($service->label) ? $service->label : '',
    '#description' => t('The human-readable name of this service.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($service->name) ? $service->name : '',
    '#maxlength' => 32,
    '#disabled' => $op != 'add' && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'party_service_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this service. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug Mode'),
    '#default_value' => !empty($service->debug),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form submit callback.
 */
function party_service_form_submit($form, &$form_state) {
  // Build the process type.
  $service = entity_ui_form_submit_build_entity($form, $form_state);
  if (empty($service->authentication)) {
    $service->authentication = array();
  }
  if (empty($service->settings)) {
    $service->settings = array();
  }

  $service->save();
  $form_state['redirect'] = 'admin/config/services/party_service';
}
