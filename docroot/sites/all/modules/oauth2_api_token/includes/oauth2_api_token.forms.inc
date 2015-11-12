<?php
/**
 * @file
 * Forms for the OAuth2 API Token module.
 */

/**
 * The API token editing form.
 */
function oauth2_api_token_form($form, &$form_state, OAuth2ServerToken $token = NULL) {
  form_load_include($form_state, 'inc', 'module', 'includes/oauth2_api_token.forms');

  $form['actions'] = array('#type' => 'actions', '#weight' => 100);

  $cancel_path = 'user/' . $GLOBALS['user']->uid . '/api-tokens';
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    $cancel_path = $destination['destination'];
  }
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $cancel_path,
    '#weight' => 1,
  );

  if (empty($form_state['password_confirmed'])) {
    $form['description'] = array(
      '#prefix' => '<p>',
      '#markup' => t('Confirm your password to continue.'),
      '#suffix' => '</p>',
    );
    $form['password'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#size' => 25,
      '#required' => TRUE,
      '#attributes' => array('autocomplete' => 'off'),
      '#element_validate' => array('oauth2_api_token_element_validate_password'),
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
    );
    return $form;
  }

  $server_name = variable_get('oauth2_api_token_server');
  $server = $server_name ? oauth2_server_load($server_name) : NULL;
  if (!$server) {
    drupal_set_message(t('OAuth2 server not found'), 'error');
    watchdog('oauth2_api_token', 'OAuth2 server not found, or not configured', array(), WATCHDOG_ERROR);
    $form['#disabled'] = TRUE;
    return $form;
  }

  /** @var OAuth2ServerToken $token */
  $token = $token ?: entity_get_controller('oauth2_server_token')->create(array(
    'type' => 'api_token',
    'uid' => $GLOBALS['user']->uid,
    'expires_in' => NULL,
  ));

  $form_state['token'] = $token;

  // Add the Application field.
  field_attach_form('oauth2_server_token', $token, $form, $form_state, NULL, array(
    'field_name' => 'api_token_app',
  ));

  // Find the current scopes for the token.
  $token_scopes = array();
  if (!empty($token->scopes)) {
    foreach ($token->scopes[LANGUAGE_NONE] as $reference) {
      $token_scopes[] = $reference['target_id'];
    }
  }

  // Load all the scopes for the server.
  $api_scopes = variable_get('oauth2_api_token_scopes', array('account'));
  $server_scopes = oauth2_server_scope_load_multiple($server->name, $api_scopes);
  $default_scopes = array();
  $scope_options = array();
  foreach ($server_scopes as $scope) {
    $scope_id = $scope->internalIdentifier();
    $scope_options[$scope_id] = t('@name (@description)', array(
      '@name' => $scope->name,
      '@description' => $scope->description,
    ));
    if (empty($token->token_id) && $server->settings['default_scope'] == $scope->name) {
      $default_scopes[] = $scope_id;
    }
    elseif (in_array($scope_id, $token_scopes)) {
      $default_scopes[] = $scope_id;
    }
  }

  if ($scope_options) {
    $form['scopes'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Scopes'),
      '#options' => $scope_options,
      '#description' => t('Scopes restrict what the client is authorized to access using this API token.'),
      '#weight' => 80,
      '#default_value' => $default_scopes,
    );
  }

  if (user_access('oauth2_api_token administer')) {
    $default_account = menu_get_object('user') ?: $GLOBALS['user'];
    $token_user = user_load($token->uid);
    $form['username'] = array(
      '#type' => 'textfield',
      '#title' => t('User account'),
      '#description' => t('The account associated with the token.'),
      '#size' => 30,
      '#maxlength' => 60,
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($token->token_id) ? $default_account->name : $token_user->name,
      '#weight' => 90,
      '#required' => TRUE,
    );
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => empty($token->token_id) ? t('Create API token') : t('Update API token'),
  );

  return $form;
}

/**
 * Validation callback for the API token form.
 */
function oauth2_api_token_form_validate(&$form, &$form_state) {
  // We are at the password confirm stage: no validation is required.
  if (isset($form_state['values']['password'])) {
    return;
  }

  field_attach_validate('oauth2_server_token', $form_state['token'], array(
    'field_name' => 'api_token_app',
  ));

  if (!empty($form_state['values']['username'])) {
    $account = user_load_by_name($form_state['values']['username']);
    if ($account) {
      $form_state['token_owner'] = $account;
    }
    else {
      form_set_error('username', t('User not found: @name', array('@name' => $form_state['values']['username'])));
    }
  }

  $server_name = variable_get('oauth2_api_token_server');
  $client_key = 'api_token_' . $server_name;
  $client = oauth2_server_client_load($client_key);
  if ($client && $client->server == $server_name) {
    $form_state['client_id'] = $client->client_id;
  }
  else {
    form_set_error('client', t('Client not found: @key', array('@key' => $client_key)));
  }
}

/**
 * Element validate callback for confirming the user's password.
 */
function oauth2_api_token_element_validate_password($element, &$form_state) {
  // Only validate the password for the current user.
  $account = $GLOBALS['user'];
  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
  if (!$element['#value'] || !user_check_password($element['#value'], $account)) {
    form_error($element, t("Incorrect password."));
  }
}

/**
 * Submit callback for the API token form.
 */
function oauth2_api_token_form_submit(&$form, &$form_state) {
  // Password confirmation.
  if (isset($form_state['values']['password'])) {
    $form_state['password_confirmed'] = TRUE;
    $form_state['rebuild'] = TRUE;
    return;
  }

  /** @var OAuth2ServerToken $token */
  $token = $form_state['token'];

  field_attach_submit('oauth2_server_token', $token, $form, $form_state);

  $token->scopes = array();
  foreach (array_unique($form_state['values']['scopes']) as $scope_id) {
    if ($scope_id) {
      $token->wrapper()->scopes[$scope_id] = $scope_id;
    }
  }

  if (!empty($form_state['token_owner'])) {
    $token->uid = $form_state['token_owner']->uid;
  }

  $token->client_id = $form_state['client_id'];

  $access_token = new OAuth2ApiTokenAccessToken();

  $new = empty($token->token_id);

  $success = $access_token->saveApiToken($token);
  if ($success && $new) {
    drupal_set_message(t('Created API token for %application: <code>@value</code>', array(
      '%application' => $token->wrapper()->api_token_app->value(),
      '@value' => $token->token,
    )), 'statue', FALSE);
  }
  elseif ($success) {
    drupal_set_message(t('Updated API token for %application', array(
      '%application' => $token->wrapper()->api_token_app->value(),
    )));
  }
  else {
    drupal_set_message(t('Failed to save API token'), 'error');
  }

  $form_state['redirect'] = 'user/' . $token->uid . '/api-tokens';
}

/**
 * Confirmation form for deleting a token.
 */
function oauth2_api_token_delete_form($form, &$form_state, OAuth2ServerToken $token) {
  $account = oauth2_api_token_get_account($token);
  $form_state['account'] = $account;
  $form_state['token'] = $token;

  return confirm_form(
    $form,
    t('Are you sure you want to delete this token?'),
    'user/' . $account->uid . '/api-tokens'
  );
}

/**
 * Confirmation form for deleting a token.
 */
function oauth2_api_token_delete_form_submit($form, &$form_state) {
  /** @var OAuth2ServerToken $token */
  $token = $form_state['token'];
  $account = $form_state['account'];
  $token->delete();
  drupal_set_message(t('The API token was successfully deleted'));
  $form_state['redirect'] = 'user/' . $account->uid . '/api-tokens';
}

/**
 * Form for viewing a token's value.
 */
function oauth2_api_token_view_value_form($form, &$form_state, OAuth2ServerToken $token, $account = NULL) {
  form_load_include($form_state, 'inc', 'module', 'includes/oauth2_api_token.forms');

  $account = $account ?: $GLOBALS['user'];
  $form_state['account'] = $account;

  $form['token'] = entity_view('oauth2_server_token', array($token));
  unset($form['token']['token']);

  $form['actions'] = array('#type' => 'actions', '#weight' => 100);

  if (empty($form_state['password_confirmed'])) {
    $form['description'] = array(
      '#prefix' => '<p>',
      '#markup' => t('Confirm your password to view the API token'),
      '#suffix' => '</p>',
    );
    $form['password'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#size' => 25,
      '#required' => TRUE,
      '#attributes' => array('autocomplete' => 'off'),
      '#element_validate' => array('oauth2_api_token_element_validate_password'),
    );
    $form['actions']['cancel'] = array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'user/' . $account->uid . '/api-tokens',
      '#weight' => 1,
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
    );
    return $form;
  }

  $form['token_value'] = array(
    '#type' => 'item',
    '#title' => t('Token'),
    '#attributes' => array(
      'class' => array('oauth2-api-token-value'),
    ),
    '#markup' => '<code>' . check_plain($token->token) . '</code>',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('OK'),
  );

  return $form;
}

/**
 * Submit callback for the API token 'view' form.
 */
function oauth2_api_token_view_value_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['password'])) {
    $form_state['password_confirmed'] = TRUE;
    $form_state['rebuild'] = TRUE;
    return;
  }
  $account = $form_state['account'];
  $form_state['redirect'] = 'user/' . $account->uid . '/api-tokens';
}
