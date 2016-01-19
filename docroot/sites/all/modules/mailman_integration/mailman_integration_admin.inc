<?php

/**
 * @file
 * Page callback file for the mailman integration module.
 */

/**
 * Ajax callback for user select.
 */
function mailman_integration_user_ac_callback($string = "") {
  $matches = array();
  if ($string) {
    $result = db_select('users')
      ->fields('users', array('name', 'uid'))
      ->condition('name', db_like($string) . '%', 'LIKE')
      ->range(0, 10)
      ->execute();
    foreach ($result as $user) {
      $matches[$user->name] = check_plain($user->name);
    }
  }
  drupal_json_output($matches);
}

/**
 * Get the drupal roles.
 */
function mailman_integration_get_role_list($roles, $return_uid = 0) {
  $user_list = array();
  if (!count($roles)) {
    return $user_list;
  }
  $query = db_select('users_roles', 'ur');
  $query->leftJoin('users', 'u', 'u.uid = ur.uid');
  $query->fields('u', array('uid', 'mail', 'name'));
  $query->fields('ur', array('rid'));
  $query->condition('ur.rid', $roles, 'IN');
  $result = $query->execute();
  if ($result) {
    if ($return_uid) {
      foreach ($result as $row) {
        $user_list[$row->mail] = array('name' => $row->name, 'uid' => $row->uid);
      }
    }
    else {
      foreach ($result as $row) {
        $user_list[$row->mail] = $row->name;
      }
    }
  }
  return $user_list;
}

/**
 * Insert the mailman list into  Mailman Integration custom table.
 */
function mailman_integration_insert_list_data($params) {
  $entity_id     = ($params['entity_id']) ? $params['entity_id'] : NULL;
  $entity_type   = $params['entity_type'];
  $bundle        = $params['bundle'];
  $list_name     = $params['listname'];
  $list_owners   = isset($params['list_owner']) ? $params['list_owner'] : '';
  $list_desc     = isset($params['list_desc']) ? $params['list_desc'] : '';
  $list_visible = isset($params['visible_list']) ? $params['visible_list'] : 0;
  $insert_stmt = db_insert('mailman_integration_list');
  $insert_stmt->fields(array(
    'entity_id' => $entity_id,
    'entity_type' => $entity_type,
    'bundle' => $bundle,
    'list_name' => $list_name,
    'list_owners' => $list_owners,
    'description' => $list_desc,
    'visible_to_user' => $list_visible,
    'created' => REQUEST_TIME,
  ));
  $mailman_id = $insert_stmt->execute();
  return $mailman_id;
}

/**
 * Update the mailman list into Mailman Integration custom table.
 */
function mailman_integration_update_list_data($params) {
  $list_name     = $params['list_name'];
  $list_owners   = $params['list_owner'] ? $params['list_owner'] : '';
  $list_desc     = $params['list_desc'] ? $params['list_desc'] : '';
  $list_visible = isset($params['visible_list']) ? $params['visible_list'] : 0;
  $upd_stmt = db_update('mailman_integration_list');
  $upd_stmt->fields(array(
    'list_owners'    => $list_owners,
    'description'    => $list_desc,
    'visible_to_user' => $list_visible,
    'changed'        => REQUEST_TIME,
  ));
  $upd_stmt->condition('list_name', $list_name);
  $upd_stmt->execute();
}

/**
 * Delete the mailman list into Mailman Integration custom table.
 */
function mailman_integration_delete_list_data($params) {
  $entity_id    = ($params['entity_id']) ? $params['entity_id'] : NULL;
  $entity_type  = $params['entity_type'];
  $bundle       = $params['bundle'];
  $list_name     = $params['listname'];
  $delete_stmt = db_delete('mailman_integration_list');
  if ($entity_id) {
    $delete_stmt->condition('entity_id', $entity_id);
  }
  if ($entity_type) {
    $delete_stmt->condition('entity_type', $entity_type);
  }
  if ($bundle) {
    $delete_stmt->condition('bundle', $bundle);
  }
  if ($list_name) {
    $delete_stmt->condition('list_name', $list_name);
  }
  $is_del = $delete_stmt->execute();
  return $is_del;
}

/**
 * Select the mailman list from Mailman Integration custom table.
 */
function mailman_integration_select_list_data($params) {
  $entity_id    = ($params['entity_id']) ? $params['entity_id'] : NULL;
  $entity_type  = $params['entity_type'];
  $bundle       = $params['bundle'];
  $list_name     = $params['listname'];
  $select_stmt = db_select('mailman_integration_list', 'ma');
  $select_stmt->fields('ma', array('list_id',
    'entity_id',
    'entity_type',
    'bundle',
    'list_name',
    'list_owners',
    'description',
    'visible_to_user',
    )
  );
  if ($entity_id) {
    $select_stmt->condition('ma.entity_id', $entity_id);
  }
  if ($entity_type) {
    $select_stmt->condition('ma.entity_type', $entity_type);
  }
  if ($bundle) {
    $select_stmt->condition('ma.bundle', $bundle);
  }
  if ($list_name) {
    $select_stmt->condition('ma.list_name', $list_name);
  }
  $result = $select_stmt->execute()->fetchAll();
  if (isset($params['role_list']) && $params['role_list']) {
    $role_list = array();
    foreach ($result as $role) {
      $role_list[] = check_plain($role->entity_id);
    }
    return $role_list;
  }
  elseif (isset($params['list_type']) && $params['list_type']) {
    $list_type = array();
    foreach ($result as $res) {
      $list_type[$res->list_name] = $res->bundle;
    }
    return $list_type;
  }
  return $result;
}

/**
 * Insert the mailman list roles into  Mailman Integration roles table.
 */
function mailman_integration_insert_list_roles($params) {
  $role_id    = ($params['role_id']) ? $params['role_id'] : NULL;
  $list_id    = $params['list_id'];
  $list_name   = $params['listname'];
  $insert_stmt = db_insert('mailman_list_roles');
  $insert_stmt->fields(array(
    'list_id'   => $list_id,
    'role_id'   => $role_id,
    'list_name' => $list_name,
    'created'   => REQUEST_TIME,
    )
  );
  $list_id = $insert_stmt->execute();
  return $list_id;
}

/**
 * Remove the mailman list role into  Mailman Integration roles table.
 */
function mailman_integration_delete_list_role($params) {
  $role_id    = ($params['role_id']) ? $params['role_id'] : NULL;
  $list_id    = $params['list_id'];
  $list_name   = $params['listname'];
  $delete_stmt = db_delete('mailman_list_roles');
  if ($list_id) {
    $delete_stmt->condition('list_id', $list_id);
  }
  if ($role_id) {
    $delete_stmt->condition('role_id', $role_id);
  }
  if ($list_name) {
    $delete_stmt->condition('list_name', $list_name);
  }
  $delete_stmt->execute();
}

/**
 * Select the mailman list roles from Mailman Integration roles table.
 */
function mailman_integration_select_list_roles($params) {
  $role_id    = ($params['role_id']) ? $params['role_id'] : NULL;
  $list_id    = $params['list_id'];
  $list_name   = $params['listname'];
  $select_stmt = db_select('mailman_list_roles', 'ma');
  $select_stmt->fields('ma', array('id',
    'list_id',
    'role_id',
    'list_name',
    'created',
    )
  );
  if ($role_id) {
    $select_stmt->condition('ma.role_id', $role_id);
  }
  if ($list_id) {
    $select_stmt->condition('ma.list_id', $list_id);
  }
  if ($list_name) {
    $select_stmt->condition('ma.list_name', $list_name);
  }
  $result = $select_stmt->execute()->fetchAll();
  if (isset($params['role_list']) && $params['role_list']) {
    $role_list = array();
    foreach ($result as $role) {
      $role_list[] = check_plain($role->role_id);
    }
    return $role_list;
  }
  return $result;
}

/**
 * Get the mailman list id using list name from mailman custom table.
 *
 * @param string $list_name
 *   List name.
 */
function mailman_integration_get_list_id($list_name) {
  if (!$list_name) {
    return '';
  }
  $params = array();
  $params['entity_id']    = '';
  $params['entity_type']  = '';
  $params['bundle']       = '';
  $params['listname']     = $list_name;
  $list                   = mailman_integration_select_list_data($params);
  $list_id                 = $list[0]->list_id;
  return $list_id;
}

/**
 * Insert mailman users into mailman custom table.
 */
function mailman_integration_insert_users($list_name, $mail, $list_id = 0, $uid = 0, $created_by = 0) {
  global $user;
  if (!$created_by) {
    $created_by = ($user->uid) ? $user->uid : 0;
  }
  if (!$list_id) {
    $list_id = mailman_integration_get_list_id($list_name);
  }
  if (!$list_id || !$uid) {
    return;
  }
  $insert_stmt = db_insert('mailman_list_users');
  $insert_stmt->fields(array(
    'uid'    => $uid,
    'list_id' => $list_id,
    'mail'    => $mail,
    'created_by' => $created_by,
    'created'    => REQUEST_TIME,
  ));
  $insert_stmt->execute();
}

/**
 * List the mailman users from mailman custom table.
 */
function mailman_integration_select_users($list_id = 0, $search_val = '', $execute = 0) {
  if (!$list_id) {
    return array();
  }
  $select_stmt = db_select('mailman_list_users', 'list_user');
  $select_stmt->leftJoin('users', 'u', 'u.uid = list_user.uid');
  $select_stmt->fields('list_user', array('id',
    'uid',
    'list_id',
    'mail',
    'created_by',
    'created',
    )
  );
  $select_stmt->fields('u', array('name'));
  if ($list_id) {
    $select_stmt->condition('list_user.list_id', $list_id);
  }
  if ($search_val) {
    $select_stmt->condition(db_or()
        ->condition('u.name', '%' . db_like($search_val) . '%', 'LIKE')
        ->condition('list_user.mail', '%' . db_like($search_val) . '%', 'LIKE')
      );
  }
  if ($execute) {
    $result = $select_stmt->execute()->fetchAll();
    return $result;
  }
  else {
    return $select_stmt;
  }
}

/**
 * Delete the mailman users from mailman custom table.
 */
function mailman_integration_remove_users($list_name, $mail = 0, $list_id = 0, $force_delete_by_id = 0) {
  if (!$list_id) {
    $list_id = mailman_integration_get_list_id($list_name);
  }
  $delete_stmt = db_delete('mailman_list_users');
  if ($list_id && $mail) {
    $delete_stmt->condition('list_id', $list_id);
    $delete_stmt->condition('mail', $mail);
  }
  elseif ($force_delete_by_id) {
    $delete_stmt->condition('list_id', $list_id);
  }
  $delete_stmt->execute();
}

/**
 * Get the mailman list if user can subscribe.
 *
 * @param int $uid
 *   User Id.
 */
function mailman_integration_user_subscribe_list($uid) {
  if (!$uid) {
    return array();
  }
  $select_stmt = db_select('mailman_integration_list', 'list');
  $select_stmt->leftJoin('mailman_list_users', 'u', 'u.list_id = list.list_id and u.uid =' . $uid);
  $select_stmt->fields('list', array('list_id',
    'list_name',
    'list_owners',
    'description',
    )
  );
  $select_stmt->fields('u', array('uid'));
  $select_stmt->condition('list.visible_to_user', 1);
  $select_stmt->groupBy('list.list_id');
  $result = $select_stmt->execute()->fetchAll();
  return $result;
}

/**
 * Get the mailman list if user can subscribe.
 *
 * @param int $uid
 *   User Id.
 */
function mailman_integration_get_userlist($uid) {
  if (!$uid) {
    return array();
  }
  $select_stmt = db_select('mailman_integration_list', 'list');
  $select_stmt->leftJoin('mailman_list_users', 'u', 'u.list_id = list.list_id');
  $select_stmt->fields('list', array('list_id', 'list_name'));
  $select_stmt->condition('u.uid', $uid);
  $select_stmt->groupBy('list.list_name');
  $result = $select_stmt->execute()->fetchAll();
  return $result;
}

/**
 * Menu calback for add roll based mail list.
 */
function mailman_integration_add_role_user_callback($list_name = '') {
  $build = array();
  $build['list_form']           = drupal_get_form('mailman_integration_add_rolelist_form', $list_name);
  $build['divider1']['#markup'] = mailman_integration_container_divider();
  $build['list_user']           = drupal_get_form('mailman_integration_list_users', $list_name);
  return $build;
}

/**
 * Add role based mailman list form .
 *
 * @see mailman_integration_add_rolelist_form_validate()
 * @see mailman_integration_add_rolelist_form_submit()
 */
function mailman_integration_add_rolelist_form($form, &$form_state, $list_name = '') {
  $form = array();
  $form['add_roles'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="lists-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  $list_id = '';
  if ($list_name) {
    $list_val = mailman_integration_get_list_general($list_name);
    if (!isset($list_val['real_name'])) {
      drupal_not_found();
      drupal_exit();
    }
    $form['add_roles']['name_of_list'] = array(
      '#type' => 'hidden',
      '#name' => 'name_of_list',
      '#value' => $list_name,
    );
    $description = isset($list_val['description']) ? $list_val['description'] : '';
    $desc = mailman_integration_match_desc($description);
    $form['add_roles']['list_description'] = array(
      '#type' => 'textarea',
      '#title' => t('List Description'),
      '#default_value' => $desc['description'],
      '#rows' => 2,
    );
    $form['add_roles']['list_mail_address'] = array(
      '#type' => 'textarea',
      '#required' => TRUE,
      '#title' => t('Initial list E-Mail address'),
      '#default_value' => isset($list_val['owner']) ? $list_val['owner'] : '',
      '#description' => t('Multiple E-Mail separated by new line.'),
    );
    $list_id = mailman_integration_get_list_id($list_name);
  }
  else {
    $form['add_roles']['name_of_list'] = array(
      '#type' => 'textfield',
      '#required' => TRUE,
      '#title' => t('Name of Role List'),
    );
    $form['add_roles']['list_description'] = array(
      '#type' => 'textarea',
      '#title' => t('List Description'),
      '#rows' => 2,
    );
    $form['add_roles']['list_mail_address'] = array(
      '#type' => 'textfield',
      '#required' => TRUE,
      '#title' => t('Initial list E-Mail address'),
    );
    $form['add_roles']['initial_list_password'] = array(
      '#type' => 'password',
      '#required' => TRUE,
      '#title' => t('Initial list password'),
    );
  }
  $form['add_roles']['mailman_listid'] = array(
    '#type' => 'hidden',
    '#name' => 'mailman_listid',
    '#value' => $list_id,
  );
  $form['add_roles']['mm_form_type'] = array(
    '#type' => 'hidden',
    '#name' => 'mm_form_type',
    '#value' => ($list_name) ? 'edit' : 'add',
  );
  $form['add_roles']['roleblock'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Roles'),
    '#attributes' => array('class' => array('mailman-role-block')),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $roles = array_map('check_plain', user_roles(TRUE));
  $drupal_auth_role_id = $roles[DRUPAL_AUTHENTICATED_RID];
  $checkbox_authenticated = array(
    '#type' => 'checkbox',
    '#title' => check_plain($drupal_auth_role_id),
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );
  // unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $mailman_roles     = array();
  if ($list_name) {
    $params = array();
    $params['role_id']      = '';
    $params['list_id']      = '';
    $params['listname']     = $list_name;
    $params['role_list']    = 1;
    $mailman_roles           = mailman_integration_select_list_roles($params);
  }
  $form['add_roles']['roleblock']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => (count($mailman_roles)) ? $mailman_roles : array(),
    '#options' => $roles,
    DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
  );
  $form['add_roles']['link_mockup_start'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="submit-link-part">',
  );
  $form['add_roles']['submit'] = array(
    '#type' => 'submit',
    '#name' => 'add_rolelist',
    '#attributes' => array('class' => array('list-submit')),
    '#value' => t('Submit'),
  );
  $view_list_link = l(t('Cancel'), 'admin/config/system/mailman_integration/list', array('attributes' => array('id' => 'view_list')));
  $form['add_roles']['link_mockup'] = array(
    '#type' => 'markup',
    '#markup' => $view_list_link,
  );
  $form['add_roles']['link_mockup_end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );
  return $form;
}

/**
 * Form validation handler for mailman_integration_add_rolelist_form().
 *
 * @see mailman_integration_add_rolelist_form()
 */
function mailman_integration_add_rolelist_form_validate($form, &$form_state) {
  $trigger_element = $form_state['triggering_element']['#name'];
  $list_name = $form_state['values']['name_of_list'];
  $form_type = $form_state['values']['mm_form_type'];
  $error = 0;
  if ($form_type == 'add') {
    if (preg_match('/[^A-Za-z0-9-_]/', $list_name)) {
      form_set_error('name_of_list', t('Name of List accept only Alphanumeric, underscore(_) and hyphen(-)'));
      $error = 1;
    }
    if ($form_state['values']['list_mail_address']) {
      if (!valid_email_address($form_state['values']['list_mail_address'])) {
        form_set_error('list_mail_address', t('The Email address appears to be invalid.'));
        $error = 1;
      }
    }
    if (!$form_state['values']['list_mail_address'] || !$list_name || !$form_state['values']['initial_list_password']) {
      $error = 1;
    }
  }
  else {
    $bulk_emails = $form_state['values']['list_mail_address'];
    $mail_addrs = preg_split('/(\r?\n)+/', $bulk_emails);
    foreach ($mail_addrs as $mail) {
      $mail = trim($mail);
      if ($mail && !valid_email_address($mail)) {
        form_set_error('list_mail_address', t('%mail - Email address appears to be invalid.', array('%mail' => $mail)));
        $error = 1;
      }
    }
    if (count($mail_addrs) !== count(array_unique($mail_addrs))) {
      form_set_error('list_mail_address', t('Repeated Email address not allowed.'));
      $error = 1;
    }
    $list_val = mailman_integration_get_list_general($list_name);
    if (!isset($list_val['real_name']) || strtolower($list_val['real_name']) != strtolower($list_name)) {
      form_set_error('name_of_list', t('Invalid Mailman List'));
      $error = 1;
    }
    if (!$form_state['values']['list_mail_address'] || !$list_name) {
      $error = 1;
    }
  }
  if (!$error && $trigger_element == 'add_rolelist') {
    if ($form_type == 'add') {
      $result = mailman_integration_createlist($form_state['values']);
      if ($result['status']) {
        $params = array();
        $params['entity_id']    = '';
        $params['entity_type']  = 'role';
        $params['bundle']       = 'role';
        $params['listname']     = $list_name;
        $params['list_owner']   = $form_state['values']['list_mail_address'];
        $params['list_desc']    = $form_state['values']['list_description'];
        $mailman_id = mailman_integration_insert_list_data($params);
      }
    }
    else {
      $admin_url = mailman_integration_get_admin_url();
      $authenticate_pass = mailman_integration_get_auth_pass();
      $mailman   = new MailmanIntegration($admin_url, '', $authenticate_pass, $list_name);
      $list_desc  = $form_state['values']['list_description'];
      $upd_params = array();
      $upd_params['description'] = $list_desc;
      $upd_params['owner']       = $form_state['values']['list_mail_address'];
      $upd_params['real_name']   = $list_name;
      $mailman->mailmanListUpdate($upd_params);
      // Update the list owners in custom mailman data.
      $upd_custom               = array();
      $upd_custom['list_name']  = $list_name;
      $upd_custom['list_owner'] = $form_state['values']['list_mail_address'];
      $upd_custom['list_desc']  = $list_desc;
      mailman_integration_update_list_data($upd_custom);
      $result['status'] = 1;
      $mailman_id = $form_state['values']['mailman_listid'];
    }
    if (!$result['status']) {
      form_set_error('name_of_list', t('Unable to create Mailman List. %msg', array('%msg' => $result['msg'])));
    }
    else {
      $form_value_roles = $form_state['values']['roles'];
      if (($key = array_search(DRUPAL_ANONYMOUS_RID, $form_value_roles)) !== FALSE) {
        unset($form_value_roles[$key]);
      }
      $roles = array_flip($form_value_roles);
      unset($roles[0]);
      $submitted_roles   = array_keys($roles);
      $old_roles = array();
      if ($list_name) {
        $params = array();
        $params['role_id']      = '';
        $params['list_id']      = '';
        $params['listname']     = $list_name;
        $params['role_list']    = 1;
        $old_roles               = mailman_integration_select_list_roles($params);
      }
      $removed_role    = array_diff($old_roles, $submitted_roles);
      $added_role      = array_diff($submitted_roles, $old_roles);
      if (count($added_role) || count($removed_role)) {
        if (count($added_role)) {
          $old_role_diff    = array();
          $added_role_diff  = array();
          $old_role_list    = mailman_integration_get_role_list($old_roles, 1);
          foreach ($old_role_list as $mail => $row) {
            $old_role_diff[$mail] = $row['name'];
          }
          $added_role_list  = mailman_integration_get_role_list($added_role, 1);
          foreach ($added_role_list as $mail => $row) {
            $added_role_diff[$mail] = $row['name'];
          }
          $want_to_add_mailman      = array_diff($added_role_diff, $old_role_diff);
          if (count($want_to_add_mailman)) {
            // Subcribe $want_to_add_mailman list.
            foreach ($want_to_add_mailman as $mail => $name) {
              $already_member = mailman_integration_is_member_inlist($list_name, $mail);
              if (!count($already_member)) {
                mailman_integration_subscribe($list_name, $mail);
                // Update user option.
                mailman_integration_set_user_option($list_name, $mail, 'fullname', $name);
                // Insert into mailman user table.
                mailman_integration_insert_users($list_name, $mail, $mailman_id, $added_role_list[$mail]['uid']);
              }
            }
          }
          foreach ($added_role as $role_val) {
            $params = array();
            $params['role_id']   = $role_val;
            $params['listname']  = $list_name;
            $params['list_id']   = $mailman_id;
            mailman_integration_insert_list_roles($params);
          }
        }
        if (count($removed_role)) {
          $removed_role_list    = mailman_integration_get_role_list($removed_role);
          $submitted_role_list  = mailman_integration_get_role_list($submitted_roles);
          $want_to_remove_mailman = array_diff($removed_role_list, $submitted_role_list);
          if (count($want_to_remove_mailman)) {
            // Unsubcribe $want_to_remove_mailman list.
            foreach ($want_to_remove_mailman as $mail => $name) {
              mailman_integration_unsubscribe($list_name, $mail);
              // Remove from mailman user table.
              mailman_integration_remove_users($list_name, $mail, $mailman_id);
            }
          }
          foreach ($removed_role as $role_val) {
            $params = array();
            $params['role_id']    = $role_val;
            $params['list_id']    = $mailman_id;
            $params['listname']   = $list_name;
            mailman_integration_delete_list_role($params);
          }
        }
      }
    }
  }
}

/**
 * Form submit handler for mailman_integration_add_rolelist_form().
 *
 * @see mailman_integration_add_rolelist_form()
 */
function mailman_integration_add_rolelist_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/system/mailman_integration/list/rolelist/user/' . $form_state['values']['name_of_list'];
}

/**
 * Simple diver css class.
 */
function mailman_integration_container_divider() {
  $build = '<div class="mailman-divider"></div>';
  return $build;
}

/**
 * Add Manual mailman list form.
 *
 * @see mailman_integration_create_list_form_validate()
 * @see mailman_integration_create_list_form_submit()
 */
function mailman_integration_create_list_form($form, &$form_state) {
  $form = array();
  $form['add_list'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="lists-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  $form['add_list']['name_of_list'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Name of List'),
  );
  $form['add_list']['list_description'] = array(
    '#type' => 'textarea',
    '#title' => t('List Description'),
    '#rows' => 2,
  );
  $form['add_list']['list_mail_address'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Initial list E-Mail address'),
  );
  $form['add_list']['user_can_subscribe'] = array(
    '#type' => 'checkbox',
    '#title' => t('User can Subscribe'),
    '#description' => t('If checked, User can Subscribe this List.'),
  );
  $form['add_list']['initial_list_password'] = array(
    '#type' => 'password',
    '#required' => TRUE,
    '#title' => t('Initial list password'),
  );
  $form['add_list']['userblock'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Users'),
    '#attributes' => array('class' => array('mailman-user-block')),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if (empty($form_state['no_of_users'])) {
    $form_state['no_of_users'] = 1;
  }
  for ($i = 0; $i < $form_state['no_of_users']; $i++) {
    $form['add_list']['userblock']['user_name'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('User Name'),
      '#autocomplete_path' => 'mailman_integration/ajax/user_ac_callback',
    );
  }
  $form['add_list']['userblock']['add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add User'),
    '#name' => 'add_more',
    '#submit' => array('mailman_integration_user_add_one'),
    '#ajax' => array(
      'callback' => 'mailman_integration_list_user_callback',
      'wrapper' => 'lists-fieldset-wrapper',
    ),
    '#limit_validation_errors' => array(),
  );
  if ($form_state['no_of_users'] > 1) {
    $form['add_list']['userblock']['remove_name'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#name' => 'remove_one',
      '#submit' => array('mailman_integration_list_user_remove_one'),
      '#ajax' => array(
        'callback' => 'mailman_integration_list_user_callback',
        'wrapper' => 'lists-fieldset-wrapper',
      ),
      '#limit_validation_errors' => array(),
    );
  }
  $form['add_list']['submit'] = array(
    '#type' => 'submit',
    '#name' => 'add_list',
    '#attributes' => array('class' => array('list-submit')),
    '#value' => t('Submit'),
  );
  $view_list_link = l(t('Back'), 'admin/config/system/mailman_integration/list', array('attributes' => array('id' => 'view_list')));
  $form['add_list']['link_mockup'] = array(
    '#type' => 'markup',
    '#markup' => $view_list_link,
  );
  return $form;
}

/**
 * Selects and returns the fieldset with the names in it.
 */
function mailman_integration_list_user_callback($form, $form_state) {
  return $form['add_list'];
}

/**
 * Increments the max counter and causes a rebuild.
 */
function mailman_integration_user_add_one($form, &$form_state) {
  if (!isset($form_state['no_of_users'])) {
    $form_state['no_of_users'] = 1;
  }
  $form_state['no_of_users']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Decrements the max counter and causes a form rebuild.
 */
function mailman_integration_list_user_remove_one($form, &$form_state) {
  if ($form_state['no_of_users'] > 1) {
    $form_state['no_of_users']--;
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Form validate handler for mailman_integration_create_list_form().
 *
 * @see mailman_integration_create_list_form()
 */
function mailman_integration_create_list_form_validate($form, &$form_state) {
  $trigger_element = $form_state['triggering_element']['#name'];
  $list_name = $form_state['values']['name_of_list'];
  if ($trigger_element == 'add_more' || $trigger_element == 'remove_one') {
    return;
  }
  $error = 0;
  if (preg_match('/[^A-Za-z0-9-_]/', $list_name)) {
    form_set_error('name_of_list', t('Name of List accept only Alphanumeric, underscore(_) and hyphen(-)'));
    $error = 1;
  }
  if ($form_state['values']['list_mail_address']) {
    if (!valid_email_address($form_state['values']['list_mail_address'])) {
      form_set_error('list_mail_address', t('The Email address appears to be invalid.'));
      $error = 1;
    }
  }
  if (!$form_state['values']['list_mail_address'] || !$list_name || !$form_state['values']['initial_list_password']) {
    $error = 1;
  }
  foreach ($form['add_list']['userblock']['user_name'] as $key => $user_val) {
    if (isset($user_val['#type']) && $user_val['#type'] == 'textfield' && $user_val['#value']) {
      $user = user_load_by_name($user_val['#value']);
      if (!$user) {
        form_set_error($key, t('Invalid User - %val', array('%val' => $user_val['#value'])));
        $error = 1;
      }
    }
  }
  if (!$error && $trigger_element == 'add_list') {
    $result = mailman_integration_createlist($form_state['values']);
    if (!$result['status']) {
      $msg = isset($result['msg']) ? $result['msg'] : '';
      form_set_error('name_of_list', t('Unable to create Mailman List. %msg', array('%msg' => $msg)));
    }
    else {
      // Insert the mailman list into  Mailman Integration custom table.
      $params = array();
      $params['entity_id']    = '';
      $params['entity_type']  = '';
      $params['bundle']       = 'manual';
      $params['listname']     = $list_name;
      $params['list_owner']   = $form_state['values']['list_mail_address'];
      $params['list_desc']    = $form_state['values']['list_description'];
      $params['visible_list'] = $form_state['values']['user_can_subscribe'];
      $mailman_id = mailman_integration_insert_list_data($params);
      foreach ($form['add_list']['userblock']['user_name'] as $key => $user_val) {
        if (isset($user_val['#type']) && $user_val['#type'] == 'textfield' && $user_val['#value']) {
          $user = user_load_by_name($user_val['#value']);
          if ($user->mail) {
            $already_member = mailman_integration_is_member_inlist($list_name, $user->mail);
            if (!count($already_member)) {
              mailman_integration_subscribe($list_name, $user->mail);
              // Update user option.
              mailman_integration_set_user_option($list_name, $user->mail, 'fullname', $user->name);
              // Insert into mailman user table.
              mailman_integration_insert_users($list_name, $user->mail, $mailman_id, $user->uid);
            }
          }
        }
      }
    }
  }
}

/**
 * Form submit handler for mailman_integration_create_list_form().
 *
 * @see mailman_integration_create_list_form()
 */
function mailman_integration_create_list_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/system/mailman_integration/list/user/' . $form_state['values']['name_of_list'];
}

/**
 * API call for create mailman list.
 */
function mailman_integration_createlist($values) {
  $admin_url = mailman_integration_get_admin_url();
  $list_pass = ($values['initial_list_password']) ? $values['initial_list_password'] : mailman_integration_get_auth_pass();
  $authenticate_pass = mailman_integration_get_auth_pass();
  try {
    $mailman = new MailmanIntegration($admin_url, $list_pass, $authenticate_pass, $values['name_of_list']);
    $params = array();
    $params['autogen'] = 0;
    $params['doit']    = 'Create List';
    $params['langs']   = 'en';
    $params['listname'] = $values['name_of_list'];
    $params['moderate'] = 0;
    $params['notify']  = 1;
    $params['owner']   = $values['list_mail_address'];
    $create = $mailman->mailmanListCreate($params);
    // $create = $mailman->mailmanListCreateByCmd($params);
    // Update the list once created.
    if ($create) {
      $list_desc  = $values['list_description'];
      $upd_params = array();
      $upd_params['description'] = $list_desc;
      $upd_params['real_name'] = $values['name_of_list'];
      $mailman->mailmanListUpdate($upd_params);
      // By Default List creadted by Public - Visible to public.
      /*
      $upd_params = array();
      $upd_params['description'] = $list_desc;
      // 0 => private, 1 => public
      $upd_params['advertised'] = 0;
      $update_privacy = $mailman->mailmanListSetPrivate($upd_params);
       */
    }
    return $create;
  }
  catch (MailmanIntegrationException $e) {
    mailman_integration_set_error_msg($e);
  }
}

/**
 * Edit mailman list page.
 */
function mailman_integration_add_user_callback($list_name) {
  $build = array();
  $build['list_form']           = drupal_get_form('mailman_integration_subscribe_user_form', $list_name);
  $build['divider1']['#markup'] = mailman_integration_container_divider();
  $build['list_user']           = drupal_get_form('mailman_integration_list_users', $list_name);
  return $build;
}

/**
 * Edit manual mailman list form.
 *
 * @see mailman_integration_subscribe_user_form_validate()
 * @see mailman_integration_subscribe_user_form_submit()
 */
function mailman_integration_subscribe_user_form($form, $form_state, $list_name) {
  $form = array();
  $form['add_list'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="lists-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  $list_val = mailman_integration_get_list_general($list_name);
  if (!isset($list_val['real_name'])) {
    // Calling to page not found.
    drupal_not_found();
    drupal_exit();
  }
  $form['add_list']['name_of_list'] = array(
    '#type' => 'hidden',
    '#name' => 'name_of_list',
    '#value' => $list_name,
  );
  $form['add_list']['list_mail_address'] = array(
    '#type' => 'textarea',
    '#required' => TRUE,
    '#title' => t('Initial list E-Mail address'),
    '#default_value' => isset($list_val['owner']) ? $list_val['owner'] : '',
    '#description' => t('Multiple E-Mail separated by new line.'),
  );
  $description = isset($list_val['description']) ? $list_val['description'] : '';
  $desc        = mailman_integration_match_desc($description);
  $form['add_list']['list_description'] = array(
    '#type' => 'textarea',
    '#title' => t('List Description'),
    '#default_value' => $desc['description'],
    '#rows' => 2,
  );
  $params = array();
  $params['entity_id']    = '';
  $params['entity_type']  = '';
  $params['bundle']       = '';
  $params['listname']     = $list_name;
  $list                   = mailman_integration_select_list_data($params);
  $form['add_list']['user_can_subscribe'] = array(
    '#type' => 'checkbox',
    '#title' => t('User can Subscribe'),
    '#description' => t('If checked, User can Subscribe this List.'),
    '#default_value' => $list[0]->visible_to_user,
  );
  $form['add_list']['userblock'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Users'),
    '#attributes' => array('class' => array('mailman-user-block')),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // $form_state['no_of_users'] to determine the number of textfields to build.
  if (empty($form_state['no_of_users'])) {
    $form_state['no_of_users'] = 1;
  }
  for ($i = 0; $i < $form_state['no_of_users']; $i++) {
    $div_start = ($i == ($form_state['no_of_users'] - 1)) ? '<div id="subscribe-block">' : '';
    $form['add_list']['userblock']['user_name'][$i] = array(
      '#type' => 'textfield',
      '#title' => t('User Name'),
      '#autocomplete_path' => 'mailman_integration/ajax/user_ac_callback',
      '#prefix' => $div_start,
    );
  }
  $form['add_list']['userblock']['add_name'] = array(
    '#type' => 'submit',
    '#value' => t('Add User'),
    '#name' => 'add_more',
    '#submit' => array('mailman_integration_user_add_one'),
    '#ajax' => array(
      'callback' => 'mailman_integration_list_user_callback',
      'wrapper' => 'lists-fieldset-wrapper',
    ),
    '#limit_validation_errors' => array(),
  );
  if ($form_state['no_of_users'] > 1) {
    $form['add_list']['userblock']['remove_name'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#name' => 'remove_one',
      '#submit' => array('mailman_integration_list_user_remove_one'),
      '#ajax' => array(
        'callback' => 'mailman_integration_list_user_callback',
        'wrapper' => 'lists-fieldset-wrapper',
      ),
      '#limit_validation_errors' => array(),
    );
  }
  $form['add_list']['userblock']['moreuser_mockup'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );
  $form['add_list']['link_mockup_start'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="submit-link-part">',
  );
  $form['add_list']['submit'] = array(
    '#type' => 'submit',
    '#name' => 'add_list',
    '#attributes' => array('class' => array('list-submit')),
    '#value' => t('Submit'),
  );
  $view_list_link = l(t('Cancel'), 'admin/config/system/mailman_integration/list', array('attributes' => array('id' => 'view_list')));
  $form['add_list']['link_mockup'] = array(
    '#type' => 'markup',
    '#markup' => $view_list_link,
  );
  $form['add_list']['link_mockup_end'] = array(
    '#type' => 'markup',
    '#markup' => '</div>',
  );
  return $form;
}

/**
 * Form validate handler for mailman_integration_subscribe_user_form().
 *
 * @see mailman_integration_subscribe_user_form()
 */
function mailman_integration_subscribe_user_form_validate(&$form, &$form_state) {
  $trigger_element = $form_state['triggering_element']['#name'];
  if ($trigger_element == 'remove_one' || $trigger_element == 'add_more') {
    return;
  }
  $bulk_emails = $form_state['values']['list_mail_address'];
  $list_name    = $form_state['values']['name_of_list'];
  $mail_addrs  = preg_split('/(\r?\n)+/', $bulk_emails);
  foreach ($mail_addrs as $key => $mail) {
    $mail = trim($mail);
    if ($mail && !valid_email_address($mail)) {
      form_set_error('list_mail_address', t('%mail - Email address appears to be invalid.', array('%mail' => $mail)));
    }
  }
  if (count($mail_addrs) !== count(array_unique($mail_addrs))) {
    form_set_error('list_mail_address', t('Repeated Email address not allowed.'));
  }
  foreach ($form['add_list']['userblock']['user_name'] as $key => $user_val) {
    if (isset($user_val['#type']) && $user_val['#type'] == 'textfield' && $user_val['#value']) {
      $user = user_load_by_name($user_val['#value']);
      if (!$user) {
        form_set_error($key, t('Invalid User - %val', array('%val' => $user_val['#value'])));
      }
      else {
        $already_member = mailman_integration_is_member_inlist($list_name, $user->mail);
        if (count($already_member)) {
          form_set_error($key, t('User already Exists1 - %val', array('%val' => $user_val['#value'])));
        }
      }
    }
  }
  $list_val = mailman_integration_get_list_general($list_name);
  if (!isset($list_val['real_name']) || strtolower($list_val['real_name']) != strtolower($list_name)) {
    form_set_error('name_of_list', t('Invalid Mailman List'));
  }
}

/**
 * Form submit handler for mailman_integration_subscribe_user_form().
 *
 * @see mailman_integration_subscribe_user_form()
 */
function mailman_integration_subscribe_user_form_submit(&$form, &$form_state) {
  $trigger_element = $form_state['triggering_element']['#name'];
  if ($trigger_element == 'remove_one' || $trigger_element == 'add_more') {
    return;
  }
  $list_name = $form_state['values']['name_of_list'];
  // Update the list owner and description.
  $admin_url = mailman_integration_get_admin_url();
  $authenticate_pass = mailman_integration_get_auth_pass();
  $mailman = new MailmanIntegration($admin_url, '', $authenticate_pass, $list_name);
  $upd_params = array();
  $list_desc  = $form_state['values']['list_description'];
  $upd_params['description'] = $list_desc;
  $upd_params['owner']       = $form_state['values']['list_mail_address'];
  $upd_params['real_name']   = $list_name;
  $mailman->mailmanListUpdate($upd_params);
  // Update the list owners in custom mailman data.
  $upd_custom                = array();
  $upd_custom['list_name']   = $list_name;
  $upd_custom['list_owner']  = $form_state['values']['list_mail_address'];
  $upd_custom['list_desc']   = $list_desc;
  $upd_custom['visible_list'] = $form_state['values']['user_can_subscribe'];
  mailman_integration_update_list_data($upd_custom);
  foreach ($form['add_list']['userblock']['user_name'] as $user_val) {
    if (isset($user_val['#type']) && $user_val['#type'] == 'textfield' && $user_val['#value']) {
      $user = user_load_by_name($user_val['#value']);
      if ($user) {
        $already_member = mailman_integration_is_member_inlist($list_name, $user->mail);
        if (!count($already_member)) {
          mailman_integration_subscribe($list_name, $user->mail);
          // Update user option.
          mailman_integration_set_user_option($list_name, $user->mail, 'fullname', $user->name);
          mailman_integration_insert_users($list_name, $user->mail, 0, $user->uid);
        }
      }
      if (!$list_name) {
        form_set_error('name_of_list', t('Error in user Subscribe'));
      }
    }
  }
}

/**
 * List the mailman users form with tableselect theme.
 *
 * @see mailman_integration_list_users_validate()
 * @see mailman_integration_list_users_submit()
 */
function mailman_integration_list_users($form, $form_state, $list_name, $search_input = array()) {
  if (!$list_name) {
    return array();
  }
  $form = array();
  $search_input = isset($_GET['name']) && $_GET['name'] ? $_GET['name'] : '';
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search Users'),
    '#attributes' => array('class' => array('mailman-role-block')),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['search']['name'] = array(
    '#title' => t('Name/Email'),
    '#type' => 'textfield',
    '#attributes' => array(
      'id' => 'search-name',
    ),
    '#default_value' => $search_input,
  );
  $form['search']['submit'] = array(
    '#value' => t('Search'),
    '#type'  => 'submit',
    '#prefix' => '<div id="search-submit">',
    '#name'  => 'search_submit',
    // '#limit_validation_errors' => array(),
  );
  $form['search']['reset'] = array(
    '#value'  => t('Reset'),
    '#type'   => 'submit',
    '#suffix' => '</div>',
    '#attributes' => array(
      'class' => array('btn-primary'),
      'id'    => 'reset-submit',
    ),
    '#name'   => 'reset',
    // '#limit_validation_errors' => array(),
  );
  $list_id = mailman_integration_get_list_id($list_name);
  $header = array(
    'name' => array(
      'data' => t('Name'),
      'width' => '40%',
      'field' => 'name',
      'sort' => 'asc',
    ),
    'mail' => array('data' => t('Email'), 'width' => '50%', 'field' => 'mail'),
  );
  if ($search_input) {
    $lists_qry = mailman_integration_select_users($list_id, $search_input);
  }
  else {
    $lists_qry = mailman_integration_select_users($list_id);
  }
  $select_stmt = $lists_qry->extend('TableSort')->orderByHeader($header)->extend('PagerDefault')->limit(variable_get('mailman_integration_list_pagination', 10));
  $lists = $select_stmt->execute();
  $rows = array();
  foreach ($lists as $list) {
    $rows[$list->mail] = array(
      'name' => array('data' => $list->name),
      'mail' => array('data' => $list->mail),
    );
  }
  $form['user_list']['name_of_list'] = array(
    '#type' => 'hidden',
    '#name' => 'name_of_list',
    '#value' => $list_name,
  );
  $form['user_list']['mailman_listid'] = array(
    '#type' => 'hidden',
    '#name' => 'mailman_listid',
    '#value' => $list_id,
  );
  $form['user_list']['user_table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No users found'),
    '#attributes' => array('class' => array('list-mailman-user')),
  );
  if (count($rows)) {
    $form['user_list']['submit'] = array(
      '#type' => 'submit',
      '#name' => 'delete_selected',
      '#value' => t('Delete Selected'),
    );
  }
  $form['pager'] = array('#theme' => 'pager');
  return $form;
}

/**
 * Form validate handler for mailman_integration_list_users().
 *
 * @see mailman_integration_list_users()
 */
function mailman_integration_list_users_validate(&$form, &$form_state) {
  $trigger_element = $form_state['triggering_element']['#name'];
  if ($trigger_element == 'delete_selected') {
    $delete_list = array_filter($form_state['values']['user_table']);
    if (!count($delete_list)) {
      form_set_error('', t('Choose any one of user.'));
    }
  }
}

/**
 * Form submit handler for mailman_integration_list_users().
 *
 * @see mailman_integration_list_users()
 */
function mailman_integration_list_users_submit(&$form, &$form_state) {
  $trigger_element = $form_state['triggering_element']['#name'];
  if ($trigger_element == 'delete_selected') {
    $delete_list = array_filter($form_state['values']['user_table']);
    $list_name   = $form_state['values']['name_of_list'];
    $list_id     = $form_state['values']['mailman_listid'];
    foreach ($delete_list as $mail => $name) {
      mailman_integration_unsubscribe($list_name, $mail);
      // Remove from mailman user table.
      mailman_integration_remove_users($list_name, $mail, $list_id);
    }
  }
  elseif ($trigger_element == 'reset') {
    $form_state['redirect'] = $_GET['q'];
    return;
  }
  elseif ($trigger_element == 'search_submit') {
    $name  = $form_state['values']['name'];
    $qs = array();
    if (isset($name) && $name) {
      $qs['name'] = $name;
    }
    $options = array('query' => $qs);
    drupal_goto($_GET['q'], $options);
  }
}

/**
 * API call for user general set option.
 */
function mailman_integration_set_user_option($list_name, $user_mail, $opt, $val) {
  $admin_url = mailman_integration_get_admin_url();
  if (!$admin_url) {
    return array();
  }
  $authenticate_pass = mailman_integration_get_auth_pass();
  try {
    $mailman = new MailmanIntegration($admin_url, '', $authenticate_pass, $list_name);
    $set_option = $mailman->setMailmanUserOption($user_mail, $opt, $val);
    return $set_option;
  }
  catch (MailmanIntegrationException $e) {
    mailman_integration_set_error_msg($e);
  }
}

/**
 * API call for getting mailman list general option.
 *
 * @param string $list_name
 *   List name.
 *
 * @return array $list_val
 *   Array of list option.
 */
function mailman_integration_get_list_general($list_name) {
  $admin_url = mailman_integration_get_admin_url();
  if (!$admin_url) {
    return array();
  }
  $authenticate_pass = mailman_integration_get_auth_pass();
  try {
    $mailman = new MailmanIntegration($admin_url, '', $authenticate_pass, $list_name);
    $list_val = $mailman->getMailmanListGeneral();
    return $list_val;
  }
  catch (MailmanIntegrationException $e) {
    mailman_integration_set_error_msg($e);
  }
}

/**
 * Parse mailman list description.
 */
function mailman_integration_match_desc($desc = '') {
  $match_value = array();
  if (stripos($desc, 'no description available') !== FALSE) {
    $desc = '';
  }
  $match_value['description'] = $desc;
  return $match_value;
}

/**
 * Menu callback for Mailman integration user form.
 *
 * @see mailman_integration_user_form_submit()
 */
function mailman_integration_user_form($form, &$form_state, $account = NULL) {
  $form['mailman_integration']['mail_lists'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mailing Lists'),
    '#description' => t('Subscribe or unsubscribe to a mailing list.'),
    '#collapsible' => FALSE,
    '#attributes' => array('id' => array('mailman_integration_userlist'), 'class' => array('mailman_userlist_fieldset')),
  );
  $mail_lists = mailman_integration_user_subscribe_list($account->uid);
  if (empty($mail_lists) || !$account->uid) {
    $form['mailman_integration']['mail_lists']['empty_list'] = array(
      '#type' => 'markup',
      '#prefix' => '<p>',
      '#markup' => t('No mailing lists are available.'),
      '#suffix' => '</p>',
    );
    return $form;
  }
  foreach ($mail_lists as $list) {
    $list_id = $list->list_id;
    $description = mailman_integration_match_desc($list->description);
    $desc = $description['description'];
    $form['mailman_integration']['mail_lists']['list-' . $list_id] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($list->list_name),
      '#description' => $desc,
      '#collapsible' => TRUE,
      '#attributes' => array('class' => array('mailman_userlist_inner_fieldset')),
    );
    $form['mailman_integration']['mail_lists']['list-' . $list_id]['subscribe-' . $list_id] = array(
      '#type' => 'checkbox',
      '#title' => t('Subscribe'),
      '#default_value' => ($list->uid) ? 1 : 0,
      '#description' => t('Check for subscribe this mailing list.'),
      '#weight' => 1,
    );
  }
  if (!empty($mail_lists)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['mailman_user_account'] = array(
      '#type' => 'value',
      '#value' => $account,
    );
    $form['mailman_user_old_status'] = array(
      '#type' => 'value',
      '#value' => $mail_lists,
    );
  }
  return $form;
}

/**
 * Form submit handler for mailman_integration_user_form().
 *
 * @see mailman_integration_user_form()
 */
function mailman_integration_user_form_submit(&$form, &$form_state) {
  global $user;
  $old_subscribe_status = $form_state['values']['mailman_user_old_status'];
  $acount = $form_state['values']['mailman_user_account'];
  foreach ($old_subscribe_status as $value) {
    $subscribe_status = $form_state['values']['subscribe-' . $value->list_id];
    $old_status = ($value->uid) ? $value->uid : '';
    $list_name = $value->list_name;
    $changes = 0;
    if (!$old_status && $subscribe_status) {
      $changes = 1;
      // Subscribe Mailman.
      mailman_integration_subscribe($list_name, $acount->mail);
      // Update user option.
      mailman_integration_set_user_option($list_name, $acount->mail, 'fullname', $acount->name);
      // Insert into mailman user table.
      mailman_integration_insert_users($list_name, $acount->mail, $value->list_id, $acount->uid, $user->uid);
    }
    elseif ($old_status && !$subscribe_status) {
      $changes = 1;
      // Unsubscribe Mailman.
      mailman_integration_unsubscribe($list_name, $acount->mail);
      // Remove from mailman user table.
      mailman_integration_remove_users($list_name, $acount->mail, $value->list_id);
    }
    if ($changes) {
      drupal_set_message(t('The changes have been saved.'));
    }
  }
}
