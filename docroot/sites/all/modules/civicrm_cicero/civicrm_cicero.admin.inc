<?php
/**
 * @file
 * CiviCRM Cicero Account Settings page form.
 */

/**
 * Display the admin settings form.
 */
function civicrm_cicero_admin_account_settings($form, &$form_state) {
  $cicero_types_all = civicrm_cicero_get_district_types();
  $active_count = db_query('SELECT COUNT(*) FROM {civicrm_cicero_sync_runs} WHERE status = :status', array(':status' => 'active'))->fetchAssoc();
  if ($active_count['COUNT(*)'] > 0) {
    $change_cicero_user_disabled = TRUE;
    $desc = t('You cannot change the Cicero account information while there are active Cicero sync runs.');
  }
  else {
    $change_cicero_user_disabled = FALSE;
    $desc = t('');
  }
  $form['civicrm_cicero_account_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cicero Account settings.'),
    '#disabled' => $change_cicero_user_disabled,
    '#description' => $desc,
  );
  $form['civicrm_cicero_account_info']['civicrm_cicero_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Cicero Account API Key'),
    '#default_value' => variable_get('civicrm_cicero_api_key', NULL),
    '#description' => t('Your Cicero API Key. API key is preferable to username and password.'),
    '#size' => 55,
  );
  $form['civicrm_cicero_account_info']['civicrm_cicero_account_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Cicero Account User Name'),
    '#default_value' => variable_get('civicrm_cicero_account_name', NULL),
    '#description' => t("Your Cicero API login name. Only necessary if you don't have an API key."),
    '#size' => 55,
  );
  $form['civicrm_cicero_account_info']['civicrm_cicero_account_pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Cicero Account Password'),
    '#maxlength' => 64,
    '#size' => 55,
    '#description' => t("This value is stored and passed in cleartext. Do NOT reuse a password used for other services. Only necessary if you don't have an API key."),
    '#default_value' => variable_get('civicrm_cicero_account_pass', NULL),
  );

  $form['civicrm_cicero_civicrm_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t("Civicrm General Settings."),
  );
  $form['civicrm_cicero_civicrm_settings']['civicrm_cicero_sync_on_new'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically sync when a new contact is added?'),
    '#default_value' => variable_get('civicrm_cicero_sync_on_new', FALSE),
  );
  $form['civicrm_cicero_civicrm_settings']['civicrm_cicero_sync_on_edit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically sync when a new contact is edited (only if matching fields are empty?)'),
    '#default_value' => variable_get('civicrm_cicero_sync_on_edit', FALSE),
  );
  $form['civicrm_cicero_civicrm_settings']['civicrm_cicero_cron_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of contacts to sync with Cicero per cron run?'),
    '#size' => 4,
    '#maxlength' => 4,
    '#default_value' => variable_get('civicrm_cicero_cron_limit', 100),
    '#required' => TRUE,
  );
  $form['civicrm_cicero_civicrm_settings']['civicrm_cicero_last_updated_field'] = array(
    '#type' => 'select',
    '#title' => t('Select CiviCRM Last Updated field'),
    '#options' => civicrm_cicero_admin_get_civi_contact_field_options(),
    '#description' => t('Select the CiviCRM date field that you would like to update when the contact gets syncronized with Cicero.'),
    '#default_value' => variable_get('civicrm_cicero_last_updated_field', NULL),
  );
  $loc_types_default = variable_get('civicrm_cicero_location_types', civicrm_cicero_get_default_location_types());
  $form['civicrm_cicero_civicrm_settings']['civicrm_cicero_location_types'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter Address types to use (in preferred order)'),
    '#description' => t('When selecting the address to use for the lookup, only use the following address types. If you enter more than one address type (separated by line break), use the first one that exists. Available types are: %types', array('%types' => implode(', ', civicrm_cicero_admin_get_civicrm_location_type_options()))),
    '#default_value' => $loc_types_default,
  );
  $form['civicrm_cicero_cicero_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Choose Cicero district types and map them to CiviCRM fields.'),
    '#description' => t('Note: Using both Legislative and Nonlegislative district types will result in two lookups from the Cicero API.'),
  );
  $form['civicrm_cicero_cicero_fields']['legislative_districts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Legistlative Districts'),
    '#collapsible' => TRUE,
    '#collapsed'  => TRUE,
  );
  $form['civicrm_cicero_cicero_fields']['non_legislative_districts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Non Legistlative Districts'),
    '#collapsible' => TRUE,
    '#collapsed'  => TRUE,
  );

  $default_values = variable_get('civicrm_cicero_contact_field_map', array()); 
  foreach ($cicero_types_all as $leg_stat => $districts) {
    foreach ($districts as $cicero_field_val => $cicero_field_label) {
      $default_value = NULL;
      if(array_key_exists($cicero_field_val, $default_values)) {
        $default_value = $default_values[$cicero_field_val];
      }
      $form['civicrm_cicero_cicero_fields'][$leg_stat]['civicrm_cicero_cicero_fields_' . $cicero_field_val] = array(
        '#type' => 'checkbox',
        '#title' => $cicero_field_label,
        '#default_value' => is_null($default_value) ? 0 : 1,
      );
      $form['civicrm_cicero_cicero_fields'][$leg_stat]['civi_field_map_row_' . $cicero_field_val] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('civicrm-cicero-map-row')),
        '#description' => t('Select the CiviCRM field that you would like to map to the "%cicero" Cicero field', array('%cicero' => $cicero_field_label)),
        '#states' => array(
          'visible' => array(
            ':input[name="civicrm_cicero_cicero_fields_' . $cicero_field_val . '"]' => array('checked' => TRUE),
          ),
        ),
      );
      $form['civicrm_cicero_cicero_fields'][$leg_stat]['civi_field_map_row_' . $cicero_field_val]['cicero_field_' . $cicero_field_val . '_display'] = array(
        '#markup' => '<div class="form-item cicero-field-heading"><strong>' . t('Cicero field') . ':</strong><br />' . t($cicero_field_label) . '</div>',
      );
      $form['civicrm_cicero_cicero_fields'][$leg_stat]['civi_field_map_row_' . $cicero_field_val]['cicero_field_' . $cicero_field_val] = array(
        '#type' => 'value',
        '#value' => $cicero_field_val,
      );

      $form['civicrm_cicero_cicero_fields'][$leg_stat]['civi_field_map_row_' . $cicero_field_val]['civi_field_select_' . $cicero_field_val] = array(
        '#type' => 'select',
        '#title' => t('Select CiviCRM Contact field'),
        '#options' => civicrm_cicero_admin_get_civi_contact_field_options(),
        '#default_value' => $default_value,
      );
    }
  }
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'civicrm_cicero') . '/civicrm_cicero.css',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * CiviCRM Cicero Account Settings page form validation.
 */
function civicrm_cicero_admin_account_settings_validate($form, &$form_state) {
  $select_vals = array();
  $user = $form_state['values']['civicrm_cicero_account_name'];
  $pass = $form_state['values']['civicrm_cicero_account_pass'];
  $api_key = $form_state['values']['civicrm_cicero_api_key'];
  if(empty($api_key) && (empty($user) || empty($pass))) {
    form_set_error('civicrm_cicero_api_key', t('You must set either your API key or both your account user name and account password.'));
  }
  $select_vals['civicrm_cicero_last_updated_field'] = $form_state['values']['civicrm_cicero_last_updated_field'];
  foreach ($form_state['values'] as $name => $value) {
    if (strstr($name, 'civi_field_select_') && $value != 'NONE') {
      if (in_array($value, $select_vals)) {
        foreach ($select_vals as $dup_name => $dup_val) {
          if ($value == $dup_val) {
            form_set_error($name, t('You may only use a field mapping once.'));
            form_set_error($dup_name, t('You may only use a field mapping once.'));
            $_SESSION['messages']['error'] = array_unique($_SESSION['messages']['error']);
          }
        }
      }
      else {
        $select_vals[$name] = $value;
      }
    }
  }
  $loc_types = $form_state['values']['civicrm_cicero_location_types'];
  if(FALSE === civicrm_cicero_convert_to_ordered_location_types($loc_types)) {
    form_set_error('civicrm_cicero_location_types', t("At least one of your location types doesn't seem to exist."));
  }
}

/**
 * CiviCRM Cicero Account Settings page form submit.
 */
function civicrm_cicero_admin_account_settings_submit($form, &$form_state) {
  $mapping = array();
  foreach ($form_state['values'] as $key => $value) {
    $select_val = NULL;
    if (strstr($key, 'civicrm_cicero_cicero_fields_') && $value == 1) {
      $cicero_field = str_replace('civicrm_cicero_cicero_fields_', '', $key);
      $select_val = $form_state['values']['civi_field_select_' . $cicero_field];
      if ($select_val != 'NONE') {
        $mapping[$cicero_field] = $select_val;
      }
    }
  }
  variable_set('civicrm_cicero_contact_field_map', $mapping);
  return system_settings_form_submit($form, $form_state);
}

/**
 * CiviCRM Cicero Update Contacts page.
 */
function civicrm_cicero_admin_update_contacts($form, &$form_state) {
  $form = array();
  global $user;
  $form['#suffix'] = civicrm_cicero_upcoming_syncs_table();

  $form['user'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );
  $group_select = civicrm_cicero_admin_select_group_form_item();
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Information about this Cicero sync'),
    '#attributes' => array('class' => array('civicrm-cicero-other-info')),
  );
  if ($group_select) {
    $form['info']['group'] = $group_select;
  }


  $form['info']['when_to_run'] = array(
    '#type' => 'date',
    '#title' => t('On what day should this sync run?'),
    '#states' => array(
      'invisible' => array(
        ':input[name="repeat"]' => array('checked' => TRUE),
      ),
    ),
    '#default_value' => array(
      'month' => date('n'),
      'day' => date('j', strtotime('tomorrow')),
      'year' => date('Y'),
    ),
  );
  $form['info']['time'] = array(
    '#type' => 'fieldset',
    '#title' => t('At what time of day should this sync run?'),
  );
  $form['info']['time']['hour'] = array(
    '#type' => 'select',
    '#title' => t('Hour'),
    '#options' => array(
      1 => 1,
      2 => 2,
      3 => 3,
      4 => 4,
      5 => 5,
      6 => 6,
      7 => 7,
      8 => 8,
      9 => 9,
      10 => 10,
      11 => 11,
      12 => 12,
    ),
  );
  $form['info']['time']['minute'] = array(
    '#type' => 'select',
    '#title' => t('Minute'),
    '#options' => array('00' => '00', 15 => 15, 30 => 30, 45 => 45),
  );
  $form['info']['time']['ampm'] = array(
    '#type' => 'select',
    '#title' => t('AM or PM'),
    '#options' => array('am' => 'AM', 'pm' => 'PM'),
  );
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'civicrm_cicero') . '/civicrm_cicero.css',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Schedule CiviCRM Cicero Contact Sync'),
  );
  return $form;
}

/**
 * CiviCRM Update Contacts page validation function.
 */
function civicrm_cicero_admin_update_contacts_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['group']) || $form_state['values']['group'] == 0) {
    form_set_error('group', t('You must select a Group.'));
  }
}

/**
 * CiviCRM Update Contacts page submit function.
 */
function civicrm_cicero_admin_update_contacts_submit($form, &$form_state) {
  $values = $form_state['values'];
  $group_id = $values['group'];
  $strtime = $values['when_to_run']['month'] . '/' . $values['when_to_run']['day'] . '/' . $values['when_to_run']['year'];
  $strtime .= ' ' . $values['hour'] . ':' . $values['minute'] . $values['ampm'];
  $start_time = strtotime($strtime);
  $cron_limit = variable_get('civicrm_cicero_cron_limit', 100);
  $status = 'pending';

  civicrm_cicero_use_civi_api();
  $params = array(
    'version' => '3',
    'filter.group_id' => $group_id,
    'rowCount' => 100000,
    'return.id' => 1,
  );
  $results = civicrm_api("Contact", "get", $params);
  $progress = array();
  if ($results['count'] == 0) {
    $status = 'completed';
  }
  $progress = new stdClass();
  if ($results['is_error'] == 0) {
    $progress->total = $results['count'];
    $progress->contact_ids = array();
    foreach ($results['values'] as $key => $contact) {
      $progress->contact_ids[] = $contact['contact_id'];
    }
    $progress->start_num = 0;
  }
  $id = db_insert('civicrm_cicero_sync_runs')
    ->fields(array(
      'civi_group_id' => (int) $group_id,
      'drupal_uid' => $values['user'],
      'date_to_run' => (int) $start_time,
      'cron_run_limit' =>  (int) $cron_limit,
      'status' => $status,
      'contact_progress' => serialize($progress),
    ))
    ->execute();
}

/**
 * Helper function to build a form item out of Civi Groups listing.
 *
 * @return
 *   A Drupal Form API select form item array populated with
 *   CiviCRM groups.
 */
function civicrm_cicero_admin_select_group_form_item() {
  civicrm_cicero_use_civi_api();
  $query = "SELECT id, title FROM civicrm_group WHERE is_hidden = 0 AND is_active = 1";
  $dao = CRM_Core_DAO::executeQuery($query);
  $options = array();
  while($dao->fetch()) {
    $options[$dao->id] = $dao->title;
  } 
  if (count($options) > 0) {
    asort($options);
  }
  $item = array(
    '#type'  => 'select',
    '#title' => t('Group to sync'),
    '#options' => $options,
    '#description' => t('Select the group whose Contact addresses will get looked up and updated by Cicero.'),
    '#required' => TRUE,
  );
  return $item;
}

/**
 * Helper function to build form options array of CiviCRM contact fields.
 *
 * @return
 *   An associative array to be used as Drupal forms API select options.
 */
function civicrm_cicero_admin_get_civi_contact_field_options() {
  $options = array();
  $options['NONE'] = t('-- Select a field --');
  civicrm_cicero_use_civi_api();
  $results = civicrm_api("Contact", "getfields", array('version' => '3'));
  if ($results['is_error'] == 0) {
    foreach ($results['values'] as $key => $value) {

      if (isset($value['label'])) {
        $options[$key] = $value['label'];
      }
    }
  }
  return $options;
}

/**
 * Assemble in an array of avaialble district types from Cicero.
 *
 * @return
 *   An associative array with legislative, and non legistlative disticts.
 **/
function civicrm_cicero_get_district_types() {
  // Some of the cicero fields have long/ bad language, using
  // this array to override.
  $field_lang = array(
    'LOCAL' => t('City/Local District'),
    'STATE_LOWER' => t('State Assembly/Lower House'),
    'STATE_UPPER' => t('State Senate or Unicameral Body'),
    'NATIONAL_LOWER' => t('National Lower aka House of Representatives)'),
  );
  // Some hard coded exclusions because they are not useful and
  // (in the case of VOTING they don't seem to work).
  $exclude = array(
    'NATIONAL_EXEC',
    'NATIONAL_UPPER',
    'WATERSHED',
    'POLICE',
    'STATE_EXEC',
    'LOCAL_EXEC',
    'TRANSNATIONAL_EXEC',
    'TRANSNATIONAL_LOWER',
    'TRANSNATIONAL_UPPER',
    // Excluding _2010 fields because district transition is complete.
    'LOCAL_2010',
    'NATIONAL_LOWER_2010',
    'STATE_LOWER_2010',
    'STATE_UPPER_2010'
  );

  $req_fields = array();
  $req_fileds['legislative_districts'] = array();
  $req_fileds['non_legislative_districts'] = array();
  $cicero_district_types = civicrm_cicero_get_response('http://cicero.azavea.com/district_type?format=json');
  $district_types = $cicero_district_types->response->results->district_types;
  foreach ($district_types as $district) {
    if (!in_array($district->name_short, $exclude)) {
      if (array_key_exists($district->name_short, $field_lang)) {
        $desc = $field_lang[$district->name_short];
      }
      elseif ($district->name_long == '') {
        $desc = str_replace('_', ' ', strtolower($district->name_short));
        $desc = ucfirst($desc);
      }
      else {
        $desc = $district->name_long;
      }

      if ($district->is_legislative == TRUE) {
        $req_fields['legislative_districts'][$district->name_short] = $desc;
      }
      else {
        $req_fields['non_legislative_districts'][$district->name_short] = $desc;
      }
    }
  }
  return $req_fields;
}

/**
 * Create a table of upcoming sync events.
 *
 * Helper function to create a paged table of upcoming sync events.
 *
 *
 * @return
 *   A themed HTML table with upcoming CiviCRM Cicero sync runs.
 */
function civicrm_cicero_upcoming_syncs_table() {
  $per_page = 20;
  $query = db_select('civicrm_cicero_sync_runs')->extend('PagerDefault');
  $results = $query
    ->fields('civicrm_cicero_sync_runs')
    ->orderBy('date_to_run', 'DESC')
    ->limit($per_page)
    ->execute();
  $vars = array();
  $vars['header'] = array(
    array('data' => 'Civi Group', 'field' => 'civi_group_id', 'sort' => 'asc'),
    array('data' => 'Date', 'field' => 'date_to_run', 'sort' => 'asc'),
    array('data' => 'Run Limit'),
    array('data' => 'Total contacts'),
    array('data' => 'Status'),
    array('data' => 'Drupal User', 'field' => 'drupal_uid'),
    array('data' => 'Cicero User'),
    array('data' => ''),
  );
  $vars['rows'] = array();
  civicrm_cicero_use_civi_api();
  foreach($results as $run) {
    $run = (array) $run;
    $group = civicrm_api("Group", "get", array('version' => '3', 'id' => $run['civi_group_id']));
    $groupname = '';
    if ($group['is_error'] == 0 && $group['count'] == 1) {
      $groupname = $group['values'][$run['civi_group_id']]['title'];
    }
    $progress = unserialize($run['contact_progress']);
    $user = user_load($run['drupal_uid']);
    $date = date('D, M d, Y g:ia', $run['date_to_run']);
    $cancel_link = ($run['status'] != 'completed') ? l(t('Cancel'), 'admin/config/civicrm/civicrm-cicero/delete-run/' . $run['sync_id']) : NULL;
    $vars['rows'][] = array(
      $groupname,
      $date,
      $run['cron_run_limit'],
      $progress->total,
      $run['status'],
      $user->name,
      $run['cicero_user'],
      $cancel_link,
    );
  }
  $vars['attributes'] = array();
  $vars['colgroups'] = array();
  $vars['caption'] = t('Upcoming sync runs');
  $vars['sticky'] = TRUE;
  $vars['empty'] = t('There are no scheduled CiviCRM Cicero sync runs.');
  return theme_table($vars) . theme('pager');
}

/**
 * Cancel sync run confirmation form.
 */
function civicrm_cicero_admin_delete_run($form, &$form_state) {
  $arg = arg(5);
  if (!is_numeric($arg)) {
    return FALSE;
  }
  if (is_numeric(db_query('SELECT sync_id FROM {civicrm_cicero_sync_runs} WHERE sync_id = :sync_id', array(':sync_id' => $arg))->fetchObject()->sync_id)) {
    $form['sync_id'] = array('#type' => 'value', '#value' => $arg);
    return confirm_form($form, t('Are you sure you want to delete the Cicero sync run?'), 'admin/config/civicrm-cicero', t('Any contacts that have not yet been syncronized with Cicero will not be. This sync run will be permanently lost.'), t('Delete'), t('Cancel'));
  }
  return FALSE;
}

/**
 * Cancel sync run confirmation form submit.
 */
function civicrm_cicero_admin_delete_run_submit($form, &$form_state) {
  db_delete('civicrm_cicero_sync_runs')
    ->condition('sync_id', $form_state['values']['sync_id'])
    ->execute();

  drupal_set_message(t('The CiviCRM Cicero sync has been canceled.'));
  watchdog('civicrm-cicero', 'CiviCRM Cicero: sync run %sync_id deleted.', array('%sync_id' => $form_state['values']['sync_id']), WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/config/civicrm-cicero';
}

/**
 * Helper function to pull in account info from Cicero.
 */
function civicrm_cicero_admin_cicero_accnt_stats() {
  $token = civicrm_cicero_get_token();
  if ($token) {
    $cicero = civicrm_cicero_get_response('http://cicero.azavea.com/v3.0/account/credits_remaining?user=' . $token->user . '&token=' . $token->token);
    if (count($cicero->response->errors) == 0) {
      return $cicero->response->results;
    }
  }
  return;
}

