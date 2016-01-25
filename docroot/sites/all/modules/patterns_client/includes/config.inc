<?php
/**
 * @file
 * Forms and settings for Patterns Client
 */

module_load_include('inc', 'd2d', 'includes/d2d.forms');

/**
 * Outgoing configuration form.
 * @return $form
 */
function patterns_client_form_outgoing_configure() {
  $form = array();
  $form['address'] = array(
    '#title' => 'Patterns server address',
    '#required' => TRUE,
    '#default_value' => variable_get('patterns_client_server', PATTERNS_CLIENT_OFFICIAL_SERVER),
    '#description' => t('Visit the <a href="@url">D2D admin page</a> to add more servers to the list.', array('@url' => url('admin/d2d'))),
  );
  d2d_forms_select_friends($form['address'], 'url', 'url', TRUE);
  $form['auto'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('patterns_client_auto_publish', 'NOFRIEND'),
    '#title' => t('Automatically publish patterns'),
    '#description' => t('If checked, a pattern will be automatically pushed to the Patterns Server when marked as \'public\'.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('patterns_client_register_server_submit'),
  );
  return $form;
}

/**
 * Implements hook form_submit().
 */
function patterns_client_register_server_submit($form, &$form_state) {
  $server = $form_state['values']['address'];
  if ($server === 'NOFRIEND') {
    variable_set('patterns_client_server', NULL);
  }
  else {
    variable_set('patterns_client_server', $server);
  }
  variable_set('patterns_client_auto_publish', $form_state['values']['auto'] ? TRUE : FALSE);
  drupal_set_message(t('The changes have been saved.'));
}

/**
 * Main configuration form.
 * @return $form
 */
function patterns_client_config() {
  // Parameters: verbose, pclient.
  $info = patterns_client_check_credentials_and_display_warns(TRUE, TRUE);

  $build['main'] = array(
    '#title' => t('Patterns Server'),
    '#type' => 'fieldset',
  );
  $build['main'][] = drupal_get_form('patterns_client_form_outgoing_configure');
  if ($info['url']) {
    // Show credentials.
    $build['credentials'] = array(
      '#title' => 'Your current credentials',
      '#type' => 'fieldset',
      '#description' => t('You can edit this data directly in the Patterns Server web site. Then use the "link to an existing username" form.')
    );
    $build['credentials'][] = drupal_get_form('patterns_client_credentials_form', $info);

    // Register new user.
    $build['register'] = array(
      '#title' => $info['credentials'] ? t('Register a new username (overwrites your current credentials)') : t('Register a new username'),
      '#type' => 'fieldset',
    );
    $build['register'][] = drupal_get_form('patterns_client_register_remoteuser');
    // Link to existing account.
    $build['link'] = array(
      '#title' => $info['credentials'] ? t('Link to an existing username (overwrites your current credentials)') : t('Link to an existing username'),
      '#type' => 'fieldset',
      '#description' =>  t('If you do not remember your username or password visit your Patterns Server web site and try a password recovery procedure there.'), );
    $build['link'][] = drupal_get_form('patterns_client_link_remoteuser');
  }
  return $build;
}

function patterns_client_credentials_form($form, &$form_state, $info) {
  $form = array();
  $credentials = $info['credentials'];
  $url = $info['url'];
  if ($credentials) {
    $rows = array();
    $rows['Server'][] = t('Server');
    $rows['Server'][] = $url;
    $rows['User'][] = t('User');
    $rows['User'][] = $credentials['user'];
    $rows['Token'][] = t('Token');
    $rows['Token'][] = $credentials['token'];
    $rows['Email'][] = t('Email');
    $rows['Email'][] = $credentials['email'];

    $form['credentials_table'] = array(
      '#markup' => theme('table', array(
        'rows' => $rows,
        'attributes' => array('class' => 'patterns-list'))
      ),
    );
    $form['credentials_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Reset current credentials'),
      '#suffix' => t('Your credentials will be deleted only locally, and this action cannot be undone.'),
      '#submit' => array('patterns_client_delete_credentials_submit'),
    );
  }
  else {
    $form['no_credentials'] = array(
      '#markup' => t('No valid credentials found.'),
    );
    // Messages are already displayed.

  }
  return $form;
}

/**
 * Implements hook_submit(). Deletes current credentials from local db.
 */
function patterns_client_delete_credentials_submit($form, &$form_state) {
  $server = variable_get('patterns_client_server');
  if ($server) {
    $current = variable_get(PATTERNS_CLIENT_CREDENTIALS, array());
    unset($current[$server]);
    variable_set(PATTERNS_CLIENT_CREDENTIALS, $current);
    drupal_set_message('Credentials reset.');
  }
  else {
    drupal_set_message('Could not reset credentials. You are not registered to a Patterns Server', 'warning');
  }
}

/**
 * Form for remote user.
 * @return $form
 */
function patterns_client_link_remoteuser() {
  $form = array();
  $form['user'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Existing username on server'),
    '#size' => 40,
    '#maxlength' => PATTERNS_CLIENT_USER_MAX_LENGTH,
    '#required' => TRUE,
  );
  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Token'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Link'),
  );
  return $form;
}

/**
 * Implements hook_validate().
 */
function patterns_client_link_remoteuser_validate($form, &$form_state) {
  if (!variable_get('patterns_client_server', FALSE)) {
    drupal_set_message(t('No server found to link to an existing user'), 'error');
    return FALSE;
  }
  return TRUE;
}

/**
 * Implements hook_submit().
 */
function patterns_client_link_remoteuser_submit($form, &$form_state) {
  $url = variable_get('patterns_client_server'); // TODO here
  $user = $form_state['values']['user'];
  $pwd = $form_state['values']['password'];
  patterns_client_link_user_on_server($url, $user, $pwd);
}

/**
 * Form for registering remote user.
 * @return $form
 */
function patterns_client_register_remoteuser() {
  $form = array();
  $form['user'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('New username to register on server'),
    '#size' => 40,
    '#maxlength' => PATTERNS_CLIENT_USER_MAX_LENGTH,
    '#required' => TRUE,
  );
  $form['password'] = array(
    '#type' => 'password_confirm',
    '#description' => t('Must be at least 6 characters/digits long'),
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Register'),
  );
  return $form;
}

/**
 * Implements hook_validate().
 */
function patterns_client_register_remoteuser_validate($form, &$form_state) {
  if (!variable_get('patterns_client_server', FALSE)) {
    drupal_set_message(t('No server found to register a new user'), 'error');
    return FALSE;
  }
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('The email address appears to be invalid.'));
  }
  if (strlen($form_state['values']['password']) < 6) {
    form_set_error('password', t('Password must be at least 6 characters/digits long'));
  }
  return TRUE;
}

/**
 * Implements hook_submit().
 */
function patterns_client_register_remoteuser_submit($form, &$form_state) {
  $url = variable_get('patterns_client_server'); // TODO here
  $user = $form_state['values']['user'];
  $pwd = $form_state['values']['password'];
  $email = $form_state['values']['email'];
  patterns_client_register_user_on_server($url, $user, $pwd, $email);
}
