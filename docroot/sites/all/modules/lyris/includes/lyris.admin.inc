<?php
/**
 * @file
 * Administration settings for Lyris API integration.
 */
/**
 * Lyris Settings Form.
 * FORM
 */
function lyris_connection_settings_form($form, &$form_state) {
  $wsdl_sb    = variable_get('lyris_wsdl_sandbox');
  $wsdl_prod  = variable_get('lyris_wsdl_production');
  $lyris_sb   = ($wsdl_sb ? lyris('admin', 'sandbox') : NULL);
  $lyris_prod = ($wsdl_prod ? lyris('admin', 'production') : NULL);
  $user       = lyris_global_user();

  $admin = variable_get('lyris_connection');

  // Connection mode
  $form['lyris_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Connection Mode'),
    '#description' => t('<strong>Offline:</strong> Create and edit Lyris lists, content and members locally without a connection to Lyris.  Data will be synced when Lyris is accessible.') . '<br />' .
                      t('<strong>Sandbox:</strong> Only lists, content and members belonging to the Sandbox server are shown and therefore editable.') . '<br />' .
                      t('<strong>Production:</strong> Only lists, content and members belonging to the Production server are shown and therefore editable.'),
    '#options' => _lyris_options('lyris_mode'),
    '#default_value' => variable_get('lyris_mode', LYRIS_MODE_DEFAULT),
    '#required' => TRUE,
  );

  $form['lyris_mode_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mode Display'),
    '#description' => t('Display the mode Lyris connection mode on administrative pages.'),
    '#default_value' => variable_get('lyris_mode_display', LYRIS_MODE_DISPLAY_DEFAULT),
  );

  // Sandbox server
  $form['lyris_sandbox'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sandbox Server'),
    '#description' => t('Information about the Lyris Sandbox server.'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($wsdl_sb),
  );
  $form['lyris_sandbox']['lyris_wsdl_sandbox'] = array(
    '#type' => 'textfield',
    '#title' => t('WSDL Location'),
    '#description' => t('Provide the location to your WSDL files.'),
    '#default_value' => $wsdl_sb,
  );
  $form['lyris_sandbox']['lyris_admin_sandbox'] = array(
    '#type' => 'item',
    '#title' => t('Admin Credentials'),
    '#markup' => (isset($admin['sandbox']) && !empty($admin['sandbox']) ? t('Admin credentials are set.') : t('Admin credentials need to be defined in your settings.php file.')),
  );
  $form['lyris_sandbox']['lyris_api_display_sandbox'] = array(
    '#type' => 'item',
    '#title' => t('Lyris API Version'),
    '#markup' => ($wsdl_sb && $lyris_sb->isConnected() ? $lyris_sb->api_version : t('Not connected to Lyris.')),
  );

  // Production Server
  $form['lyris_production'] = array(
    '#type' => 'fieldset',
    '#title' => t('Production Server'),
    '#description' => t('Information about the Lyris Production server.'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($wsdl_prod),
  );
  $form['lyris_production']['lyris_wsdl_production'] = array(
    '#type' => 'textfield',
    '#title' => t('WSDL Location'),
    '#description' => t('Provide the location to your WSDL files.'),
    '#default_value' => $wsdl_prod,
  );
  $form['lyris_production']['lyris_admin_production'] = array(
    '#type' => 'item',
    '#title' => t('Admin Credentials'),
    '#markup' => (isset($admin['production']) && !empty($admin['production']) ? t('Admin credentials are set.') : t('Admin credentials need to be defined in your settings.php file.')),
  );
  $form['lyris_production']['lyris_api_display_production'] = array(
    '#type' => 'item',
    '#title' => t('Lyris API Version'),
    '#markup' => ($wsdl_prod && $lyris_prod->isConnected() ? $lyris_prod->api_version : t('Not connected to Lyris.')),
  );

  $form['lyris_log_connection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log Connection Requests'),
    '#description' => t('Log all API connection requests to watchdog.'),
    '#default_value' => variable_get('lyris_log_connection'),
  );

  return system_settings_form($form);
}

/**
 * Lyris settings form.
 * VALIDATE
 */
function lyris_connection_settings_form_validate($form, &$form_state) {
  $vals = $form_state['values'];

  // Ensure we have credentials provided for the mode the user selected.
  if (empty($vals['lyris_wsdl_' . $vals['lyris_mode']]) && $vals['lyris_mode'] != LYRIS_MODE_OFFLINE) {
    form_set_error('lyris_wsdl_' . $vals['lyris_mode'], t('A @mode WSDL location must be provided to enable @mode mode.', array('@mode' => $vals['lyris_mode'])));
  }
}

/**
 * Lyris settings form.
 * SUBMIT
 */
function lyris_connection_settings_form_submit($form, &$form_state) {
  // Rebuild the menu to enable certain features depending on settings.
  menu_rebuild();
}

/**
 * Basic List settings form.
 */
function lyris_basic_settings_form($form, &$form_state) {
  $form['lyris_list_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('List Name Prefix'),
    '#description' => t('Add a prefix to each listname to make them easier to locate in Lyris and to prevent collisions with other similarly named lists. (Only applies to new lists.)') . '<br /><em>' . t('Value may contain only lowercase alphanumerics, underscores and hyphens.') . '</em>',
    '#maxlength' => 16,
    '#size' => 20,
    '#default_value' => variable_get('lyris_list_prefix'),
    '#element_validate' => array('lyris_validate_machine_name'),
  );

  $form['lyris_member_name_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Member Name Pattern'),
    '#description' => t('When a Drupal user is saved as a Lyris member, use tokens to generate their full name.  This may be used in mailing substitutions later.  If no name is provided it will default to their e-mail address.'),
    '#default_value' => variable_get('lyris_member_name_pattern', '[user:name]'),
  );
  $form['lyris_token_ref'] = array(
    '#type' => 'lyris_token_fieldset',
    'token_tree' => array(
      '#theme' => 'token_tree',
      '#token_types' => array('user'),
      '#global_types' => FALSE,
    ),
  );

  $form['lyris_mailing_test_recipients'] = array(
    '#type' => 'textarea',
    '#title' => t('Test Recipients'),
    '#description' => t('Provide default e-mail recipients to use during mail delivery testing.  These may be changed on each test.') . ' <strong>' . t('One per line.') . '</strong><br />',
    '#rows' => 5,
    '#default_value' => variable_get('lyris_mailing_test_recipients'),
  );

  return system_settings_form($form);
}

/**
 * Manage cron settings for Lyris syncing.
 * FORM
 */
function lyris_cron_settings_form($form, &$form_state) {
  $args = array(
    '%threshold' => variable_get('lyris_sync_threshold', LYRIS_SYNC_THRESHOLD_DEFAULT),
    '%interval' => variable_get('lyris_sync_cron_interval', 60),
    '%max' => variable_get('lyris_sync_cron_batch_count', 100),
  );

  $form['lyris_sync_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Bulk Sync Threshold'),
    '#description' => t('Set a maximum number of records to directly push to Lyris before cron batching is invoked.') . '<br />' .
                      t('Ex. If set to 3, then subscribing up to 3 members at a time will push directly to Lyris on submission.  Four members or more will be updated locally and queued for sync on the next cron run.'),
    '#size' => 10,
    '#maxlength' => 5,
    '#element_validate' => '_lyris_element_validate_integer',
    '#default_value' => $args['%threshold'],
  );
  $form['lyris_sync_cron_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('Sync Interval'),
    '#description' => t('How often do you want to check for unsynced records?') . '<br /><em>' . t('Remember to configure your server\'s cron task to run at least as frequently as this setting.') . '</em>',
    '#size' => 10,
    '#maxlength' => 5,
    '#field_prefix' => t('Every'),
    '#field_suffix' => t('minutes'),
    '#element_validate' => '_lyris_element_validate_integer',
    '#default_value' => $args['%interval'],
  );
  $form['lyris_sync_cron_batch_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Sync Limit per Interval'),
    '#size' => 10,
    '#maxlength' => 5,
    '#description' => t('What is the maximum number of records to sync per attempt?  Set to 0 for unlimited.'),
    '#element_validate' => '_lyris_element_validate_integer',
    '#default_value' => $args['%max'],
  );

  $form['summary'] = array(
    '#type' => 'markup',
    '#prefix' => '<h3>' . t('Summary') . '</h3>',
    '#markup' => t('When a process involves pushing multiple records to Lyris, such as a bulk-subscription request, up to %threshold records will be pushed to Lyris and the remainder will be saved locally and marked as "unsynced."') . '<br />' . t('Every %interval minutes, the system will check for unsynced records and attempt to push up to %max of them to Lyris.', $args),
  );

  return system_settings_form($form);
}

/**
 * Admin user credentials form.
 * FORM
 */
function lyris_admin_credentials_form($form, &$form_state, $account) {
  $status = '';
  $lists = array();
  $form_state['#account'] = $account;

  // Show the user's lyris permissions
  $lyris = lyris($account);

  if ($lyris->isConnected()) {
    $lists = $lyris->loadUserAllowedLists('', 0);
    $fieldset = array(
      '#type' => 'fieldset',
      '#title' => t('Lists'),
      '#attributes' => array('class' => array('collapsible', 'collapsed')),
      '#attached' => array('js' => array('misc/collapse.js', 'misc/form.js')),
    );

    if ($lyris->isServerAdmin()) {
      $status = t('You are a Lyris Server Administrator and may access any list.');
      $class = 'status';
    }
    elseif ($site = $lyris->isSiteAdmin()) {
      $status = t('You have access to all lists within the \'%site\' subset.', array('%site' => $site));
      $fieldset['#children'] = theme('item_list', array('items' => $lists));
      $status .= drupal_render($fieldset);
      $class = 'status';
    }
    elseif (count($lists) > 0) {
      $status = t('You are an administrator on the following lists:');
      $fieldset['#children'] = theme('item_list', array('items' => $lists));
      $status .= drupal_render($fieldset);
      $class = 'status';
    }
  }
  elseif ($lyris->username) {
    $status = t('%name does not have administrative rights to any lists or the username and password are not valid.  Check your credentials or contact your Lyris Adminstrator.', array('%name' => $lyris->username));
    $class = 'error';
  }
  else {
    $status = t('Provide your Lyris username and password.');
    $class = 'warning';
  }

  $form['lyris_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Lyris Username'),
    '#size' => 30,
    '#maxlength' => 128,
    '#default_value' => (isset($account->data['lyris_username']) ? $account->data['lyris_username'] : ''),
  );
  $form['lyris_userpass'] = array(
    '#type' => 'password',
    '#title' => t('Lyris Password'),
    '#description' => t('Password won\'t be changed if left empty.'),
    '#size' => 30,
    '#maxlength' => 128,
    '#default_value' => (isset($account->data['lyris_userpass']) ? $account->data['lyris_userpass'] : ''),
  );

  $form['status'] = array(
    '#type' => 'markup',
    '#markup' => $status,
    '#prefix' => '<div class="lyris-user-status messages ' . $class . '">',
    '#suffix' => '</div>',
    '#attached' => array('css' => array(LYRIS_MOD_PATH . '/lyris.css')),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Admin user credentials form.
 * SUBMIT
 */
function lyris_admin_credentials_form_submit($form, &$form_state) {
  $vals = $form_state['values'];
  $account = $form_state['#account'];

  $edit['data']['lyris_username'] = $vals['lyris_username'];
  if (!empty($vals['lyris_userpass'])) {
    $edit['data']['lyris_userpass'] = lyris_password_encrypt($vals['lyris_userpass']);
  }

  if (user_save($account, $edit)) {
    drupal_set_message(t('Credentials saved successfully.'));
  }
  else {
    drupal_set_message(t('Unable to save credentials.'), 'error');
  }
}
