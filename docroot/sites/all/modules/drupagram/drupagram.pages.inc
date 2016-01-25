<?php

/**
 * @file
 * Drupagram admin page callaback functions.
 */

/**
 * Form builder; Instagram settings form.
 */
function drupagram_admin_form($form, &$form_state) {
  $form['drupagram_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import and display the Instagram statuses of site users who have entered their Instagram account information.'),
    '#default_value' => variable_get('drupagram_import', 1),
  );
  $form['drupagram_expire'] = array(
    '#type' => 'select',
    '#title' => t('Delete old items'),
    '#default_value' => variable_get('drupagram_expire', 0),
    '#options' => array(0 => t('Never')) + drupal_map_assoc(array(604800, 2592000, 7776000, 31536000), 'format_interval'),
    '#states' => array(
      'visible' => array(
        ':input[name=drupagram_import]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['oauth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instagram OAuth Settings'),
    '#access' => module_exists('oauth_common'),
    '#description' => t('To enable OAuth based access for drupagram, you must <a href="@url" target="_blank">register your application</a> with Instagram and add the provided keys here.', array('@url' => DRUPAGRAM_URL_APP_REGISTER)),
  );
  $form['oauth']['drupagram_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram Client ID'),
    '#default_value' => variable_get('drupagram_client_id', NULL),
  );
  $form['oauth']['drupagram_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram Client Secret'),
    '#default_value' => variable_get('drupagram_client_secret', NULL),
  );
  $form['oauth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Callback URL'),
    '#markup' => url('instagram/oauth', array('absolute' => TRUE)),
  );

  return system_settings_form($form);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function drupagram_user_settings($account) {
  module_load_include('inc', 'drupagram');

  $output = array();
  if (!empty($account->drupagram_accounts)) {
    $output['list_form'] = drupal_get_form('drupagram_account_list_form', $account->drupagram_accounts);
  }
  $output['form'] = drupal_get_form('drupagram_account_form', $account);

  return $output;
}

/**
 * Account List form builder.
 */
function drupagram_account_list_form($form, $form_state, $drupagram_accounts = array()) {
  $form['#tree'] = TRUE;
  $form['accounts'] = array();

  foreach ($drupagram_accounts as $drupagram_account) {
    $form['accounts'][] = _drupagram_account_list_row($drupagram_account);
  }

  if (!empty($drupagram_accounts)) {
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
    );
  }

  return $form;
}

function _drupagram_account_list_row($account) {
  $form['#account'] = $account;

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $account->id,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['username'] = array(
    '#type' => 'value',
    '#value' => $account->username,
  );

  $form['image'] = array(
    '#markup' => theme('image', array('path' => $account->profile_picture)),
  );

  $form['visible_name'] = array(
    '#markup' => l($account->username, 'http://web.stagram.com/user/' . $account->id, array('attributes' => array('target' => '_blank'))),
  );

  $form['description'] = array(
    '#markup' => filter_xss($account->bio),
  );

  // Here we use user_access('import own media') to check permission
  // instead of user_access('import own media', $account->uid)
  // because we allow roles with sufficient permission to overwrite
  // the user's import settings.
  if (variable_get('drupagram_import', TRUE) && user_access('import own media')) {
    $form['import'] = array(
      '#type' => 'checkbox',
      '#default_value' => user_access('import own media') ? $account->import : '',
    );
  }

  $form['delete'] = array(
    '#type' => 'checkbox',
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_drupagram_account_list_form($variables) {
  $form = $variables['form'];

  if (variable_get('drupagram_import', TRUE) && user_access('import own media')) {
    $header = array('', t('Name'), t('Description'), t('Import'), t('Delete'));
  }
  else {
    $header = array('', t('Name'), t('Description'), t('Delete'));
  }

  $rows = array();

  foreach (element_children($form['accounts']) as $key) {
    $element = &$form['accounts'][$key];
    if (variable_get('drupagram_import', TRUE) && user_access('import own media')) {
      $row = array(
        drupal_render($element['image']),
        drupal_render($element['id']) . drupal_render($element['username']) . drupal_render($element['visible_name']),
        drupal_render($element['description']),
        drupal_render($element['import']),
        drupal_render($element['delete']),
      );
    }
    else {
      $row = array(
        drupal_render($element['image']),
        drupal_render($element['id']) . drupal_render($element['username']) . drupal_render($element['visible_name']),
        drupal_render($element['description']),
        drupal_render($element['delete']),
      );
    }

    $rows[] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function drupagram_account_list_form_submit($form, &$form_state) {
  $accounts = $form_state['values']['accounts'];
  foreach ($accounts as $account) {
    if (empty($account['delete'])) {
      drupagram_account_save($account);
      drupal_set_message(t('The Instagram account settings were updated.'));
    }
    else {
      drupagram_account_delete($account['id']);
      drupal_set_message(t('The Instagram account was deleted.'));
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function drupagram_user_make_global($form, $form_state, $account, $drupagram_id) {
  module_load_include('inc', 'drupagram');

  $drupagram_account = drupagram_account_load($drupagram_id);

  $form = array();

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['drupagram_id'] = array(
    '#type' => 'value',
    '#value' => $drupagram_id,
  );

  if ($drupagram_account->is_global) {
    $text = t('Are you sure you want to remove %username from the global accounts?', array('%username' => $drupagram_account->username));
    $description = t('This means other users will no longer be allowed to post using this account.');
  }
  else {
    $text = t('Are you sure you want to allow other users to access the %username account?', array('%username' => $drupagram_account->username));
    $description = t('This will allow other users to post using this account.');
  }

  return confirm_form($form, $text, 'user/' . $account->uid . '/edit/drupagram', $description);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function drupagram_user_make_global_submit($form, &$form_state) {
  db_update('drupagram_account')
    ->expression('is_global', '(1 - is_global)')
    ->condition('drupagram_id', $form_state['values']['drupagram_id'])
    ->execute();

  $form_state['redirect'] = 'user/' . $form_state['values']['uid'] . '/edit/drupagram';
}

/**
 * Form to add a Instagram account
 *
 * If OAuth is not enabled, a text field lets users to add their
 * Instagram screen name. If it is, a submit button redirects to
 * Instagram.com asking for authorisation.
 */
function drupagram_account_form($form, $form_state, $account = NULL) {
  global $user;
  if (empty($account)) {
    $account = $user;
  }

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  if (_drupagram_use_oauth()) {
    $form['#validate'] = array('drupagram_account_oauth_validate');
  }
  elseif ($user->uid == 1 || user_access('add drupagram accounts')) {
    $form['error_message'] = array(
      '#type' => 'item',
      '#title' => t('Link your Instagram to this account'),
      '#markup' => t('oAuth Settings have not been set up. Please !fix this issue and try again.', array('!fix' => l(t('fix'), 'admin/config/services/drupagram'))),
    );
  }

  $form['import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import statuses from this account'),
    '#default_value' => TRUE,
    '#access' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add account'),
    '#access' => _drupagram_use_oauth(),
  );

  return $form;
}

/**
 * Implements hook_FORM_ID_submit().
 *
 * Loads Instagram account details and adds them to the user account
 */
function drupagram_account_form_submit($form, &$form_state) {
  module_load_include('lib.php', 'drupagram');
  module_load_include('inc', 'drupagram');

  $name = $form_state['values']['username'];
  $drupagram = new Instagram($name);
  $account = $drupagram->user_info($name);
  drupagram_account_save($account, TRUE, user_load($form_state['values']['uid']));
}

/**
 * If OAuth is enabled, intercept submission of 'Add Account' form on
 * user/%/edit/drupagram page and redirect to Instagram for auth.
 */
function drupagram_account_oauth_validate($form, &$form_state) {
  module_load_include('lib.php', 'oauth_common');
  module_load_include('lib.php', 'drupagram');
  $callback_uri = url('instagram/oauth', array('absolute' => TRUE));

  $client_id = variable_get('drupagram_client_id', '');
  $client_secret = variable_get('drupagram_client_secret', '');
  if ($client_id == '' || $client_secret == '') {
    form_set_error('', t('Please configure your Instagram client id and secret.'));
  }

  $drupagram = new InstagramOAuth($client_id, $client_secret);
  // $token = $drupagram->get_request_token();

  $_SESSION['drupagram_oauth']['account'] = user_load($form['uid']['#value']);
  // $_SESSION['drupagram_oauth']['token'] = $token;
  $_SESSION['drupagram_oauth']['destination'] = $_GET['q'];
  drupal_goto($drupagram->get_authorize_url($callback_uri));
}

/**
 * @TODO This code should probably be reviewed.
 *
 * Wrapper to call drupal_form_submit() which wasn't required in D6.
 */
function drupagram_oauth_callback() {
  $form_state['values']['code'] = $_GET['code'];
  drupal_form_submit('drupagram_oauth_callback_form', $form_state);
}

/**
 * Form builder function. In D6 this form was built in response to the
 * oauth return request from Instagram, and the setting of
 * $form['#post'] seems to have caused the form to be validated and
 * processed.
 */
function drupagram_oauth_callback_form($form, &$form_state) {
  if (isset($_GET['code'])) {
    $form['#post']['code'] = $_GET['code'];
  }
  $form['code'] = array(
    '#type' => 'hidden',
    '#default_value' => $_GET['code'],
  );
  return $form;
}

/**
 * Validate results from Instagram OAuth return request.
 */
function drupagram_oauth_callback_form_validate($form, &$form_state) {
  $client_id = variable_get('drupagram_client_id', '');
  $client_secret = variable_get('drupagram_client_secret', '');
  $callback_uri = url('instagram/oauth', array('absolute' => TRUE));

  if ($client_id == '' || $client_secret == '') {
    form_set_error('', t('Please configure your Instagram consumer key and secret.'));
  }

  if (isset($_SESSION['drupagram_oauth'])) {
    $form_state['drupagram_oauth'] = $_SESSION['drupagram_oauth'];
    unset($_SESSION['drupagram_oauth']);
  }
  else {
    form_set_error('oauth_token', 'Invalid Instagram OAuth request: Session mismatch.');
  }

  module_load_include('lib.php', 'oauth_common');
  module_load_include('lib.php', 'drupagram');
  module_load_include('inc', 'drupagram');

  if ($drupagram = new InstagramOAuth($client_id, $client_secret, $callback_uri)) {
    if ($response = $drupagram->get_access_token($form_state['values']['code'], $callback_uri)) {
      $form_state['drupagram_oauth']['response'] = $response;
    }
    else {
      form_set_error('oauth_token', t('Invalid Instagram OAuth request: Token invalid'));
    }
  }
  else {
    form_set_error('oauth_token', t('Invalid Instagram OAuth request: Invalid oAuth Settings'));
  }
}

/**
 * Handle a Instagram OAuth return request and store the account creds
 * in the DB. Redirects to user/%/edit/drupagram
 *
 * @TODO Redirect better.
 *
 * I don't much like the use of drupal_goto() here as it might
 * interfere with other modules trying to piggyback on the form
 * submission, but setting $form['redirect'] was leaving us at the
 * drupagram/oauth URL.
 */
function drupagram_oauth_callback_form_submit(&$form, &$form_state) {
  $client_id = variable_get('drupagram_client_id', '');
  $client_secret = variable_get('drupagram_client_secret', '');
  $response = $form_state['drupagram_oauth']['response'];

  $drupagram = new InstagramOAuth($client_id, $client_secret, $response['access_token']);
  $drupagram_account = new InstagramUser($response['user']);
  // $drupagram_account->set_auth($response);
  $account = $form_state['drupagram_oauth']['account'];
  drupagram_account_save($drupagram_account, TRUE, $account);

  $form['#programmed'] = FALSE;

  $form_state['redirect'] = url('user/' . $account->uid . '/edit/drupagram');
  // redirect isn't firing - because we're using drupal_submit_form()?
  // - so adding drupal_goto() here (but not liking it).
  drupal_goto('user/' . $account->uid . '/edit/drupagram');
}
