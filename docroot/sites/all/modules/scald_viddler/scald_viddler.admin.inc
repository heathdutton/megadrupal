<?php
/**
 * @file
 *   Provides admin overview page listing all config pages.
 */

/**
 * Defines the authentication settings form.
 */
function scald_viddler_authentication_form() {
  $saved_settings = variable_get('scald_viddler_authentication', array());
  $form = array();

  $form['preamble'] = array(
    '#type' => 'item',
    '#title' => '',
    '#markup' => 'In order to use Viddler\'s API you must have a Viddler account and an API key. If you haven\'t got an account, you can sign up at <a href="http://www.viddler.com/">viddler.com</a>.',
  );
  $form['wrapper'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $form['wrapper']['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User credentials'),
    '#tree' => TRUE,
  );
  $form['wrapper']['user']['username'] = array(
    '#type' => 'textfield',
    '#title' => 'Viddler username',
    '#default_value' => isset($saved_settings['username']) ? $saved_settings['username'] : '',
    '#required' => TRUE,
  );
  $form['wrapper']['user']['password'] = array(
    '#type' => 'textfield',
    '#title' => 'Viddler password',
    '#default_value' => isset($saved_settings['password']) ? $saved_settings['password'] : '',
    '#required' => TRUE,
  );
  $form['wrapper']['user']['ssl'] = array(
    '#type' => 'checkbox',
    '#title' => 'Use SSL when signing in.',
    '#default_value' => isset($saved_settings['use_ssl']) ? $saved_settings['use_ssl'] : '1',
  );
  $form['wrapper']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('API V2 credentials'),
    '#tree' => TRUE,
  );
  $form['wrapper']['api']['api_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Viddler API key',
    '#default_value' => isset($saved_settings['api_key']) ? $saved_settings['api_key'] : '',
    '#description' => t('To find your API key, please log into viddler.com and navigate to \'Your Username > Settings > API\'.'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Check Credentials'),
  );
  return $form;
}

/**
 * Handles the validation of the authentication settings form.
 * Tries to authenticate using the supplied credentials and throws a form error
 */
function scald_viddler_authentication_form_validate($form, &$form_state) {
  $username = $form_state['values']['wrapper']['user']['username'];
  $password = $form_state['values']['wrapper']['user']['password'];
  $use_ssl = $form_state['values']['wrapper']['user']['ssl'];
  $api_key = $form_state['values']['wrapper']['api']['api_key'];
  $options = array(
    'query' => array(
      'key' => $api_key,
      'user' => $username,
      'password' => $password,
      'get_record_token' => '1',
    ),
  );

  $endpoint = VIDDLER_API_V2_ENDPOINT;
  if ($use_ssl) {
    $endpoint = str_replace('http', 'https', $endpoint);
    $options['https'] = TRUE;
  }

  $url = url($endpoint . 'viddler.users.auth.json', $options);
  $response = drupal_http_request($url);

  if (isset($response->data)) {
    $response_data = drupal_json_decode($response->data);
  }
  else {
    $response_data = array();
  }

  if ($response->code != 200) {
    // Check what the API error code was and set a form error.
    $code = isset($response_data['error']['code']) ? $response_data['error']['code'] : 'Unknown';
    $description = isset($response_data['error']['description']) ? $response_data['error']['description'] : 'Unknown';
    form_set_error('wrapper', t('Viddler authentication failed. Code: @code, Description: @description.', array('@code' => $code, '@description' => $description)));
  }
  else {
    // Authorisation successful.
    // If we have a sessionid, return it.
    if (isset($response_data['auth']['sessionid']) && !empty($response_data['auth']['sessionid'])) {
      return $response_data['auth']['sessionid'];
    }
    // No sessionid set, so flag a form error.
    form_set_error('wrapper', t('Authentication failed. No sessionid was returned by the Viddler API.'));
  }
}

/**
 * Handles the submission of the authentication settings form.
 */
function scald_viddler_authentication_form_submit($form, &$form_state) {
  drupal_set_message(t('The credentials have been verified and saved.'));
  $authentication = array();
  $authentication['username'] = $form_state['values']['wrapper']['user']['username'];
  $authentication['password'] = $form_state['values']['wrapper']['user']['password'];
  $authentication['use_ssl'] = $form_state['values']['wrapper']['user']['ssl'];
  $authentication['api_key'] = $form_state['values']['wrapper']['api']['api_key'];
  variable_set('scald_viddler_authentication', $authentication);
}

/**
 * Themes the current imports form.
 */
function theme_scald_viddler_imports_table($variables) {
  $form = $variables['form'];
  $header = array(t('Type'), t('Identifier'));
  $rows = array();
  foreach (element_children($form) as $key) {
    $rows[] = array(
      'data' => array(
        drupal_render($form[$key]['type']),
        drupal_render($form[$key]['value']),
      ),
    );
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'scald-viddler-imports')));
  $output .= drupal_render_children($form);
  return $output;
}
