<?php
/**
 * @file
 * Functions related to allow admin's to alter the settings and options.
 */

/**
 * Form constructor for editing the settings.
 *
 * @see eve_api_admin_settings_form_validate()
 * @see eve_api_admin_settings_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_settings_form($form, &$form_state) {
  $form['enter_api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enter API Key'),
    '#description' => t("Enter the Alliance Executor's or Corporation CEO's API Key."),
    '#weight' => 0,
    '#collapsed' => variable_get('eve_api_enable', FALSE),
    '#collapsible' => TRUE,
  );

  $form['enter_api']['keyID'] = array(
    '#type' => 'textfield',
    '#title' => t('Key ID'),
    '#description' => t('Please enter your Key ID from the EVE API Page located <a href="@url" target="_blank">here</a>.', array('@url' => 'http://community.eveonline.com/support/api-key/CreatePredefined?accessMask=67108863')),
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 15,
    '#weight' => 0,
    '#default_value' => variable_get('eve_api_corp_keyid', ''),
  );

  $form['enter_api']['vCode'] = array(
    '#type' => 'textfield',
    '#title' => t('Verification Code'),
    '#description' => t('Please enter your Verification Code from the EVE API Page located <a href="@url" target="_blank">here</a>.', array('@url' => 'http://community.eveonline.com/support/api-key/CreatePredefined?accessMask=67108863')),
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 64,
    '#weight' => 10,
    '#default_value' => variable_get('eve_api_corp_vcode', ''),
  );

  $form['enter_api']['enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable EVE API'),
    '#default_value' => variable_get('eve_api_enable', FALSE),
    '#weight' => 20,
  );

  if (variable_get('eve_api_first_run', FALSE) && variable_get('eve_api_enable', FALSE)) {
    $form['enter_api']['force'] = array(
      '#type' => 'checkbox',
      '#title' => t('Force Update API Information via a cron task.'),
      '#default_value' => FALSE,
      '#weight' => 30,
    );

    $form['settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Settings'),
      '#description' => t('General configurable settings.'),
      '#weight' => 10,
      '#collapsed' => !variable_get('eve_api_enable', FALSE),
      '#collapsible' => TRUE,
    );

    $form['settings']['enable_cron'] = array(
      '#type' => 'radios',
      '#title' => t('Enable EVE API Cron'),
      '#description' => t('If you experience problems, or are updating the module, you can disable the cron related to EVE API.'),
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#weight' => 0,
      '#default_value' => (int) variable_get('eve_api_enable_cron', TRUE),
    );

    $form['settings']['debug'] = array(
      '#type' => 'radios',
      '#title' => t('Enable EVE API Debug'),
      '#description' => t('If you experience problems with EVE API, you can enable the verbose logging.'),
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#weight' => 10,
      '#default_value' => (int) variable_get('eve_api_debug', FALSE),
    );

    $form['settings']['nag_user'] = array(
      '#type' => 'radios',
      '#title' => t('Nag User to Select Character'),
      '#description' => t('Display a message on every page telling the user to select a main character if they have not selected a main character.'),
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#weight' => 20,
      '#default_value' => (int) variable_get('eve_api_nag_user', TRUE),
    );
  }
  else {
    $form['enter_api']['force'] = array(
      '#type' => 'hidden',
      '#value' => FALSE,
    );

    $form['enter_api']['enable_cron'] = array(
      '#type' => 'hidden',
      '#value' => TRUE,
    );

    $form['enter_api']['debug'] = array(
      '#type' => 'hidden',
      '#value' => (int) variable_get('eve_api_debug', FALSE),
    );

    $form['enter_api']['nag_user'] = array(
      '#type' => 'hidden',
      '#value' => (int) variable_get('eve_api_nag_user', TRUE),
    );
  }

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#weight' => 20,
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['enter_api']['enable']['#default_value'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_settings_form_validate');
  $form['#submit'] = array('eve_api_admin_settings_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_settings_form().
 *
 * @see eve_api_admin_settings_form_submit()
 */
function eve_api_admin_settings_form_validate($form, &$form_state) {
  $key_id = (int) $form_state['values']['keyID'];
  $v_code = (string) $form_state['values']['vCode'];

  if (empty($key_id) || empty($v_code) || preg_match('/[^a-z0-9]/i', $v_code) || preg_match('/[^0-9]/', $key_id) || strlen($key_id) > 15 || strlen($v_code) > 64 || strlen($v_code) < 20) {
    form_set_error('keyID', t('Invalid input, please try again.'));
    form_set_error('vCode');
    return;
  }

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  if (isset($characters['error'])) {
    form_set_error('keyID', t('There was an error with the API.'));
    form_set_error('vCode');
    variable_set('eve_api_enable', FALSE);
  }
  else {
    if ($characters['expires'] || $characters['type'] != 'Corporation' || ($characters['accessMask'] & 270680) != 270680) {
      form_set_error('keyID', t('Please ensure that all boxes are highlighted and selected for the API, the "Character" drop down box is set to your Alliance Executor or Corporation CEO, the "Type" drop down box is set to "Corporation", and that the "No Expiry" checkbox is ticked.'));
      form_set_error('vCode');
      variable_set('eve_api_enable', FALSE);
    }

    $form_state['values']['characters'] = $characters;
  }
}

/**
 * Form submission handler for eve_api_admin_settings_form().
 *
 * @see eve_api_admin_settings_form_validate()
 */
function eve_api_admin_settings_form_submit($form, &$form_state) {
  $characters = (array) $form_state['values']['characters'];

  variable_set('eve_api_corp_keyid', (int) $form_state['values']['keyID']);
  variable_set('eve_api_corp_vcode', (string) $form_state['values']['vCode']);
  variable_set('eve_api_enable_cron', (bool) $form_state['values']['enable_cron']);
  variable_set('eve_api_debug', (bool) $form_state['values']['debug']);
  variable_set('eve_api_nag_user', (bool) $form_state['values']['nag_user']);

  // It's not pretty but it works.
  foreach ($characters['characters'] as $character) {
    variable_set('eve_api_allianceID', (int) $character['allianceID']);
    variable_set('eve_api_corporationID', (int) $character['corporationID']);
  }

  if (((bool) $form_state['values']['enable'] && (bool) $form_state['values']['force']) || ((bool) $form_state['values']['enable'] && !variable_get('eve_api_enable', FALSE))) {
    variable_set('eve_api_first_run', TRUE);

    $queue = DrupalQueue::get('eve_api_cron_api_alliance_fetch');
    $queue->createItem(1);

    $queue = DrupalQueue::get('eve_api_cron_api_alliance_sync');
    $queue->createItem(1);

    $queue = DrupalQueue::get('eve_api_cron_api_mask_sync');
    $queue->createItem(1);

    $queue = DrupalQueue::get('eve_api_cron_api_skill_tree');
    $queue->createItem(1);

    $queue = DrupalQueue::get('eve_api_cron_api_error_list');
    $queue->createItem(1);

    drupal_set_message(t('The Alliance API Info is set to be retrieved on the next cron job, it can take up to a minute for the cron task to be triggered.'));
  }
  elseif ((bool) $form_state['values']['enable']) {
    drupal_set_message(t('Settings Updated.'));
  }
  else {
    variable_set('eve_api_first_run', FALSE);
    variable_set('eve_api_enable', FALSE);
    drupal_set_message(t('Registrations have been disabled, EVE API cron tasks have been disabled, and all user menus have been disabled.'));
  }

  menu_rebuild();
}

/**
 * Form constructor for the admin edit roles form.
 *
 * @see eve_api_admin_roles_form_validate()
 * @see eve_api_admin_roles_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_roles_form($form, &$form_state) {
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#weight' => 0,
  );

  $role_list = eve_api_role_list();

  $form['roles']['blue_role'] = array(
    '#type' => 'select',
    '#title' => t('Select Role for Blue Standings'),
    '#description' => t('Please select the default role for blues.'),
    '#options' => $role_list,
    '#default_value' => variable_get('eve_api_blue_role', 2),
    '#weight' => 0,
  );

  $form['roles']['alliance_role'] = array(
    '#type' => 'select',
    '#title' => t('Select Role for Alliance Members'),
    '#description' => t('Please select an additional role assigned to all members of the Alliance.'),
    '#options' => $role_list,
    '#default_value' => variable_get('eve_api_alliance_role', 2),
    '#weight' => 10,
  );

  $form['roles']['unverified_role'] = array(
    '#type' => 'select',
    '#title' => t('Select Role for Unverified'),
    '#description' => t('Please select the default role for Unverified members.'),
    '#options' => $role_list,
    '#default_value' => variable_get('eve_api_unverified_role', 2),
    '#weight' => 20,
  );

  $form['roles']['ceo_role'] = array(
    '#type' => 'select',
    '#title' => t('Select Role for CEOs'),
    '#description' => t('Please select the default role for CEOs.'),
    '#options' => $role_list,
    '#default_value' => variable_get('eve_api_ceo_role', 2),
    '#weight' => 30,
  );

  $form['roles']['director_role'] = array(
    '#type' => 'select',
    '#title' => t('Select Role for Directors'),
    '#description' => t('Please select the default role for directors.'),
    '#options' => $role_list,
    '#default_value' => variable_get('eve_api_director_role', 2),
    '#weight' => 40,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_roles_form_validate');
  $form['#submit'] = array('eve_api_admin_roles_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_roles_form().
 *
 * @see eve_api_admin_roles_form_submit()
 */
function eve_api_admin_roles_form_validate($form, &$form_state) {
  if (user_role_load((int) $form_state['values']['blue_role']) == FALSE) {
    form_set_error('blue_role', t('The role does not exist.'));
  }

  if (user_role_load((int) $form_state['values']['alliance_role']) == FALSE) {
    form_set_error('alliance_role', t('The role does not exist.'));
  }

  if (user_role_load((int) $form_state['values']['unverified_role']) == FALSE) {
    form_set_error('unverified_role', t('The role does not exist.'));
  }

  if (user_role_load((int) $form_state['values']['ceo_role']) == FALSE) {
    form_set_error('ceo_role', t('The role does not exist.'));
  }

  if (user_role_load((int) $form_state['values']['director_role']) == FALSE) {
    form_set_error('director_role', t('The role does not exist.'));
  }
}

/**
 * Form submission handler for eve_api_admin_roles_form().
 *
 * @see eve_api_admin_roles_form_validate()
 */
function eve_api_admin_roles_form_submit($form, &$form_state) {
  if ((int) variable_get('eve_api_blue_role', 2) != 2) {
    $uids = array();
    $result = db_query('SELECT uid FROM {users_roles} WHERE rid = :rid', array(
      ':rid' => (int) variable_get('eve_api_blue_role', 2),
    ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $uids[] = $row->uid;
      }
    }

    if (!empty($uids)) {
      user_multiple_role_edit($uids, 'remove_role', (int) variable_get('eve_api_blue_role', 2));
      user_multiple_role_edit($uids, 'add_role', (int) $form_state['values']['blue_role']);
    }
  }

  if ((int) variable_get('eve_api_alliance_role', 2) != 2) {
    $uids = array();
    $result = db_query('SELECT uid FROM {users_roles} WHERE rid = :rid', array(
      ':rid' => (int) variable_get('eve_api_alliance_role', 2),
    ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $uids[] = $row->uid;
      }
    }

    if (!empty($uids)) {
      user_multiple_role_edit($uids, 'remove_role', (int) variable_get('eve_api_alliance_role', 2));
      user_multiple_role_edit($uids, 'add_role', (int) $form_state['values']['alliance_role']);
    }
  }

  if ((int) variable_get('eve_api_unverified_role', 2) != 2) {
    $uids = array();
    $result = db_query('SELECT uid FROM {users_roles} WHERE rid = :rid', array(
      ':rid' => (int) variable_get('eve_api_unverified_role', 2),
    ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $uids[] = $row->uid;
      }
    }

    if (!empty($uids)) {
      user_multiple_role_edit($uids, 'remove_role', (int) variable_get('eve_api_unverified_role', 2));
      user_multiple_role_edit($uids, 'add_role', (int) $form_state['values']['unverified_role']);
    }
  }

  if ((int) variable_get('eve_api_ceo_role', 2) != 2) {
    $uids = array();
    $result = db_query('SELECT uid FROM {users_roles} WHERE rid = :rid', array(
      ':rid' => (int) variable_get('eve_api_ceo_role', 2),
    ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $uids[] = $row->uid;
      }
    }

    if (!empty($uids)) {
      user_multiple_role_edit($uids, 'remove_role', (int) variable_get('eve_api_ceo_role', 2));
      user_multiple_role_edit($uids, 'add_role', (int) $form_state['values']['ceo_role']);
    }
  }

  if ((int) variable_get('eve_api_director_role', 2) != 2) {
    $uids = array();
    $result = db_query('SELECT uid FROM {users_roles} WHERE rid = :rid', array(
      ':rid' => (int) variable_get('eve_api_director_role', 2),
    ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $uids[] = $row->uid;
      }
    }

    if (!empty($uids)) {
      user_multiple_role_edit($uids, 'remove_role', (int) variable_get('eve_api_director_role', 2));
      user_multiple_role_edit($uids, 'add_role', (int) $form_state['values']['director_role']);
    }
  }

  variable_set('eve_api_blue_role', (int) $form_state['values']['blue_role']);
  variable_set('eve_api_alliance_role', (int) $form_state['values']['alliance_role']);
  variable_set('eve_api_unverified_role', (int) $form_state['values']['unverified_role']);
  variable_set('eve_api_ceo_role', (int) $form_state['values']['ceo_role']);
  variable_set('eve_api_director_role', (int) $form_state['values']['director_role']);

  drupal_set_message(t('The roles have been updated and will be updated with the next scheduled cron tasks.'));
}

/**
 * Form constructor for the altering the access mask form.
 *
 * @see eve_api_admin_registration_form_validate()
 * @see eve_api_admin_registration_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_registration_form($form, &$form_state) {
  $header = array(
    'name' => array(
      'data' => t('Name'),
    ),
    'description' => array(
      'data' => t('Description'),
    ),
  );

  $access_mask = variable_get('eve_api_access_mask', 268435455);

  $form['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Modify Registration Settings'),
    '#description' => t('Edit your required user and active registration settings.'),
    '#weight' => 0,
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );

  $form['registration']['standing'] = array(
    '#type' => 'textfield',
    '#title' => t('Standings'),
    '#description' => t('Set the required standing to register on the site. Standing required will be equal to or greater than the value supplied. Accepted values are 10.0 to -10.0'),
    '#maxlength' => 5,
    '#size' => 10,
    '#weight' => 10,
    '#required' => TRUE,
    '#default_value' => round((float) variable_get('eve_api_required_standing', 0.1), 1),
  );

  $form['registration']['blue'] = array(
    '#type' => 'radios',
    '#title' => t('Allow Anyone to Register'),
    '#description' => t('By Default, only characters who are blue to the alliance/corporation may register, you can disable this requirment so non blue characters can be used to register.'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#weight' => 20,
    '#default_value' => (int) variable_get('eve_api_require_blue', FALSE),
  );

  $form['registration']['expires'] = array(
    '#type' => 'radios',
    '#title' => t('Allow Keys to Expire'),
    '#description' => t('API keys can be set to expire at a certain date and time. You can allow users to register with API Keys that will expire, but this may cause issues if the user forgets that the API Key has/will expire.'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#weight' => 30,
    '#default_value' => (int) variable_get('eve_api_require_expires', FALSE),
  );

  $form['registration']['type'] = array(
    '#type' => 'radios',
    '#title' => t('Require All Characters'),
    '#description' => t('The API Key can list all characters on the account, or only show a single character.'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#weight' => 40,
    '#default_value' => (int) variable_get('eve_api_require_type', TRUE),
  );

  $form['mask'] = array(
    '#type' => 'fieldset',
    '#title' => t('Required Access Mask'),
    '#description' => t('Edit your Access Mask that is required to register on the site.'),
    '#weight' => 10,
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
  );

  $form['mask']['current_mask'] = array(
    '#type' => 'markup',
    '#prefix' => '<p>',
    '#markup' => t('Current Mask: @access_mask', array('@access_mask' => $access_mask)),
    '#suffix' => '</p>',
    '#weight' => 0,
  );

  $form['mask']['access_mask'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 10,
  );

  $result = db_select('eve_api_call_list', 'am')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('am', array(
      'accessmask',
      'name',
      'groupID',
      'description',
    ))
    ->condition('am.type', 'Character', '=')
    ->limit(30)
    ->orderByHeader($header)
    ->execute();

  if ($result->rowCount()) {
    $rows = $result->fetchAll();
  }

  $result_a = db_query('SELECT DISTINCT amg.groupID, amg.name, amg.description
    FROM {eve_api_call_list_groups} amg
    LEFT JOIN {eve_api_call_list} am ON am.groupID = amg.groupID
    WHERE am.type = \'Character\' ORDER BY amg.groupID');

  if ($result_a->rowCount()) {
    foreach ($result_a->fetchAll() as $row_a) {
      $options = $selected = array();

      foreach ($rows as $row) {
        if ($row_a->groupID != $row->groupID) {
          continue;
        }

        $options[$row->accessmask] = array(
          'name' => check_plain($row->name),
          'description' => check_plain($row->description),
        );

        if (($access_mask & (int) $row->accessmask) == TRUE) {
          $selected[$row->accessmask] = TRUE;
        }
      }

      $form['mask']['access_mask'][(string) $row_a->groupID] = array(
        '#type' => 'fieldset',
        '#title' => (string) $row_a->name,
        '#group' => 'access_mask',
        '#collapsed' => FALSE,
        '#collapsible' => TRUE,
        '#weight' => (int) $row_a->groupID,
      );

      $group_name = strtolower((string) $row_a->name);
      $group_name = str_replace(' ', '', $group_name);

      $form['mask']['access_mask'][(string) $row_a->groupID][$group_name] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#default_value' => $selected,
        '#empty' => t('Accessmask not retrieved.'),
        '#weight' => 0,
      );
    }
  }

  $form['mask']['access_mask'][3]['privateinformation'][8]['#disabled'] = TRUE;
  $form['mask']['access_mask'][4]['publicinformation'][8388608]['#disabled'] = TRUE;

  $form['mask']['pager'] = array(
    '#theme' => 'pager',
    '#weight' => 20,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Update Mask'),
    '#weight' => 20,
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_registration_form_validate');
  $form['#submit'] = array('eve_api_admin_registration_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_registration_form().
 *
 * @see eve_api_admin_registration_form_submit()
 */
function eve_api_admin_registration_form_validate($form, &$form_state) {
  if ((float) $form_state['values']['standing'] != round($form_state['values']['standing'], 1) || $form_state['values']['standing'] != (float) $form_state['values']['standing'] || (float) $form_state['values']['standing'] < -10.0 || (float) $form_state['values']['standing'] > 10.0) {
    form_set_error('standing', t('Standings are required to be between -10.0 and 10.0'));
  }
}

/**
 * Form submission handler for eve_api_admin_registration_form().
 *
 * @see eve_api_admin_registration_form_validate()
 */
function eve_api_admin_registration_form_submit($form, &$form_state) {
  $new_access_mask = 8388616;

  $result = db_query('SELECT DISTINCT amg.name, amg.groupID
    FROM {eve_api_call_list_groups} amg
    LEFT JOIN {eve_api_call_list} am ON am.groupID = amg.groupID
    WHERE am.type = \'Character\'');

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      $group_name = strtolower((string) $row->name);
      $group_name = str_replace(' ', '', $group_name);

      foreach ((array) $form_state['values'][$group_name] as $access_bit) {
        $new_access_mask = $new_access_mask + (int) $access_bit;
      }
    }
  }

  $access_mask = variable_get('eve_api_access_mask', 268435455);

  variable_set('eve_api_access_mask', $new_access_mask);
  variable_set('eve_api_require_blue', (bool) $form_state['values']['blue']);

  if ($access_mask != $new_access_mask || round((float) variable_get('eve_api_required_standing', 0.1), 1) != round((float) $form_state['values']['standing'], 1) || (int) variable_get('eve_api_require_expires', FALSE) != (int) $form_state['values']['expires'] || (int) variable_get('eve_api_require_type', TRUE) != (int) $form_state['values']['type']) {
    variable_set('eve_api_required_standing', round((float) $form_state['values']['standing'], 1));
    variable_set('eve_api_require_expires', (bool) $form_state['values']['expires']);
    variable_set('eve_api_require_type', (bool) $form_state['values']['type']);

    $queue = DrupalQueue::get('eve_api_cron_api_user_sync');

    $result = db_query('SELECT DISTINCT uid FROM {eve_api_characters} WHERE deleted = 0');

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $item) {
        $queue->createItem(array(
          'uid' => $item->uid,
          'runs' => 1,
        ));
      }
    }
  }

  drupal_set_message(t('The registration settings have been updated, please be advised any user with incorrect API Masks or standing will have the roles removed.'));
}

/**
 * Form constructor for the admin list blues form.
 *
 * @see eve_api_admin_list_blues_form_validate()
 * @see eve_api_admin_list_blues_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_list_blues_form($form, &$form_state) {
  $header = array(
    'contactID' => array(
      'data' => t('ID'),
      'field' => 'b.contactID',
    ),
    'contactName' => array(
      'data' => t('Name'),
      'field' => 'b.contactName',
    ),
    'standing' => array(
      'data' => t('Standing'),
      'field' => 'b.standing',
    ),
    'manual' => array(
      'data' => t('Manually Entered'),
      'field' => 'b.manual',
    ),
    'action' => array(
      'data' => t('Action'),
    ),
    'memo' => array(
      'data' => t('Memo'),
    ),
  );

  $options = $selected = array();

  $result = db_select('eve_api_contact_list', 'b')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('b', array(
      'contactID',
      'contactName',
      'standing',
      'manual',
      'memo',
    ))
    ->limit(20)
    ->orderByHeader($header)
    ->execute();

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      $options[$row->contactID] = array(
        'contactID' => check_plain($row->contactID),
        'contactName' => check_plain($row->contactName),
        'standing' => check_plain($row->standing),
        'manual' => ($row->manual) ? t('Yes') : t('No'),
        'action' => l(t('Edit'), 'admin/eve_api/list_blues/edit_blue/' . $row->contactID),
        'memo' => check_plain($row->memo),
      );
    }
  }

  $form['list_blues'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update Blue Standing'),
    '#description' => t('Toggle the checkbox to remove/add an Existing Blue Standing.'),
    '#weight' => 0,
  );

  $form['list_blues']['blues'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#default_value' => $selected,
    '#empty' => t('There are no standings.'),
    '#weight' => 10,
  );

  $form['list_blues']['page'] = array(
    '#theme' => 'pager',
    '#weight' => 20,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Update Blue Standings'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_list_blues_form_validate');
  $form['#submit'] = array('eve_api_admin_list_blues_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_list_blues_form().
 *
 * @see eve_api_admin_list_blues_form_submit()
 */
function eve_api_admin_list_blues_form_validate($form, &$form_state) {
  if (!is_array($form_state['values']['blues']) || !count(array_filter((array) $form_state['values']['blues']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Form submission handler for eve_api_admin_list_blues_form().
 *
 * @see eve_api_admin_list_blues_form_validate()
 */
function eve_api_admin_list_blues_form_submit($form, &$form_state) {
  $deleted_blues = array();
  $deleted = array_filter((array) $form_state['values']['blues']);

  foreach ($deleted as $blue_id) {
    $deleted_blues[] = (int) $blue_id;
  }

  if (!empty($deleted_blues)) {
    $result = db_query('SELECT contactID FROM {eve_api_contact_list} WHERE manual = 0 AND contactID IN (:contactIDs)', array(
      ':contactIDs' => $deleted_blues,
    ));

    if ($result->rowCount()) {
      drupal_set_message(t('Deleted Blue Standings will get re-added if not removed from your EVE Alliance Standings during the next daily cron job.'), 'warning');
    }

    db_delete('eve_api_contact_list')->condition('contactID', $deleted_blues, 'IN')->execute();

    $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
    $result = db_query('SELECT DISTINCT uid FROM {eve_api_characters} WHERE deleted = 0 AND (corporationID = :corporationID OR characterID = :characterID OR allianceID = :allianceID)', array(
      ':corporationID' => $deleted_blues,
      ':characterID' => $deleted_blues,
      ':allianceID' => $deleted_blues,
    ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $item) {
        $queue->createItem(array(
          'uid' => $item->uid,
          'runs' => 1,
        ));
      }
    }

    drupal_set_message(t('The Alliance Blue Standings List has been updated, the users have been manually added to the cron queue to check standings. This can take a while to process.'));
  }
  else {
    drupal_set_message(t('Nothing changed.'), 'warning');
  }
}

/**
 * Form constructor for the admin add blues form.
 *
 * @see eve_api_admin_add_blue_form_validate()
 * @see eve_api_admin_add_blue_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_add_blue_form($form, &$form_state) {
  $form['add_blue'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Blue Standing'),
    '#description' => t('Add a custom blue immediately, if the blue is found on the next API pull, it will be converted to a full Blue Standing.'),
    '#weight' => 0,
  );

  $form['add_blue']['contactName'] = array(
    '#type' => 'textfield',
    '#title' => t('Character/Corporation/Alliance Name'),
    '#description' => t('Please enter the Name of the Character/Corporation/Alliance, if found, details will be retrieved from the API.'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 50,
    '#weight' => 0,
  );

  $form['add_blue']['standing'] = array(
    '#type' => 'textfield',
    '#title' => t('Standing'),
    '#description' => t('Enter the standing to assign the new blue. Accepted values are 10.0 to -10.0'),
    '#required' => TRUE,
    '#maxlength' => 5,
    '#size' => 10,
    '#weight' => 10,
  );

  $form['add_blue']['memo'] = array(
    '#type' => 'textarea',
    '#title' => t('Memo'),
    '#description' => t('Some information about why you manually adding a blue standing.'),
    '#required' => TRUE,
    '#default_value' => '',
    '#weight' => 20,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Add Blue Standing'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_add_blue_form_validate');
  $form['#submit'] = array('eve_api_admin_add_blue_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_add_blue_form().
 *
 * @see eve_api_admin_add_blue_form_submit()
 */
function eve_api_admin_add_blue_form_validate($form, &$form_state) {
  $contact_name = (string) $form_state['values']['contactName'];

  if ((float) $form_state['values']['standing'] != round($form_state['values']['standing'], 1) || $form_state['values']['standing'] != (float) $form_state['values']['standing'] || (float) $form_state['values']['standing'] < -10.0 || (float) $form_state['values']['standing'] > 10.0) {
    form_set_error('standing', t('Standings are required to be between -10.0 and 10.0'));
  }

  if (!preg_match("/^[a-z0-9 '.\-]+$/i", $contact_name)) {
    form_set_error('contactName', t('Contact Name may only contain the characters A-Z, a-z, 0-9, and single quotation. (Corporation names may also include minus and dot characters.)'));
    return;
  }

  if (!empty($contact_name)) {
    $query = array('names' => $contact_name);
    $contact_data = eve_api_get_character_id_api($query);
    $contact_name = strtoupper($contact_name);
  }

  if (isset($contact_data['error'])) {
    form_set_error('contactName', t('There was an error with the API.'));
  }
  else {
    if (isset($contact_data[$contact_name]) && (int) $contact_data[$contact_name]['characterID'] != 0) {
      $result = db_query('SELECT contactID FROM {eve_api_contact_list} WHERE contactID = :contactID', array(
        ':contactID' => (int) $contact_data[$contact_name]['characterID'],
      ));

      if ($result->rowCount()) {
        form_set_error('contactName', t('Character/Corporation/Alliance already added to the standings.'));
      }
      else {
        $form_state['contactID'] = (int) $contact_data[$contact_name]['characterID'];
        $form_state['contactName'] = (string) $contact_data[$contact_name]['name'];
      }
    }
    else {
      form_set_error('contactName', t('No match found.'));
    }
  }
}

/**
 * Form submission handler for eve_api_admin_add_blue_form().
 *
 * @see eve_api_admin_add_blue_form_validate()
 */
function eve_api_admin_add_blue_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/eve_api/list_blues';
  $standing = round((float) $form_state['values']['standing'], 1);

  db_insert('eve_api_contact_list')->fields(array(
    'contactID' => (int) $form_state['contactID'],
    'contactName' => (string) $form_state['contactName'],
    'standing' => (float) $standing,
    'manual' => 1,
    'memo' => (string) $form_state['values']['memo'],
  ))->execute();

  $queue = DrupalQueue::get('eve_api_cron_api_user_sync');

  $result = db_query('SELECT DISTINCT uid FROM {eve_api_characters} WHERE deleted = 0 AND (corporationID = :corporationID OR characterID = :characterID OR allianceID = :allianceID)', array(
    ':corporationID' => (int) $form_state['contactID'],
    ':characterID' => (int) $form_state['contactID'],
    ':allianceID' => (int) $form_state['contactID'],
  ));

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $item) {
      $queue->createItem(array(
        'uid' => $item->uid,
        'runs' => 1,
      ));
    }
  }

  drupal_set_message(t('A new Blue Standing has been added!'));
}

/**
 * Form constructor for the admin edit blue form.
 *
 * @see eve_api_admin_edit_blue_form_validate()
 * @see eve_api_admin_edit_blue_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_edit_blue_form($form, &$form_state, $contact_id) {
  $form['edit_blue'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit Blue Standing'),
    '#description' => t('Edit the memo for a Blue Standing.'),
    '#weight' => 0,
  );

  $result = db_query('SELECT memo, standing FROM {eve_api_contact_list} WHERE contactID = :contactID', array(
    ':contactID' => $contact_id,
  ));

  if (!$result->rowCount()) {
    drupal_goto('admin/eve_api/list_blues');
  }

  $row = $result->fetchAssoc();
  $memo = $row['memo'];
  $standing = $row['standing'];

  $form['edit_blue']['standing'] = array(
    '#type' => 'textfield',
    '#title' => t('Standing'),
    '#description' => t('Enter the standing to assign the new blue. Accepted values are 10.0 to -10.0'),
    '#required' => TRUE,
    '#maxlength' => 5,
    '#size' => 10,
    '#default_value' => check_plain($standing),
    '#weight' => 0,
  );

  $form['edit_blue']['memo'] = array(
    '#type' => 'textarea',
    '#title' => t('Memo'),
    '#description' => t('Some information about the Blue Standing.'),
    '#required' => TRUE,
    '#default_value' => check_plain($memo),
    '#weight' => 10,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Edit Blue Standing'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form_state['contactID'] = $contact_id;
  $form['#validate'] = array('eve_api_admin_edit_blue_form_validate');
  $form['#submit'] = array('eve_api_admin_edit_blue_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_edit_blue_form().
 *
 * @see eve_api_admin_edit_blue_form_submit()
 */
function eve_api_admin_edit_blue_form_validate($form, &$form_state) {
  $contact_id = (int) $form_state['contactID'];

  if ((float) $form_state['values']['standing'] != round($form_state['values']['standing'], 1) || $form_state['values']['standing'] != (float) $form_state['values']['standing'] || (float) $form_state['values']['standing'] < -10.0 || (float) $form_state['values']['standing'] > 10.0) {
    form_set_error('standing', t('Standings are required to be between -10.0 and 10.0'));
  }

  $result = db_query('SELECT contactID FROM {eve_api_contact_list} WHERE contactID = :contactID', array(
    ':contactID' => (int) $contact_id,
  ));

  if (!$result->rowCount()) {
    form_set_error('', t('Blue Standing does not exist.'));
  }
}

/**
 * Form submission handler for eve_api_admin_edit_blue_form().
 *
 * @see eve_api_admin_edit_blue_form_validate()
 */
function eve_api_admin_edit_blue_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/eve_api/list_blues';
  $contact_id = (int) $form_state['contactID'];
  $memo = (string) $form_state['values']['memo'];
  $standing = round((float) $form_state['values']['standing'], 1);

  db_update('eve_api_contact_list')->fields(array(
      'memo' => $memo,
      'standing' => $standing,
    ))->condition('contactID', (int) $contact_id, '=')->execute();

  drupal_set_message(t('Blue Standing has been updated!'));
}

/**
 * Form constructor for the admin list corporations form.
 *
 * @see eve_api_admin_list_corporations_form_validate()
 * @see eve_api_admin_list_corporations_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_list_corporations_form($form, &$form_state) {
  $header = array(
    'corporationName' => array(
      'data' => t('Corporation Name'),
      'field' => 'ac.corporationName',
    ),
    'manual' => array(
      'data' => t('Manually Entered'),
      'field' => 'ac.manual',
    ),
    'action' => array(
      'data' => t('Action'),
    ),
    'memo' => array(
      'data' => t('Memo'),
    ),
  );

  $options = $selected = array();

  $result = db_select('eve_api_alliance_list_corporations', 'ac')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('ac', array(
      'corporationID',
      'rid',
      'corporationName',
      'corporationTicker',
      'manual',
      'memo',
    ))
    ->limit(20)
    ->orderByHeader($header)
    ->execute();

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      $options[$row->corporationID] = array(
        'corporationName' => check_plain($row->corporationName),
        'manual' => ($row->manual) ? t('Yes') : t('No'),
        'action' => l(t('Edit'), 'admin/eve_api/list_corporations/edit_corporation/' . $row->corporationID),
        'memo' => check_plain($row->memo),
      );
    }
  }

  $form['list_corporations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update Alliance Corporations'),
    '#description' => t('Toggle the checkbox to remove an Existing Corporation.'),
    '#weight' => 0,
  );

  $form['list_corporations']['corporations'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#default_value' => $selected,
    '#empty' => t('There are no corporations.'),
    '#weight' => 10,
  );

  $form['list_corporations']['page'] = array(
    '#theme' => 'pager',
    '#weight' => 20,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Update Alliance Corporations'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_list_corporations_form_validate');
  $form['#submit'] = array('eve_api_admin_list_corporations_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_list_corporations_form().
 *
 * @see eve_api_admin_list_corporations_form_submit()
 */
function eve_api_admin_list_corporations_form_validate($form, &$form_state) {
  if (!is_array($form_state['values']['corporations']) || !count(array_filter((array) $form_state['values']['corporations']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Form submission handler for eve_api_admin_list_corporations_form().
 *
 * @see eve_api_admin_list_corporations_form_validate()
 */
function eve_api_admin_list_corporations_form_submit($form, &$form_state) {
  $deleted_corporations = array();
  $deleted = array_filter((array) $form_state['values']['corporations']);

  foreach ($deleted as $corporation_id) {
    $deleted_corporations[] = (int) $corporation_id;
  }

  if (!empty($deleted_corporations)) {
    $result = db_query('SELECT corporationID FROM {eve_api_alliance_list_corporations} WHERE manual = 0 AND corporationID IN (:corporationIDs)', array(
      ':corporationIDs' => $deleted_corporations,
    ));

    if ($result->rowCount()) {
      drupal_set_message(t('Deleted Corporations will get re-added if not removed from your EVE Alliance during the next daily cron job.'), 'warning');
    }

    $result = db_query('SELECT corporationName FROM {eve_api_alliance_list_corporations} WHERE corporationID IN (:corporationIDs)', array(
      ':corporationIDs' => $deleted_corporations,
    ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $corp_data) {
        if (user_role_load_by_name($corp_data->corporationName) == TRUE) {
          user_role_delete($corp_data->corporationName);
        }
      }
    }

    db_delete('eve_api_alliance_list_corporations')->condition('corporationID', $deleted_corporations, 'IN')->execute();

    $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
    $result = db_query('SELECT DISTINCT uid FROM {eve_api_characters} WHERE deleted = 0 AND corporationID IN (:corporationIDs)', array(
      ':corporationIDs' => $deleted_corporations,
    ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $item) {
        $queue->createItem(array(
          'uid' => $item->uid,
          'runs' => 1,
        ));
      }
    }

    drupal_set_message(t('The Alliance Corporations have been updated, the users have been manually added to the cron queue to check standings. This can take a while to process.'));
  }
  else {
    drupal_set_message(t('Nothing changed.'), 'warning');
  }
}

/**
 * Form constructor for the admin add alliance corporation form.
 *
 * @see eve_api_admin_add_corporation_form_validate()
 * @see eve_api_admin_add_corporation_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_add_corporation_form($form, &$form_state) {
  $form['add_corporation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Alliance Corporation'),
    '#description' => t('Add a custom corporation immediately, if the corporation is found on the next API pull, it will be converted to a full Alliance Corporation.'),
    '#weight' => 0,
  );

  $form['add_corporation']['corporationName'] = array(
    '#type' => 'textfield',
    '#title' => t('Corporation'),
    '#description' => t('Please enter the Name of the Corporation, if found, details will be retrieved from the API.'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 50,
    '#weight' => 0,
  );

  $form['add_corporation']['memo'] = array(
    '#type' => 'textarea',
    '#title' => t('Memo'),
    '#description' => t('Some information about why you manually adding the corporation.'),
    '#required' => TRUE,
    '#default_value' => '',
    '#weight' => 20,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Add Corporation'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_add_corporation_form_validate');
  $form['#submit'] = array('eve_api_admin_add_corporation_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_add_corporation_form().
 *
 * @see eve_api_admin_add_corporation_form_submit()
 */
function eve_api_admin_add_corporation_form_validate($form, &$form_state) {
  $corporation_name = (string) $form_state['values']['corporationName'];

  if (!preg_match("/^[a-z0-9 '.\-]+$/i", $corporation_name)) {
    form_set_error('corporationName', t('Contact Name may only contain the characters A-Z, a-z, 0-9, and single quotation. (Corporation names may also include minus and dot characters.)'));
    return;
  }

  if (!empty($corporation_name)) {
    $query = array('names' => $corporation_name);
    $contact_data = eve_api_get_character_id_api($query);
    $corporation_name = strtoupper($corporation_name);
  }

  if (isset($contact_data['error'])) {
    form_set_error('corporationName', t('There was an error with the API.'));
  }
  else {
    if (isset($contact_data[$corporation_name]) && (int) $contact_data[$corporation_name]['characterID'] != 0) {
      $result = db_query('SELECT corporationID FROM {eve_api_alliance_list_corporations} WHERE corporationID = :corporationID', array(
        ':corporationID' => (int) $contact_data[$corporation_name]['characterID'],
      ));

      if ($result->rowCount()) {
        form_set_error('corporationName', t('Corporation already added.'));
      }
      else {
        $query = array('corporationID' => (int) $contact_data[$corporation_name]['characterID']);
        $corp_info = eve_api_get_corporation_sheet_api($query);

        if (isset($corp_info['corporationName'])) {
          $form_state['corp_info'] = $corp_info;
        }
        else {
          form_set_error('corporationName', t('No match found.'));
        }
      }
    }
    else {
      form_set_error('corporationName', t('No match found.'));
    }
  }
}

/**
 * Form submission handler for eve_api_admin_add_corporation_form().
 *
 * @see eve_api_admin_add_corporation_form_validate()
 */
function eve_api_admin_add_corporation_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/eve_api/list_corporations';
  $corp_info = $form_state['corp_info'];
  $memo = (string) $form_state['values']['memo'];

  if (!($role = user_role_load_by_name($corp_info['corporationName']))) {
    $user_role = new stdClass();
    $user_role->name = $corp_info['corporationName'];

    if (!empty($corp_info['corporationName'])) {
      user_role_save($user_role);
    }

    $role = user_role_load_by_name($corp_info['corporationName']);
  }

  db_insert('eve_api_alliance_list_corporations')->fields(array(
    'corporationID' => (int) $corp_info['corporationID'],
    'rid' => (int) $role->rid,
    'corporationName' => (string) $corp_info['corporationName'],
    'corporationTicker' => (string) $corp_info['corporationTicker'],
    'ceoID' => (int) $corp_info['ceoID'],
    'ceoName' => (string) $corp_info['ceoName'],
    'checked' => 0,
    'manual' => 1,
    'memo' => $memo,
  ))->execute();

  $queue = DrupalQueue::get('eve_api_cron_api_user_sync');

  $result = db_query('SELECT DISTINCT uid FROM {eve_api_characters} WHERE deleted = 0 AND corporationID = :corporationID', array(
    ':corporationID' => (int) $corp_info['corporationID'],
  ));

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $item) {
      $queue->createItem(array(
        'uid' => $item->uid,
        'runs' => 1,
      ));
    }
  }

  drupal_set_message(t('A new corporation has been added!'));
}

/**
 * Form constructor for the admin edit alliance corporation form.
 *
 * @see eve_api_admin_edit_corporation_form_validate()
 * @see eve_api_admin_edit_corporation_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_edit_corporation_form($form, &$form_state, $corporation_id) {
  $form['edit_corporation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit Alliance Corporation'),
    '#description' => t('Edit the memo for an Alliance Corporation.'),
    '#weight' => 0,
  );

  $result = db_query('SELECT memo FROM {eve_api_alliance_list_corporations} WHERE corporationID = :corporationID', array(
    ':corporationID' => $corporation_id,
  ));

  if (!$result->rowCount()) {
    drupal_goto('admin/eve_api/list_corporations');
  }

  $row = $result->fetchAssoc();
  $memo = $row['memo'];

  $form['edit_corporation']['memo'] = array(
    '#type' => 'textarea',
    '#title' => t('Memo'),
    '#description' => t('Some information about the Alliance Corporation.'),
    '#required' => TRUE,
    '#default_value' => check_plain($memo),
    '#weight' => 0,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Edit Corporation'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form_state['corporation_id'] = $corporation_id;
  $form['#validate'] = array('eve_api_admin_edit_corporation_form_validate');
  $form['#submit'] = array('eve_api_admin_edit_corporation_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_edit_corporation_form().
 *
 * @see eve_api_admin_edit_corporation_form_submit()
 */
function eve_api_admin_edit_corporation_form_validate($form, &$form_state) {
  $corporation_id = (int) $form_state['corporation_id'];

  $result = db_query('SELECT corporationID FROM {eve_api_alliance_list_corporations} WHERE corporationID = :corporationID', array(
    ':corporationID' => (int) $corporation_id,
  ));

  if (!$result->rowCount()) {
    form_set_error('', t('Corporation does not exist.'));
  }
}

/**
 * Form submission handler for eve_api_admin_edit_corporation_form().
 *
 * @see eve_api_admin_edit_corporation_form_validate()
 */
function eve_api_admin_edit_corporation_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/eve_api/list_corporations';
  $corporation_id = (int) $form_state['corporation_id'];
  $memo = (string) $form_state['values']['memo'];

  db_update('eve_api_alliance_list_corporations')->fields(array('memo' => $memo))->condition('corporationID', (int) $corporation_id, '=')->execute();

  drupal_set_message(t('Corporation has been updated!'));
}

/**
 * Form constructor for the admin list whitelist form.
 *
 * @see eve_api_admin_list_whitelist_form_validate()
 * @see eve_api_admin_list_whitelist_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_list_whitelist_form($form, &$form_state) {
  $header = array(
    'characterID' => array(
      'data' => t('Character ID'),
      'field' => 'w.characterID',
    ),
    'characterName' => array(
      'data' => t('Character Name'),
      'field' => 'w.characterName',
    ),
    'memo' => array(
      'data' => t('Memo'),
    ),
  );

  $options = $selected = array();

  $result = db_select('eve_api_whitelist', 'w')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('w', array(
      'characterID',
      'characterName',
      'memo',
    ))
    ->limit(20)
    ->orderByHeader($header)
    ->execute();

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      $options[$row->characterID] = array(
        'characterID' => check_plain($row->characterID),
        'characterName' => check_plain($row->characterName),
        'memo' => check_plain($row->memo),
      );
    }
  }

  $form['whitelist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update Whitelist'),
    '#description' => t('Toggle the checkbox to remove a Whitelisted Character.'),
    '#weight' => 0,
  );

  $form['whitelist']['whitelist'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#default_value' => $selected,
    '#empty' => t('No Characters Whitelisted.'),
    '#weight' => 10,
  );

  $form['whitelist']['page'] = array(
    '#theme' => 'pager',
    '#weight' => 20,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Update Whitelist'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_list_whitelist_form_validate');
  $form['#submit'] = array('eve_api_admin_list_whitelist_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_list_whitelist_form().
 *
 * @see eve_api_admin_list_whitelist_form_submit()
 */
function eve_api_admin_list_whitelist_form_validate($form, &$form_state) {
  if (!is_array($form_state['values']['whitelist']) || !count(array_filter((array) $form_state['values']['whitelist']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Form submission handler for eve_api_admin_list_whitelist_form().
 *
 * @see eve_api_admin_list_whitelist_form_validate()
 */
function eve_api_admin_list_whitelist_form_submit($form, &$form_state) {
  $deleted_whitelist = array();
  $deleted = array_filter((array) $form_state['values']['whitelist']);

  foreach ($deleted as $whitelist) {
    $deleted_whitelist[] = (int) $whitelist;
  }

  if (!empty($deleted_whitelist)) {
    db_delete('eve_api_whitelist')->condition('characterID', $deleted_whitelist, 'IN')->execute();

    $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
    $result = db_query('SELECT DISTINCT uid FROM {eve_api_characters} WHERE deleted = 0 AND characterID IN (:characterIDs)', array(':characterIDs' => $deleted_whitelist));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $item) {
        $queue->createItem(array(
          'uid' => $item->uid,
          'runs' => 1,
        ));
      }
    }

    drupal_set_message(t('The Alliance White List has been updated.'));
  }
  else {
    drupal_set_message(t('Nothing changed.'), 'warning');
  }
}

/**
 * Form constructor for the admin add whitelist form.
 *
 * @see eve_api_admin_add_whitelist_form_validate()
 * @see eve_api_admin_add_whitelist_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_add_whitelist_form($form, &$form_state) {
  $form['add_whitelist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Whitelist'),
    '#description' => t('Add characters to the whitelist, the key is not stored here, it is used to retrieve the characters on the API Key and add them to the whitelist. There are times when you will have high profile members join your site, they may have sensitive e-mails or items in locations. With this whitelist you are able to allow the user to register with a very limited and basic API Key, but still provide full functionality on the site. Please provide the following <a href="@url" target="_blank">link</a> to have the new user create a new limited API Key, the user has the option to list all characters, or just a single character.', array('@url' => 'http://community.eveonline.com/support/api-key/CreatePredefined?accessMask=8388680')),
    '#weight' => 0,
  );

  $form['add_whitelist']['keyID'] = array(
    '#type' => 'textfield',
    '#title' => t('Key ID'),
    '#description' => t('Please enter the Key ID.'),
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 15,
    '#weight' => 0,
  );

  $form['add_whitelist']['vCode'] = array(
    '#type' => 'textfield',
    '#title' => t('Verification Code'),
    '#description' => t('Please enter the Verification Code.'),
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 64,
    '#weight' => 10,
  );

  $form['add_whitelist']['memo'] = array(
    '#type' => 'textarea',
    '#title' => t('Memo'),
    '#description' => t('Some information about why you adding to the whitelist.'),
    '#required' => TRUE,
    '#default_value' => '',
    '#weight' => 20,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Add Whitelist'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_add_whitelist_form_validate');
  $form['#submit'] = array('eve_api_admin_add_whitelist_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_add_whitelist_form().
 *
 * @see eve_api_admin_add_whitelist_form_submit()
 */
function eve_api_admin_add_whitelist_form_validate($form, &$form_state) {
  $key_id = (int) $form_state['values']['keyID'];
  $v_code = (string) $form_state['values']['vCode'];

  if (empty($key_id) || empty($v_code) || preg_match('/[^a-z0-9]/i', $v_code) || preg_match('/[^0-9]/', $key_id) || strlen($key_id) > 15 || strlen($v_code) > 64 || strlen($v_code) < 20) {
    form_set_error('keyID', t('Invalid input, please try again.'));
    form_set_error('vCode');
    return;
  }

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  if (isset($characters['error'])) {
    form_set_error('keyID', t('There was an error with the API.'));
    form_set_error('vCode');
  }
  else {
    if ($characters['expires'] || ($characters['accessMask'] & 8388680) != 8388680) {
      form_set_error('keyID', t('Please ensure that only the following boxes are highlighted and selected for the API: (Public Information -> (Characterinfo and FacWarStats), (Private Information) -> (CharacterSheet))'));
      form_set_error('keyID', t('In addition the "Type" drop down box is set to "Character", and that the "No Expiry" checkbox is ticked.'));
      form_set_error('vCode');
    }

    if (!eve_api_verify_blue($characters)) {
      form_set_error('keyID', t('No characters associated with the key are currently blue, please add the characters to the blue standings.'));
      form_set_error('vCode');
    }

    $form_state['values']['characters'] = $characters;
  }
}

/**
 * Form submission handler for eve_api_admin_add_whitelist_form().
 *
 * @see eve_api_admin_add_whitelist_form_validate()
 */
function eve_api_admin_add_whitelist_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/eve_api/list_whitelist';
  $characters = (array) $form_state['values']['characters'];

  foreach ($characters['characters'] as $character) {
    db_merge('eve_api_whitelist')->key(array('characterID' => (int) $character['characterID']))->fields(array(
      'characterID' => (int) $character['characterID'],
      'characterName' => (string) $character['characterName'],
      'memo' => (string) $form_state['values']['memo'],
    ))->execute();
  }

  drupal_set_message(t('Whitelisted Characters successfully added!'));
}

/**
 * Form constructor for the admin list roles whitelist form.
 *
 * @see eve_api_admin_role_ceo_director_form_validate()
 * @see eve_api_admin_role_ceo_director_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_role_ceo_director_form($form, &$form_state) {
  $header = array(
    'characterName' => array(
      'data' => t('Character Name'),
      'field' => 'c.characterName',
    ),
    'corporationName' => array(
      'data' => t('Corporation'),
      'field' => 'c.corporationName',
    ),
    'isCEO' => array(
      'data' => t('CEO'),
      'field' => 'rcd.isCEO',
    ),
    'isDirector' => array(
      'data' => t('Director'),
      'field' => 'rcd.isDirector',
    ),
    'memo' => array(
      'data' => t('Memo'),
    ),
  );

  $options = array();

  $query = db_select('eve_api_role_ceo_director', 'rcd')
    ->extend('PagerDefault')
    ->extend('TableSort');
  $query->join('eve_api_characters', 'c', 'rcd.characterID = c.characterID');
  $result = $query->fields('rcd', array(
        'isCEO',
        'isDirector',
        'memo',
      ))
    ->fields('c', array(
        'characterID',
        'characterName',
        'corporationName',
      ))
    ->limit(20)
    ->orderByHeader($header)
    ->execute();

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      $options[$row->characterID] = array(
        'characterName' => check_plain($row->characterName),
        'corporationName' => check_plain($row->corporationName),
        'isCEO' => ($row->isCEO) ? t('Yes') : t('No'),
        'isDirector' => ($row->isDirector) ? t('Yes') : t('No'),
        'memo' => check_plain($row->memo),
      );
    }
  }

  $form['role_ceo_director'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update CEO/Director Roles'),
    '#description' => t('Toggle the checkbox to remove the  CEO/Director Roles for the Character.'),
    '#weight' => 0,
  );

  $form['role_ceo_director']['role_ceo_director'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No Character CEO/Director Roles.'),
    '#weight' => 10,
  );

  $form['role_ceo_director']['page'] = array(
    '#theme' => 'pager',
    '#weight' => 20,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Update Roles'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_role_ceo_director_form_validate');
  $form['#submit'] = array('eve_api_admin_role_ceo_director_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_role_ceo_director_form().
 *
 * @see eve_api_admin_role_ceo_director_form_submit()
 */
function eve_api_admin_role_ceo_director_form_validate($form, &$form_state) {
  if (!is_array($form_state['values']['role_ceo_director']) || !count(array_filter((array) $form_state['values']['role_ceo_director']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Form submission handler for eve_api_admin_role_ceo_director_form().
 *
 * @see eve_api_admin_role_ceo_director_form_validate()
 */
function eve_api_admin_role_ceo_director_form_submit($form, &$form_state) {
  $deleted_ceo_director = array();
  $deleted = array_filter((array) $form_state['values']['role_ceo_director']);

  foreach ($deleted as $ceo_director) {
    $deleted_ceo_director[] = (int) $ceo_director;
  }

  if (!empty($deleted_ceo_director)) {
    db_delete('eve_api_role_ceo_director')->condition('characterID', $deleted_ceo_director, 'IN')->execute();

    $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
    $result = db_query('SELECT DISTINCT uid FROM {eve_api_characters} WHERE deleted = 0 AND characterID IN (:characterIDs)', array(':characterIDs' => $deleted_ceo_director));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $item) {
        $queue->createItem(array(
          'uid' => $item->uid,
          'runs' => 1,
        ));
      }
    }

    drupal_set_message(t('The Character CEO/Director Roles have been updated.'));
  }
  else {
    drupal_set_message(t('Nothing changed.'), 'warning');
  }
}

/**
 * Form constructor for the admin add roles whitelist form.
 *
 * @see eve_api_admin_add_role_ceo_director_form_validate()
 * @see eve_api_admin_add_role_ceo_director_form_submit()
 *
 * @ingroup forms
 */
function eve_api_admin_add_role_ceo_director_form($form, &$form_state) {
  $form['add_role_ceo_director'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add/Update CEO/Director Roles'),
    '#description' => t('Add characters to the CEO/Director roles, specify if they are a CEO or a Director.'),
    '#weight' => 0,
  );

  $form['add_role_ceo_director']['characterName'] = array(
    '#type' => 'textfield',
    '#title' => t('Character Name'),
    '#description' => t('Please enter the Character Name, this character must already exist in the database.'),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 50,
    '#weight' => 0,
  );

  $form['add_role_ceo_director']['isCEO'] = array(
    '#type' => 'checkbox',
    '#title' => t('CEO'),
    '#description' => t('Manually add the role CEO to this character.'),
    '#weight' => 10,
  );

  $form['add_role_ceo_director']['isDirector'] = array(
    '#type' => 'checkbox',
    '#title' => t('Director'),
    '#description' => t('Manually add the role Director to this character.'),
    '#weight' => 20,
  );

  $form['add_role_ceo_director']['memo'] = array(
    '#type' => 'textarea',
    '#title' => t('Memo'),
    '#description' => t('Some information about why you manually adding the roles to the character.'),
    '#required' => TRUE,
    '#default_value' => '',
    '#weight' => 30,
  );

  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Add CEO/Director Roles'),
  );

  if (variable_get('eve_api_first_run', FALSE) && !variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Please wait until the cron tasks have pulled the Alliance API Info.'));
    $form['submit_button']['#disabled'] = TRUE;
  }

  $form['#validate'] = array('eve_api_admin_add_role_ceo_director_form_validate');
  $form['#submit'] = array('eve_api_admin_add_role_ceo_director_form_submit');

  return $form;
}

/**
 * Form validation handler for eve_api_admin_add_role_ceo_director_form().
 *
 * @see eve_api_admin_add_role_ceo_director_form_submit()
 */
function eve_api_admin_add_role_ceo_director_form_validate($form, &$form_state) {
  $character_name = (string) $form_state['values']['characterName'];
  $is_ceo = ($form_state['values']['isCEO'] == TRUE) ? TRUE : FALSE;
  $is_director = ($form_state['values']['isDirector'] == TRUE) ? TRUE : FALSE;

  if (!$is_ceo && !$is_director) {
    form_set_error('isCEO', t('At least one option must be selected.'));
    form_set_error('isDirector');
  }

  $chars = eve_api_get_basic_character_info($character_name);

  if (!$chars) {
    form_set_error('characterName', t('The Character does not exist, please try again.'));
  }
}

/**
 * Form submission handler for eve_api_admin_add_role_ceo_director_form().
 *
 * @see eve_api_admin_add_role_ceo_director_form_validate()
 */
function eve_api_admin_add_role_ceo_director_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/eve_api/role_ceo_director';

  $character_name = (string) $form_state['values']['characterName'];
  $is_ceo = ($form_state['values']['isCEO'] == TRUE) ? 1 : 0;
  $is_director = ($form_state['values']['isDirector'] == TRUE) ? 1 : 0;

  $character = eve_api_get_basic_character_info($character_name);

  $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
  $queue->createItem(array(
    'uid' => $character['uid'],
    'runs' => 1,
  ));

  db_merge('eve_api_role_ceo_director')->key(array('characterID' => (int) $character['characterID']))->fields(array(
    'characterID' => (int) $character['characterID'],
    'isCEO' => $is_ceo,
    'isDirector' => $is_director,
    'memo' => (string) $form_state['values']['memo'],
  ))->execute();

  drupal_set_message(t('CEO/Director Roles for Character successfully added!'));
}
