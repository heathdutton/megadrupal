<?php

/*
 * @file
 * Admin menu callbacks for cd_civicrm.
 */

/**
 * Admin settings form.
 *
 * @see cd_civicrm_settings_form_validate()
 * @see cd_civicrm_settings_form_submit()
 *
 * @ingroup forms
 */
function cd_civicrm_settings_form() {
  civicrm_initialize();
  $form['geocode'] = array(
    '#type' => 'fieldset',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#title' => 'Advanced Batch Geocoding',
    '#description' => t('<a href="!url_geocode">Geocoding within CiviCRM</a> should be enabled '
      . "for more accurate determination of a contact's congressional district (some congressional "
      . 'districts contain more than one zip code).  However geocoding can put a big load on the '
      . 'server if several contacts are saved per second from multiple HTTP requests. '
      . 'Instead CiviCRM Sunlight Congressional District has the ability to override CiviCRM and '
      . 'perform geocoding only in batches during cron and on any CiviCRM profiles used for '
      . '"View/Edit User Account".  To use this feature first <a href="!url_setup">setup '
      . ' a user, password and key for CiviCRM Command-line usage</a>. Enter these values below. '
      . '<br /><em>NOTE: 95% of sites will not require or see any benefit from batch geocoding.</em>'
      , array(
      '!url_geocode' => url('civicrm/admin/setting/mapping'),
      '!url_setup' => 'http://wiki.civicrm.org/confluence/display/CRMDOC/Command-line+Script+Configuration',
    )),
  );
  $form['geocode']['cd_civicrm_geocoding_off'] = array(
    '#type' => 'checkbox',
    '#title' => t('Run CiviCRM geocoding only during cron and some CiviCRM profiles'),
    '#default_value' => variable_get('cd_civicrm_geocoding_off', FALSE),
  );
  $form['geocode']['cd_civicrm_geocode_key'] = array(
    '#title' => t('CiviCRM command-line script key'),
    '#type' => 'item',
    '#markup' => defined('CIVICRM_SITE_KEY') && CIVICRM_SITE_KEY ? 'Is defined' : 'Is not defined',
    '#description' => t('The CiviCRM command-line key.'),
  );
  $form['geocode']['cd_civicrm_geocode_crypt'] = array(
    '#title' => t('Crypt libraries'),
    '#type' => 'item',
    '#markup' => _cd_civicrm_crypt_enabled() ? 'Available' : 'Not available',
    '#description' => t('The <a href=!url>PHP MCrypt extension</a> is required for storing the '
      . 'username. ', array(
      '!url' => 'http://www.php.net/manual/en/book.mcrypt.php',
    )),
  );
  $form['geocode']['cd_civicrm_geocode_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of the Drupal user for running CiviCRM geocode script'),
    '#disabled' => !_cd_civicrm_crypt_enabled(),
    '#default_value' => _cd_civicrm_crypt_decrypt(variable_get('cd_civicrm_geocode_username', '')),
    '#description' => t('An administrator account should be avoided for security reasons.'),
    '#attributes' => array('autocomplete' => 'off'),
  );
  $form['geocode']['cd_civicrm_geocode_password'] = array(
    '#type' => 'item',
    '#title' => t('Password of the Drupal user for running CiviCRM geocode script'),
    '#markup' => defined('CD_CIVICRM_GEOCODE_PASSWORD') && CD_CIVICRM_GEOCODE_PASSWORD ? 'Is defined' : 'Is not defined',
    '#description' => t('Define the password in a constant called !const in settings.php',
      array('!const' => 'CD_CIVICRM_GEOCODE_PASSWORD')),
    '#attributes' => array('autocomplete' => 'off'),
  );
  $form['geocode']['http_auth'] = array(
    '#type' => 'fieldset',
    '#description' => t('If this site requires HTTP authentication.  Please enter the credentials ' .
      'below'),
    '#title' => 'HTTP Authentication',
  );
  $form['geocode']['http_auth']['cd_civicrm_http_auth_user'] = array(
    '#type' => 'textfield',
    '#title' => 'User',
    '#disabled' => !_cd_civicrm_crypt_enabled(),
    '#default_value' => _cd_civicrm_crypt_decrypt(variable_get('cd_civicrm_http_auth_user', '')),
    '#attributes' => array('autocomplete' => 'off'),
  );
  $form['geocode']['http_auth']['cd_civicrm_http_auth_password'] = array(
    '#type' => 'textfield',
    '#title' => 'Password',
    '#disabled' => !_cd_civicrm_crypt_enabled(),
    '#default_value' => _cd_civicrm_crypt_decrypt(variable_get('cd_civicrm_http_auth_password', '')),
    '#attributes' => array('autocomplete' => 'off'),
  );
  $form['geocode']['test'] = array(
    '#type' => 'fieldset',
    '#description' => t('Any issues shown here, or in <a href="!url">the logs</a> (PHP timeouts, ' .
        'etc.) must be resolved before batch geocoding will work. ', array(
      '!url' => url('admin/reports/dblog'),
    )),
  );
  $form['geocode']['test']['test'] = array(
    '#type' => 'submit',
    '#value' => t('Test batch geocoding'),
    '#submit' => array('_cd_civicrm_contacts_geocode'),
    '#disabled' => !_cd_civicrm_crypt_enabled() ||
      !defined('CIVICRM_SITE_KEY') ||
      !defined('CD_CIVICRM_GEOCODE_PASSWORD') ||
      !variable_get('cd_civicrm_geocode_username', ''),
  );

  return $form;
}

/**
 * Admin settings form validator.
 *
 * @see cd_civicrm_settings_form()
 * @see cd_civicrm_settings_form_submit()
 *
 * @ingroup forms
 */
function cd_civicrm_settings_form_validate($form, &$form_state) {

  // Test the geocoding username.
  if ($user_name = $form_state['values']['cd_civicrm_geocode_username']) {
    $message = 'The geocoding user does not exist';
    $user_valid = FALSE;
    $account = array_shift(user_load_multiple(array(), array('name' => $user_name)));
    if ($account && $account->uid) {
      $message = 'The geocoding user exists';
      if (user_access('access all custom data', $account)) {
        $message .= ' with correct permissions';
        drupal_set_message($message);
        $user_valid = TRUE;
      }
      else {
        $message .= ' but without correct permissions';
      }
    }
    if (!$user_valid) {
      form_set_error('cd_civicrm_geocode_username', $message);
    }

    // Encrypt the geocoding username.
    if (_cd_civicrm_crypt_enabled()) {
      $user_name_encrypted = _cd_civicrm_crypt_encrypt($user_name);
      $form_state['values']['cd_civicrm_geocode_username'] = $user_name_encrypted;
    }
  }

  // Encrypt HTTP Auth credentials.
  if ($http_user = $form_state['values']['cd_civicrm_http_auth_user']) {
    if (_cd_civicrm_crypt_enabled()) {
      $http_user_encrypted = _cd_civicrm_crypt_encrypt($http_user);
      $form_state['values']['cd_civicrm_http_auth_user'] = $http_user_encrypted;
      $http_password_encrypted = _cd_civicrm_crypt_encrypt($form_state['values']['cd_civicrm_http_auth_password']);
      $form_state['values']['cd_civicrm_http_auth_password'] = $http_password_encrypted;
    }
  }

}

/**
 * Form to choose CiviCRM custom fields that stores data.
 *
 * @ingroup forms
 *
 * @see cd_civicrm_settings_fields_form_validate()
 * @see cd_civicrm_settings_fields_form_submit()
 */
function cd_civicrm_settings_fields_form($form, &$form_state) {
  global $db_prefix;

  // Get options.
  $fields = array();
  if (!db_table_exists('civicrm_custom_field')) {
    drupal_set_message(t('Table civicrm_custom_field does not exist. You must configure <a href="!url">table prefixes for CiviCRM tables</a> first.', array('!url' => url('admin/reports/status'))), 'error');
  }
  else {
    // TODO: Some day we need to use the CiviCRM internal API.
    $query = db_select('civicrm_custom_field', 'f')
      ->fields('f', array('id', 'custom_group_id', 'label'))
      ->condition('data_type', 'string')
      ->orderBy('custom_group_id')
      ->orderBy('weight')
      ->execute();
    while ($field = $query->fetchAssoc()) {
      $fields[$field['id']] = $field['custom_group_id'] . ': ' . check_plain($field['label']);
    }
  }

  // Form.
  $form = array(
    '#prefix' => '<p>' . t(
      'CiviCRM Congressional District stores congressional district data for each CiviCRM '
      . 'contact in a set of CiviCRM custom fields.  First create these fields <a href="!url">within '
      . 'CiviCRM</a> according to the descriptions below. ',
      array('!url' => url('civicrm/admin/custom/group'))) . '</p>',
  );

  $field_cd_civicrm_cd = variable_get('cd_civicrm_cd', '');
  $field_cd_civicrm_cd = explode('_', $field_cd_civicrm_cd);
  $field_cd_civicrm_cd = array_pop($field_cd_civicrm_cd);
  $form['field_cd_civicrm_cd'] = array(
    '#type' => 'select',
    '#title' => 'Congressional District Field',
    '#default_value' => check_plain($field_cd_civicrm_cd),
    '#options' => $fields,
    '#description' => t('Create one alphanumeric text field to store the contact\'s congressional '
      . 'district (should be searchable, database length 16. If you are '
      . 'using a version of CiviCRM where bug <a href="!CRM-586">CRM-5686</a> is fixed you '
      . 'probably want to make the field view-only).',
      array('!CRM-586' => 'http://issues.civicrm.org/jira/browse/CRM-5686')),
    '#required' => TRUE,
  );

  $field_cd_civicrm_cd_override = variable_get('cd_civicrm_cd_override', '');
  $field_cd_civicrm_cd_override = explode('_', $field_cd_civicrm_cd_override);
  $field_cd_civicrm_cd_override = array_pop($field_cd_civicrm_cd_override);
  $form['field_cd_civicrm_cd_override'] = array(
    '#type' => 'select',
    '#title' => 'Congressional District Override Field',
    '#default_value' => check_plain($field_cd_civicrm_cd_override),
    '#options' => $fields,
    '#description' => t('Create a second alphanumeric text field to allow administrators to '
      . 'manually override the value queried from Sunlight (should be searchable, database length 16). '),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  return $form;

}

/**
 * Validator for field binding form. 
 * 
 * @see cd_civicrm_settings_fields_form()
 * @see cd_civicrm_settings_fields_form_submit()
 *
 * @in_group forms
 */
function cd_civicrm_settings_fields_form_validate($form, &$form_state) {

  // Check that both fields are in the same table.
  $table_1 = db_select('civicrm_custom_field', 'f')
    ->fields('f', array('custom_group_id'))
    ->condition('id', $form_state['values']['field_cd_civicrm_cd'])
    ->execute()
    ->fetchField();
  $table_2 = db_select('civicrm_custom_field', 'f')
    ->fields('f', array('custom_group_id'))
    ->condition('id', $form_state['values']['field_cd_civicrm_cd_override'])
    ->execute()
    ->fetchField();
  if ($table_1 != $table_2) {
    form_set_error('field_cd_civicrm_cd_override', t('Choose two fields from the same group.'));
  }
}

/**
 * Submit handler for field binding form.
 *
 * @in_group forms
 *
 * @see cd_civicrm_settings_fields_form()
 * @see cd_civicrm_settings_fields_form_validate()
 */
function cd_civicrm_settings_fields_form_submit($form, &$form_state) {

  // Retrieve the fields data.
  $query = db_select('civicrm_custom_field', 'f');
  $query->join('civicrm_custom_group', 'g', 'f.custom_group_id = g.id');
  $field_cd_civicrm_cd = $query->fields('f', array('id', 'column_name'))
    ->fields('g', array('table_name'))
    ->condition('f.id', $form_state['values']['field_cd_civicrm_cd'])
    ->execute()
    ->fetchObject();
  $query = db_select('civicrm_custom_field', 'f');
  $query->join('civicrm_custom_group', 'g', 'f.custom_group_id = g.id');
  $field_cd_civicrm_cd_override = $query->fields('f', array('id', 'column_name'))
    ->fields('g', array('table_name'))
    ->condition('f.id', $form_state['values']['field_cd_civicrm_cd_override'])
    ->execute()
    ->fetchObject();

  // Save the variables.
  variable_set('cd_civicrm_custom_table',
    $field_cd_civicrm_cd->table_name);
  variable_set('cd_civicrm_cd',
    'custom_' . $field_cd_civicrm_cd->id);
  variable_set('cd_civicrm_cd_override',
    'custom_' . $field_cd_civicrm_cd_override->id);
  variable_set('cd_civicrm_custom_field_cd',
    $field_cd_civicrm_cd->column_name);
  variable_set('cd_civicrm_custom_field_cd_override',
    $field_cd_civicrm_cd_override->column_name);

  drupal_set_message(t('The fields have been saved.'));
}
