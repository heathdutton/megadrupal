<?php

// ======================================
// CONDITIONS
// ======================================

/*
 * Conditions:
 */


// ======================================
// ACTIONS
// ======================================

/**
 * Implements hook_rules_action_info()
 */
function ms_files_rules_action_info() {
  return array(
    'ms_files_action_add_file' => array(
      'label' => t('Add a File to a User'),
      'parameter' => array(
        'pfid' => array('type' => 'integer', 'label' => t('File Plan'), 'options list' => 'ms_files_get_files'),
        'account' => array('type' => 'user', 'label' => t('User')),
      ),
      'group' => 'Files Download Suite',
    ),
  );
}

/**
 * Action: Add a file to a user
 */
function ms_files_action_add_file($pfid, $account) {
  $f_plan = ms_files_load_file_plan($pfid);
  return ms_files_insert_file_user($f_plan, $account->uid);
}

// ======================================
// TRIGGERS
// ======================================

/*
 * Triggers:
 * + File Purchased
 */

/**
 * Implements hook_rules_event_info()
 */
function ms_files_rules_event_info() {
  $items = array(
    'ms_files_event_file_purchased' => array(
      'label' => t('File Purchased'),
      'group' => 'File Downloads Suite',
      'help' => t("Triggers when a file is purchased."),
      'variables' => array(
        'account' => array('type' => 'user', 'label' => t('Account')),
        'd_file' => array('type' => 'ms_file', 'label' => t('File Download Record')),
        'f_plan' => array('type' => 'ms_file_plan', 'label' => t('File Download Plan')),
      ),
    ),
  );

  return $items;
}


// ======================================
// DATA TYPES
// ======================================

/**
 * Implements hook_rules_data_type_info().
 */
function ms_files_rules_data_type_info() {
  return array(
    'ms_file_plan' => array(
      'wrap' => TRUE,
      'group' => t('File Downloads Suite'),
    ),
    'ms_file' => array(
      'wrap' => TRUE,
      'group' => t('File Downloads Suite'),
    ),
  );
}
