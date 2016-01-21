<?php

require_once 'redmine_sync.constants.inc';

/**
 * redmine_sync_settings_database_form().
 */
function redmine_sync_settings_database_form($form, &$form_state) {

  drupal_add_css(REDMINE_SYNC_MODULE_PATH.'/redmine_sync.css');

  $db_init = redmine_sync_init_db_rdm();
  if ($db_init == REDMINE_SYNC_DB_INIT_EMPTY_PARAMETERS) {
    $form['error_message_1'] = array(
      '#markup' => theme('form_instead_message', array('status' => 'warning', 'message' =>
        t('Redmine database parameters is not set!')
      )),
    );
  }
  if ($db_init == REDMINE_SYNC_DB_INIT_INVALID_PARAMETERS_OR_SERVER_NOT_AVAILABLE) {
    $form['error_message_2'] = array(
      '#markup' => theme('form_instead_message', array('status' => 'error', 'message' =>
        t('Redmine database parameters is invalid or database server is no available or table %table_name is not exist!', array('%table_name' => 'time_entries'))
      )),
    );
  }

  // Database settings.
  $form['src_db_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Source database settings (Redmine)'),
  );
  $form['src_db_settings']['driver'] = array(
    '#type' => 'select',
    '#title' => t('Selected'),
    '#options' => drupal_map_assoc(array('mysql')),
    '#default_value' => variable_get('redmine_sync_src_db_driver', REDMINE_SYNC_DEF_VAL_SRC_DB_DRIVER),
    '#required' => true,
  );
  $form['src_db_settings']['host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#default_value' => variable_get('redmine_sync_src_db_host', REDMINE_SYNC_DEF_VAL_SRC_DB_HOST),
    '#maxlength' => 128,
    '#required' => true,
  );
  $form['src_db_settings']['port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#default_value' => variable_get('redmine_sync_src_db_port', REDMINE_SYNC_DEF_VAL_SRC_DB_PORT),
    '#maxlength' => 5,
    '#size' => 5,
    '#required' => true,
  );
  $form['src_db_settings']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Database name'),
    '#default_value' => variable_get('redmine_sync_src_db_name', REDMINE_SYNC_DEF_VAL_SRC_DB_NAME),
    '#maxlength' => 64,
    '#size' => 20,
    '#required' => true,
  );
  $form['src_db_settings']['user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('User name'),
    '#default_value' => variable_get('redmine_sync_src_db_user_name'),
    '#maxlength' => 64,
    '#size' => 20,
    '#required' => true,
  );
  $form['src_db_settings']['user_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => str_repeat('#', strlen(variable_get('redmine_sync_src_db_user_pass'))),
    '#maxlength' => 64,
    '#size' => 20,
    '#required' => true,
  );

  // Other settings.
  if ($db_init == REDMINE_SYNC_DB_INIT_OK) {

    // Custom fields on Redmine side.
    $custom_fields = redmine_sync_db_get_rdm_custom_fields();
    $form['src_cf_rdm_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Custom fields on Redmine side'),
    );
    $form['src_cf_rdm_settings']['custom_field_for_node'] = array(
      '#type' => 'select',
      '#title' => t('Field that relate each Redmine time entry with each Drupal node'),
      '#options' => array('' => t('n/a')) + $custom_fields,
      '#default_value' => variable_get('redmine_sync_custom_field_for_node', REDMINE_SYNC_DEF_VAL_NID_FIELD_ID),
      '#description' => t('If you change this parameter, please clear all records and make new synchronization.'),
    );

    // Synchronization settings.
    $form['src_sync_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Synchronization'),
    );
    $form['src_sync_settings']['items_import_per_batch'] = array(
      '#type' => 'select',
      '#title' => t('Items import per batch'),
      '#options' => drupal_map_assoc(array(1, 10, 25, 50, 100, 250, 500, 1000, 2500, 5000, 10000, 25000, 50000)),
      '#default_value' => variable_get('redmine_sync_items_import_per_batch', REDMINE_SYNC_DEF_VAL_ITEMS_IMPORT_PER_BATCH),
      '#required' => true,
    );

  }

  // Actions.
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


function redmine_sync_settings_database_form_validate($form, &$form_state) {
  // Check port.
  $port = $form_state['values']['port'];
  if ($port !== (string)(int)$port || (int)$port < 1) {
    form_set_error('port', t('Field %field must contain an integer positive value.', array('%field' => t('Port'))));
  }
  // Prepare password.
  $password = $form_state['values']['user_pass'];
  if ($password && $password == str_repeat('#', strlen($password))) {
    $password = variable_get('redmine_sync_src_db_user_pass');
  }
  // Check if source database is available.
  $src_db_params = array(
    'driver'   => $form_state['values']['driver'],
    'host'     => $form_state['values']['host'],
    'port'     => $form_state['values']['port'],
    'database' => $form_state['values']['name'],
    'username' => $form_state['values']['user_name'],
    'password' => $password,
  );
  if (!in_array('', $src_db_params)) {
    try {
      Database::addConnectionInfo('src_db', 'default', $src_db_params);
      db_set_active('src_db');
      if (!db_table_exists('time_entries')) {
        form_set_error('name', t('Table %table_name in redmine database is not exist!', array('%table_name' => 'time_entries')));
      }
    } catch (Exception $e) {
      drupal_set_message(t('Redmine database parameters is invalid! --- !err_msg', array('!err_msg' => strtolower($e->getMessage()))), 'error');
      switch ($e->getCode()) {
        case 2002: form_set_error('host'); form_set_error('port'); break;
        case 1049: form_set_error('name'); break;
        case 1045: form_set_error('user_name'); form_set_error('user_pass'); break;
      }
    }
    db_set_active();
  }
}


function redmine_sync_settings_database_form_submit($form, &$form_state) {
  // Prepare password.
  $password = $form_state['values']['user_pass'];
  if ($password && $password == str_repeat('#', strlen($password))) {
    $password = variable_get('redmine_sync_src_db_user_pass');
  }
  // Save variables.
  variable_set('redmine_sync_src_db_driver',    $form_state['values']['driver']);
  variable_set('redmine_sync_src_db_host',      $form_state['values']['host']);
  variable_set('redmine_sync_src_db_port',      $form_state['values']['port']);
  variable_set('redmine_sync_src_db_name',      $form_state['values']['name']);
  variable_set('redmine_sync_src_db_user_name', $form_state['values']['user_name']);
  variable_set('redmine_sync_src_db_user_pass', $password);
  if (isset($form_state['values']['custom_field_for_node']))  variable_set('redmine_sync_custom_field_for_node',  $form_state['values']['custom_field_for_node']);
  if (isset($form_state['values']['items_import_per_batch'])) variable_set('redmine_sync_items_import_per_batch', $form_state['values']['items_import_per_batch']);
  // Show message.
  drupal_set_message(t('Settings was saved.'));
}


/**
 * redmine_sync_settings_rest_api_form().
 */
function redmine_sync_settings_rest_api_form($form, &$form_state) {

  // Confirmation form.
  if (isset($form_state['is_need_confirm'])) {

    // Retranslation of values.
    $form['host']           = array('#type' => 'hidden', '#value' => $form_state['values']['host']);
    $form['port']           = array('#type' => 'hidden', '#value' => $form_state['values']['port']);
    $form['rest_auth_mode'] = array('#type' => 'hidden', '#value' => $form_state['values']['rest_auth_mode']);
    $form['admin_key']      = array('#type' => 'hidden', '#value' => $form_state['values']['admin_key']);

    // Message.
    $form['message'] = array(
      '#markup' => t('You change Authentication mode form "Use personal user keys" to "Use admin key".').REDMINE_SYNC_HTML_BR.
                   t('In this case, the "Redmine access key" field with all user keys will be removed!').REDMINE_SYNC_HTML_BR.
                   t('Do you want to save changes?'),
    );

    // Actions.
    $form['actions'] = array(
      '#type' => 'actions',
    );
    $form['actions']['button_save_after_confirm'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['actions']['button_cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    );

  // Default form.
  } else {

    $form['#attached']['js'][] = REDMINE_SYNC_MODULE_PATH.'/redmine_sync.js';
    $form['src_host_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Source host settings (Redmine)'),
    );
    $form['src_host_settings']['host'] = array(
      '#type' => 'textfield',
      '#title' => t('Host'),
      '#default_value' => variable_get('redmine_sync_rest_api_src_host'),
      '#maxlength' => 128,
      '#required' => true,
    );
    $form['src_host_settings']['port'] = array(
      '#type' => 'textfield',
      '#title' => t('Port'),
      '#default_value' => variable_get('redmine_sync_rest_api_src_port'),
      '#maxlength' => 5,
      '#size' => 5,
    );
    $form['src_host_settings']['rest_auth_mode'] = array(
      '#type' => 'select',
      '#title' => t('Authentication mode'),
      '#options' => array(
        REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY => t('Use admin key'),
        REDMINE_SYNC_REST_API_AUTH_MODE_USER_KEYS => t('Use personal user keys')),
      '#default_value' => variable_get('redmine_sync_rest_api_auth_mode', REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY),
      '#description' => t('In case with personal user keys each user must specify own Redmine API key in their Drupal profile.'),
    );
    $form['src_host_settings']['admin_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Admin key'),
      '#default_value' => str_repeat('#', strlen(variable_get('redmine_sync_rest_api_admin_key'))),
      '#maxlength' => strlen(REDMINE_SYNC_REST_API_ADMIN_KEY_EXAMPLE),
      '#size' => strlen(REDMINE_SYNC_REST_API_ADMIN_KEY_EXAMPLE),
      '#description' => t('Maximum !num characters.', array('!num' => strlen(REDMINE_SYNC_REST_API_ADMIN_KEY_EXAMPLE))),
    );
  
    // Actions.
    $form['actions'] = array(
      '#type' => 'actions',
    );
    $form['actions']['button_save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

  }

  return $form;
}


function redmine_sync_settings_rest_api_form_validate($form, &$form_state) {
  // Check port.
  $port = $form_state['values']['port'];
  if (strlen($port)) {
    if ($port !== (string)(int)$port || (int)$port < 1) {
      form_set_error('port', t('Field %field must contain an integer positive value.', array('%field' => t('Port'))));
    }
  }

  $admin_key = $form_state['values']['admin_key'];
  // Restore default admin key.
  if ($admin_key && $admin_key == str_repeat('#', strlen($admin_key))) {
    $admin_key = variable_get('redmine_sync_rest_api_admin_key');
  }
  // Check admin key.
  if ($admin_key == '' && $form_state['values']['rest_auth_mode'] == REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY) {
    form_set_error('admin_key', t('Field %field is required when Authentication mode is used of admin key.', array('%field' => t('Admin key'))));
  }

  // Check if source server is available.
  $src_rest_api_params = array(
    'host'      => $form_state['values']['host'],
    'port'      => $form_state['values']['port'],
    'admin_key' => $admin_key,
  );
  if (!in_array('', $src_rest_api_params)) {
    $request = redmine_sync_rest_request('users/current', 'GET', null, null, $src_rest_api_params);
    if ($request) {
      switch ($request->code) {
        case -65: form_set_error('host',      t('Error: %error', array('%error' => $request->error))); break;
        case -61: form_set_error('port',      t('Error: %error', array('%error' => $request->error))); break;
        case 401: form_set_error('admin_key', t('Error: %error', array('%error' => $request->error))); break;
      }
    }
  }

  // Confirmation part.
  if ($form_state['clicked_button']['#id'] == 'edit-button-save' &&
      $form_state['values']['rest_auth_mode'] == REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY &&
      variable_get('redmine_sync_rest_api_auth_mode') != REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY) {
    $form_state['rebuild'] = true;         // cancel submit
    $form_state['is_need_confirm'] = true; // show confirm form
  }
  if ($form_state['clicked_button']['#id'] == 'edit-button-save-after-confirm') {
    $form_state['rebuild'] = false;        // go to submit
    unset($form_state['is_need_confirm']); // hide confirm form
  } 
  if ($form_state['clicked_button']['#id'] == 'edit-button-cancel') {
    $form_state['rebuild'] = true;         // cancel submit
    unset($form_state['is_need_confirm']); // hide confirm form
  }

}


function redmine_sync_settings_rest_api_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-button-save' ||
      $form_state['clicked_button']['#id'] == 'edit-button-save-after-confirm') {
    variable_set('redmine_sync_rest_api_src_host', $form_state['values']['host']);
    variable_set('redmine_sync_rest_api_src_port', $form_state['values']['port']);
    // Auth mode and admin key.
    $rest_auth_mode = $form_state['values']['rest_auth_mode'];
    $admin_key      = $form_state['values']['admin_key'];
    // Restore default admin key.
    if ($admin_key && $admin_key == str_repeat('#', strlen($admin_key))) {
      $admin_key = variable_get('redmine_sync_rest_api_admin_key');
    }
    // Add or remove field of user for Redmine personal key.
    if (variable_get('redmine_sync_rest_api_auth_mode') != $rest_auth_mode) {
      switch ($rest_auth_mode) {
        case REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY : redmine_sync_field_del_redmine_access_key(); break;
        case REDMINE_SYNC_REST_API_AUTH_MODE_USER_KEYS : redmine_sync_field_add_redmine_access_key(); break;
      }
    }
    // Save variables.
    variable_set('redmine_sync_rest_api_auth_mode', $rest_auth_mode);
    variable_set('redmine_sync_rest_api_admin_key', $rest_auth_mode != REDMINE_SYNC_REST_API_AUTH_MODE_ADMIN_KEY ? null : $admin_key);
    // Show message.
    drupal_set_message(t('Settings was saved.'));
  }
}

