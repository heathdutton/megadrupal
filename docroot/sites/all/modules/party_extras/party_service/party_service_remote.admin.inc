<?php
/**
 * @file
 * Contains admin code for remotes.
 */

/**
 * Form Callback for Party Service Remotes.
 */
function party_service_remote_form($form, &$form_state, $remote = NULL, $op = 'edit') {
  if ($op == 'add' && is_null($remote)) {
    $remote = entity_create('party_service_remote', array());
  }

  $form_state['party_service_remote'] = $form['#party_service_remote'] = $remote;

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => !empty($remote->label) ? $remote->label : '',
    '#description' => t('The human-readable name of this service.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($remote->name) ? $remote->name : '',
    '#maxlength' => 32,
    '#disabled' => $op != 'add' && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'party_service_remote_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this remote. It must only contain lowercase letters, numbers, and underscores.'),
  );
  $form['url'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#default_value' => !empty($remote->url) ? $remote->url : '',
    '#description' => t('The URL for this remote.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['authentication'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Authentication Options'),
    '#tree' => TRUE,
  );
  $form['authentication']['api_key'] = array(
    '#title' => t('API Key'),
    '#description' => t('If the service has given you and API key enter it here.'),
    '#type' => 'textfield',
    '#default_value' => !empty($remote->authentication['api_key']) ? $remote->authentication['api_key'] : '',
    '#size' => 30,
  );

  $form['debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug Mode'),
    '#default_value' => !empty($remote->debug),
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
function party_service_remote_form_submit($form, &$form_state) {
  // Build the process type.
  $remote = entity_ui_form_submit_build_entity($form, $form_state);
  if (empty($remote->authentication)) {
    $remote->authentication = array();
  }
  if (empty($remote->settings)) {
    $remote->settings = array();
  }

  $remote->save();
  $form_state['redirect'] = 'admin/config/services/party_remote';
}
