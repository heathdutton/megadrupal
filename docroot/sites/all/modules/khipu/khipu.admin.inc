<?php

/**
 * @file
 * This file has the administrative pages for this module.
 */


/**
 * Page to see all khipu accounts.
 */
function khipu_admin_accounts() {
  $khipu_accounts = khipu_get_all_accounts();
  $environments = khipu_get_environments();
  $rows = array();
  foreach ($khipu_accounts as $khipu_account) {
    $user_account = user_load($khipu_account->uid);
    $rows[] = array(
      $khipu_account->receiver_name,
      $user_account->name,
      $environments[$khipu_account->environment],
      l(t('Test'), 'admin/config/khipu/accounts/' . $khipu_account->kaid . '/test'),
      l(t('Edit'), 'admin/config/khipu/accounts/' . $khipu_account->kaid . '/edit'),
      l(t('Delete'), 'admin/config/khipu/accounts/' . $khipu_account->kaid . '/delete'),
    );
  }
  $header = array(
    t('Receiver Name'),
    t('Account'),
    t('Environment'),
    array(
      'data' => t('Operation'),
      'colspan' => 3,
    ),
  );
  $variables = array(
    'rows' => $rows,
    'header' => $header,
    'empty' => t('No Accounts'),
  );

  return theme('table', $variables);
}


/**
 * Form to create or edit a khipu account.
 *
 * @param array $form
 *   Form element
 * @param array $form_state
 *   Form state
 * @param object $khipu_account
 *   Khipu Account object
 */
function khipu_admin_account_form($form, $form_state, $khipu_account = NULL) {
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

  if (isset($khipu_account->uid)) {
    $account = user_load($khipu_account->uid);
  }
  else {
    global $user;
    $account = $user;
  }
  $form['receiver_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver Name'),
    '#required' => TRUE,
    '#default_value' => isset($khipu_account->receiver_name) ? $khipu_account->receiver_name : '',
  );
  $form['username'] = array(
    '#type' => 'textfield',
    '#title' => 'Username',
    '#autocomplete_path' => 'user/autocomplete',
    '#required' => TRUE,
    '#default_value' => $account->name,
  );
  $form['receiver_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver Id'),
    '#required' => TRUE,
    '#default_value' => isset($khipu_account->receiver_id) ? $khipu_account->receiver_id : '',
  );
  $form['receiver_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver Key'),
    '#required' => TRUE,
    '#default_value' => isset($khipu_account->receiver_key) ? $khipu_account->receiver_key : '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Form validation handler for khipu_admin_account_form().
 */
function khipu_admin_account_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $account = user_load_by_name($values['username']);
  if ($account) {
    $form_state['values']['uid'] = $account->uid;
  }
  else {
    $message_error = t('The username %username does not exist', array('%username' => $values['username']));
    form_set_error('username', $message_error);
  }

  // Validate with Khipi API if the Receiver ID and Receiver Key exist.
  if (!($environment = khipu_check_account($values['receiver_id'], $values['receiver_key']))) {
    form_set_error('receiver_id', t('Khipu not recognize the entered credentials.'));
    form_set_error('receiver_key');
  }
  else {
    $form_state['values']['environment'] = $environment;
  }
}


/**
 * Form submit handler for khipu_admin_account_form().
 */
function khipu_admin_account_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $account = new stdClass();
  $account->receiver_name = $values['receiver_name'];
  $account->uid = $values['uid'];
  $account->receiver_id = $values['receiver_id'];
  $account->receiver_key = $values['receiver_key'];
  $account->environment = $values['environment'];
  if (!$values['kaid']) {
    $account->created = time();
  }
  else {
    $account->kaid = $values['kaid'];
  }

  khipu_save_account($account);
  drupal_set_message(t('Account Khipu was been created'));


  $form_state['redirect'] = 'admin/config/khipu/accounts';
}


/**
 * Form to test a khipu account.
 */
function khipu_admin_test_account($form, $form_state, $khipu_account) {
  if (arg(6) == 'return-from-khipu') {
    drupal_set_message(t('You returned from Khipu, payment will be confirmed within minutes.'));
    drupal_goto('admin/config/khipu/accounts/' . $khipu_account->kaid . '/test');
  }
  global $base_url;

  $fid = variable_get('khipu_test_image_fid', '');
  $image_test = file_load($fid);
  $picture_url = '';
  if ($image_test) {
    $picture_url = file_create_url($image_test->uri);
  }

  $cancel_url = url('admin/config/khipu/accounts', array('absolute' => TRUE));
  $return_url = url('admin/config/khipu/accounts/' . $khipu_account->kaid . '/test/return-from-khipu', array('absolute' => TRUE));
  $notify_url = url('khipu/listen-notification/khipu', array('absolute' => TRUE));

  $data = array(
    'subject' => t('Title of this test'),
    'body' => t('This is a test description'),
    'amount' => 10000,
    'return_url' => $return_url,
    'cancel_url' => $cancel_url,
    'transaction_id' => 1,
    'payer_email' => '',
    'picture_url' => $picture_url,
    'custom' => 'Custom Variable',
    'notify_url' => $notify_url,
  );
  $form = khipu_redirect_form($form, $form_state, $data, $khipu_account->kaid, 'khipu', '100x25');
  return $form;
}


/**
 * Form to confirm if delete a khipu account.
 */
function khipu_admin_account_delete_form($form, $form_state, $khipu_account) {
  $form['kaid'] = array(
    '#type' => 'hidden',
    '#value' => $khipu_account->kaid,
  );
  $form['khipu_account'] = array(
    '#type' => 'value',
    '#value' => $khipu_account,
  );
  $args = array(
    '@name' => $khipu_account->receiver_name,
  );
  $question = t('Are you sure you want to delete the khipu account of @name?', $args);
  $path = 'admin/config/khipu/accounts';

  return confirm_form($form, $question, $path);
}


/**
 * Form validation handler for khipu_admin_account_delete_form().
 */
function khipu_admin_account_delete_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['confirm']) {
    khipu_delete_account($values['kaid']);
    watchdog('Khipu', 'Khipu account of %receiver_name has been delete.', array('%receiver_name' => $values['khipu_account']->receiver_name));
    drupal_set_message(t('Khipu account of %receiver_name has been delete.', array('%receiver_name' => $values['khipu_account']->receiver_name)));
  }

  $form_state['redirect'] = 'admin/config/khipu/accounts';
}


/**
 * Form basic settings.
 */
function khipu_admin_settings_form($form, &$form_state) {

  $default_khipu_test_image_fid = variable_get('khipu_test_image_fid', '');
  $form['khipu_test_image_fid'] = array(
    '#title' => t('Image for Test'),
    '#type' => 'managed_file',
    '#description' => t('The uploaded image will be displayed on khipu page on test.'),
    '#default_value' => $default_khipu_test_image_fid,
    '#upload_location' => 'public://khipu_test/',
    '#upload_validators' => array('file_validate_extensions' => array('png gif jpg')),
  );

  $form['current_khipu_test_image_fid'] = array(
    '#type' => 'hidden',
    '#value' => $default_khipu_test_image_fid,
  );

  $form['khipu_urls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Url from Khipu'),
  );
  $field_prefix = url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q=');
  $form['khipu_urls']['khipu_return_url'] = array(
    '#title' => t('Return url'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => t('Define the url after the user successfully pay. Eg: node/1'),
    '#default_value' => variable_get('khipu_return_url', '<front>'),
    '#element_validate' => array('khipu_validate_path'),
    '#field_prefix' => $field_prefix,
  );
  $form['khipu_urls']['khipu_cancel_url'] = array(
    '#title' => t('Cancel url'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => t('Define the url when there is an error in the process.'),
    '#default_value' => variable_get('khipu_cancel_url', '<front>'),
    '#element_validate' => array('khipu_validate_path'),
    '#field_prefix' => $field_prefix,
  );

  $form['#submit'] = array('khipu_admin_settings_submit_image');
  return system_settings_form($form);
}


/**
 * Form element validation handler for path.
 */
function khipu_validate_path($element, &$form_state) {
  if (!drupal_valid_path($element['#value'])) {
    form_error($element, t('This path is invalid or missing.'));
  }
}


/**
 * Form submission handler for khipu_admin_settings_form().
 */
function khipu_admin_settings_submit_image($form, &$form_state) {
  $values = $form_state['values'];
  $fid = $values['khipu_test_image_fid'];
  if (is_numeric($fid) && $fid != $values['current_khipu_test_image_fid']) {
    // First we remove the current image.
    $old_fid = $values['current_khipu_test_image_fid'];
    $old_file = file_load($old_fid);
    if ($old_file) {
      file_usage_delete($old_file, 'khipu', 'khipu_test', $old_file->fid);
      file_delete($old_file);
    }

    // Load the file via file.fid.
    $file = file_load($fid);
    // Change status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    // Save.
    file_save($file);

    file_usage_add($file, 'khipu', 'khipu_test', $file->fid);
  }

}
