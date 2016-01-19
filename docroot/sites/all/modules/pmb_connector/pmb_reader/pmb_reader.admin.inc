<?php

/**
 * @file
 * PMB reader administration form.
 */

/**
 * Reader admin form.
 *
 * @ingroup forms
 * @see pmb_admin_form()
 */
function pmb_reader_admin_form() {
  $form = array();

  $form['gateway']['pmb_reader_auth_mode'] = array(
    '#type' => 'select',
    '#title' => t("Readers' authentication"),
    '#options' => array(
      'plain' => t('Send login and password as plain text'),
      'md5' => t('Hash password with MD5'),
      'full_hash' => t('Hash login and password in one item with SHA256'),
      'drupal' => t('Validate against Drupal base'),
    ),
    '#default_value' => pmb_variable_get('pmb_reader_auth_mode'),
    '#description' => t('This choice depends on your installation.') . ' ' . t('This option increases security by crypting the password before to send it on the gateway.') . '<br />'
      . '<ul><li>' . t('To send plain text login and password is unsecure, but needed with LDAP authentication on PMB side.') . ' ' . t('Note: Remote LDAP authentication is bugged in Drupal (PMB 3.4.13, see mailing list), so a patch is needed to use it.') . '</li>'
      . '<li>' . t('Default PMB installation (3.4.13) allows only "plain text" and "MD5 hash". Furthermore, if you want to allow readers to change password, you should choose "plain text", else you need to patch PMB.') . ' ' . t('A patch is needed on PMB also to use secure authentication modes "Full SHA256" and "Drupal".') . '</li>'
      . '<li>' . t('Authentication with Drupal is the most flexible (lots of authentication modules can be used) and more secure by default (PMB stores all passwords of readers as plain text).') . '</li></ul>'
      . '<strong>' . t('Warning') . ':</strong> ' . t("Before changing this parameter, put your site in <a href='!link'>maintenance mode</a>, else all logged readers won't be able to continue their session.", array(
        '!link' => url('admin/config/development/maintenance'),
      )),
  );
  $form['gateway']['pmb_reader_session_duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration of the local session of a reader, in seconds'),
    '#default_value' => pmb_variable_get('pmb_reader_session_duration'),
    '#size' => 10,
    '#maxlength' => 10,
    '#description' => t('This setting allows to check user rights access more or less frequently.') . ' ' . t('If a remote session expires, a new one will open automatically.'),
    '#required' => TRUE,
  );

  $form['user_interface']['pmb_menu_attach_reader'] = array(
    '#type'          => 'select',
    '#title'         => t('Attach "My reader account" menu to') . ':',
    '#options'       => array(),
    '#default_value' => pmb_variable_get('pmb_menu_attach_reader'),
  );
  // menu_get_names() provides all menus and we want only user's ones.
  // Use of query used in menu_overview_page().
  $result = db_query("SELECT * FROM {menu_custom} ORDER BY title", array(), array('fetch' => PDO::FETCH_ASSOC));
  foreach ($result as $menu) {
    $form['user_interface']['pmb_menu_attach_reader']['#options'][$menu['menu_name']] = $menu['menu_name'];
  }
  $form['user_interface']['pmb_menu_attach_reader']['#options'][pmb_variable_get_default('pmb_menu_attach_reader')] .= ' (' . t('default menu') . ')';

  $form['user_interface']['pmb_tabs_reader_default'] = array(
    '#type' => 'select',
    '#title' => t('Default Reader tab'),
    '#options' => array(
      'summary' => t('Summary'),
      'cart' => t('Cart'),
      'reservation' => t('Reservations'),
      'loan' => t('Loans'),
      'reading_list' => t('Reading lists'),
      'suggestion' => t('Suggestions'),
    ),
    '#default_value' => pmb_variable_get('pmb_tabs_reader_default'),
  );

  return $form;
}

/**
 * Reader password change form.
 *
 * @ingroup forms
 *
 * @see pmb_reader_pass_form_validate()
 * @see pmb_reader_pass_form_submit()
 */
function pmb_reader_pass_form($form, &$form_state, $account = NULL) {
  global $user;

  if (!$account
      || (isset($account->uid) && !$account->uid)
      || !$user->uid
      || ($user->uid != $account->uid)
    ) {
    drupal_access_denied();
  }

  if (pmb_variable_get('pmb_reader_auth_mode') != 'plain') {
    drupal_set_message(t('Current settings does not allow readers to change password.'), 'warning');
    drupal_goto('user');
  }

  $form = array();

  $form['account'] = array(
    '#type'   => 'container',
  );
  $form['account']['old_pass'] = array(
    '#type' => 'fieldset',
    '#title' => t('Current password'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['account']['old_pass']['current_pass'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#size' => 25,
    '#description' => t('Enter your current password here'),
    '#required' => TRUE,
    '#attributes' => array('autocomplete' => 'off'),
  );
  $form['account']['new_pass'] = array(
    '#type' => 'fieldset',
    '#title' => t('New password'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['account']['new_pass']['pass'] = array(
    '#type' => 'password_confirm',
    '#size' => 25,
    '#description' => t('To change your password, enter the new one in both fields.'),
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * @see pmb_reader_pass_form()
 */
function pmb_reader_pass_form_validate($form, &$form_state) {
  // Other checks are made by the Drupal pass form.
  if ($form_state['values']['current_pass'] == $form_state['values']['pass']) {
    form_set_error('pass', t('Same strings are set for old and new passwords.'));
  }
  $trim_pass = trim($form_state['values']['pass']);
  if (($trim_pass != $form_state['values']['pass'])
      || empty($trim_pass)
    ) {
    form_set_error('pass', t("A password can't begin or end with a space."));
  }
}

/**
 * @see pmb_reader_pass_form()
 */
function pmb_reader_pass_form_submit($form, &$form_state) {
  global $user;

  $old_password = trim($form_state['values']['current_pass']);
  $new_password = trim($form_state['values']['pass']);

  require_once(drupal_get_path('module', 'pmb') . '/pmb.class_data.inc');
  $pmb_data = new pmb_data();
  $pmb_data->set_reader($user);
  // Currently only use plain text gateway.
  $result = $pmb_data->reader_change_password($old_password, $new_password);

  // Check success.
  if ($result) {
    drupal_set_message(t('Successfully changed password.'));

    // Remote session continue, but update credentials of user for next session.
    $to_save = array('pmb_reader' => $user->data['pmb_reader']);
    $to_save['pmb_reader']['credentials'] = pmb_reader_credentials($user->data['pmb_reader']['credentials']['login'], $new_password);
    user_save($user, $to_save);
  }
  // Error on PMB side.
  else {
    drupal_set_message(t('Password unchanged. You give a wrong current password or an error occured on remote server. <a href="!link">Retry</a> later.', array(
      '!link' => url(_pmb_p('reader/') . $user->uid . _pmb_p('/password')),
    )), 'error');
  }

  $form_state['redirect'] = 'user';
}
