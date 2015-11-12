<?php

/**
 * @file
 * Admin functionality.
 */

function services_sso_client_profile_flag_admin_form($form_state) {
  services_sso_client_profile_flag_load_includes();

  // Pull in some analytics info about flags.
  $info = _services_sso_client_profile_flag_analytics();

  $form = array();

  $form['services_sso_client_profile_flag_endpoint'] = array(
    '#title' => t('Endpoint name'),
    '#description' => t('The name of the endpoint on the Services REST service. Example: profile-flag'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('services_sso_client_profile_flag_endpoint', ''),
  );

  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => 'Information and statistics',
  );
  $form['info']['stats'] = array(
    '#markup' => theme('services_sso_client_profile_flag_info', $info),
  );

  // Cron settings
  $form['cron_batch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cron and batch settings'),
  );
  $form['cron_batch']['services_sso_client_profile_flag_batch_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of flags to run at batch'),
    '#size' => 5,
    '#max_length' => 3,
    '#required' => TRUE,
    '#description' => t('The number of flags to process during each increment of the batch operation. Increase or decreaes this number based on the speed of your webserver and the settings of PHP timeout.'),
    '#default_value' => variable_get('services_sso_client_profile_flag_batch_num', 20),
  );
  $form['cron_batch']['services_sso_client_profile_flag_cron_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of flags to run on cron'),
    '#size' => 5,
    '#max_length' => 3,
    '#required' => TRUE,
    '#description' => t('The number of flags to process at every cron run. You might want to increase or decrease this number based on the speed of your webserver or the processing load in terms of other cron jobs.'),
    '#default_value' => variable_get('services_sso_client_profile_flag_cron_num', 20),
  );

  // Generate options
  $options = array(
    '1' => t('Immediately'),
    15*60 => t('15 mins'),
    30*60 => t('30 mins'),
    45*60 => t('45 mins'),
    3600 => t('1 hr'),
    3*3600 => t('3 hrs'),
    5*3600 => t('5 hrs'),
    9*3600 => t('9 hrs'),
    12*3600 => t('12 hrs'),
    18*3600 => t('18 hrs'),
    24*3600 => t('1 day'),
    36*3600 => t('36 hrs'),
    48*3600 => t('2 days'),
    72*3600 => t('3 days'),
    7*24*3600 => t('1 week'),
    2*7*24*3600 => t('2 weeks'),
  );
  $form['cron_batch']['services_sso_client_profile_flag_cron_time_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Minimum threshold between flag processing'),
    '#required' => TRUE,
    '#options' => $options,
    '#description' => t('The amount of time between initial flag processing and every subsequent re-processing of that particular flag.'),
    '#default_value' => variable_get('services_sso_client_profile_flag_cron_time_threshold', 60*60*12),
  );

  $form['sitewide'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls'),
  );
  $form['sitewide']['retrieve_all'] =  array(
    '#type' => 'button',
    '#value' => t('Retrieve all flags'),
    '#executes_submit_callback' => TRUE,
    '#submit' => array('services_sso_client_profile_flag_retrieve_all'),
    '#weight' => 0,
  );
  $form['sitewide']['retrieve_new'] =  array(
    '#type' => 'button',
    '#value' => t('Retrieve only new flags'),
    '#executes_submit_callback' => TRUE,
    '#submit' => array('services_sso_client_profile_flag_retrieve_new'),
    '#weight' => 0,
  );
  // $form['sitewide']['reset'] = array(
  //   '#type' => 'button',
  //   '#value' => t('Reset ALL profile flags to their default configuration on the SSO server'),
  //   '#executes_submit_callback' => TRUE,
  //   '#submit' => array('services_sso_client_profile_flag_reset_all'),
  //   '#weight' => 5,
  // );
  $form['sitewide']['apply_all'] = array(
    '#type' => 'button',
    '#value' => t('Apply all flags'),
    '#executes_submit_callback' => TRUE,
    '#submit' => array('services_sso_client_profile_flag_batch'),
    '#weight' => 5,
  );

  return system_settings_form($form);
}

function services_sso_client_profile_flag_reset_all() {
  services_sso_client_profile_flag_reset(array(), TRUE);
}

function services_sso_client_profile_flag_retrieve_all() {
  module_load_include('php', 'services_sso_client_profile_flag', 'includes/profileFlag.class');

  if (ProfileFlag::retrieve(TRUE)) {
    drupal_set_message(t('All flags have been retrieved.'));
  }
}

function services_sso_client_profile_flag_retrieve_new() {
  module_load_include('php', 'services_sso_client_profile_flag', 'includes/profileFlag.class');

  ProfileFlag::retrieveNew(TRUE);
}

/**
 * Return some statistical info.
 */
function _services_sso_client_profile_flag_analytics() {
  $info = array();

  $info['total'] = db_query("SELECT COUNT(DISTINCT fid) AS count FROM {services_sso_client_profile_flag}")->fetchField();
  $info['unprocessed'] = db_query("SELECT COUNT(DISTINCT fid) AS count FROM {services_sso_client_profile_flag} WHERE last_processed < 1")->fetchField();
  $info['last_retrieval'] = variable_get('services_sso_client_profile_flag_last_retrieval', 0);

  return $info;
}

function services_sso_client_profile_flag_actions_ajax_callback($form, &$form_state) {
  $partial_form = _services_sso_client_profile_flag_actions_get_form_widget($form_state['triggering_element']['#array_parents'], $form, '_select');
  return $partial_form;
}

function _services_sso_client_profile_flag_actions_get_form_widget($parents, &$form, $suffix) {
  if (count($parents) > 1) {
    $parent = array_shift($parents);
    return _services_sso_client_profile_flag_actions_get_form_widget($parents, $form[$parent], $suffix);
  }
  return $form[$parents[0] . $suffix];
}

function _services_sso_client_profile_flag_actions_get_tabs($form, $form_state) {
  return $form['tab_controls'];
}

/**
 * Return an array containing all groups - suitable for a form item.
 */
function _services_sso_client_profile_flag_actions_og_all_groups_options() {
  $result = db_query("SELECT gid, label FROM {og} o WHERE state = 1 ORDER BY label ASC");

  foreach ($result as $row) {
    $options[$row->gid] = $row->label;
  }
  return isset($options) ? $options : array();
}

/**
 * Compare the triggering element key and $comparison, also if the value on triggering element.
 *
 * @return bool
 *  AND behavior on the key comparison result and triggering element value.
 */
function _services_sso_client_profile_flag_actions_get_element_key($form_state, $comparison) {
  if (!isset($form_state['triggering_element'])) {
    return TRUE;
  }
  if (!empty($form_state['triggering_element']['#array_parents'])) {
    $key = $form_state['triggering_element']['#array_parents'][count($form_state['triggering_element']['#array_parents']) - 1];

    if ($comparison == $key && empty($form_state['triggering_element']['#value'])) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Creates flag action assignment interface form.
 *
 * Uses AHAH Helper module for dynamic reloading of the form elements. For each flag,
 * Creates options for adding or removing groups and roles.
 *
 */
function services_sso_client_profile_flag_actions($form, &$form_state) {
  services_sso_client_profile_flag_load_includes();

  drupal_add_css(drupal_get_path('module', 'services_sso_client_profile_flag') . '/css/styles.css');
  drupal_add_js(drupal_get_path('module', 'services_sso_client_profile_flag') . '/js/ui.js');

  $form = array('search_box' => array(), 'tab_controls' => array());

  $all_terms = ProfileFlag::load('weight_asc');

  //initialize array of flag ID values
  $form['profile_flag'] = array(
    '#type' => 'value',
    '#value' => array(),
  );

  $actions = array('role_addto', 'role_removefrom', 'group_addto', 'group_removefrom');
  $tabs = array();
  $term_tabs = array();

  //submit button for the form
  $form['submit_top'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#submit' => array('services_sso_client_profile_flag_actions_submit'),
    '#weight' => -999999997,
  );
  $form['submit_bottom'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#submit' => array('services_sso_client_profile_flag_actions_submit'),
    '#weight' => 999999999,
  );

  //make a fieldset for each flag and store that flag's ID in the form as a value.
  foreach ($all_terms as $term) {
    $term_id = $term->fid;
    $form['profile_flag']['#value'][$term_id] = $term_id;

    // Construct the controller tabs set.
    $term_tabs[] = $term->string;
    $tabs[] = l($term->string, '', array('attributes' => array('rel' => $term->string, 'class' => 'tab-control tab-control-' . $term->string)));

    // Figure out a weight to actually use if multiple items have the same weight
    $weight_default_value = (isset($last_term_weight) && ($term->weight === $last_term_weight || $term->weight < $last_term_weight)) ? $last_term_weight + 1 : $term->weight;

    $form[$term_id] = array(
      '#type' => 'fieldset',
      '#title' => t($term->string),
      '#prefix' => '<div id="' . $term_id . '_wrapper" class="tab-content tab-content-' . $term->string . '">',
      '#suffix' => '</div>',
      '#tree' => TRUE,
      '#weight' => $weight_default_value,
    );

    //initialize variable values
    $add_group_check = 0;
    $remove_group_check = 0;
    $add_role_check = 0;
    $remove_role_check = 0;
    $role_add_default = array();
    $role_remove_default = array();
    $group_add_default = array();
    $group_remove_default = array();


    //sets default values on initial load of the form
    foreach ($term->actions as $action) {
      //add roles
      if ($action->type == 'role_addto') {
        $role_add_default = $action->options;
        $add_role_check = 1;
        if ($form_state['submitted'] == FALSE) {
          $form_state['values'][$term_id][$term_id . '_role_addto'] = 1;
        }
      }
      //remove roles
      if ($action->type == 'role_removefrom') {
        $role_remove_default = $action->options;
        $remove_role_check = 1;
        if ($form_state['submitted'] == FALSE) {
          $form_state['values'][$term_id][$term_id . '_role_removefrom'] = 1;
        }
      }
      //add groups
      if ($action->type == 'group_addto') {
        $group_add_default = $action->options;
        $add_group_check = 1;
        if ($form_state['submitted'] == FALSE) {
          $form_state['values'][$term_id][$term_id . '_group_addto'] = 1;
        }
      }
      //remove groups
      if ($action->type == 'group_removefrom') {
        $group_remove_default = $action->options;
        $remove_group_check = 1;
        if ($form_state['submitted'] == FALSE) {
          $form_state['values'][$term_id][$term_id . '_group_removefrom'] = 1;
        }
      }
    }//foreach, line 97


    /*** Each checkbox and associated action choices handled below. ***/

    //Add Roles
    $form[$term_id][$term_id . '_role_addto'] = array(
      '#type' => 'checkbox',
      '#title' => 'Add roles',
      '#default_value' => $add_role_check,
      '#ajax' => array(
        //'callback'    => ahah_helper_path(array($term_id, $term_id . '_role_addto_select')),
        'callback' => 'services_sso_client_profile_flag_actions_ajax_callback',
        'wrapper' => $term_id . '_role_addto_wrapper',
      ),
      '#prefix' => '<div class="checkbox-wrapper">',
      '#suffix' => '</div>',
    );
    $form[$term_id][$term_id . '_role_addto_select'] = array(
      '#type' => 'hidden',
      '#prefix' => '<div id="' . $term_id . '_role_addto_wrapper" class="select-wrapper">',
      '#suffix' => '</div>'
    );
    if (!empty($form_state['values'][$term_id][$term_id . '_role_addto']) && $form_state['values'][$term_id][$term_id . '_role_addto']) {
      if (_services_sso_client_profile_flag_actions_get_element_key($form_state, $term_id . '_role_addto')) {
        $roles_array = user_roles(TRUE);
        $form[$term_id][$term_id . '_role_addto_select'] = array(
          '#type' => 'select',
          '#title' => t('Available Roles'),
          '#options' => $roles_array,
          '#multiple' => TRUE,
          '#default_value' => $role_add_default,
          '#prefix' => '<div id="' . $term_id . '_role_addto_wrapper">',
          '#suffix' => '</div>',
          '#size' => 5,
        );
      }
    }

    //Remove Roles
    $form[$term_id][$term_id . '_role_removefrom'] = array(
      '#type' => 'checkbox',
      '#title' => 'Remove roles',
      '#default_value' => $remove_role_check,
      '#ajax' => array(
        'callback' => 'services_sso_client_profile_flag_actions_ajax_callback',
        'wrapper' => $term_id . '_role_removefrom_wrapper',
      ),
      '#prefix' => '<div class="checkbox-wrapper">',
      '#suffix' => '</div>',
    );

    $form[$term_id][$term_id . '_role_removefrom_select'] = array(
      '#type' => 'hidden',
      '#prefix' => '<div id="' . $term_id . '_role_removefrom_wrapper" class="select-wrapper">',
      '#suffix' => '</div>'
    );

    if (!empty($form_state['values'][$term_id][$term_id . '_role_removefrom']) && $form_state['values'][$term_id][$term_id . '_role_removefrom']) {
      if (_services_sso_client_profile_flag_actions_get_element_key($form_state, $term_id . '_role_removefrom')) {
        $roles_array = user_roles(TRUE);
        $form[$term_id][$term_id . '_role_removefrom_select'] = array(
          '#type' => 'select',
          '#title' => t('Available Roles'),
          '#options' => $roles_array,
          '#multiple' => TRUE,
          '#default_value' => $role_remove_default,
          '#prefix' => '<div id="' . $term_id . '_role_removefrom_wrapper">',
          '#suffix' => '</div>',
          '#size' => 5,
        );
      }
    }

    //Add Groups
    $form[$term_id][$term_id . '_group_addto'] = array(
      '#type' => 'checkbox',
      '#title' => 'Add groups',
      '#default_value' => $add_group_check,
      '#ajax' => array(
        'callback' => 'services_sso_client_profile_flag_actions_ajax_callback',
        'wrapper' => $term_id . '_group_addto_wrapper',
      ),
      '#prefix' => '<div class="checkbox-wrapper">',
      '#suffix' => '</div>',
    );

    $form[$term_id][$term_id . '_group_addto_select'] = array(
      '#type' => 'hidden',
      '#prefix' => '<div id="' . $term_id . '_group_addto_wrapper" class="select-wrapper">',
      '#suffix' => '</div>'
    );

    if (!empty($form_state['values'][$term_id][$term_id . '_group_addto']) && $form_state['values'][$term_id][$term_id . '_group_addto']) {
      if (_services_sso_client_profile_flag_actions_get_element_key($form_state, $term_id . '_group_addto')) {
        module_load_include('module', 'og');
        $groups_array = _services_sso_client_profile_flag_actions_og_all_groups_options();
        $form[$term_id][$term_id . '_group_addto_select'] = array(
          '#type' => 'select',
          '#title' => t('Available Groups'),
          '#options' => $groups_array,
          '#multiple' => TRUE,
          '#default_value' => $group_add_default,
          '#prefix' => '<div id="' . $term_id . '_group_addto_wrapper" class="select-wrapper">',
          '#suffix' => '</div>',
          '#size' => 5,
        );
      }
    }

    //Remove Group
    $form[$term_id][$term_id . '_group_removefrom'] = array(
      '#type' => 'checkbox',
      '#title' => 'Remove groups',
      '#default_value' => $remove_group_check,
      '#ajax' => array(
        'callback' => 'services_sso_client_profile_flag_actions_ajax_callback',
        'wrapper' => $term_id . '_group_removefrom_wrapper',
      ),
      '#prefix' => '<div class="checkbox-wrapper">',
      '#suffix' => '</div>',
    );

    $form[$term_id][$term_id . '_group_removefrom_select'] = array(
      '#type' => 'hidden',
      '#prefix' => '<div id="' . $term_id . '_group_removefrom_wrapper" class="select-wrapper">',
      '#suffix' => '</div>'
    );

    if (!empty($form_state['values'][$term_id][$term_id . '_group_removefrom']) && $form_state['values'][$term_id][$term_id . '_group_removefrom']) {
      if (_services_sso_client_profile_flag_actions_get_element_key($form_state, $term_id . '_group_removefrom')) {
        module_load_include('module', 'og');
        $groups_array = _services_sso_client_profile_flag_actions_og_all_groups_options();
        $form[$term_id][$term_id . '_group_removefrom_select'] = array(
          '#type' => 'select',
          '#title' => t('Available Groups'),
          '#options' => $groups_array,
          '#multiple' => TRUE,
          '#default_value' => $group_remove_default,
          '#prefix' => '<div id="' . $term_id . '_group_removefrom_wrapper">',
          '#suffix' => '</div>',
          '#size' => 5,
        );
      }
    }

    //weight
    $form[$term_id][$term_id . '_weight'] = array(
      '#type' => 'textfield',
      '#title' => t('Weight'),
      '#prefix' => '<div id="weight-wrapper">',
      '#suffix' => '</div>',
      '#default_value' => $weight_default_value,
      '#size' => 6,
    );

    // Remember the weight we actually used.
    $last_term_weight = $weight_default_value;

  } // end foreach, line 73

  //defines search box for flags in left column
  $form['search_box'] = array(
    '#type' => 'textfield',
    '#title' => 'Filter Flags',
    '#ajax' => array(
      'callback' => '_services_sso_client_profile_flag_actions_get_tabs',
      'wrapper' => 'tab-controls',
      'event' => 'change',
      'effect' => 'fade',
      'keypress' => TRUE,
    ),
    '#size' => 10,
    '#weight' => -999999999,
    '#description' => t('Press enter have enter partial search string to filter.'),
  );

  //AHAH helper for search box reloads
  if (!empty($form_state['values']['search_box'])) {
    $tabs = array();
    foreach ($term_tabs as $key => $term) {
      if (strpos($term, trim($form_state['values']['search_box'])) === FALSE) {
        unset($term_tabs[$key]);
      }
      else {
        $tabs[] = l($term, '', array('attributes' => array('rel' => $term, 'class' => 'tab-control tab-control-' . $term)));
      }
    }
  }

  // Deposit the tabs to the front of the $form array.
  $form['tab_controls'] = array(
    '#markup' => '<div id="tab-controls" class="tab-controls">' . theme('item_list', array('items' => $tabs)) . '</div>',
    '#weight' => -999999998,
  );

  return $form;
}

/**
 * Submit function for flag actions form above.
 *
 * the body of the submit is a foreach loop that checks each submitted flag
 * and compares each (including selected actions) against existing actions attached
 * to the flag.
 */

function services_sso_client_profile_flag_actions_submit($form, &$form_state) {
  services_sso_client_profile_flag_load_includes();

  $action_types = array('role_addto', 'role_removefrom', 'group_addto', 'group_removefrom');
  foreach ($form_state['values']['profile_flag'] as $flag_id) {
    $flag = ProfileFlag::loadByID($flag_id);
    $flag = current($flag);

    $arrays = $form_state['values'][$flag_id];

    $flag->weight = $arrays[$flag_id . '_weight'];

    foreach ($action_types as $action_type) {
      $old_action = FALSE;
      $old_action_pos = 0;

      // Find the action that was already in the profileFlag object and remember
      // its array indices for later use.
      foreach ($flag->actions as $key => $val) {
        if ($val->type == $action_type) {
          $old_action = $val;
          $old_action_pos = $key;
          break;
        }
      }

      if (!empty($arrays[$flag_id . '_' . $action_type]) && $arrays[$flag_id . '_' . $action_type]) { //case: action is checked
        if ($old_action) {
          //the action already exists; update it
          $flag->deleteAction($old_action_pos);
          $old_action->options = $arrays[$flag_id . '_' . $action_type . '_select'];
          $flag->pushAction($old_action);
        }
        else {
          //the action doesn't exist; create a new action.

          //check if the selection is empty; no point in creating an action with no options.
          if (!empty($arrays[$flag_id . '_' . $action_type . '_select'])) {
            $action = new ProfileFlagAction((object) array('type' => $action_type, 'fid' => $flag_id, 'options' => $arrays[$flag_id . '_' . $action_type . '_select']));
            $action = $action->map();
            $flag->pushAction($action);
          }
        }
      }
      else { //case: action is unchecked.
        if ($old_action) {
          //action exists; delete it.
          $flag->deleteAction($old_action_pos);
        }
      }
    } //end foreach

    $flag->save();
  }//end foreach, line 266
  drupal_set_message(t('Flags successfully saved'));
} //end function


