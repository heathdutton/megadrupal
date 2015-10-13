<?php
/**
 * @file
 * Include file for the services_token_access module.
 */

/**
 * Access callback for Services authentication.
 *
 * @see hook_services_authentication_info()
 */
function _services_token_access_authenticate_call($settings, $method, $args) {
  if (!empty($_GET['services_token'])) {
    // Load the user ID based on token.
    if ($uid = services_token_access_load_by_token($_GET['services_token'], TRUE)) {
      // Load the user and switch the session to use this user.
      if ($desired_user = user_load($uid)) {
        // Check the permission to use tokens.
        if (user_access('services use token authentication', $desired_user)) {
          global $user;
          // Disabling the session writing before switching user. This prevents
          // that the user from the token will be logged in to an existing
          // session.
          drupal_save_session(FALSE);
          $user = $desired_user;
        }
        else {
          watchdog('services_token_access', 'The user !uid does not have the permission to use Services Token Access.', array('!uid' => $uid), WATCHDOG_ERROR);
        }
      }
      else {
        watchdog('services_token_access', 'The configured user ID !uid could not be found', array('!uid' => $uid), WATCHDOG_ERROR);
      }
    }
    else {
      watchdog('services_token_access', 'Invalid access token specified by consumer: !token', array('!token' => $_GET['services_token']), WATCHDOG_ERROR);
    }
  }
}

/**
 * Settings callback for Services authentication.
 *
 * @see hook_services_authentication_info()
 */
function _services_token_access_security_settings($settings, &$form_state) {
  $form = array();
  global $user;
  // Instructions.
  $form['instructions'] = array(
    '#type' => 'item',
    '#title' => t('Usage instructions'),
    '#markup' => t('Go to !permissions_link and enable the Services Token Access permissions for the appropriate roles. Then go to !user_edit_link and generate a token for the given user. When accessing the webservice, add %token_parameter as an URL parameter, like %token_url_example', array(
      '!permissions_link' => l(t('Permissions'), 'admin/people/permissions'),
      '!user_edit_link' => l(t('the user page'), 'user/' . $user->uid . '/services_token'),
      '%token_parameter' => 'services_token=<YOUR TOKEN>',
      '%token_url_example' => 'http://example.com/service/resource?services_token=678874321232...',
    )),
  );

  // Add some statistics to the form.
  $count = db_query('SELECT count(uid) AS tokens FROM {services_token_access_tokens}')->fetchField();
  $oldest = db_select('services_token_access_tokens')
    ->fields('services_token_access_tokens', array('updated'))
    ->orderBy('updated', 'ASC')
    ->range(0, 1)
    ->execute()
    ->fetchField();
  $newest = db_select('services_token_access_tokens')
    ->fields('services_token_access_tokens', array('updated'))
    ->orderBy('updated', 'DESC')
    ->range(0, 1)
    ->execute()
    ->fetchField();
  $form['statistics'] = array(
    '#type' => 'item',
    '#title' => t('Statistics'),
    '#markup' => t('The database contains !count tokens.', array('!count' => $count)),
  );
  // Add ages if tokens are found.
  if ($count) {
    $form['statistics']['#markup'] .= ' ' . t('The oldest token is from !oldest ago and the newest is from !newest ago.', array('!oldest' => format_interval(REQUEST_TIME - $oldest), '!newest' => format_interval(REQUEST_TIME - $newest)));
  }

  // Warning with checkbox to inform the user before doing risky actions.
  $form['remove_all_warning'] = array(
    '#type' => 'item',
    '#title' => t('Warning'),
    '#markup' => t('Removing all tokens for all users may have unpredictable side effects depending on the number of external systems using these tokens. This should only be used as a panic action of a list of access tokens have beem compromised. Always make sure that you know what you are doing. Check the box above to confirm that you understand the risk.'),
  );

  $form['remove_all_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check this box if you want to remove all tokens.'),
  );

  // The "remove all" button are only shown if the checkbox is checked.
  $form['remove_all'] = array(
    '#type' => 'submit',
    '#value' => t('Remove tokens for all users.'),
    '#submit' => array('_services_token_access_remove_all_submit'),
    '#states' => array(
      'visible' => array(
        '#edit-services-token-access-remove-all-check' => array('checked' => TRUE),
      ),
    ),
  );
  return $form;
}

/**
 * Submit callback that removes all tokens from the database.
 */
function _services_token_access_remove_all_submit($form, &$form_state) {
  if ($form_state['values']['services_token_access']['remove_all_check']) {
    services_token_access_remove_all();
    drupal_set_message(t('All access token have been removed.'));
  }
}

/**
 * Form callback for the user token management form.
 */
function services_token_access_user_form($form, &$form_state, $account) {
  // Get the current token if it exists.
  if ($token_data = services_token_access_load_by_user($account->uid)) {
    $token = $token_data['token'];
    $updated = $token_data['updated'];
  }
  else {
    $token = NULL;
  }

  // Generate the form.
  $form = array();
  $form['current_token'] = array(
    '#type' => 'item',
    '#title' => t('Current token'),
    '#markup' => $token ? '<code>' . $token . '</code>' : t('None'),
  );

  // Add the token specific stuff.
  if ($token) {
    $form['token_age'] = array(
      '#type' => 'item',
      '#title' => t('Token age'),
      '#markup' => format_interval(REQUEST_TIME - $updated),
    );

    // Warning with checkbox to inform the user before doing risky actions.
    $form['warning'] = array(
      '#type' => 'item',
      '#title' => t('Warning'),
      '#markup' => t('Generating or removing tokens may results in issues elsewhere. Make sure that all external systems that uses this token are updated as well, as soon as a new token is generated.'),
    );
    $form['action_check'] = array(
      '#type' => 'checkbox',
      '#title' => t('Check this box if you want to remove or regenerate the token.'),
    );

    // The remove/generate buttons are only shown if the checkbox is checked.
    $form['remove_button'] = array(
      '#type' => 'submit',
      '#value' => t('Remove token'),
      '#submit' => array('_services_token_access_remove_submit'),
      '#states' => array(
        'visible' => array(
          '#edit-action-check' => array('checked' => TRUE),
        ),
      ),
    );
  }

  // The generate button is always shown if no token exists.
  $form['generate_button'] = array(
    '#type' => 'submit',
    '#value' => t('Generate new token'),
    '#submit' => array('_services_token_access_renew_submit'),
    '#states' => array(
      'visible' => array(
        '#edit-action-check' => array('checked' => TRUE),
      ),
    ),
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );
  return $form;
}

/**
 * Submit callback for removing the token.
 */
function _services_token_access_remove_submit($form, &$form_state) {
  services_token_access_update_token($form_state['values']['uid'], TRUE);
  drupal_set_message(t('The token has been removed.'));
}

/**
 * Submit callback for renewing the token.
 */
function _services_token_access_renew_submit($form, &$form_state) {
  services_token_access_update_token($form_state['values']['uid']);
  drupal_set_message(t('The token has been updated.'));
}
