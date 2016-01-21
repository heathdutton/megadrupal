<?php

/**
 * @file
 * Provides administrative functionality for the CollectiveAccess module
 */

/**
 * Generates the collectiveaccess_instance editing form
 */
function collectiveaccess_instance_form($form, &$form_state, $collectiveaccess_instance, $op = 'edit') {
  $connectors = collectiveaccess_get_connectors();

  if ($op == 'clone') {
    $collectiveaccess_instance->label .= ' (cloned)';
    $collectiveaccess_instance->name .= '_clone';
  }

  // Description
  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
    '#default_value' => isset($collectiveaccess_instance->description) ? $collectiveaccess_instance->description : '',
    '#description' => t('The human-readable description of this CollectiveAccess instance'),
    '#required' => TRUE,
  );

  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($collectiveaccess_instance->name) ? $collectiveaccess_instance->name : '',
    '#disabled' => entity_has_status('collectiveaccess_instance', $collectiveaccess_instance, ENTITY_IN_CODE),
    '#machine_name' => array(
      'exists' => 'collectiveaccess_instance_load',
      'source' => array('description'),
    ),
    '#description' => t('A unique machine-readable name for this CollectiveAccess instance. It must only contain lowercase letters, numbers, and underscores.'),
    '#element_validate' => array('form_validate_machine_name', 'entity_ui_validate_machine_name'),
  );

  $form['connector'] = array(
    '#type' => 'radios',
    '#title' => t('Webservice connection method'),
    '#description' => t('Choose the method to use for connecting to the CollectiveAccess webservice'),
    '#required' => TRUE,
    '#options' => $connectors,
    '#default_value' => !empty($collectiveaccess_instance->connector) ? $collectiveaccess_instance->connector : 'CollectiveAccessSOAPConnector',
  );

  $form['service_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Webservice URL'),
    '#description' => t('Specify the CollectiveAccess webservice URL (ending with service.php)'),
    '#required' => TRUE,
    '#default_value' => !empty($collectiveaccess_instance->service_url) ? $collectiveaccess_instance->service_url : '',
  );

  $form['connector_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Specify a username to authenticate with CollectiveAccess. This user be in a role with permission to access the Web Services API. If access control is disabled for web services, leave blank (not recommended).'),
    '#required' => FALSE,
    '#default_value' => !empty($collectiveaccess_instance->connector_username) ? $collectiveaccess_instance->connector_username : '',
  );

  if (isset($collectiveaccess_instance->connector_password)) {
    $plain_password = aes_decrypt($collectiveaccess_instance->connector_password);
  }
  else {
    $plain_password = '';
  }
  $form['connector_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('Enter the password that matches the username above. The password will be stored encrypted with AES based on your configuration of the AES module.'),
    '#default_value' => $plain_password,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save CollectiveAccess instance'),
    '#weight' => 40,
  );

  if (!entity_has_status('collectiveaccess_instance', $collectiveaccess_instance, ENTITY_IN_CODE) && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete CollectiveAccess instance'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('collectiveaccess_instance_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Validation callback.
 */
function collectiveaccess_instance_form_validate($form, &$form_state) {
  $service_url = $form_state['values']['service_url'];
  if (!collectiveaccess_validate_webservice_url($service_url)) {
    form_set_error('service_url', t('Please provide a valid service.php URL'));
  }

  $plain_password = $form_state['values']['connector_password'];
  $encrypted_password = aes_encrypt($plain_password);
  $form_state['values']['connector_password'] = $encrypted_password;
  entity_form_field_validate('collectiveaccess_instance', $form, $form_state);
}

/**
 * Form API submit callback for the CollectiveAccess instance form.
 */
function collectiveaccess_instance_form_submit(&$form, &$form_state) {
  $collectiveaccess_instance = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $collectiveaccess_instance->save();
  $form_state['redirect'] = 'admin/structure/collectiveaccess';
}

/**
 * Form API submit callback for the delete button.
 */
function collectiveaccess_instance_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/collectiveaccess/manage/' . $form_state['collectiveaccess_instance']->name . '/delete';
}
