<?php

/**
 * @file
 * Functionality and helper functions for feature flags administration.
 */

/**
 * Help hook for main overview page
 */
function featureflags_help($path, $arg) {
  switch ($path) {
    case 'admin/config/development/featureflags':
      return '<p>' . t('Feature Flags allow site administrators to slowly roll out new features on a website based on user roles, user names, or random percentages.') . '</p>';
  }
}

/**
 * Main listing for admin management
 */
function featureflags_admin_overview() {
  $header = array(
    array('data' => t('Flag Name'), 'field' => 'name', 'sort' => 'ASC'),
    array('data' => t('Machine Name'), 'field' => 'flag_name'),
    array('data' => t('Access Type')),
    array('data' => t('Status'), 'field' => 'status'),
    array('data' => t('Operations'), 'colspan' => 2)
  );

  $select = db_select('featureflags', 'f')->extend('TableSort');
  $flags = $select->fields('f', array('name', 'flag_name', 'status'))->orderByHeader($header)->execute();

  $rows = array();
  foreach ($flags as $flag) {
    $row = array();
    $row[] = l($flag->name, 'admin/config/development/featureflags/edit/' . $flag->flag_name);
    $row[] = $flag->flag_name;

    // display the existing rules enabled
    $options = array(
      'role' => RULE_ROLEID,
      'user' => RULE_USERID,
      'percentage' => RULE_PERCENTAGE,
    );
    $access_type = '';

    if (!empty($flag->flag_name)) {
      foreach ($options as $key => $rule) {
        $rules = featureflags_rule_loadmultiple_bytype($flag->flag_name, $rule);
        if (!empty($rules)) {
          $access_type .= $key . ', ';
        }
      }
    }

    $row[] = substr($access_type, 0, -2);
    $row[] = ($flag->status)?"Enabled":"Disabled";
    $row[] = l(t('edit'), 'admin/config/development/featureflags/edit/' . $flag->flag_name);
    $row[] = l(t('delete'), 'admin/config/development/featureflags/delete/' . $flag->flag_name);
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array(
      'colspan' => 5,
      'data' => t('There are currently no feature flags. <a href="!url">Add a new one</a>.', array('!url' => url('admin/config/development/featureflags/add'))),
    ));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Edit and new flag form
 */
function featureflags_admin_editflag($form, &$form_state, $editflag = NULL) {
  $flag = featureflags_load($editflag);
  $form = array();

  // set the title appropriately if we're editing or adding a new flag
  if ($flag != NULL) {
    drupal_set_title(t('Edit') . ' <em>' . $flag->name . '</em>', PASS_THROUGH);
  }
  else{
    drupal_set_title(t('Add Feature Flag'));
  }

  $form['flag_edit'] = array(
    '#type' => 'hidden',
    '#value' => $editflag,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Flag Name'),
    '#description' => t('User friendly flag name'),
    '#required' => TRUE,
    '#default_value' => !empty($flag->name)?$flag->name:'',
  );

  $form['flag_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Flag Machine Name'),
    '#description' => t('A unique name to reference the feature flag. It must only contain lowercase letters, numbers and underscores.'),
    '#machine_name' => array(
      'exists' => 'featureflags_flag_name_exists',
      'source' => array('name'),
      'label' => t('Machine Name'),
      'replace_pattern' => '[^a-z0-9_]+',
      'replace' => '_',
    ),
    '#default_value' => !empty($flag->flag_name)?$flag->flag_name:'',
    '#disabled' => !empty($flag->flag_name)?TRUE:FALSE, // disable editing once the rule is created
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Flag Description'),
    '#rows' => 5,
    '#default_value' => !empty($flag->description)?$flag->description:'',
  );

  $form['adv_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['flag_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
    '#group' => 'adv_settings',
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'vertical_tabs_example') . '/vertical_tabs_example.js',
      ),
    ),
  );

  $form['flag_settings']['status'] = array(
    '#type' => 'select',
    '#title' => t('Flag Status'),
    '#description' => t('This setting overrides any individual access settings'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => !empty($flag->status)?$flag->status:0,
  );

  // build the default options for the primary checkboxes
  $options = array(
    'role' => RULE_ROLEID,
    'user' => RULE_USERID,
    'percentage' => RULE_PERCENTAGE,
  );
  $default_value = array();
  if (!empty($flag->flag_name)) {
    foreach ($options as $key => $rule) {
      $rules = featureflags_rule_loadmultiple_bytype($flag->flag_name, $rule);
      if (!empty($rules)) {
        $default_value[] = $key;
      }
    }
  }

  $form['flag_settings']['access_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Limit Access By'),
    '#options' => array(
      'role' => t('Role'),
      'user' => t('User Name'),
      'percentage' => t('Percentage')
    ),
    '#description' => t('A user will be granted access if they match ANY of the conditions'),
    '#default_value' => $default_value,
  );

  $form['access_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access Settings'),
    '#description' => t('All enabled access configuration options will appear here. If multiple access options are enabled, they will grant access if ANY of the conditions apply.'),
    '#group' => 'adv_settings',
    '#collapsible' => FALSE,
    '#attached' => array(
      'js' => array(
        'vertical-tabs' => drupal_get_path('module', 'vertical_tabs_example') . '/vertical_tabs_example.js',
      ),
    ),
  );

  // build the default roles checkboes
  $checked_roles = array();
  if (!empty($flag->flag_name)) {
    $roles = featureflags_rule_loadmultiple_bytype($flag->flag_name, RULE_ROLEID);
    foreach ($roles as $role) {
      $checked_roles[] = $role->rule_data;
    }
  }

  $form['access_settings']['limit_role'] = array(
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#title' => t('Roles'),
    '#states' => array(
      'invisible' => array(
        'input[name="access_options[role]"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => $checked_roles,
  );

  // build the default user textbox value
  $userdefault_value = '';
  if (!empty($flag->flag_name)) {
    $userrules = featureflags_rule_loadmultiple_bytype($flag->flag_name, RULE_USERID);
    foreach ($userrules as $rule) {
      $userdefault_value .= $rule->rule_data . ', ';
    }
    $userdefault_value = substr($userdefault_value, 0, -2);
  }

  $form['access_settings']['limit_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Users'),
    '#description' => t('Comma separated list of usernames'),
    '#autocomplete_path' => 'user/autocomplete/multiple',
    '#states' => array(
      'invisible' => array(
        'input[name="access_options[user]"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => $userdefault_value,
  );

  // get the percentage
  $percentage = NULL;
  if (!empty($flag->flag_name)) {
    $percentage = featureflags_rule_loadmultiple_bytype($flag->flag_name, RULE_PERCENTAGE);
    if (!empty($percentage)) {
      $percentage = $percentage[0]->rule_data*100; // we know that only one percentage value will ever exist so this is ok
    }
  }

  $form['access_settings']['limit_percentage'] = array(
    '#type' => 'textfield',
    '#title' => t('Percentage'),
    '#size' => 3,
    '#max_length' => 3,
    '#field_suffix' => '%',
    '#states' => array(
      'invisible' => array(
        'input[name="access_options[percentage]"]' => array('checked' => FALSE),
      ),
    ),
    '#default_value' => $percentage,
    '#description' => t('This percentage is persistent for both authenticated and anonymous users. If a user falls in the percentage applied here, they will always be granted access regardless if the value is increased. Anonymous users are persistent based on their IP address.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Feature Flag'),
  );

  // if this is editing, provide the cancel link
  if ($flag != NULL) {
    $form['submit']['#suffix'] = l(t('Cancel'), 'admin/config/development/featureflags');
  }

  return $form;
}

/**
 * Submit handler for edit flag. Edits and updates rules as indicated by the user.
 */
function featureflags_admin_editflag_submit($form, &$form_state) {
  $flag = array();
  $flag_name = $form_state['values']['flag_name'];

  $flag['flag_name'] = $flag_name;
  $flag['name'] = $form_state['values']['name'];
  $flag['description'] = $form_state['values']['description'];
  $flag['status'] = $form_state['values']['status'];

  featureflags_save($flag);

  // set the user role values
  $rolerules = featureflags_rule_loadmultiple_bytype($flag_name, RULE_ROLEID);
  if (isset($form_state['groups']['adv_settings'][0]['access_options']['#value']['role'])) {
    foreach ($form_state['values']['limit_role'] as $key => $role) { // for each role checkbox
      if ($key == $role) { // they checked this role, or it was pre-checked
        $exists = FALSE;
        foreach ($rolerules as $rule) {
          if ($rule->rule_data == $role) {
            $exists = TRUE;
          }
        }
        if (!$exists) {
          $rule = array(
            'flag_name' => $flag_name,
            'rule_type' => RULE_ROLEID,
            'rule_data' => $role,
          );
          featureflags_rule_save($rule);
        }
      }
      else{ // they unchecked this role
        foreach ($rolerules as $rule) {
          if ($rule->rule_data == $key) {
            featureflags_rule_delete($rule->rid);
          }
        }
      }
    }
  }
  else{
     // they unchecked role from the maain page, so delete everything
    foreach ($rolerules as $rule) {
      featureflags_rule_delete($rule->rid);
    }
  }

  // set the user name values
  $userrules = featureflags_rule_loadmultiple_bytype($flag_name, RULE_USERID);
  if (isset($form_state['groups']['adv_settings'][0]['access_options']['#value']['user']) && !empty($form_state['values']['limit_user'])) {
    $names = $form_state['values']['limit_user'];
    $names = explode(', ', $form_state['values']['limit_user']);
    foreach ($names as $username) {
      $exists = FALSE;
      foreach ($userrules as $rule) {
        if ($rule->rule_data == $username) {
          $exists = TRUE;
        }
      }
      if (!$exists) {
        $rule = array(
          'flag_name' => $flag_name,
          'rule_type' => RULE_USERID,
          'rule_data' => $username,
        );
        featureflags_rule_save($rule);
      }
    }
    // now delete the rest of the users on this flag
    foreach ($userrules as $rule) {
      if (!in_array($rule->rule_data, $names)) {
        featureflags_rule_delete($rule->rid);
      }
    }
  }
  else{
     // they unchecked user, so delete everything
    foreach ($userrules as $rule) {
      featureflags_rule_delete($rule->rid);
    }
  }

  // set the percentage value
  $percentrule = featureflags_rule_loadmultiple_bytype($flag_name, RULE_PERCENTAGE);
  if (isset($form_state['groups']['adv_settings'][0]['access_options']['#value']['percentage']) && !empty($form_state['values']['limit_percentage'])) {
    $percentage = $form_state['values']['limit_percentage']/100;
    $rule = array(
      'flag_name' => $flag_name,
      'rule_type' => RULE_PERCENTAGE,
      'rule_data' => $percentage,
    );
    if (!empty($percentrule)) {
      $rule['rid'] = $percentrule[0]->rid; // since only one percentage will exist, this is OK
    }
    featureflags_rule_save($rule);
  }
  else{
     // they unchecked percentage, so remove it
    foreach ($percentrule as $rule) {
      featureflags_rule_delete($rule->rid);
    }
  }

  drupal_set_message(filter_xss('<em>' . $flag['name'] . '</em>' . t('saved')));
  drupal_goto('admin/config/development/featureflags');
}

/**
 * Check if a flag name exists. Used in the machine_name edit form
 */
function featureflags_flag_name_exists($flag_name) {
  $flag = featureflags_load($flag_name);
  if (!empty($flag)) {
    return TRUE;
  }
  else{
    return FALSE;
  }
}

/**
 * Autocomplete for multiple user names. Used in the user role access rule.
 * Based off of multiple_users_autocomplete
 */
function featureflags_autocomplete_multiple($string = '') {
  $array = drupal_explode_tags($string);

  $last_string = trim(array_pop($array));
  $matches = array();

  if ($last_string != '') {
    $result = db_select('users')->fields('users', array('name'))->condition('name', db_like($last_string) . '%', 'LIKE')->range(0, 10)->execute();
    $prefix = count($array) ? implode(', ', $array) . ', ' : '';
    while ($user = $result->fetchObject()) {
      $n = $user->name;
      $matches[$prefix . $n] = check_plain($user->name);
    }
  }

  drupal_json_output($matches);
}

/**
 * Flag delete confirmation form
 */
function featureflags_admin_deleteflag($form, &$form_state, $flag_name) {
  $flag = featureflags_load($flag_name);

  drupal_set_title(t('Are you sure you want to delete') . '<em>' . $flag->name . '</em>?', PASS_THROUGH);

  $form = array();

  $form['notice'] = array(
    '#markup' => '<p>' . t('This action cannot be undone.') . '</p>',
  );

  $form['feature_flag'] = array(
    '#type' => 'hidden',
    '#value' => $flag_name,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#suffix' => l(t('Cancel'), 'admin/config/development/featureflags'),
  );

  return $form;
}

/**
 * Flag delete submit handler
 */
function featureflags_admin_deleteflag_submit($form, &$form_state) {
  $flag = featureflags_load($form_state['values']['feature_flag']);;
  featureflags_delete($form_state['values']['feature_flag']);
  drupal_set_message(filter_xss(t('Feature flag') . '<em>' . $flag->name . '</em> ' . t('has been deleted')));
  drupal_goto('admin/config/development/featureflags');
}