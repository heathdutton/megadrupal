<?php
/**
 * @file
 *  Admin page implementations.
 */

/**
 * General module settings form.
 */
function og_default_members_admin_form($form, &$form_state) {
  $form['#theme'] = 'og_default_members_admin_form';

  // Load the default group members.
  $default_users = variable_get('og_default_members_default_users', array());
  if (!empty($default_users)) {
    foreach ($default_users as $key => $uid) {
      $user = entity_load_single('user', $uid);
      if (!$user) {
        unset($default_users[$key]);
        variable_set('og_default_members_default_users', $default_users);
        continue;
      }

      $form['og_default_members_default_users'][$uid]['name'] = array(
        '#type' => 'link',
        '#title' => $user->name,
        '#href' => "user/$uid",
      );

      $form['og_default_members_default_users'][$uid]['mail'] = array(
        '#markup' => $user->mail,
      );

      $query = array();
      $query['token'] = drupal_get_token("og-default-members/remove-default-member/$uid");
      $form['og_default_members_default_users'][$uid]['delete'] = array(
        '#markup' => l('delete', "og-default-members/remove-default-member/$uid",
          array('query' => $query)),
      );
    }
  }

  $form['og_default_memberes_default_user_description'] = array(
    '#prefix' => '<div class="description">',
    '#markup' => t("Default membership for all Organic groups in the system. For trusted roles only. To manage user membership for individual groups, please visit the group specific settings page. Delete will only remove the user from this list, but won't remove the user from the assigned Groups."),
    '#suffix' => '</div>',
  );

  $form['og_default_members_default_member'] = array(
    '#type' => 'textfield',
    '#title' => t('Default member user lookup'),
    '#description' => t(''),
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['og_default_members_add_default_member_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add member'),
    '#submit' => array('og_default_members_add_default_member_submit'),
  );

  $form['initialize_memberships'] = array(
    '#type' => 'submit',
    '#value' => t('Initialize default memberships'),
    '#submit' => array('og_default_members_initialize_memberships_submit'),
  );


  $form['og_default_members_enabled_roles_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default roles'),
    '#description' => t('These roles are coming from the system specific roles. The selected roles will be available on Group specific settings page.'),
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
  );

  $form['og_default_members_enabled_roles_fieldset']['og_default_members_enabled_roles'] = array(
    '#title' => '',
    '#type' => 'checkboxes',
    '#options' => user_roles(TRUE),
    '#default_value' => variable_get('og_default_members_enabled_roles', array()),
  );

  $form['og_default_members_enabled_roles_fieldset']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Process the og_default_members_admin_form and save the configurations.
 */
function og_default_members_admin_form_submit($form, &$form_state) {
  $diff = array();
  $default_roles = variable_get('og_default_members_enabled_roles', array());

  // Collect roles that became disabled after the last form submit.
  if (!empty($default_roles)) {
    foreach ($default_roles as $rid => $value) {
      // Search for roles that become disabled.
      if ($value != 0 && $form_state['values']['og_default_members_enabled_roles'][$rid] == 0) {
        $diff[] = $rid;
      }
    }
  }

  // Clear disabled roles from the database.
  if (!empty($diff)) {
    _og_default_members_remove_default_group_roles($diff);
  }

  // Save the default roles settings.
  variable_set('og_default_members_enabled_roles', $form_state['values']['og_default_members_enabled_roles']);

  drupal_set_message(t("The configurations have been saved."));
}

/**
 * Submit callback to extend the default group members with the given user.
 */
function og_default_members_add_default_member_submit(&$form, &$form_state) {
  // Load default group members.
  $default_users = variable_get('og_default_members_default_users', array());
  if (!empty($form_state['values']['og_default_members_default_member'])) {
    // Load the user.
    $user = user_load_by_name($form_state['values']['og_default_members_default_member']);

    if (!empty($user)) {
      $default_users[$user->uid] = $user->uid;
      variable_set('og_default_members_default_users', $default_users);
      drupal_set_message(t("The configurations have been saved."));
    }
  }
}

/**
 * Remove the given user from the default group members list.
 */
function og_default_members_remove_default_member($form, &$form_state, $user) {
  // Check the token is valid.
  if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], "og-default-members/remove-default-member/$user->uid")) {
    return MENU_ACCESS_DENIED;
  }

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to take the default group member privilege from: %user?', array('%user' => $user->name)),
    'admin/config/group/default-members',
    t('After this operation the user will keep each membership.'),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Remove a user from the default group members list.
 */
function og_default_members_remove_default_member_submit($form, &$form_state) {
  $uid = $form_state['values']['uid'];
  $default_users = variable_get('og_default_members_default_users', array());

  if (!empty($default_users[$uid])) {
    unset($default_users[$uid]);
    variable_set('og_default_members_default_users', $default_users);
  }

  $form_state['redirect'] = 'admin/config/group/default-members';
}

/**
 * Initialize default memberships.
 */
function og_default_members_initialize_memberships_submit(&$form, &$form_state) {
  _og_default_members_initialize_memberships();
}

/**
 * Generates the default members form.
 *
 * @param $form
 * @param $form_state
 * @param $group_type
 * @param $gid
 *
 * @return
 *   The form array.
 */
function og_default_members_manage_default_members($form, &$form_state, $group_type, $gid) {
  // Set breadcrumb based on the context.
  og_set_breadcrumb($group_type, $gid, array(l(t('Group'), "$group_type/$gid/group")));
  $form['group_type'] = array('#type' => 'value', '#value' => $group_type);
  $form['gid'] = array('#type' => 'value', '#value' => $gid);

  $options = array();
  // Load roles that was enabled on the admin configuration page.
  $enabled_roles = variable_get('og_default_members_enabled_roles', array());
  foreach (user_roles(TRUE) as $rid => $role_name) {
    if (!empty($enabled_roles[$rid])) {
      $options[$rid] = $role_name;
    }
  }

  $form['roles'] = array(
    '#title' => t('Default roles'),
    '#description' => t("Users with the selected roles will become members of this group after <em>Save and Update</em>. If you don't want to import users right now, then just use <em>Save</em>, users will become members after saving their user profiles."),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => _og_default_members_get_roles_by_group($gid),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['import'] = array(
    '#type' => 'submit',
    '#value' => t('Save and Update'),
    '#submit' => array('og_default_members_admin_members_import'),
  );

  return $form;
}

/**
 * Save a group default roles.
 */
function og_default_members_manage_default_members_submit($form, &$form_state) {
  if (!empty($form_state['values']['roles'])) {
    $gid = $form_state['values']['gid'];
    $group_type = $form_state['values']['group_type'];
    $roles = $form_state['values']['roles'];
    _og_default_members_save($gid, $group_type, $roles);

    drupal_set_message(t("The configurations have been saved."));
  }
}

/**
 * Store group's roles into the database and import user by the according roles.
 */
function og_default_members_admin_members_import($form, &$form_state) {
  $gid = $form_state['values']['gid'];
  $group_type = $form_state['values']['group_type'];

  if (!empty($form_state['values']['roles'])) {
    // Save group settings.
    $roles = $form_state['values']['roles'];
    _og_default_members_save($gid, $group_type, $roles);
  }
  // Import group members according to the group configuration.
  _og_default_members_update_group_members($gid, $group_type);

  drupal_set_message(t("The configurations have been saved."));
}
