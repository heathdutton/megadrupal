<?php

/**
 * @file
 * Zendesk administration forms.
 */

/**
 * Form builder callback for Zendesk settings form.
 *
 * @param array $form
 *   Nested array of form elements that comprise the form.
 * @param array $form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array $form
 *   Returns array of form elements.
 */
function dynamic_zendesk_forms_admin_form(array $form, array &$form_state) {
  $fields_configuration = variable_get('dynamic_zendesk_forms_fields_configuration', array());

  $form['dynamic_zendesk_forms_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Zendesk API key'),
    '#default_value' => variable_get('dynamic_zendesk_forms_api_key', ''),
    '#description' => t('Zendesk API Key. This will be used for making Zendesk api calls.'),
    '#required' => TRUE,
  );

  $form['dynamic_zendesk_forms_user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Zendesk username'),
    '#default_value' => variable_get('dynamic_zendesk_forms_user_name', ''),
    '#description' => t('The username of your Zendesk agent.'),
    '#required' => TRUE,
  );

  $form['dynamic_zendesk_forms_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Zendesk API url'),
    '#default_value' => variable_get('dynamic_zendesk_forms_api_url', ''),
    '#description' => t('Zendesk API url. The url is where you access your Zendesk backend.'),
    '#required' => TRUE,
  );

  $form['dynamic_zendesk_forms_ticket_form_id'] = array(
    '#title' => t('Zendesk Ticket Form'),
    '#type' => 'select',
    '#options' => _dynamic_zendesk_forms_list_forms(),
    '#default_value' => variable_get('dynamic_zendesk_forms_ticket_form_id', ''),
    '#description' => t('Zendesk Ticket forms.'),
  );

  $form['dynamic_zendesk_forms_fields_configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Zendesk System to Custom Field Mapping'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $ticket_fields = _dynamic_zendesk_forms_list_ticket_fields();
  $form['dynamic_zendesk_forms_fields_configuration']['description'] = array(
    '#title' => t('Description'),
    '#type' => 'select',
    '#options' => $ticket_fields,
    '#default_value' => isset($fields_configuration['description']) ? $fields_configuration['description'] : '',
    '#description' => t('Custom Field for Description.'),
  );

  $form['dynamic_zendesk_forms_fields_configuration']['requester_name'] = array(
    '#title' => t('Requester Name'),
    '#type' => 'select',
    '#options' => $ticket_fields,
    '#default_value' => isset($fields_configuration['requester_name']) ? $fields_configuration['requester_name'] : '',
    '#description' => t('Custom Field for Requester Name.'),
  );

  $form['dynamic_zendesk_forms_fields_configuration']['requester_email'] = array(
    '#title' => t('Requester Email'),
    '#type' => 'select',
    '#options' => $ticket_fields,
    '#default_value' => isset($fields_configuration['requester_email']) ? $fields_configuration['requester_email'] : '',
    '#description' => t('Custom Field for Requester Email.'),
  );
  // Duration are in seconds.
  $form['dynamic_zendesk_forms_cache_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Minimum cache lifetime'),
    '#description' => t('Cached Zendesk content will not be re-created at least for this much time has elapsed.'),
    '#options' => array(
      0 => t('no cache'),
      10800 => t('3 hours'),
      21600 => t('6 hours'),
      86400 => t('1 day'),
      172800 => t('2 days'),
      345600 => t('4 days'),
      604800 => t('1 week'),
    ),
    '#default_value' => variable_get('dynamic_zendesk_forms_cache_lifetime', 172800),
  );

  return system_settings_form($form);
}

/**
 * Returns all the forms created in zendesk.
 *
 * @return array
 *   Returns array comprising the list of forms created in Zendesk.
 */
function _dynamic_zendesk_forms_list_forms() {
  $ticket_forms = array();
  $response = _dynamic_zendesk_forms_perform_curl_request(DYNAMIC_ZENDESK_FORMS_REQUEST_LIST_FORMS);
  if (isset($response['ticket_forms'])) {
    foreach ($response['ticket_forms'] as $forms) {
      if ($forms['active']) {
        $ticket_forms[$forms['id']] = $forms['name'];
      }
    }
  }

  return $ticket_forms;
}

/**
 * Returns all the ticket fields.
 *
 * @return array
 *   Returns the list of ticket fields.
 */
function _dynamic_zendesk_forms_list_ticket_fields() {
  $ticket_fields = array();
  $fields_list = _dynamic_zendesk_forms_perform_curl_request(DYNAMIC_ZENDESK_FORMS_REQUEST_LIST_FIELDS);

  if (isset($fields_list['ticket_fields'])) {
    foreach ($fields_list['ticket_fields'] as $field) {
      if (!$field['active'] || $field['type'] == 'description') {
        continue;
      }
      $ticket_fields[$field['id']] = $field['title'];
    }
  }

  return $ticket_fields;
}
