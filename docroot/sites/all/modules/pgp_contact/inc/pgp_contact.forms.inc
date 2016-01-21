<?php
/**
 * @file
 * Forms for the PGP Contact Forms module
 */

/**
 * Upload form for the public key.
 */
function pgp_contact_pubkey($form, &$form_state, $type, $arg2 = NULL) {

  _pgp_contact_secure_form(t('Your connection is unsecured.'));

  _pgp_contact_warnings();

  _pgp_contact_lib_load();
  drupal_add_js(drupal_get_path('module', 'pgp_contact') . '/js/pgp_contact.info.js', array(
  	'type' => 'file',
  	'scope' => 'footer',
  ));

  $form = array(
    '#tree' => TRUE,
  );

  switch ($type) {

    case 'user':
      $id = $arg2->uid;
      $form['uid'] = array(
        '#type' => 'hidden',
        '#value' => $id,
      );
      break;

    case 'contact':
      $id = NULL;
      break;

  }

  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );

  // Fetch info about the key.
  $key_info = _pgp_contact_user_key($id, $type);

  $form['public_key'] = array(
    '#type' => 'textarea',
    '#title' => t('Paste your public key here'),
    '#required' => TRUE,
    '#description' => t('This will not work without javascript enabled.'),
    '#default_value' => ($key_info) ? $key_info->public_key : t('Paste your public key here.'),
    '#rows' => 20,
  );


  if (variable_get('pgp_contact_sitewide_policy', 'user') == 'user') {
    $form['key_policy'] = _pgp_contact_policy_field();
    if ($key_info) {
      $form['key_policy']['pgp_contact_policy']['#default_value'] = $key_info->policy;
    }
  }

  
  $form['verify'] = array(
    '#type' => 'button',
    '#value' => t('Verify key'),
  );
  
  $form['key_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Key Information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#prefix' => '<div id="pgp-key-dynamic-info">',
    '#suffix' => '</div>',
  );

  $form['key_info']['user'] = array(
    '#type' => 'textfield',
    '#title' => t('Account'),
    '#size' => 50,
    '#maxlength' => 128,
  );

  $form['key_info']['algorithm'] = array(
    '#type' => 'textfield',
    '#title' => t('Algorithm'),
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => TRUE,
  );

  $form['key_info']['fingerprint'] = array(
    '#type' => 'textfield',
    '#title' => t('Fingerprint'),
    '#description' => '',
    '#size' => 50,
    '#maxlength' => 50,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit key'),
  );

  return $form;
}

/**
 * Validate function for public key submission.
 */
function pgp_contact_pubkey_validate($form, &$form_state) {

  // Make sure the user has the right to change this key.
  switch ($form_state['values']['type']) {

    case 'user':
      $account = new stdClass();
      $account->uid = check_plain($form_state['values']['uid']);
      if (user_edit_access($account)) {
        break;
      }

    case 'contact':
      if (user_access('administer contact forms')) {
        break;
      }

    default:
      form_set_error('type', 'Access error.');
      break;
  }

}

/**
 * Submit function for public key submission.
 */
function pgp_contact_pubkey_submit($form, &$form_state) {

  // Prepare the form values for submission.
  $record = new stdClass();
  $record->uid = (isset($form_state['values']['uid'])) ? check_plain($form_state['values']['uid']) : 0;
  $record->fingerprint = check_plain($form_state['values']['key_info']['fingerprint']);
  $record->algorithm = check_plain($form_state['values']['key_info']['algorithm']);
  $record->type = $form_state['values']['type'];
  $record->public_key = check_plain($form_state['values']['public_key']);
  if (variable_get('pgp_contact_sitewide_policy', 'user') == 'user') {
    $record->policy = check_plain($form_state['values']['pgp_contact_policy']);
  }
  else {
    $record->policy = 'always';
  }

  // Check if this is an update or an insert.
  $update = array();
  switch ($record->type) {

    case 'user':
      if (_pgp_contact_user_key($record->uid)) {
        $update[] = 'uid';
      }
      break;

    case 'contact':
      if (_pgp_contact_user_key(NULL, 'contact')) {
        $update[] = 'type';
      }
      break;

  }
  drupal_write_record('pgp_contact_pub_keys', $record, $update);
  drupal_set_message(t('Public key successfully uploaded.'));
}

/**
 * Helper generates the policy fields.
 *
 * @param string $field_name
 *   The desired name for the field
 */
function _pgp_contact_policy_field($field_name = 'pgp_contact_policy') {
  $fields = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Encryption Policy'),
    '#tree' => FALSE,
  );

  $fields[$field_name] = array(
    '#type' => 'radios',
    '#title' => t('How should we decide when to encrypt contact forms?'),
    '#options' => array(
      'user' => t('Allow visitors to choose to encrypt their messages.'),
      'always' => t('Force encryption with this key.'),
    ),
    '#description' => t('Note that forcing encryption will prevent visitors without javascript from submitting contact forms with encryption enabled.'),
    '#default_value' => variable_get('pgp_contact_sitewide_policy', 'user'),
  );

  return $fields;
}

