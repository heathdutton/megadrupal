<?php

/**
 * @file
 * Page callbacks.
 */

/**
 * Prints the list of user accounts khipu.
 *
 * @param object $account
 *   The user account object.
 */
function khipu_user_page_setting($account) {
  $rows = array();
  $environments = khipu_get_environments();
  if (isset($account->khipu_accounts)) {
    foreach ($account->khipu_accounts as $khipu_account) {
      $row = array(
        $khipu_account->receiver_name,
        $khipu_account->receiver_id,
        substr($khipu_account->receiver_key, 0, -36) . '...............',
        $environments[$khipu_account->environment],
        l(t('Test'), 'user/' . $account->uid . '/edit/khipu_account/' . $khipu_account->kaid . '/test'),
        l(t('Edit'), 'user/' . $account->uid . '/edit/khipu_account/' . $khipu_account->kaid . '/edit'),
        l(t('Delete'), 'user/' . $account->uid . '/edit/khipu_account/' . $khipu_account->kaid . '/delete'),
      );

      $rows[] = $row;
    }
  }
  $header = array(
    t('Receiver Name'),
    t('Receiver Id'),
    t('Receiver Key'),
    t('Environment'),
    array(
      'data' => t('Operation'),
      'colspan' => 3,
    ),
  );
  $variables = array(
    'rows' => $rows,
    'header' => $header,
    'empty' => t('No Khipu Accounts'),
  );

  $output = '<p>' . t('Here you can manage your khipu accounts, which can be gained in this <a href="!khipu_url" target="_blank">site</a>', array('!khipu_url' => 'http://khipu.com')) . '</p>';

  $output .= theme('table', $variables);
  return $output;
}


/**
 * Form to create or edit a khipu account.
 *
 * @param array $form
 *   Form element.
 * @param array $form_state
 *   Form state.
 * @param object $account
 *   The user account object.
 * @param object $khipu_account
 *   The khipu account object.
 */
function khipu_user_account_form($form, &$form_state, $account = NULL, $khipu_account = NULL) {
  if (is_null($khipu_account)) {
    $form['kaid'] = array(
      '#type' => 'value',
      '#value' => 0,
    );
  }
  else {
    $form['kaid'] = array(
      '#type' => 'value',
      '#value' => $khipu_account->kaid,
    );
  }

  if (is_null($account)) {
    global $user;
    $uid = $user->uid;
  }
  else {
    $uid = $account->uid;
  }
  $form['receiver_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver Name'),
    '#required' => TRUE,
    '#default_value' => isset($khipu_account->receiver_name) ? $khipu_account->receiver_name : '',
    '#description' => t('This name helps to identify the account on the site. For example: My personal account, family account, etc...'),
  );
  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $uid,
  );
  $form['receiver_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver Id'),
    '#required' => TRUE,
    '#default_value' => isset($khipu_account->receiver_id) ? $khipu_account->receiver_id : '',
    '#description' => t('This ID is requested in Khipu.com'),
  );
  $form['receiver_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver Key'),
    '#required' => TRUE,
    '#default_value' => isset($khipu_account->receiver_key) ? $khipu_account->receiver_key : '',
    '#description' => t('The key is a unique string from the khipu account.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Form submission handler for khipu_user_account_form().
 */
function khipu_user_account_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $khipu_account = new stdClass();
  $khipu_account->receiver_name = $values['receiver_name'];
  $khipu_account->uid = $values['uid'];
  $khipu_account->receiver_id = $values['receiver_id'];
  $khipu_account->receiver_key = $values['receiver_key'];

  if (!$values['kaid']) {
    $khipu_account->created = time();
  }
  else {
    $khipu_account->kaid = $values['kaid'];
  }

  if (khipu_save_account($khipu_account)) {
    drupal_set_message(t('Your Account Khipu was been save'));
  }

  $form_state['redirect'] = 'user/' . $values['uid'] . '/edit/khipu_account';
}


/**
 * Form for test one khipu account.
 *
 * @param array $form
 *   Form element.
 * @param array $form_state
 *   Form state.
 * @param object $account
 *   The user account object.
 * @param object $khipu_account
 *   The khipu account object.
 */
function khipu_user_test_account($form, &$form_state, $account, $khipu_account) {
  module_load_include('inc', 'khipu', 'khipu.admin');
  $form = khipu_admin_test_account($form, $form_state, $khipu_account);

  $url = url('user/' . $account->uid . '/edit/khipu-setting', array('absolute' => TRUE));
  $form['return_url']['#value'] = $form['cancel_url']['#value'] = $url;

  $form['amount'] = khipu_element_amount_form();

  return $form;
}


/**
 * Form to confirm if delete a khipu account.
 */
function khipu_user_account_delete_form($form, &$form_state, $account, $khipu_account) {
  $form['kaid'] = array(
    '#type' => 'hidden',
    '#value' => $khipu_account->kaid,
  );
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );
  $form['khipu_account'] = array(
    '#type' => 'value',
    '#value' => $khipu_account,
  );
  $args = array(
    '@name' => $khipu_account->receiver_name,
  );
  $question = t('Are you sure you want to delete the khipu account of @name?', $args);
  $path = 'user/' . $account->uid . '/edit/khipu_account';

  return confirm_form($form, $question, $path);
}


/**
 * Form submission handler for khipu_user_account_delete_form().
 */
function khipu_user_account_delete_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['confirm']) {
    khipu_delete_account($values['kaid']);
    watchdog('Khipu User', 'Khipu account of %receiver_name has been delete.', array('%receiver_name' => $values['khipu_account']->receiver_name));
    drupal_set_message(t('Khipu account of %receiver_name has been delete.', array('%receiver_name' => $values['khipu_account']->receiver_name)));
  }

  $form_state['redirect'] = 'user/' . $values['uid'] . '/edit/khipu_account';
}
