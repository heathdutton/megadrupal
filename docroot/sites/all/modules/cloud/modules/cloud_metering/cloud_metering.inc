<?php

/**
 * @file
 * Contains private and non-hook methods used by Cloud Metering module
 * Works with Cloud Metering module
 *
 * Copyright (c) 2011 DOCOMO Innovations, Inc.
 *
 */

/**
 *  List all cloud instances for metering configuration
 */
function cloud_metering_instances_list() {
  $clouds = cloud_get_all_clouds();
  $all_instances = array();
  
  // get all the instances from the database
  foreach ($clouds as $cloud) {
    $instances = cloud_get_instances($cloud);
    if (is_array($instances)) {
      $all_instances = array_merge($all_instances, $instances);
    }
  }

  $rows = array();
  ksort($all_instances);

  foreach ($all_instances as $instance) {
    $cloud_context   = $instance['cloud_context'];
    $instance_id     = $instance['instance_id'];
    $lock_status     = cloud_perform_action('', 'get_instance_lock_status', $cloud_context, array('instance_id' => $instance_id));
    $instance_locked = ( $lock_status == 'Lock' ? TRUE : FALSE );
    $cloud_menu_path = cloud_get_menu_path($cloud_context);
    $budget_data = _cloud_metering_get_instance_budget($instance['cloud_context'], $instance['instance_id']);
    $hostname = $instance['hostname'];
    
    $public_dns = substr($hostname, 0, strpos($hostname,   '.'));
    if (strlen($public_dns) > 3) {
      $public_dns = substr($hostname, 0, strpos($hostname,   '.')) . '...';
    }
    else {
      $public_dns = $hostname;
    }

    if (!empty($public_dns)) {
      $public_dns = l($public_dns, 'http://' . $hostname, array('attributes' => array('target' => '_blank')));
    }
    
    // if instance is locked, do not let users edit
    if ($instance_locked) {
      $link = l(t('Instance locked'), $cloud_menu_path . '/instances/describe/' . $instance_id . '/info', array('query' => array('instance_id' => urlencode($instance_id)), 'fragment' => 'edit-lock-text'));
    }
    else {
      $link = l(t('Edit Budget'), 'admin/config/cloud_metering/threshold/' . $cloud_context . '/' . $instance_id, array('query' => drupal_get_destination()));
    }

    $rows[] = array(
      l($instance['name'], $cloud_menu_path . '/instances/describe/' . $instance_id . '/info', array('query' => array('instance_id' => urlencode($instance_id)))),
      $instance['instance_id'],
      $public_dns,
      ucwords($instance['state']),
      number_format($budget_data['initial_budget'], 2, '.', ','),
      isset($budget_data['cost_threshold']) ? $budget_data['cost_threshold'] : 0,
      $link,
    );
  }

  $headers = array(t('Name'         ),
                   t('Id'           ),
                   t('Host Name'    ),
                   t('State'        ),
                   t('Budget ($)'   ),
                   t('Threshold (%)'),
                    t('Operations') );

  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 * Add/Edit Group and Users for Initial Budget
 */
function cloud_metering_user_list($form_id, $form_state, $rid, $operation) {

  $groups = _cloud_metering_get_group_details($rid);
  $group_details = $groups[$rid];

  if (!user_access('configure user budget')) {
    drupal_set_message(t('You do not have permission to Edit the Group Budget or Group ID is invalid!'), 'error');
  }
  elseif (!empty($gid) && empty($group_details)) {
    drupal_set_message(t('You do not have permission to Edit the Group Budget or Group ID is invalid!'), 'error');
  }
  else {
    // initialize some defaults
    $group_budget_value = 0;
    $admin_users = array();
    $group_threshold_value = 0;
    
    $form = array();
    $form['wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => '',
      '#weight' => -1,
    );
    $form['wrapper']['group_name'] = array(
      '#type' => 'item',
      '#title' => t('Group Name: !group_name', array('!group_name' => $group_details['name'])),
      '#value' => $group_details['name'],
    );
    
    $group_budget = _cloud_metering_get_group_budget('', $rid);
    if (!empty($group_budget)) {
      $group_budget_value = $group_budget[$rid]['group_budget'];
      $admin_users = unserialize($group_budget[$rid]['user_data']);
      $group_threshold_value = $group_budget[$rid]['threshold'];
    }
  
    $form['wrapper']['group_budget'] = array(
      '#type' => 'textfield',
      '#title' => t('Group Budget ($)'),
      '#default_value' => $group_budget_value,
      '#description' => t('Enter the Group Budget Value. Put only the numeric value without a currency symbol.'),
      '#required' => TRUE,
      '#size' => 20,
    );
  
    $form['wrapper']['group_threshold'] = array(
      '#type' => 'textfield',
      '#title' => t('Group Threshold (%)'),
      '#default_value' => $group_threshold_value,
      '#description' => t('Enter the Group Threshold limit. Put only the numeric value without a percentage symbol.'),
      '#required' => TRUE,
      '#size' => 20,
    );
  
    //Get all the users for specific role  
    $users = _cloud_metering_get_users_list_by_role($rid);
    $val = array();
    
    foreach ($users as $key => $value) {
      $user_budget_amount = _cloud_metering_get_user_budget($key);
      $form[$key]['name'] = array('#markup' => $value['name']);
      $form[$key]['mail'] = array('#markup' => $value['mail']);
      $form[$key]['user_budget_' . $key] = array(
        '#type' => 'textfield',
        '#default_value' => $user_budget_amount,
        '#size' => '20',
      );
      $form[$key]['user_budget_value_' . $key] = array(
        '#type' => 'hidden',
        '#value' => $user_budget_amount,
      );
      //Construct the select boxes values
      $val[$key] = '';
    }
  
    $form['process_user'] = array(
      '#type' => 'checkboxes',
      '#options' => $val,
      '#default_value' => $admin_users,
    );
  
    $form['group_id'] = array(
      '#type' => 'hidden',
      '#value' => $rid,
    );
  
    $form['group_budget_value'] = array(
      '#type' => 'hidden',
      '#value' => $group_budget_value,
    );
  
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Update',
      '#submit' => array('_cloud_metering_user_list_submit'),
      '#validate' => array('_cloud_metering_user_list_validate'),
      '#weight' => 2,
    );
  
    return $form;
  }
}

/**
 *  Theme the user listing
 */
function theme_cloud_metering_user_list($form) {
  $form = $form['form'];
  $rows = array();

  foreach (element_children($form) as $key) {
    $row = array();
    if (isset($form[$key]['mail'])) {
      $row[] = array('data' => drupal_render($form['process_user'][$key]));
      $row[] = array('data' => drupal_render($form[$key]['name']));
      $row[] = array('data' => drupal_render($form[$key]['mail']));
      $row[] = array('data' => drupal_render($form[$key]['user_budget_' . $key]));
      $rows[] = $row;
    }
  }

  // Individual table headers.
  $header = array(
    t('Group Admin'),
    t('Name'),
    t('Email'),
    t('User Budget ($)'),
  );

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Validate the group details form
 */
function _cloud_metering_user_list_validate($form, &$form_state) {

  $values = $form_state['values'];
  $group_threshold = $values['group_threshold'];
  $group_budget = $values['group_budget'];
  $group_budget_value = $values['group_budget_value'];
  $group_id = $values['group_id'];
  $operation = $values['submit'];

  if (!is_numeric($group_budget)) {
    form_set_error('group_budget', t('Please enter only numeric values only'));
  }
  elseif ($group_budget <= 0) {
    form_set_error('group_budget', t('Please enter values greater than zero'));
  }

  if (!is_numeric($group_threshold)) {
    form_set_error('group_threshold', t('Please enter only numeric values only'));
  }
  elseif ($group_threshold <= 0) {
    form_set_error('group_threshold', t('Please enter values greater than zero'));
  }
  elseif ($group_threshold > 100) {
    form_set_error('group_threshold', t('Group Threshold percentage cannot be more than 100%'));
  }
  
  // make sure all the calculations are correct
  if ($operation == 'Update') {
    $group_user_details = _cloud_metering_get_users_list_by_role($group_id);
    $total_new_budget = 0;
    $total_user_budget = 0;
    
    foreach ($group_user_details as $key => $value) {
      if ($value['rid'] == $group_id) {
        $get_budget = _cloud_metering_get_user_budget($key);
      }
      $new_budget_value = $values['user_budget_' . $key];
      $total_user_budget += $get_budget;
      $total_new_budget += $new_budget_value;
    }

    if ($group_budget < $total_user_budget || $group_budget < $total_new_budget) {
      form_set_error('group_budget', t('Group Budget cannot be less than the Total Budget allocated to the Users of the Group.'));
    }
  }
}

/**
 * Submit the new Group Details
 */
function _cloud_metering_user_list_submit($form, &$form_state) {

  $values = $form_state['values'];
  $group_id = $values['group_id'];
  $group_threshold = $values['group_threshold'];
  $group_budget = $values['group_budget'];
  $group_budget_value = $values['group_budget_value'];
  $user_data = serialize($values['process_user']);

  //Get all users for the Group
  $all_users = _cloud_metering_get_users_list_by_role($group_id);
  if (!$group_budget_value) {
    if (_cloud_metering_add_group_budget($group_id, $group_threshold, $group_budget, $user_data)) {
      //Add the User Budget values
      foreach ($all_users as $key => $budget) {
        $user_budget = $values['user_budget_' . $key];
        _cloud_metering_add_user_budget($key, $group_id, $group_threshold, $user_budget);
      }
      drupal_set_message(t('Group budget settings saved.'));
    }
  }
  else {
    _cloud_metering_update_group_budget($group_id, $group_threshold, $group_budget, $user_data);
    //Update user budget values
    foreach ($all_users as $key => $budget) {
      $user_budget = $values['user_budget_' . $key];
      // check to see if the update exists
      if (_cloud_metering_get_user_budget($key)) {
        _cloud_metering_update_user_budget($key, $group_id, $user_budget);
      }
      else {
        // add a new budget line
        _cloud_metering_add_user_budget($key, $group_id, $group_threshold, $user_budget);
      }
    }
    drupal_set_message(t('Group details are updated.'));
  }
  $form_state['redirect'] = 'admin/config/cloud_metering/group_settings';
}

/**
 * Group Budget Configuration Screen
 */
function cloud_metering_group_budget_configure() {

  $headers = array(
    t('Group Role'),
    t('Group Budget ($)'),
    t('Threshold (%)'),
    t('Admin User'),
    t('Group Settings'),
  );

  $rows = array();
  $groups = _cloud_metering_get_group_details();
  // build the group listing table
  foreach ($groups as $g) {
    $row = array();
    $group_budget = _cloud_metering_get_group_budget('', $g['rid']);
    $budget_value     = 0;
    $threshold_value  = 0;
    $group_admin_data = array();
    
    if (!empty($group_budget) && isset($group_budget[$g['rid']])) {
      $budget_value     = $group_budget[$g['rid']]['group_budget'];
      $threshold_value  = $group_budget[$g['rid']]['threshold'];
      $group_admin_data = unserialize($group_budget[$g['rid']]['user_data']);
    }
    
    $row[] = $g['name'];
    $row[] = ($budget_value) ? number_format($budget_value, 2, '.', ',') : 0;
    $row[] = ($budget_value) ? number_format($threshold_value, 0) : 0;
    
    // Admin User Data
    $display_user_data = '';
    if (!empty($group_admin_data)) {
      foreach ($group_admin_data as $key => $user) {
        if ($user > 0) {
          $user_data = user_load($user);
          $display_user_data .= $user_data->mail . '<br/>';
        }
      }
    }
    else {
      $display_user_data = t('N/A');
    }
    $row[] = $display_user_data;
    $row[] = user_access('configure group budget') ? l(t('Edit'), 'admin/config/cloud_metering/group_settings/' . $g['rid'] . '/edit' ) : '--';

    $rows[] = $row;
  }
  return theme('table', array('header' => $headers, 'rows' => $rows));
}

/**
 *  Set the cost threshold for the given instance
 *  @param string $cloud_context The cloud context
 *  @param string $instance_id The instance Id for which to set the threshold
 *  @return array The form array
 */
function cloud_metering_set_threshold($form, $form_state, $cloud_context, $instance_id) {
  // Check whether this instance exists
  if (!cloud_metering_get_instance($cloud_context, $instance_id)) {
    drupal_set_message(t('@instance_id is not a valid instance.', array('@instance_id' => $instance_id)), 'warning');
    drupal_goto('admin/config/cloud_metering');
  }
  // Check instance lock status
  $lock_status = cloud_perform_action('', 'get_instance_lock_status', $cloud_context, array('instance_id' => $instance_id));
  $locked      = ($lock_status == 'Lock' ? TRUE : FALSE);
  if ($locked) {
    drupal_set_message(t('@instance_id is a locked instance.', array('@instance_id' => $instance_id)));
    drupal_goto('admin/config/cloud_metering');
  }
  // load the instance budget data  
  $budget_data = _cloud_metering_get_instance_budget($cloud_context, $instance_id);
  $form = array();
  $form['wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instance Budget'),
    '#description' => t('Set the budget and threshold for this instance'),
  );
  $form['wrapper']['initial_budget'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial Budget ($) for @instance_id', array('@instance_id' => $instance_id)),
    '#description' => t('Enter Zero(0) for unlimited budget.'),
    '#default_value' => $budget_data['initial_budget'],
    '#size' => 20,
  );
  $form['wrapper']['cost_threshold'] = array(
    '#title' => t('Threshold (%) for @instance_id', array('@instance_id' => $instance_id)),
    '#description' => t('The cost threshold percentage. Threshold value will be percent of the initial budget.'),
    '#type' => 'textfield',
    '#default_value' => $budget_data['cost_threshold'],
    '#size' => 20,
  );
  $form['wrapper']['terminate'] = array(
    '#title' => t('Terminate instance if budget is surpassed'),
    '#description' => t('If checked, the instance will be automatically terminated if the budget is surpassed'),
    '#type' => 'checkbox',
    '#default_value' => $budget_data['terminate'],
    '#disabled' => $locked,
  );
  $form['wrapper']['cloud_context'] = array(
    '#type' => 'value',
    '#value' => $cloud_context,
  );
  $form['wrapper']['instance_id'] = array(
    '#type' => 'value',
    '#value' => $instance_id,
  );
  $form['wrapper']['submit'] = array(
    '#type' => 'submit',
    '#validate' => array('_cloud_metering_validate_template_budget'),
    '#submit' => array('_cloud_metering_update_instance_budget'),
    '#value' => t('Save'),
  );
  return $form;
}

/**
 *  Validation handler for cloud_metering_set_threshold
 *  @param array $form Form object
 *  @param array $form_state Form values
 */
function cloud_metering_set_threshold_validate($form, &$form_state) {
  // Check the entered threshold value
  $threshold = $form_state['values']['cost_threshold'];
  if (!is_numeric($threshold)) {
    form_set_error('threshold', t('Please enter a numeric value for the threshold'));
  }
  if (is_numeric($threshold) && $threshold < 0) {
    form_set_error('threshold', t('The threshold value should not be less than 0'));
  }

  $initial_budget  = $form_state['values']['initial_budget'];
  if (!is_numeric($initial_budget)) {
    form_set_error('initial_budget', t('Please enter a numeric value in Initial Budget'));
  }
  if (is_numeric($initial_budget) && $initial_budget < 0) {
    form_set_error('initial_budget', t('The Initial Budget value should not be less than 0'));
  }
}

/**
 * System setting form for mail settings
 */
function cloud_metering_mail_settings($form, $form_state) {
  $form = array();
  $description = t('Customize the format of the mail that will be sent whenever an instance crosses the cost threshold.');
  // help message for tokens that are available
  $instance_token_help = t('Available variables are: [site:name], [site:url], [cloud_metering_instance:username], [cloud_metering_instance:instance-id], [cloud_metering_instance:instance-name] and [cloud_metering_instance:terminate-url].');
  $user_token_help = t('Available variables are: [site:name], [site:url], [cloud_metering_user:username], [cloud_metering_user:user_cost] and [cloud_metering_user:user_budget]');
  $group_token_help =t('Available variables are: [site:name], [site:url], [cloud_metering_group:username], [cloud_metering_group:group_budget], [cloud_metering_group:group_cost] and [cloud_metering_group:group_threshold]');
  
  $form['wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mail Settings'),
    '#description' => t('The metering module sends out notification mails when a cloud instance\'s cost has reached the budget threshold or when an instance\'s cost has gone past the budget allocated and needs to be terminated. The format of the mails can be configured using the following forms.'),
    '#collapsible' => FALSE,
  );
  $form['wrapper']['threshold_mails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instance cost has crossed the threshold'),
    '#description' => $description . '<br>' . $instance_token_help,
    '#collapsible' => TRUE,
  );
  $form['wrapper']['threshold_mails']['cloud_metering_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => CLOUD_METERING_DEFAULT_THRESHOLD_MAIL_SUBJECT,
  );
  $form['wrapper']['threshold_mails']['cloud_metering_mail_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => CLOUD_METERING_DEFAULT_THRESHOLD_MAIL_BODY,
  );
  $form['wrapper']['terminate_warning_mails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instance cost has crossed the intended budget.'),
    '#description' => '<strong>' . t('This is a warning email telling the user to terminate the instance.') . '</strong><br>' . $description . '<br>' . $instance_token_help,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wrapper']['terminate_warning_mails']['cloud_metering_terminate_warning_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => CLOUD_METERING_DEFAULT_TERMINATE_WARNING_MAIL_SUBJECT,
  );
  $form['wrapper']['terminate_warning_mails']['cloud_metering_terminate_warning_mail_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => CLOUD_METERING_DEFAULT_TERMINATE_WARNING_MAIL_BODY,
  );
  
  $form['wrapper']['terminate_mails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instance cost has crossed budget.'),
    '#description' => '<strong> ' . t('This is a termination email informing users the instance will be terminated') . '</strong><br>' . $description . '<br>' . $instance_token_help,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wrapper']['terminate_mails']['cloud_metering_terminate_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => CLOUD_METERING_DEFAULT_TERMINATE_MAIL_SUBJECT,
  );
  $form['wrapper']['terminate_mails']['cloud_metering_terminate_mail_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => CLOUD_METERING_DEFAULT_TERMINATE_MAIL_BODY,
  );
  $form['wrapper']['user_mails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Total cost of all instances of a user has crossed the budget'),
    '#description' => $description . '<br>' . $user_token_help,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wrapper']['user_mails']['cloud_metering_user_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => CLOUD_METERING_DEFAULT_USER_MAIL_SUBJECT,
  );
  $form['wrapper']['user_mails']['cloud_metering_user_mail_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => CLOUD_METERING_DEFAULT_USER_MAIL_BODY,
  );
  $form['wrapper']['group_mails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Total cost of all instances of a group has crossed the budget'),
    '#description' => $description . '<br>' . $group_token_help,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wrapper']['group_mails']['cloud_metering_group_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => CLOUD_METERING_DEFAULT_GROUP_MAIL_SUBJECT,
  );

  $form['wrapper']['group_mails']['cloud_metering_group_mail_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => CLOUD_METERING_DEFAULT_GROUP_MAIL_BODY,
  );
  return system_settings_form($form);
}

/**
 *  Notify a user that their threshold has hit
 */
function cloud_metering_notify_instance_users($instances) {
  foreach ($instances as $instance) {
    $cloud_context = $instance['cloud_context'];
    $instance_id   = $instance['instance_id'];
    $instance_name = $instance['name'];

    // Check if a notification has already been sent for this instance
    // send out threshold mail every hour
    if (!cloud_metering_notification_sent('threshold', $cloud_context, $instance_id, 0, 0, CLOUD_METERING_THRESHOLD_MAIL_INTERVAL)) {
      // Send a notification
      $user = _cloud_metering_get_instance_user($instance);
      $mail_result = cloud_metering_send_mail($user->uid, 'threshold_mail', array('user' => $user, 'cloud_context' => $cloud_context, 'instance_id' => $instance_id, 'instance_name' => $instance_name));
      if ($mail_result['result']) {
        cloud_metering_save_notification_info('threshold', $cloud_context, $instance_id);
      }
      else {
        _cloud_metering_activity_audit(t('Failed to send notification mail for instance %instance_id', array('%instance_id' => $instance_id)), 'user_activity', '', WATCHDOG_ERROR);
      }
    }
  }
}

/**
 *  Terminate instances whose costs have gone past allocated budget
 *  @param String maxed_instances
 */
function cloud_metering_terminate_budget_crossed_instances($maxed_instances) {

  foreach ($maxed_instances as $instance) {
    if ($instance['state'] == 'running') {
      // Backup->Detach->Terminate the instance
      $cloud_context = $instance['cloud_context'];
      $instance_id   = $instance['instance_id'];

      $instance_volumes = cloud_get_instance_volumes($cloud_context, $instance_id);
      $volumes          = array();
      
      // look at the configurable flag. 
      $budget = _cloud_metering_get_instance_budget($cloud_context, $instance_id);
      if ($budget['terminate'] == TRUE) {
        // if only one volume and if it is the root volume, it can't be detached
        if (is_array($instance_volumes) && sizeof($instance_volumes) > 0) {
          foreach ($instance_volumes as $volume_id => $volume) {
            $volumes[$volume_id] = array();
          }
          // Initiate a backup task
          _cloud_metering_add_task($cloud_context, $instance_id, CLOUD_METERING_BACKUP, array('volumes' => $volumes));
        }
        else {
          // Initiate terminate
          _cloud_metering_add_task($cloud_context, $instance_id, CLOUD_METERING_TERMINATE, array());
        }
  
        // Send notification if it hasn't been sent
        if (!cloud_metering_notification_sent('terminate', $cloud_context, $instance_id)) {
          // The Drupal associated with the instance
          $user = _cloud_metering_get_instance_user($instance);
          $mail_result = cloud_metering_send_mail($user->uid, 'terminate_mail', array('user' => $user, 'cloud_context' => $cloud_context, 'instance_id' => $instance_id, 'instance_name' => $instance['name']));
  
          if ($mail_result['result']) {
            cloud_metering_save_notification_info('terminate', $cloud_context, $instance_id);
          }
          else {
            _cloud_metering_activity_audit(t('Failed to send notification mail for instance %instance_id.', array('%instance_id' => $instance_id)), 'user_activity', '', WATCHDOG_ERROR);
          }
        }
      }
      else {
        // send out a terminate warning every 10 minutes
        if (!cloud_metering_notification_sent('terminate_warning', $cloud_context, $instance_id, 0, 0, CLOUD_METERING_TERMINATE_WARNING_INTERVAL)) {
          $user = _cloud_metering_get_instance_user($instance);
          $mail_result = cloud_metering_send_mail($user->uid, 'terminate_warning_mail', array('user' => $user, 'cloud_context' => $cloud_context, 'instance_id' => $instance_id, 'instance_name' => $instance['name']));
  
          if ($mail_result['result']) {
            cloud_metering_save_notification_info('terminate_warning', $cloud_context, $instance_id);
          }
          else {
            _cloud_metering_activity_audit(t('Failed to send notification mail for instance %instance_id.', array('%instance_id' => $instance_id)), 'user_activity', '', WATCHDOG_ERROR);
          }
        }
      }
    }
  }
}

/**
 * Get an instance. To be deprecated.   
 */
function cloud_metering_get_instance($cloud_context, $instance_id) {
  return cloud_get_instance($cloud_context, $instance_id);
}

/**
 *  Run through all the instances and return as different groups
 */
function cloud_metering_get_grouped_instances($cloud_context = NULL) {
  $return                                = array();
  $return['threshold_crossed_instances'] = array();
  $return['budget_crossed_instances']    = array();
  $return['user_instances']              = array();
  $return['group_instances']             = array();
  $groups = _cloud_metering_get_groups();

  foreach ($groups as $group) {
    $return['group_instances'][$group['grid']] = array(
      'gid' => $group['gid'],
      'instances' => array(),
    );
  }

  if (empty($cloud_context)) {
    $clouds = cloud_get_all_clouds();
  }
  else {
    $clouds = array($cloud_context);
  }

  $end_date   = date('c');

  foreach ($clouds as $cloud) {
    $instances = cloud_get_instances($cloud);
    foreach ($instances as $instance_id => $instance) {
      $runtime        = $instance['runtime'];
      $start_date     = date('c', strtotime($instance['runtime']));
      $budget_data    = _cloud_metering_get_instance_budget($cloud, $instance_id);
      $initial_budget = $budget_data['initial_budget'];
      $threshold      = $budget_data['cost_threshold'];
      $instance['budget']    = $initial_budget;
      $instance['threshold'] = $threshold;

      if (is_numeric($initial_budget) && $initial_budget > 0) {
        $instance_data = _cloud_metering_instance_cost_data($start_date, $end_date, $cloud, $instance_id);
        $cost          = 0;

        if (is_array($instance_data)) {
          $linux_or_unix_usage = $instance_data['linux_or_unix_usage'];
          $total_hours         = _cloud_metering_instance_runtime($runtime);
          $cost                = $linux_or_unix_usage * $total_hours;
        }

        $instance['cost'] = $cost;
        $threshold = ( $initial_budget * $threshold ) / 100;

        if ($cost > 0 && $threshold > 0 && $cost < $initial_budget && $cost > $threshold) {
          $return['threshold_crossed_instances'][] = $instance;
        }
        elseif ($cost > $initial_budget) {
          $return['budget_crossed_instances'][]    = $instance;
        }

        $instance_user = _cloud_metering_get_instance_user($instance);
        $uid           = $instance_user->uid;
        if (!array_key_exists($uid, $return['user_instances'])) {
          $return['user_instances'][$uid] = array();
        }
        $return['user_instances'][$uid][] = $instance;

        if ($roles = $instance_user->roles) {
          foreach ($roles as $rid => $role) {
            if (array_key_exists($rid, $return['group_instances'])) {
              $return['group_instances'][$rid]['instances'][] = $instance;
            }
          }
        }
      }
    }
  }
  return $return;
}

/**
 * Notify the group admins that their users have 
 * surpassed their budget.
 */
function cloud_metering_notify_group_admins($group_instances) {
  foreach ($group_instances as $rid => $data) {
    $gid       = $data['gid'];
    $instances = $data['instances'];

    // Get group budget and compare with the total cost
    $group_budget = _cloud_metering_get_group_budget($gid);
    if (!empty($group_budget)) {
      $group_budget = $group_budget[$gid];
      $group_threshold = $group_budget['threshold'];
      $group_budget = $group_budget['group_budget'];
  
      if ($group_budget > 0 && $group_threshold > 0) {
        $cost = 0;
        foreach ($instances as $instance) {
          $cost += $instance['cost'];
        }
  
        if ($cost >= ($group_budget * $group_threshold / 100)) {
          // Send notification to group admins
          $group_admins = _cloud_metering_get_group_admins($gid);
          foreach ($group_admins as $uid) {
            // add in group budget, cost, and group_threshold
            cloud_metering_send_mail($uid, 'group_mail', array('group_cost' => number_format($cost, 2), 'group_budget' => number_format($group_budget, 2), 'group_threshold' => $group_threshold));
          }
        }
      }
    }
  }
}

/**
 * Notify users if they crossed their budget
 */
function cloud_metering_notify_users($user_instances) {
  foreach ($user_instances as $uid => $instances) {
    // Get user budget and compare with the total cost
    $user_budget = _cloud_metering_get_user_budget($uid);
    if ($user_budget > 0) {
      $cost = 0;
      foreach ($instances as $instance) {
        $cost += $instance['cost'];
      }
      if ($cost >= $user_budget) {
        // Send notification to user
        cloud_metering_send_mail($uid, 'user_mail', array('user_cost' => number_format($cost, 2), 'user_budget' => number_format($user_budget, 2)));
      }
    }
  }
}

/**
 * Function validating initial budget value for individual Templates
 */
function _cloud_metering_validate_template_budget($form, &$form_state) {
  $form_values = $form_state['values'];
  $initial_budget = $form_values['initial_budget'];
  $threshold = $form_values['cost_threshold'];
  $instance_id = isset($form_values['instance_id']) ? $form_values['instance_id'] : 0;
  
  // check some initial entry
  if (!is_numeric($initial_budget)) {
    form_set_error('initial_budget', t('Please enter a numeric value in Initial Budget'));
  }
  elseif ($initial_budget < 0) {
    form_set_error('initial_budget', t('The Initial Budget value should not be less than 0'));
  }
  if (!is_numeric($threshold)) {
    form_set_error('cost_threshold', t('The cost threshold should be numeric'));
  }
  elseif ($threshold < 0) {
    form_set_error('cost_threshold', t('The threshold should not be less than 0'));
  }
  
  if ($instance_id) {
    $cloud_context = $form_values['cloud_context'];
    // make sure the instance is not locked
    // Proceed to save only if instance is not locked
    $lock_status = cloud_perform_action('', 'get_instance_lock_status', $cloud_context, array('instance_id' => $instance_id));
    $locked = ($lock_status == 'Lock' ? TRUE : FALSE);
    if ($locked) {
      form_set_error('initial_budget', t('Budget settings could not be saved as the instance is locked.'));
    }
  
    // calculate the budget stuff
    $instance_details = cloud_metering_get_instance($cloud_context, $instance_id);
    $account = _cloud_metering_get_instance_user($instance_details);
    $user_budget = _cloud_metering_get_user_budget($account->uid);
    if ($user_budget > 0) {
      $allocated_budget = _cloud_metering_get_user_instances_budget($account->name);
      $total_budget = $allocated_budget + $initial_budget;
      if ($total_budget > $user_budget) {
        $total_budget = number_format($total_budget, 2, '.', ',');
        $user_budget  = number_format($user_budget, 2, '.', ',');
        $edit_link    = '';
        if (user_access('configure user budget')) {
          $edit_link = l(t('Edit user budgets'), 'admin/config/cloud_metering/group_settings');
        }
        form_set_error('initial_budget', t("Instance budget settings cannot be saved because of existing user budget constraints. Setting this budget will make the budget allocated to all instances of the user @total; but the user's budget is @user-budget. !edit-link", array('@total' => $total_budget, '@user-budget' => $user_budget, '!edit-link' => $edit_link)));
      }
    }
  }
}

/**
 *  Submit handler for cloud_metering_set_threshold and cloud_metering_form_alter
 *  @param array $form Form object
 *  @param array $form_state Form values
 */
function _cloud_metering_update_instance_budget($form, &$form_state) {
  $form_values = $form_state['values'];
  $instance_id = $form_values['instance_id'];
  $cloud_context = $form_values['cloud_context'];
  $initial_budget = $form_values['initial_budget'];
  $threshold = round($form_values['cost_threshold'], 2);
  $terminate = $form_values['terminate'];
  
  // Update records
  _cloud_metering_instance_budget_save($cloud_context, $instance_id, array('budget' => $initial_budget, 'threshold' => $threshold, 'terminate' => $terminate));
}

/**
 *  Log watchdog messages as well as save them to cloud module's audit logs
 *  @param String message The message to save
 *  @param String type The type of message (Cloud Activity module only shows the user_activity type messages)
 *  @param String link A link, required by Cloud Activity Audit, not used by metering
 *  @param String serverity The severity of the message like notice, warning or error. Use watchdog severity constants
 */
function _cloud_metering_activity_audit($message, $type = 'user_activity', $link = '', $severity = WATCHDOG_NOTICE) {
  watchdog(CLOUD_METERING_MODULE_DISPLAY_NAME, $message, NULL, $severity);
  if (module_exists('cloud_activity_audit')) {
    cloud_activity_audit_log(
      array(
        'type' => $type,
        'message' => $message,
        'link' => $link,
      )
    );
  }
  return TRUE;
}

/**
 *  Get the runtime in hours of a cloud instance
 *  @param $launch_time string The launch datetime
 *  @param $to_time string Optional "to" datetime
 *  @return integer The number of hours the instance has run
 */
function _cloud_metering_instance_runtime($launch_time, $to_time = '') {
  if (empty($launch_time)) {
    return 0;
  }

  $launch_time     = date_parse($launch_time);
  $gm_launch_time  = gmmktime( $launch_time['hour'], $launch_time['minute'], $launch_time['second'], $launch_time['month'], $launch_time['day'], $launch_time['year'] );

  if (!empty($to_time)) {
    $to_time     = date_parse($to_time);
    $gm_to_time  = gmmktime( $to_time['hour'], $to_time['minute'], $to_time['second'], $to_time['month'], $to_time['day'], $to_time['year'] );
  }
  else {
    $gm_to_time  = gmmktime();
  }

  $diff =  $gm_to_time - $gm_launch_time;
  $hours = round($diff / (3600));
  return $hours;
}

/**
 *  Save template budget
 */
function _cloud_metering_update_template_budget($form, &$form_state) {
  $form_values = $form_state['values'];
  $template_id     = $form_values['templateid'];
  $initial_budget  = $form_values['initial_budget'];
  $threshold       = round($form_values['cost_threshold'], 2);
  _cloud_metering_template_budget_save( $template_id, array('budget' => $initial_budget, 'threshold' => $threshold) );
}

/**
 *  Get the instance user.
 */
function _cloud_metering_get_instance_user($instance) {
  $cloud_context = $instance['cloud_context'];
  $ssh_key_name  = $instance['user'];

  if (!empty($ssh_key_name)) {
    $key_owner = cloud_perform_action('', 'get_ssh_key_owner', $cloud_context, array('key_name' => $ssh_key_name));
    if (!empty($key_owner)) {
      // try to load the account of the ssh key owner
      $account = user_load_by_name($key_owner);
      if ($account && $account->uid) {
        return $account;
      }
    }
  }

  $account = user_load(1);
  return $account;
}

/**
 *  Get all instances associated with a user
 *  TODO: this should be a generic function
 */
function _cloud_metering_get_user_instances($user_name, $clouds = array()) {
  $user_instances = array();

  if (empty($clouds)) {
    $clouds = cloud_get_all_clouds();
  }

  foreach ($clouds as $cloud) {
    $instances = cloud_get_instances($cloud);
    foreach ($instances as $instance) {
      $instance_user = _cloud_metering_get_instance_user($instance);
      if ($instance_user->name == $user_name) {
        $user_instances[] = $instance;
      }
    }
  }
  return $user_instances;
}

/**
 *  Get the total budget allocated to all instances of a user
 */
function _cloud_metering_get_user_instances_budget($user_name) {
  $user_instances = _cloud_metering_get_user_instances($user_name);
  $total_budget = 0;

  foreach ($user_instances as $user_instance) {
    $instace_id    = $user_instance['instance_id'];
    $cloud_context = $user_instance['cloud_context'];
    $budget        = _cloud_metering_get_instance_budget($cloud_context, $instace_id);

    if (!is_null($budget['instance_id'])) {
      $total_budget += $budget['initial_budget'];
    }
  }
  return $total_budget;
}

/**
 * Check user budget while launching instances from server templates
 * @param String template_id
 *   The Template-id
 * @param String cloud_context
 *   The cloud context
 * @return Mixed
 *   Handover to launching routine in the original cloud module if budget is not a constraint
 *   Otherwise show an error message and return to templates listing page
 */
function _cloud_metering_check_launch($template_id, $cloud_context) {

  $launch = TRUE;
  $template_budget = _cloud_metering_get_template_budget($template_id);

  // Check if budget set for template
  if (!is_null($template_budget['template_id']) && $template_budget['initial_budget'] !== 0) {
    $initial_budget = $template_budget['initial_budget'];
    
    if ($template_data = _cloud_metering_get_template_data($template_id)) {
      $key_name  = $template_data['key_name'];
      $instance_count = $template_data['count_instances'];
      $key_owner = cloud_perform_action('', 'get_ssh_key_owner', $cloud_context, array('key_name' => $key_name));

      if ($key_owner) {
        $account = user_load_by_name($key_owner);

        if ($account && $account->uid) {
          $user_budget = _cloud_metering_get_user_budget($account->uid);
          if ($user_budget > 0) {
            $total_allocated_budget = _cloud_metering_get_user_instances_budget($key_owner);
            $new_instances_total_budget = $initial_budget * $instance_count;
            $total = $total_allocated_budget + $new_instances_total_budget;
            if ($total > $user_budget) {
              $total       = number_format($total, 2, '.', ',');
              $user_budget = number_format($user_budget, 2, '.', ',');
              $launch      = FALSE;
              $edit_link   = '';
              if (user_access('edit server template')) {
                $edit_link   = _cloud_metering_get_budget_config_link(array('cloud_context' => $cloud_context, 'template_id' => $template_id));
              }
             drupal_set_message(t("Launching of instance(s) was stopped due to budget constraint. After launching instances from this template, the total budget allocated to user's instances will be @total; but the user's budget is @user-budget.", array('@total' => $total, '@user-budget' => $user_budget)), 'warning');
            }
          }
        }
      }
    }
    else {
      drupal_set_message(t('Failed to launch instance. The server template could not be found.', 'error'));
    }
  }
  if ($launch) {
    return cloud_perform_action($template_id, 'launch', $cloud_context);
  }
  else {
    drupal_goto( CLOUD_SERVER_TEMPLATES_PATH . '/list/' . $cloud_context );
  }
}

/**
 *  Get budget configuration link for any cloud
 *  @param Array params
 *    Array containing the cloud_context and either the template_id or the instance_id
 *  @return String
 *    The HTML link
 */
function _cloud_metering_get_budget_config_link($params = array()) {
  $cloud_context = $params['cloud_context'];
  if (empty($cloud_context)) {
    return '';
  }
  if (isset($params['template_id'])) {
    $template_id = $params['template_id'];
    return l(t('Edit template budget'), CLOUD_SERVER_TEMPLATES_PATH . '/' . $cloud_context . '/' . $template_id . '/edit', array('fragment' => 'edit-initial-budget'));
  }
  elseif (isset($params['instance_id'])) {
    $instance_id = $params['instance_id'];
    return l(t('Edit instance budget'), '/admin/config/cloud_metering/threshold/' . $cloud_context . '/' . $instance_id);
  }
  return '';
}
