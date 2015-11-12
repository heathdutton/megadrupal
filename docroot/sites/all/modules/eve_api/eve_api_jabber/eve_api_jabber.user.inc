<?php
/**
 * @file
 * Functions related to allowing users to alter the user settings and options.
 */

/**
 * Form constructor for the user jabber registration form.
 *
 * @param string $account
 *   The account information for the user page being viewed.
 *
 * @see eve_api_jabber_user_form_validate()
 * @see eve_api_jabber_user_form_submit()
 *
 * @ingroup forms
 */
function eve_api_jabber_user_form($form, &$form_state, $account) {
  if (!isset($form_state['user'])) {
    $form_state['user'] = $account;
  }
  else {
    $account = $form_state['user'];
  }

  $uid = $account->uid;

  $form['jabber']['info'] = array(
    '#type' => 'item',
    '#title' => t('Register for a Jabber Account.'),
    '#description' => t('If you do not have a Jabber Client already installed, you can download and install @client from <a href="@client_url" target="_blank">here</a>.', array(
      '@client' => 'Trillian',
      '@client_url' => 'https://www.trillian.im/download/',
    )),
    '#weight' => 0,
  );

  $form['jabber']['username'] = array(
    '#type' => 'item',
    '#title' => t('Username:'),
    '#description' => ($account->characterID != 0) ? eve_api_jabber_format_name($account->name) . '@' . variable_get('eve_api_jabber_domain', 'yourdomain.com') : t('<a href="@url">Please select a main character.</a>', array('@url' => url('user/' . $uid . '/edit'))),
    '#weight' => 10,
  );

  if ($account->characterID != 0) {
    $form['jabber']['password'] = array(
      '#type' => 'textfield',
      '#title' => t('Set Password'),
      '#description' => t('You can set or change your Jabber password.'),
      '#required' => TRUE,
      '#size' => 60,
      '#maxlength' => 50,
      '#weight' => 20,
      '#default_value' => '',
    );
  }

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Register'),
  );

  if ($account->characterID == 0) {
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_jabber_user_form_validate');
  $form['#submit'] = array('eve_api_jabber_user_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_jabber_user_form().
 *
 * @see eve_api_jabber_user_form_submit()
 */
function eve_api_jabber_user_form_validate($form, &$form_state) {
  $account = $form_state['user'];
  $uid = $account->uid;

  if ($account->characterID == 0) {
    drupal_goto('user/' . $uid . '/edit');
  }
}

/**
 * Form submission handler for eve_api_jabber_user_form().
 *
 * @see eve_api_jabber_user_form_validate()
 */
function eve_api_jabber_user_form_submit($form, &$form_state) {
  $account = $form_state['user'];

  $roles = user_roles();

  try {
    $query = array(
      'name' => eve_api_jabber_format_display_name($account->name),
      'email' => $account->mail,
    );

    if (!empty($form_state['values']['password'])) {
      $password = array(
        'password' => (string) $form_state['values']['password'],
      );

      $query = array_merge($query, $password);
    }

    $result = eve_api_jabber_url_query('update', $account->name, $query);

    if ($result === TRUE) {
      drupal_set_message(t('Your Jabber account was updated.'));
    }
    else {
      $result = eve_api_jabber_url_query('add', $account->name, $query);

      if ($result === TRUE) {
        drupal_set_message(t('Your Jabber account was created.'));
      }
      else {
        drupal_set_message(t('There was an error updateing your Jabber account, please try again later.'), 'error');
      }
    }
  }
  catch (EveApiException $e) {
    drupal_set_message(t('There was an error updateing your Jabber account, please try again later.'), 'error');
  }
}
