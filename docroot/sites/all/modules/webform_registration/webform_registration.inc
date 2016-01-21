<?php
/**
 * @file
 * Webform Registration Include File.
 *
 * This file holds all the end-user-facing functionality.
 * This file is only here to make the module easier to read.
 */

/**
 * Implements hook_form_alter().
 * Alters instances of registration enabled webforms.
 */
function webform_registration_inc_form_alter(&$form, &$form_state, $form_id) {
  global $user;

  //Set user object
  $form['#user'] = $user;

  //Load the webform settings
  $settings = $form['#node']->webform_registration;

  if (!$settings['enabled']) {
    return;
  }

  $haystack = &$form['submitted'];
  $fields = _webform_registration_get_fields($haystack, $settings);
  
  // Username field handling.
  if (isset($fields['name']['field'])) {
    $fields['name']['field']['#required'] = TRUE;
  }

  // Password field handling.
  if ($settings['pass'] != '<auto>') {
    $fields['pass']['field']['#type'] = 'password';
    $fields['pass']['field']['#required'] = TRUE;
  }

  // Password confirmation field handling
  if ($settings['pass_confirm'] != '<none>') {
    $fields['pass_confirm']['field']['#type'] = 'password';
  }

  // Email field handling
  if (isset($fields['mail']['field'])) {
    $fields['mail']['field']['#required'] = TRUE;
  }

  // The user is logged in. QUICK! HIDE!
  if (user_is_logged_in()) {
    // Hide everything but populate the fields in case webform needs them for
    // something, e.g. Connecting with a CRM or such
    //Load User Data
    $user_data = user_load($user->uid);

    if (is_array($settings['account_fields'])) {
      foreach ($settings['account_fields'] as $account_field => $field) {
        $fields[$account_field]['field']['#attributes'] = array('readonly' => 'readonly');
        $fields[$account_field]['field']['#value'] = $user_data->{$account_field}['und'][0]['value'];
      }
    }

    $fields['name']['field']['#attributes'] = array('readonly' => 'readonly');
    $fields['name']['field']['#value'] = $user->name;

    $fields['pass']['field']['#type'] = 'value';
    $fields['pass']['field']['#value'] = '';

    $fields['pass_confirm']['field']['#type'] = 'value';
    $fields['pass_confirm']['field']['#value'] = '';

    $fields['mail']['field']['#attributes'] = array('readonly' => 'readonly');
    $fields['mail']['field']['#value'] = $user->mail;

    $fields['mail_confirm']['field']['#attributes'] = array('readonly' => 'readonly');
    $fields['mail_confirm']['field']['#value'] = $user->mail;
    $form['#submit'][] = 'webform_registration_upgrade_submit';
  }
  else {
    $form['#validate'][] = 'webform_registration_validate';
    $form['#submit'][] = 'webform_registration_submit';
  }
  _webform_registration_set_fields($haystack, $fields);
}

/**
 * Webform Registration Validate Function.
 */
function webform_registration_validate($form, &$form_state) {

  //Load the webform settings
  $settings = $form['#node']->webform_registration;
  $account = $form['#user'];

  if (isset($form_state['values']['submitted'])) {
    $haystack = &$form_state['values']['submitted'];
  }

  $fields = _webform_registration_get_fields($haystack, $settings);
  
  // Validate new or changing username.
  if (isset($fields['name']['value'])) {
    if ($error = user_validate_name($fields['name']['value'])) {
      form_set_error($fields['name']['key'], $error);
    }
    elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('uid', $account->uid, '<>')->condition('name', db_like($fields['name']['value']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
      form_set_error($fields['name']['key'], t('The name %name is already taken.', array('%name' => $fields['name']['value'])));
    }
  }

  // Validate the e-mail address, and check if it is taken by an existing user.
  if ($error = user_validate_mail($fields['mail']['value'])) {
    form_set_error($fields['mail']['key'], $error);
  }
  elseif ((bool) db_select('users')->fields('users', array('uid'))->condition('uid', $account->uid, '<>')->condition('mail', db_like($fields['mail']['value']), 'LIKE')->range(0, 1)->execute()->fetchField()) {
    // Format error message dependent on whether the user is logged in or not.
    if (user_is_logged_in()) {
      form_set_error($fields['mail']['key'], t('The e-mail address %email is already taken.', array('%email' => $fields['mail']['value'])));
    }
    else {
      form_set_error($fields['mail']['key'], t('The e-mail address %email is already registered. <a href="@password">Have you forgotten your password?</a>', array('%email' => $fields['mail']['value'], '@password' => url('user/password'))));
    }
  }

  // Email Confirmation
  if ($settings['mail_confirm'] != '<none>' && $fields['mail']['value'] != $fields['mail_confirm']['value']) {
    form_set_error($fields['mail_confirm']['key'], t('Your email and confirmation addresses do not match'));
    return;
  }

  // Password Confirmation
  if ($settings['pass_confirm'] != '<none>' && $fields['pass']['value'] != $fields['pass_confirm']['value']) {
    form_set_error($fields['pass_confirm']['key'], t('Your password and confirmation values do not match'));
    return;
  }


}

/**
 * Webform Registration Submit Handler - existing user
 */
function webform_registration_upgrade_submit($form, &$form_state) {
  //Load the webform settings
  $settings = $form['#node']->webform_registration;

  global $user;
  $account = user_load($user->uid);
  $roles = user_roles(TRUE);
  $all_roles = $account->roles;
  if (isset($settings['roles']) && is_array($settings['roles'])) {
    foreach ($settings['roles'] as $rid => $value) {
      $all_roles[$rid] = $roles[$rid];
    }
  }
  user_save($account, array('roles' => $all_roles));
}

/**
 * Webform Registration Submit Handler - new user
 */
function webform_registration_submit($form, &$form_state) {

  //Load the webform settings
  $settings = $form['#node']->webform_registration;

  if (isset($form_state['input']['submitted'])) {
    $haystack = &$form_state['input']['submitted'];
  }
  
  // Retrieve the values and/or form references
  $fields = _webform_registration_get_fields($haystack, $settings);
  
  // Has user has opted out of registration?
  if ($settings['opt_in'] !== '<none>' && $settings['opt_in_value'] != $haystack[$settings['opt_in']]) {
    return;
  }

  watchdog('webform_registration', 'Webform Registration: Registration Triggered');

  $user_register_info = array(
    'values' => array(
      'mail' => $fields['mail']['value'],
      'name' => $fields['name']['value'],
      'notify' => 0,
      'status' => $settings['status'],
      'pass' => ($fields['pass']['key'] != '<auto>' ? $fields['pass']['value'] : user_password()),
      'roles' => $settings['roles'],
    ),
    'buttons' => array(),
  );

  //Add use field instances
  $user_field_info_instances = field_info_instances('user', 'user');
  foreach ($user_field_info_instances as $field_name => $instance) {
    if (!in_array($fields[$field_name]['value'], _webform_registration_get_special_needles())) {
      $user_register_info['values'][$field_name]['und'][0]['value'] = $fields[$field_name]['value'];
    }
  }

  user_register_submit($form, $user_register_info);

  // Update the main submission info.
  db_update('webform_submissions')
    ->fields(array(
      'uid' => $user_register_info['values']['uid'],
    ))
    ->condition('sid', $form_state['values']['details']['sid'])
    ->execute();
}

/**
 * Helper function that retrieves the values to be used with the user
 * registration. This is front-end and end-user.
 */
function _webform_registration_get_fields(&$haystack, $settings) {
  if (is_array($settings['account_fields'])) {
    $settings = array_merge($settings, $settings['account_fields']);
  }
  $fields = array();
  
  // Retrieve a list of the keys allowed to be fetched recursively
  $form_fields = _webform_registration_get_valid_form_fields();
  $special_needles = _webform_registration_get_special_needles();

  // Iterate over all the webform_registration fields and fetch field keys and values.
  foreach ($settings as $key => $needle) {
    // $key is allowed and $needle is not a special needle.
    if (in_array($key, $form_fields) && !in_array($needle, $special_needles)) {
      // go fetch recursively the $haystack.
      $fields[$key] = _webform_registration_get_form_field($needle, $haystack);
    }
    elseif (in_array($needle, $special_needles)) {
      $fields[$key] = array('key' => $needle, 'value' => $needle, 'field' => NULL);
      // @ToDo: Plug here the logic to handle special needles like <auto> for $key == 'pass';
    }
    else {
      $fields[$key] = $needle;
    }
  }
  return $fields;
}

/**
 * Takes a field reference, that may or may not contain ][ separators to
 * determine their depth within the form and returns the value.
 */
function _webform_registration_get_form_field($needle, &$haystack, $value = NULL) {
  if (!isset($needle) || is_null($needle) || $needle == '<none>' || $needle == '<auto>') {
    // This is an exception for special needles like <auto> or <none>.
    // It also handles developer ooopsies like passing an empty $needle.
    return;
  }

  $path = explode('][', $needle);
  $depth = count($path);
  $field = &$haystack;
  $i = 0;
  foreach ($path as $item) {
    if (isset($field[$item])) {
      $field = &$field[$item];
      $i++;
    }
  }

  // Assign a value only if there is one to be assigned and we have a match in
  // the $haystack.
  if ($value != NULL && $depth == $i) {
    $field = $value;
  }
  
  $ret = array(
    'key' => $needle,
    'field' => (is_array($field) ? $field : NULL),
    'value' => (is_array($field) ? (isset($field['#value']) ? $field['#value'] : NULL) : $field),
  );

  return $ret;
}

/**
 * Finds a form field with depth and sets the array that defines it.
 */
function _webform_registration_set_form_field($needle, &$haystack, $value) {
  _webform_registration_get_form_field($needle, $haystack, $value);
}

/**
 * Iterates over the $fields and assigns their values to the $haystack.
 */
function _webform_registration_set_fields(&$haystack, &$fields) {
  // These fields come from the actual rendered webform
  $form_fields = _webform_registration_get_valid_form_fields();
  // Iterate over all the webform_registration fields and fetch field keys and values.
  foreach ($fields as $key => $item) {
    if (in_array($key, $form_fields)) {
      _webform_registration_set_form_field($item['key'], $haystack, $item['field']);
    }
  }
}

/**
 * Retrieves an array of only the fields a user can modify.
 * Useful for fetching from the registration webform.
 */
function _webform_registration_get_valid_form_fields() {

  $user_field_info_instances = field_info_instances('user', 'user');
  $valid_form_fields = drupal_map_assoc(array(
    'name',
    'mail',
    'mail_confirm',
    'pass',
    'pass_confirm',
  ));

  return array_merge($valid_form_fields, array_keys($user_field_info_instances));
}

/**
 * Returns an array of special needles used when we need to get data from places
 * other than the form ($haystack).
 */
function _webform_registration_get_special_needles() {
  return array(
    '<none>',
    '<auto>',
  );
}
