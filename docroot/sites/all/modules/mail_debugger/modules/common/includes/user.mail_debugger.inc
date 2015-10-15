<?php

/**
 * Callback for the elements on the form to send system mails to users.
 * 
 * @param type $default_values
 * @return boolean
 */
function mail_debugger_user_form($default_values = array()) {
  if (!user_access('use mail debugger user')) {
    return FALSE;
  }

  // add user section of the form
  return array(
    'mail' => array(
      '#type' => 'textfield',
      '#title' => t('E-mail address'),
      '#default_value' => $default_values['mail'],
      '#autocomplete_path' => 'admin/config/development/mail_debugger/user_autocomplete_mail',
    ),
    'type' => array(
      '#type' => 'select',
      '#title' => t('Message'),
      '#options' => array(
        'register_admin_created' => t('Welcome message for user created by the admin.'),
        'register_no_approval_required' => t('Welcome message when user self-registers.'),
        'register_pending_approval' => t('Welcome message, user pending admin approval.'),
        'status_activated' => t('Account activated.'),
        'status_blocked' => t('Account blocked.'),
        'password_reset' => t('Password recovery request.'),
        'cancel_confirm' => t('Account cancellation request.'),
        'status_canceled' => t('Account canceled.'),
      ),
      '#default_value' => $default_values['type'],
    ),
  );
}

/**
 * Validate if the email address is known, and do a suggestion.
 * @param type $values
 * @param type $elements
 * @param array $form_state
 */
function mail_debugger_user_form_validate($values, $elements, &$form_state) {
  // is it a valid e-mail address?
  if (!valid_email_address($values['mail'])) {
    form_set_error($elements['mail'], t('Enter a valid e-mail address.'));
    return;
  }

  // load user by mail?
  $user = user_load_by_mail($values['mail']);

  // no user or anonimous?
  if (empty($user->uid)) {
    form_set_error($elements['mail'], t('This e-mail address does not match any user.'));
    return;
  }
}

/**
 * Send the system mail to a user
 * @param type $values
 * @param type $form_state
 */
function mail_debugger_user_form_submit($values, &$form_state) {
  // load the user account
  $account = user_load_by_mail($values['mail']);

  // send the mail
  $result = _user_mail_notify($values['type'], $account);

  // Notify about the result
  if ($result) {
    drupal_set_message(t('Message sent'));
  } else {
    drupal_set_message(t('Message sent with errors. Check the error log.'), 'warning');
  }
}

/**
 * Autocomplete the mail for a user.
 * 
 * @param type $query
 * @return type
 */
function mail_debugger_user_callback_autocomplete_mail($query = '') {
  if (empty($query)) {
    return;
  }

  $result = db_select('users', 'u')
    ->fields('u', array('mail', 'name', 'uid'))
    ->condition('u.mail', db_like($query) . '%', 'LIKE')
    ->condition('status', 1)
    ->range(0, 10)
    ->execute()
    ->fetchAll();

  $complete = array();
  foreach ($result as $obj) {
    $account = user_load($obj->uid);
    if (count($account->roles) > 1) {
      $roles = $account->roles;
      unset($roles[2]);

      $roles_str = implode(', ', $roles);

      $complete[$obj->mail] = $obj->mail . ' (' . $obj->name . ' - ' . $roles_str . ')';
    } else {
      $complete[$obj->mail] = $obj->mail . ' (' . $obj->name . ')';
    }
  }
  drupal_json_output($complete);
}
