<?php

/**
 * @file
 * Bulk deletion of users.
 */

/**
 * Form builder; Return form for deletion of users.
 */
function entitybulkdelete_users($form, &$form_state) {
  // Display page 2 if $form_state['page_num'] == 2
  if (!empty($form_state['page_num']) && $form_state['page_num'] == 2) {
    return entitybulkdelete_user_confirm_form($form, $form_state);
  }
  $form_state['page_num'] = 1;
  // Deletes users by name.
  $form['del_name'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete users, by name'),
    '#ajax' => array(
      'callback' => 'entitybulkdelete_user_callback',
      'wrapper' => 'del-name',
    ),
  );
  $form['name_placeholder'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="del-name">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  if (!empty($form_state['values']['del_name']) && $form_state['values']['del_name']) {
    $form['name_placeholder']['#type'] = 'fieldset';
    $form['name_placeholder']['#title'] = 'Delete users by name';
    $user_role = entitybulkdelete_user_role(FALSE);
    $user_role['_none'] = "Select roles";
    // Get value from $form_state['values']
    $sel_userrole = isset($form_state['values']['name_placeholder']['user_role']) ? $form_state['values']['name_placeholder']['user_role'] : '_none';
    $form['name_placeholder']['user_role'] = array(
      '#title' => 'Users role',
      '#type' => 'select',
      '#default_value' => '_none',
      '#options' => $user_role,
      '#ajax' => array(
        'callback' => 'entitybulkdelete_userrole_callback',
        'wrapper' => 'user-role',
      ),
    );
    if (!empty($sel_userrole) && isset($sel_userrole)) {
      if ($sel_userrole != '_none') {
        $user_list = entitybulkdelete_get_users($sel_userrole, FALSE);
        $form_state['users'] = $user_list;
      }
    }
    $user_list['_none'] = "Select users";
    $form['name_placeholder']['name'] = array(
      '#title' => 'Users',
      '#type' => 'select',
      '#options' => $user_list,
      '#multiple' => TRUE,
      '#prefix' => '<div id="user-role">',
      '#suffix' => '</div>',
    );
  }
  // Deletes users by user's uid.
  $form['del_uid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete users, by uid'),
    '#ajax' => array(
      'callback' => 'entitybulkdelete_uid_callback',
      'wrapper' => 'del-uid',
    ),
  );
  $form['uid_placeholder'] = array(
    '#type' => 'hidden',
    '#prefix' => '<div id="del-uid">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );
  if (!empty($form_state['values']['del_uid']) && $form_state['values']['del_uid']) {
    $form['uid_placeholder']['#type'] = 'fieldset';
    $form['uid_placeholder']['#title'] = 'Delete users by uid';
    $form['uid_placeholder']['uid'] = array(
      '#title' => 'Users uid',
      '#type' => 'textarea',
      '#rows' => 6,
      '#cols' => 40,
      '#description' => t("Enter user's uid separate by ','."),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
    '#submit' => array('entitybulkdelete_user_submit'),
  );
  return $form;
}

/**
 * Submit handler for the users deletion form.
 */
function entitybulkdelete_user_submit($form, &$form_state) {
  $valid_rows = array();
  $del_by_name = $form_state['values']['del_name'];
  if ($del_by_name) {
    $sel_users = $form_state['values']['name_placeholder']['name'];
    if (isset($form_state['users']) && !empty($form_state['users'])) {
      $users = $form_state['users'];
      foreach ($sel_users as $key => $value) {
        if (array_key_exists($key, $users)) {
          // Avoid admin user.
          if ($key != '1') {
            $valid_rows[$key] = array('uid' => $key, 'name' => check_plain($users[$key]));
          }
        }
      }
    }
  }
  $del_by_uid = $form_state['values']['del_uid'];
  if ($del_by_uid) {
    $find = array(' ', '-', '+');
    $temp_users = check_plain($form_state['values']['uid_placeholder']['uid']);
    if (!empty($temp_users)) {
      $users_uid = str_replace($find, '', $temp_users);
      $users_uid = explode(',', $users_uid);
      if (!empty($users_uid)) {
        foreach ($users_uid as $key => $value) {
          if (!is_numeric($value)) {
            unset($users_uid[$key]);
          }
          else {
            $temp_key = (int) $value;
            $usersuid[$temp_key] = $temp_key;
          }
        }
        // Get list of user's uid and name.
        $query = db_select('users', 'u');
        $query->fields('u', array('uid', 'name'));
        $query->condition('uid', $usersuid, 'IN');
        $user_list = $query->execute()->fetchAllKeyed(0, 1);
        foreach ($user_list as $key => $value) {
          // Avoid admin user.
          if ($key != '1') {
            $valid_rows[$key] = array('uid' => $key, 'name' => check_plain($value));
          }
        }
      }
    }
  }
  if (!empty($valid_rows)) {
    $form_state['valid_rows'] = $valid_rows;
    $form_state['page_num'] = 2;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Form builder; Return confirmation form for users deletion.
 */
function entitybulkdelete_user_confirm_form($form, &$form_state) {
  $header['uid'] = 'uid';
  $header['name'] = 'Name';
  $valid_row = $form_state['valid_rows'];
  $form['msg'] = array(
    '#type' => 'markup',
    '#markup' => t('Are you sure you want to deletes these users ?') .
    '<br>' . t('This action cannot be undone.') . '<br>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Confirm',
    '#submit' => array('entitybulkdelete_users_confirm_form_submit'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => 'Cancel',
    '#submit' => array('entitybulkdelete_users_cancel_form_submit'),
  );
  $form['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $valid_row,
    '#empty' => t('No Valid Row(s).'),
  );
  return $form;
}

/**
 * Submit handler(Cancel) of confirmation form for users deletion.
 */
function entitybulkdelete_users_cancel_form_submit($form, &$form_state) {
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler(Confirm) of confirmation form for users deletion.
 */
function entitybulkdelete_users_confirm_form_submit($form, &$form_state) {
  $table = $form_state['values']['table'];
  $table = array_flip($table);
  if (array_key_exists('0', $table)) {
    unset($table[0]);
  }
  if (!empty($table)) {
    $count = count($table);
    $operations = array();
    for ($i = 0; $i < $count; $i = $i + 5) {
      // Set up an operations array with 5 elements, each doing function
      // entitybulkdelete_users_delete_batch.
      $batch_tbl = array_slice($table, $i, 5);
      if (!empty($batch_tbl)) {
        $operations[] = array('entitybulkdelete_users_delete_batch',
          array($batch_tbl, &$form_state));
      }
    }
    // Execute the batch.
    $batch = array(
      'operations' => $operations,
      'title' => t('Users Delete Batch'),
      'init_message' => t('User(s) deleting is in process...'),
      'progress_message' => t('Processed @current*5 out of @total*5.'),
      'finished' => 'entitybulkdelete_users_delete_batch_finished',
      'file' => drupal_get_path('module', 'entitybulkdelete') . '/entitybulkdelete.users.inc',
    );
    batch_set($batch);
  }
  $form_state['page_num'] = 1;
  $form_state['rebuild'] = TRUE;
}

/**
 * Batch operation entitybulkdelete_users_delete_batch.
 *
 * Delete users, 5 by five after each group of 5 control
 * is returned to the batch API for later continuation.
 */
function entitybulkdelete_users_delete_batch($table, $form_state, &$context) {
  if (!empty($table)) {
    foreach ($table as $value) {
      $uids[$value] = $value;
      watchdog('entitybulkdelete', 'User %uid marked as deleted.', array('%uid' => $value), WATCHDOG_NOTICE);
    }
    // Delete users.
    user_delete_multiple($uids);
  }
}

/**
 * Batch 'finished' callback used by entitybulkdelete_users_delete_batch.
 */
function entitybulkdelete_users_delete_batch_finished($success, $results, $operations) {
  if ($success) {
    $msg = t('User(s) %success has been deleted.');
    watchdog('entitybulkdelete', $msg, array('%success' => 'successfuly'), WATCHDOG_NOTICE);
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing
      @operation with arguments : @args', array(
        '@operation' => $error_operation[0],
        '@args' => print_r($error_operation[0], TRUE))));
  }
}

/**
 * Fetch list of user role.
 *
 * @param bool $reset
 *   Contain boolean value.
 *
 * @return array $roles
 *   List of roles.
 */
function entitybulkdelete_user_role($reset = FALSE) {
  $roles = &drupal_static(__FUNCTION__, NULL, $reset);
  if (!isset($roles)) {
    if (!$reset && ($cache = cache_get("entitybulkdelete:user role")) && !empty($cache->roles)) {
      $roles = $cache->roles;
    }
    else {
      // Get list of user role.
      $roles = user_roles(TRUE);
      cache_set("entitybulkdelete:user role", $roles);
    }
  }
  return $roles;
}

/**
 * Fetch list of user's name.
 *
 * @param int $rid
 *   Contain role rid.
 * @param bool $reset
 *   Content boolean value.
 *
 * @return array $users_list
 *   List of user's name.
 */
function entitybulkdelete_get_users($rid, $reset = FALSE) {
  $users_list = &drupal_static(__FUNCTION__, NULL, $reset);
  if (!isset($users_list)) {
    if (!$reset && ($cache = cache_get("entitybulkdelete:users list")) && !empty($cache->users_list)) {
      $users_list = $cache->users_list;
    }
    else {
      // Get uid from rid.
      $query = db_select('users_roles', 'r');
      $query->fields('r', array('uid'));
      $query->condition('rid', $rid);
      $usersuid = $query->execute()->fetchAllKeyed(0, 0);
      // Get list of all users from uid.
      $users_list = array();
      $query = db_select('users', 'u');
      $query->fields('u', array('uid', 'name'));
      $query->condition('uid', $usersuid, 'IN');
      $users_list = $query->execute()->fetchAllKeyed(0, 1);
      cache_set("entitybulkdelete:users list", $users_list);
    }
  }
  return $users_list;
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The name_placeholder element.
 */
function entitybulkdelete_user_callback($form, $form_state) {
  return $form['name_placeholder'];
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The name element.
 */
function entitybulkdelete_userrole_callback($form, $form_state) {
  return $form['name_placeholder']['name'];
}

/**
 * Implements callback function .
 *
 * @return renderable array
 *   The uid_placeholder element.
 */
function entitybulkdelete_uid_callback($form, $form_state) {
  return $form['uid_placeholder'];
}
