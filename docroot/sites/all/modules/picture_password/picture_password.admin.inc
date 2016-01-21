<?php
/**
 * @file
 * picture_password.admin.inc
 *
 * Configuration options for Kaje™ Picture Passwords module.
 */

/**
 * Menu callback for admin configuration settings.
 */
function picture_password_admin_configure($form, &$form_state) {

  // Display the current Kaje system status as an informational msg.
  $response = picture_password_kaje_call('status');
  picture_password_message($response->msg);

  $form = array();
  $form['requesting_party'] = array(
    '#type' => 'fieldset',
    '#title' => t('Requesting Party ID and Secret'),
    '#collapsible' => FALSE,
  );
  $form['requesting_party']['picture_password_rp_id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID:'),
    '#description' => t('The Requestion Party ID as obtained from Kaje'),
    '#default_value' => variable_get('picture_password_rp_id'),
  );
  $form['requesting_party']['picture_password_rp_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret:'),
    '#description' => t('The Requestion Party Secret as obtained from Kaje'),
    '#default_value' => variable_get('picture_password_rp_secret'),
  );
  $form['buttons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login buttons'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['buttons']['picture_password_login_button_std_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label on default login button:'),
    '#description' => t('Leave blank to default to core. Use <em>&lt;no button&gt;</em> to suppress the default button altogether, so that the user can log in only with Kaje Picture Password.'),
    '#default_value' => variable_get('picture_password_login_button_std_label'),
  );
  $form['buttons']['picture_password_login_button_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label on Kaje™ Picture Passwords login button:'),
    '#description' => t('When left blank, defaults to %label', array('%label' => PICTURE_PASSWORD_LOGIN_BUTTON_LABEL)),
    '#default_value' => variable_get('picture_password_login_button_label'),
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['picture_password_debug_users'] = array(
    '#type' => 'textfield',
    '#title' => t('Names of users to debug'),
    '#description' => t('Displays debug messages for selected users. Separate names by commas. Use <em>anon</em> for the anonymous user.'),
    '#default_value' => variable_get('picture_password_debug_users'),
  );
  return system_settings_form($form);
}

/**
 * Admin menu callback for the Kaje user administration menu tab.
 *
 * @param array $form
 *   The form (stub).
 * @param array $form_state
 *   The form sate.
 * @param object $account
 *   The user account.
 *
 * @return array
 *   The form with additions
 */
function picture_password_admin_user($form, &$form_state, $account = NULL) {

  global $user;
  if (!isset($account)) {
    $account = $user;
  }
  if (!picture_password_get_kaje_uid($account->uid)) {
    $form['no_kaje_uid'] = array(
      '#type' => 'markup',
      '#markup' => t('This user is not known to have a Kaje account. <br/>Kaje administration functions for this user are therefore not available.'),
    );
    return $form;
  }
  $options = array(
    PICTURE_PASSWORD_RESET_KAJE_ACCOUNT => t('Reset'),
    PICTURE_PASSWORD_SUSPEND_KAJE_ACCOUNT => t('Suspend'),
    PICTURE_PASSWORD_UNSUSPEND_KAJE_ACCOUNT => t('Unsuspend'),
    PICTURE_PASSWORD_LOCK_KAJE_ACCOUNT => t('Lock'),
    PICTURE_PASSWORD_UNLOCK_KAJE_ACCOUNT => t('Unlock'),
    PICTURE_PASSWORD_DELETE_KAJE_ACCOUNT => t('Delete (no undo)'),
  );
  $form['picture_password_admin_function'] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#title' => t("Administer @name's Kaje account:", array(
      '@name' => $account->name,
    )),
    '#description' => t('None of these operations affect the @status status of this user on this website.', array(
      '@status' => user_is_blocked($account->name) ? t('blocked') : t('active'),
    )),
    '#options' => $options,
    '#default_value' => 0,
  );
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Submit to Kaje'),
    ),
  );
  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );
  return $form;
}

/**
 * User menu callback for the Kaje user account administration menu tab.
 *
 * @param array $form
 *   The form (stub).
 * @param array $form_state
 *   The form sate.
 *
 * @return array
 *   The form with additions
 */
function picture_password_user($form, &$form_state) {
  global $user;

  if (picture_password_get_kaje_uid($user->uid)) {
    $options = array(
      PICTURE_PASSWORD_RESET_KAJE_ACCOUNT => t('Reset (change picture)'),
      PICTURE_PASSWORD_DELETE_KAJE_ACCOUNT => t('Delete (no undo)'),
    );
  }
  else {
    $form['no_kaje_uid'] = array(
      '#type' => 'markup',
      '#markup' => t('You are not known to have a Kaje account. You can create one below.'),
    );
    $options = array(
      PICTURE_PASSWORD_CREATE_KAJE_ACCOUNT => t('Create Kaje account'),
    );
  }
  $form['picture_password_admin_function'] = array(
    '#type' => 'radios',
    '#required' => TRUE,
    '#title' => t("Administer your Kaje account:"),
    '#description' => t('None of these operations affect your @status status on this website.', array(
      '@status' => user_is_blocked($user->name) ? t('blocked') : t('active'),
    )),
    '#options' => $options,
    '#default_value' => 0,
  );
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Submit to Kaje'),
      '#submit' => array('picture_password_admin_user_submit'),
    ),
  );
  $form['help'] = array(
    '#type' => 'markup',
    '#markup' => t('<a target="_kaje" href="http://picturepassword.info">More help</a>.'),
  );
  $form['account'] = array(
    '#type' => 'value',
    '#value' => $user,
  );
  return $form;
}

/**
 * Submit handler for picture_password_admin_user().
 */
function picture_password_admin_user_submit($form, &$form_state) {

  $account = $form_state['values']['account'];
  $operation = $form_state['values']['picture_password_admin_function'];

  $is_allowed_self_operation = picture_password_admin_own_access($account) &&
    in_array($operation, array(
      PICTURE_PASSWORD_CREATE_KAJE_ACCOUNT,
      PICTURE_PASSWORD_RESET_KAJE_ACCOUNT,
      PICTURE_PASSWORD_DELETE_KAJE_ACCOUNT,
    ));

  if (!$is_allowed_self_operation && !user_access('administer picture passwords')) {
    picture_password_message(t('You do not have permission to execute this operation.'), 'error');
    return;
  }
  $kaje_uid = picture_password_get_kaje_uid($account->uid);

  switch ($operation) {

    case PICTURE_PASSWORD_CREATE_KAJE_ACCOUNT:
      // Should only be invoked by a logged-in user. This is tested.
      $response = picture_password_handle_kaje_init($account);
      if (empty($response)) {
        return FALSE;
      }
      if (!empty($response->url)) {
        // Redirect the user to their Kaje login.
        unset($_GET['destination']);
        drupal_goto($response->url);
      }
      break;

    case PICTURE_PASSWORD_RESET_KAJE_ACCOUNT:
      // If it was the user themeselves that initiated the Reset, then redirect
      // them to Kaje immediately, provided they have authenticated with Drupal.
      if (picture_password_admin_own_access($account)) {
        $response = picture_password_kaje_call('reset', $kaje_uid);
        if (!empty($response->url)) {
          // Redirect the user to their Kaje login.
          unset($_GET['destination']);
          drupal_goto($response->url);
        }
      }
      elseif (user_access('administer picture passwords')) {
        picture_password_set_flags($account->uid, PICTURE_PASSWORD_RESET_KAJE_ACCOUNT);
        picture_password_message(t('The Kaje account has been flagged for a reset the next time user %name logs in.', array(
          '%name' => $account->name,
        )));
        return;
      }
      break;

    case PICTURE_PASSWORD_LOCK_KAJE_ACCOUNT:
      $response = picture_password_kaje_call('lock', $kaje_uid);
      break;

    case PICTURE_PASSWORD_UNLOCK_KAJE_ACCOUNT:
      $response = picture_password_kaje_call('unlock', $kaje_uid);
      break;

    case PICTURE_PASSWORD_SUSPEND_KAJE_ACCOUNT:
      $response = picture_password_kaje_call('suspend', $kaje_uid);
      break;

    case PICTURE_PASSWORD_UNSUSPEND_KAJE_ACCOUNT:
      $response = picture_password_kaje_call('unsuspend', $kaje_uid);
      break;

    case PICTURE_PASSWORD_DELETE_KAJE_ACCOUNT:
      $response = picture_password_kaje_call('deleteUser', $kaje_uid);
      if (isset($response->user_status) && $response->user_status == 'nonexistent') {
        picture_password_delete_uid($account->uid);
      }
      break;

    default:
      picture_password_message(t('Invalid Kaje™ Picture Passwords administration function.'), 'error');
      return;
  }
  picture_password_message(empty($response->user_status) ? $response->msg : picture_password_user_status($response->user_status));
}
