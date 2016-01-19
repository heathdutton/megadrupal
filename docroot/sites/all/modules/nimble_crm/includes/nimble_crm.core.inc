<?php

/**
 * @file
 * API callbacks Nimble CRM module.
 */

/**
 * Load a Nimble OAuth2 client.
 */
function nimble_crm_oauth2_client_load($name) {
  nimble_crm_include('oauth2_client');
  $oauth2_clients = oauth2_client_get_all();

  if (!isset($oauth2_clients[$name])) {
    throw new Exception(t('No client with name @name is defined.', array('@name' => $name)));
  }
  $oauth2_client = new Drupal\nimble_crm\NimbleClient($oauth2_clients[$name], $name);
  return $oauth2_client;
}

/**
 * Gets and returns a Nimble access token.
 *
 * @link http://nimble.readthedocs.org/en/latest/obtaining_key/
 */
function nimble_crm_api_token_get() {
  try {
    $oauth_client = nimble_crm_oauth2_client_load(NIMBLE_CRM_OAUTH_CLIENT);
    $oauth_client->getAccessToken();
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}

/**
 * Performs a Nimble API request.
 *
 * @link http://nimble.readthedocs.org/en/latest/making_auth_requests/#authenticating-your-request-with-http-header
 */
function nimble_crm_api_request($method, $data, $url) {
  // Get token from database.
  $db_token = variable_get('nimble_crm_token');
  $settings_link = l(t('Check Nimble API credentials'), 'admin/config/services/nimble-crm');
  // Exit if token was not initialized and stored in database.
  if (empty($db_token['refresh_token'])) {
    _nimble_crm_log('token', 'Token was not initialized and stored in database.', NULL, WATCHDOG_WARNING, $settings_link);
    return;
  }
  // Share an access token from database with oauth2_client.
  $session_token = oauth2_client_get_token(NIMBLE_CRM_OAUTH_CLIENT);
  if (empty($session_token['access_token'])) {
    _nimble_crm_log('token', 'Token was shared from stored value.', $db_token);
    oauth2_client_set_token(NIMBLE_CRM_OAUTH_CLIENT, $db_token);
  }
  // Refresh token if it expires.
  $token = oauth2_client_get_token(NIMBLE_CRM_OAUTH_CLIENT);
  if (!empty($token['refresh_token']) && REQUEST_TIME > $token['expiration_time']) {
    _nimble_crm_log('token', 'Token was updated.');
    nimble_crm_api_token_get();
  }
  // Exit if token could not be initialized.
  if (empty($token['access_token'])) {
    _nimble_crm_log('token', 'Token was not initialized.', NULL, WATCHDOG_WARNING, $settings_link);
    return;
  }
  // Set new token value into the database if it was updated.
  if ($db_token['refresh_token'] != $token['refresh_token']) {
    variable_set('nimble_crm_token', $token);
    _nimble_crm_log('token', 'Token was updated in database.', $token);
  }

  $options = array(
    'method' => $method,
    'data' => json_encode($data),
    'headers' => array(
      'Content-Type' => 'application/json',
      'Accept' => 'application/json',
      'Authorization' => 'Bearer ' . $token['access_token'],
    ),
  );

  $response = drupal_http_request($url, $options);
  return $response;
}

/**
 * Creates new contact in Nimble CRM.
 *
 * @link http://nimble.readthedocs.org/en/latest/contacts/basic/create/
 */
function nimble_crm_api_contact_create($fields, $tags = '') {
  $method = 'POST';
  $url = 'https://api.nimble.com/api/v1/contact';
  $data = new stdClass();

  $data->record_type = 'person';
  $data->fields = $fields;
  $data->tags = $tags;

  $response = nimble_crm_api_request($method, $data, $url);
  if (!empty($response->status_message)) {
    _nimble_crm_log('contact', 'Contact Create: ' . $response->status_message, $response);
  }
}

/**
 * Search for contact in Nimble CRM.
 *
 * @link http://nimble.readthedocs.org/en/latest/contacts/basic/search/
 */
function nimble_crm_api_contact_search($fields) {
  $method = 'GET';
  $url = 'https://api.nimble.com/api/v1/contacts';

  $query = new stdClass();
  $operator = 'or';

  foreach ($fields as $key => $value) {
    if (!empty($value)) {
      $field_is = new stdClass();
      $field_is->{$key} = new stdClass();
      $field_is->{$key}->{'is'} = $value;
      $query->{$operator}[] = $field_is;
    }
  }
  $query_json = drupal_json_encode($query);

  $url = url($url, array(
    'query' => array(
      'query' => $query_json,
    ),
    'external' => TRUE,
  ));

  $response = nimble_crm_api_request($method, $query, $url);
  if (!empty($response->data) && $response->code < 300) {
    $response_data = drupal_json_decode($response->data);
    _nimble_crm_log('contact', 'Contact Search: ' . $response->status_message, $response_data['resources']);
    return $response_data['resources'];
  }
}

/**
 * Creates new contact if given phone or email are not found in Nimble CRM.
 */
function nimble_crm_contact_create($fields, $tags) {
  if (!empty($fields->email[0]->value)) {
    $search_fields['email'] = $fields->email[0]->value;
    $search_fields['phone'] = !empty($fields->phone[0]->value) ? $fields->phone[0]->value : '';
    // Check if record with given email or phone already exists in Nimble CRM.
    $search_results = nimble_crm_api_contact_search($search_fields);
    // If record not exist create new one.
    if (empty($search_results)) {
      nimble_crm_api_contact_create($fields, $tags);
    }
  }
}

/**
 * Returns list of Nimble default contact fields.
 *
 * @link http://nimble.readthedocs.org/en/latest/contacts/fields/#nimble-default-fields
 */
function nimble_crm_field_types() {
  $options = array(
    'first name' => t('First Name'),
    'last name' => t('Last Name'),
    'company name' => t('Company'),
    'email' => t('Email'),
    'phone' => t('Phone'),
  );
  return $options;
}

/**
 * Logs a system message with Nimble API responses.
 */
function _nimble_crm_log($type, $message, $log_nvp = NULL, $severity = WATCHDOG_NOTICE, $link = NULL) {
  // Check if this type of log is enabled.
  $nimble_crm_log = variable_get('nimble_crm_log');
  if (!empty($nimble_crm_log[$type])) {
    // Check if additional debug information is included.
    if (!empty($log_nvp)) {
      $message .= ' <pre>@log</pre>';
    }
    watchdog('nimble_crm', $message, array('@log' => print_r($log_nvp, TRUE)), $severity, $link);
  }
}

/**
 * Common Settings form for Nimble CRM module.
 */
function nimble_crm_common_contact_settings_form($form, &$form_state) {
  $form['#tree'] = TRUE;
  $default_values = $form_state['default_values'];

  // Check token from database.
  $db_token = variable_get('nimble_crm_token');
  if (empty($db_token['access_token'])) {
    drupal_set_message(t('You need to !authorize your Nimble CRM account!', array(
      '!authorize' => l(t('authorize'), 'admin/config/services/nimble-crm'),
    )), 'warning');
  }
  else {
    // Default settings.
    $form['nimble_crm_send'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send results of this Webform to Nimble CRM'),
      '#default_value' => isset($default_values['nimble_crm_send']) ? $default_values['nimble_crm_send'] : 0,
    );
    $form['nimble_crm_tags'] = array(
      '#type' => 'textfield',
      '#title' => t('Contact tags'),
      '#description' => t('Comma separated list of tags to assign to contacts.'),
      '#default_value' => isset($default_values['nimble_crm_tags']) ? $default_values['nimble_crm_tags'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="nimble_crm_send"]' => array('checked' => TRUE),
        ),
      ),
    );
    // Custom fields settings.
    $custom_fields_default = $default_values['nimble_crm_custom_fields'];
    $form['nimble_crm_custom_fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Custom Fields'),
      '#description' => t('Here you can set default values for your custom fields'),
      '#collapsible' => TRUE,
      '#prefix' => '<div id="custom-fields-wrapper">',
      '#suffix' => '</div>',
    );

    // Build the fieldset with the proper number of fields. We'll use
    // $form_state['num_fields'] to determine the number of fields to build.
    if (empty($form_state['num_fields'])) {
      $form_state['num_fields'] = 1;
      if (empty($custom_fields_default[0]['name'])) {
        $form['nimble_crm_custom_fields']['#collapsed'] = TRUE;
      }
    }
    for ($i = 0; $i < $form_state['num_fields']; $i++) {
      $form['nimble_crm_custom_fields'][$i] = array(
        '#type' => 'fieldset',
      );
      $form['nimble_crm_custom_fields'][$i]['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Field Name'),
        '#default_value' => !empty($custom_fields_default[$i]['name']) ? $custom_fields_default[$i]['name'] : '',
      );
      $form['nimble_crm_custom_fields'][$i]['value'] = array(
        '#type' => 'textfield',
        '#title' => t('Value'),
        '#default_value' => !empty($custom_fields_default[$i]['value']) ? $custom_fields_default[$i]['value'] : '',
      );
    }
    $form['nimble_crm_custom_fields']['add_field'] = array(
      '#type' => 'submit',
      '#value' => t('Add one more'),
      '#submit' => array('nimble_crm_add_more_add_one'),
      // See the examples in nimble_crm.module for more details on the
      // properties of #ajax.
      '#ajax' => array(
        'callback' => 'nimble_crm_add_more_callback',
        'wrapper' => 'custom-fields-wrapper',
      ),
    );
    if ($form_state['num_fields'] > 1) {
      $form['nimble_crm_custom_fields']['remove_field'] = array(
        '#type' => 'submit',
        '#value' => t('Remove one'),
        '#submit' => array('nimble_crm_add_more_remove_one'),
        '#ajax' => array(
          'callback' => 'nimble_crm_add_more_callback',
          'wrapper' => 'custom-fields-wrapper',
        ),
      );
    }

    $form['actions'] = array(
      '#type' => 'actions',
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }

  return $form;
}

/**
 * Callback for ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function nimble_crm_add_more_callback($form, $form_state) {
  return $form['nimble_crm_custom_fields'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function nimble_crm_add_more_add_one($form, &$form_state) {
  $form_state['num_fields']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function nimble_crm_add_more_remove_one($form, &$form_state) {
  if ($form_state['num_fields'] > 1) {
    $form_state['num_fields']--;
  }
  $form_state['rebuild'] = TRUE;
}
