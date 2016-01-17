<?php

/**
 * @file
 * Administrator file for Dominion.
 */

/**
 * Displays the dominion list.
 */
function dominion_admin_overview() {
  $output = '';
  $table = array();

  // Gets the displayable items per page. Default is 25.
  $items = variable_get(DOMINION_VAR_ITEMS_IN_OVERVIEW, DOMINION_VAR_ITEMS_IN_OVERVIEW_DEFAULT);

  // Retrieve a list of domains.
  $query = db_select('domain', 'd')
    ->fields('d', array('domain_id', 'sitename', 'subdomain', 'scheme', 'is_default', 'dominion_directory', 'dominion_locked'))
    ->orderBy('d.sitename', 'ASC')
    ->extend('PagerDefault')
    ->limit($items);

  // Alter the query whenever there are search input.
  if (isset($_GET['search'])) {
    $args = explode('|', $_GET['search']);
    $or_condition = db_or();
    foreach ($args as $arg) {
      $or_condition = $or_condition
      ->condition('d.sitename', '%' . db_like($arg) . '%', 'LIKE')
      ->condition('d.subdomain', '%' . db_like($arg) . '%', 'LIKE');
    }
    $and_condition = db_and()
      ->condition('d.dominion_domain', 1)
      ->condition($or_condition);
    $query = $query->condition($and_condition);
  }
  else {
    $query = $query->condition('d.dominion_domain', 1);
  }
  $result = $query->execute();

  foreach ($result as $row) {
    $operations = array();
    if (user_access('manage dominion subsites')) {
      $operations['edit'] = l(t('Edit'), 'admin/structure/dominion/list/' . $row->domain_id . '/edit');
    }
    if (user_access('administer site configuration') || (!$row->dominion_locked)) {
      if (user_access('manage dominion subsites')) {
        if (!$row->is_default) {
          $operations['delete'] = l(t('Delete'), 'admin/structure/dominion/list/' . $row->domain_id . '/delete');
        }
      }
    }
    $operations['members'] = l(t('Members'), 'admin/structure/dominion/list/' . $row->domain_id . '/members');

    drupal_alter('dominion_operations', $operations, $row);

    $table[] = array(
      l($row->sitename, $row->scheme . '://' . $row->subdomain . '/' . $row->dominion_directory),
      implode(' ', $operations),
    );
  }
  $output .= drupal_render(drupal_get_form('dominion_admin_search_form'));
  if (empty($table)) {
    $output .= t('No subsites found.');
  }
  else {
    $output .= theme('table', array(
      'header' => array(
        t('Name'),
        t('Operations')
      ),
      'rows' => $table,
    ));
    $output .= theme('pager');
  }

  return $output;
}

/**
 * Creates the search form for Dominion search.
 * @param $form
 *    The form object.
 * @param $form_state
 *    The form_state object.
 *
 * @return The form object.
 */
function dominion_admin_search_form($form, &$form_state) {
  $form = array();
  $search_arg = !empty($_GET['search']) ? check_plain($_GET['search']) : '';

  // Collapsable fieldset for the search.
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($search_arg),
  );

  // Search textfield.
  $form['search']['search'] = array(
    '#type' => 'textfield',
    '#title' => t('Keywords'),
    '#description' => t('Use a "|" to seperate keywords, i.e. brown fox|black.'),
    '#default_value' => $search_arg,
  );

  // Submit button for the searchform.
  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );
  return $form;
}

/**
 * Submit handler for the search_form.
 *
 * @param $form
 *    The form object.
 * @param $form_state
 *    The form_state object.
 */
function dominion_admin_search_form_submit($form, &$form_state) {
  $query = $form_state['values']['search'] ? array('search' => $form_state['values']['search']) : FALSE;
  drupal_goto('admin/structure/dominion', array('query' => $query));
}

/**
 * Menu callback.
 *
 * Generates the admin settings form.
 */
function dominion_admin_settings() {
  $form = array();

  $form[DOMINION_VAR_DOMAIN_SUFFIX] = array(
    '#type' => 'textfield',
    '#title' => t('Default host suffix'),
    '#required' => TRUE,
    '#default_value' => variable_get(DOMINION_VAR_DOMAIN_SUFFIX, DOMINION_VAR_DOMAIN_SUFFIX_DEFAULT),
    '#description' => t('Domains for subsites are suffixed by this string by default.'),
  );

  $form[DOMINION_VAR_EDITOR_ROLES] = array(
    '#type' => 'checkboxes',
    '#title' => t('Editor roles'),
    '#options' => user_roles(),
    '#default_value' => variable_get(DOMINION_VAR_EDITOR_ROLES, array()),
    '#description' => t('Roles which can be selected when adding an editor to a subsite.'),
  );

  $form[DOMINION_VAR_PATH_FRONTPAGE] = array(
    '#type' => 'textfield',
    '#title' => t('Static frontpage path'),
    '#default_value' => variable_get(DOMINION_VAR_PATH_FRONTPAGE, DOMINION_VAR_PATH_FRONTPAGE_DEFAULT),
    '#description' => t('Path to use for the static subsite specific frontpage.'),
    '#required' => TRUE,
  );

  $options = array(
    'subdomain' => t('Use a subdomain'),
    'domin' => t('Use a custom domainname'),
    'directory' => t('Use a directory'),
  );
  $form[DOMINION_VAR_DEFAULT_DOMAIN_TYPE] = array(
    '#type' => 'radios',
    '#title' => t('Default domain type'),
    '#default_value' => variable_get(DOMINION_VAR_DEFAULT_DOMAIN_TYPE, 'subdomain'),
    '#options' => $options,
  );

  $form[DOMINION_VAR_CUSTOM_FUNCTIONS] = array(
    '#type' => 'textarea',
    '#title' => t('Custom function'),
    '#default_value' => variable_get(DOMINION_VAR_CUSTOM_FUNCTIONS, ''),
    '#description' => t('Extra dominion functions. One item per line, each having a machine readable name and title separated by a pipe.'),
    '#required' => FALSE,
  );

  $form[DOMINION_VAR_ITEMS_IN_OVERVIEW] = array(
    '#type' => 'textfield',
    '#title' => t('Number of items in overview'),
    '#default_value' => variable_get(DOMINION_VAR_ITEMS_IN_OVERVIEW, DOMINION_VAR_ITEMS_IN_OVERVIEW_DEFAULT),
    '#required' => TRUE,
  );

  $form['#validate'][] = 'dominion_admin_settings_validate';
  $form['#submit'][] = 'dominion_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Form validate handler.
 */
function dominion_admin_settings_validate($form, &$form_state) {
  $functions = $form_state['values'][DOMINION_VAR_CUSTOM_FUNCTIONS];
  $functions = array_filter(array_map('trim', explode("\n", $functions)));
  foreach ($functions as $function) {
    $function = explode('|', $function, 2);
    if (empty($function[1])) {
      form_set_error(DOMINION_VAR_CUSTOM_FUNCTIONS, t('Missing name for function %function', array('%function' => $function[0])));
      continue;
    }
    if (!preg_match('/^[a-z0-9\\_]+$/si', $function[0])) {
      form_set_error(DOMINION_VAR_CUSTOM_FUNCTIONS, t('Function names may only contain alphanumeric characters and underscores. Illegal name for function %function', array('%function' => $function[0])));
      continue;
    }
  }
}

/**
 * Form submit handler.
 */
function dominion_admin_settings_submit($form, &$form_state) {
  dominion_path(DOMINION_VAR_PATH_FRONTPAGE, $form_state['values'][DOMINION_VAR_PATH_FRONTPAGE], TRUE);
  menu_rebuild();
}

/**
 * Menu callback for the domain users page.
 */
function dominion_admin_members($domain_id) {
  $output = '';
  $subsite = dominion_get_info($domain_id);
  if (!$subsite) {
    drupal_not_found();
  }
  drupal_set_title(t('Members of %name', array('%name' => $subsite->sitename)), PASS_THROUGH);

  $output .= drupal_render(drupal_get_form('dominion_admin_add_member_form', $domain_id));
  if (user_access('administer site configuration')) {
    $output .= drupal_render(drupal_get_form('dominion_admin_role_mapping_form', $domain_id));
  }

  $query = db_select('domain_editor', 'de');
  $query->fields('u', array('uid', 'name', 'mail'));
  $query->condition('de.domain_id', $subsite->domain_id);
  $query->join('users', 'u', 'u.uid = de.uid');
  $query->orderBy('u.name', 'ASC');
  $results = $query->execute();

  foreach ($results as $row) {
    $roles = array();
    $query = db_select('users_roles', 'ur');
    $query->fields('r', array('name'));
    $query->join('role', 'r', 'r.rid = ur.rid');
    $query->condition('ur.uid', $row->uid);
    $user_results = $query->execute();
    foreach ($user_results as $user_row) {
      $roles[] = '<strong>' . t($user_row->name) . '</strong>';
    }
    $query = db_select('dominion_editor_role', 'ur');
    $query->fields('r', array('name'));
    $query->join('role', 'r', 'r.rid = ur.rid');
    $query->condition('ur.domain_id', $domain_id);
    $query->condition('ur.uid', $row->uid);
    $user_results = $query->execute();
    foreach ($user_results as $user_row) {
      $roles[] = t($user_row->name);
    }
    $roles = implode(', ', $roles);

    // Generate a token to prevent XSRF in delete link.
    $token = drupal_get_token($subsite->domain_id . ':' . $row->uid);

    $table[] = array(
      $row->name,
      $row->mail,
      $roles,
      l(t('Remove from list'), 'admin/structure/dominion/list/' . $subsite->domain_id . '/remove-member/' . $row->uid . '/' . $token)
    );
  }
  if (empty($table)) {
    $output .= '<p>' . t('There are no members.') . '</p>';
  }
  else {
    $output .= theme('table', array(
      'header' => array(
        t('Name'),
        t('Mail account'),
        t('Roles'),
        t('Operations')
      ),
      'rows' => $table
    ));
  }

  return $output;
}

/**
 * Form for role mapping.
 */
function dominion_admin_role_mapping_form($form, &$form_state, $domain_id) {
  $form['#domain_id'] = $domain_id;

  $form['role_mapping'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role mapping'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#description' => t('With role mapping, a new role is assigned to visitors for this subsite, based on the assigned roles to the user account. To assign role A to all users with role B visiting this subsite, select A at the dropdown under B. Assigned roles are only valid for this subsite.'),
  );

  // Load current configuration from database.
  $defaults = db_select('dominion_role_mapping', 'rm')
    ->fields('rm', array('rid', 'target_rid'))
    ->condition('rm.domain_id', $domain_id)
    ->execute()
    ->fetchAllKeyed();

  $user_roles = user_roles();
  $options = array(0 => t('Disable'));
  foreach ($user_roles as $rid => $name) {
    if ($rid > 2) {
      $options[$rid] = t($name);
    }
  }
  $form['role_mapping']['roles'] = array('#tree' => TRUE);
  foreach ($user_roles as $rid => $role_name) {
    if ($rid) {
      $form['role_mapping']['roles'][$rid] = array(
        '#type' => 'select',
        '#title' => t($role_name),
        '#options' => $options,
        '#default_value' => isset($defaults[$rid]) ? $defaults[$rid] : 0,
      );
    }
  }

  $form['role_mapping']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save mapping'),
  );

  return $form;
}

/**
 * Form for role mapping.
 */
function dominion_admin_role_mapping_form_submit($form, &$form_state) {
  $domain_id = $form['#domain_id'];
  $mapping = $form_state['values']['role_mapping']['roles'];
  $mapping = array_filter($mapping);
  db_delete('dominion_role_mapping')->condition('domain_id', $domain_id)->execute();
  foreach ($mapping as $rid => $target_rid) {
    db_insert('dominion_role_mapping')->fields(array(
      'domain_id' => $domain_id,
      'rid' => $rid,
      'target_rid' => $target_rid,
    ))->execute();
  }
  drupal_set_message(t('Role mapping was saved succesfully.'));
}

/**
 * Form to add users.
 *
 * @param $form
 *    The form object.
 * @param $form_state
 *    The form_state object.
 * @param $domain_id
 *    The domain ID of the current selected domain.
 *
 * @return The form object.
 */
function dominion_admin_add_member_form($form, &$form_state, $domain_id) {
  $form['#domain_id'] = $domain_id;

  $form['add_member'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add member'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['add_member']['intro'] = array(
    '#value' => '<p>' . t('Add a member by name or email.') . '</p>'
  );

  $form['add_member']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#autocomplete_path' => 'admin/structure/dominion/list/members/autocomplete/username'
  );

  $form['add_member']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail account'),
    '#autocomplete_path' => 'admin/structure/dominion/list/members/autocomplete/email'
  );

  $editor_roles = variable_get(DOMINION_VAR_EDITOR_ROLES, array());
  $options = array();
  $user_roles = user_roles();
  foreach ($editor_roles as $rid) {
    if ($rid) {
      $options[$rid] = $user_roles[$rid];
    }
  }
  if ($options) {
    $form['add_member']['roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Additional roles'),
      '#options' => $options,
      '#description' => t('Roles checked here are added to the user. No roles will be deleted if the user already have one or more of these roles.')
    );
  }

  $form['add_member']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Validate handler for add_member_form
 *
 * @param $form
 *    The form object.
 * @param $form_state
 *    The form_state object.
 */
function dominion_admin_add_member_form_validate($form, &$form_state) {
  $name = $form_state['values']['name'];
  $email = $form_state['values']['email'];
  if (empty($name) && empty($email)) {
    form_set_error('', t('Specify a username or mail address.'));
  }
  else {
    if (empty($name)) {
      $uid = db_select('users', 'u')
      ->fields('u', array('uid'))
      ->condition('u.mail', $email)
      ->execute()->fetchField();
    }
    elseif (empty($email)) {
      $uid = db_select('users', 'u')
      ->fields('u', array('uid'))
      ->condition('u.name', $name)
      ->execute()->fetchField();
    }
    else {
      $uid = db_select('users', 'u')
      ->fields('u', array('uid'))
      ->condition('u.name', $name)
      ->condition('u.mail', $email)
      ->execute()->fetchField();
    }
    if (!$uid) {
      form_set_error('', t('No user has been found with the given username or email address.'));
    }
  }
}

/**
 * Submit handler for add_member_form
 *
 * @param $form
 *    The form object.
 * @param $form_state
 *    The form_state object.
 */
function dominion_admin_add_member_form_submit($form, &$form_state) {
  $subsite = dominion_get_info($form['#domain_id']);
  if (!$subsite) {
    drupal_not_found();
    return;
  }
  $name = $form_state['values']['name'];
  $email = $form_state['values']['email'];
  $roles = isset($form_state['values']['roles']) ? $form_state['values']['roles'] : array();
  $query = db_select('users', 'u')->fields('u', array('uid', 'name'));
  if (empty($name)) {
    $query->condition('u.mail', $email);
  }
  elseif (empty($email)) {
    $query->condition('u.name', $name);
  }
  else {
    $query->condition('u.name', $name);
  }
  if ($account = $query->execute()->fetchObject()) {
    foreach ($roles as $rid) {
      if ($rid) {
        $fields = array('uid' => $account->uid, 'domain_id' => $subsite->domain_id, 'rid' => $rid);
        db_merge('dominion_editor_role')->fields($fields)->key($fields)->execute();
      }
    }
    $fields = array('uid' => $account->uid, 'domain_id' => $subsite->domain_id);
    db_merge('domain_editor')->fields($fields)->key($fields)->execute();
    drupal_set_message(t('User %name is now a member of %sitename.', array('%name' => $account->name, '%sitename' => $subsite->sitename)));
  }
  else {
    drupal_set_message(t('No user found with given criteria.'), 'error');
  }
  return;
}

function dominion_admin_delete_member_form($form, $form_state, $domain_id, $uid, $token) {
  if ($token != drupal_get_token("$domain_id:$uid")) {
    drupal_access_denied();
    module_invoke_all('exit') & exit;
  }

  $subsite = dominion_get_info($domain_id);
  $user = user_load($uid);
  if (!($subsite && $user)) {
    drupal_not_found();
  }

  $form = array();
  $form['#domain_id'] = $domain_id;
  $form['#uid'] = $uid;
  $form['info'] = array(
    '#markup' => '<p>' . t('Do you want to remove %name from the memberlist for subsite %domain?', array('%name' => $user->name, '%domain' => $subsite->sitename)) . '</p>',
  );
  $form['buttons'] = array(
    '#prefix' => '<div class="buttons">',
    '#suffix' => '</div>',
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Remove user from memberlist'),
  );
  $form['buttons']['cancel'] = array(
    '#value' => l(t('Cancel'), 'admin/structure/dominion/list/' . $domain_id . '/members'),
  );
  return $form;
}

/**
 * Submit handler for delete_user_form
 *
 * @param $form
 *    The form object.
 * @param $form_state
 *    The form_state object.
 */
function dominion_admin_delete_member_form_submit($form, &$form_state) {
  $uid = $form['#uid'];
  $domain_id = $form['#domain_id'];
  $subsite = dominion_get_info($domain_id);
  $user = user_load($uid);

  if (!($subsite && $user)) {
    drupal_not_found();
    module_invoke_all('exit') & exit;
  }

  db_delete('domain_editor')
    ->condition('uid', $uid)
    ->condition('domain_id', $domain_id)
    ->execute();

  db_delete('dominion_editor_role')
    ->condition('uid', $uid)
    ->condition('domain_id', $domain_id)
    ->execute();

  drupal_set_message(t('The user %name has been removed from the memberlist for %domain.', array('%name' => $user->name, '%domain' => $subsite->sitename)));

  $form_state['redirect'] = 'admin/structure/dominion/list/' . $domain_id . '/members';
}

/**
 * Autocomplete handler for username.
 *
 * @param $name String
 *    The username.
 *
 * @return The autocompleted username.
 */
function dominion_admin_users_autocomplete_username($name = '') {
  $output = array();
  if (!empty($name)) {
    $result = db_select('users', 'u')
    ->fields('u', array('uid', 'name'))
    ->condition('u.name', db_like($name) . '%', 'LIKE')
    ->orderBy('u.name', 'ASC')
    ->range(0, 25)
    ->execute();

    foreach ($result as $row) {
      $output[$row->name] = check_plain($row->name);
    }
  }
  return drupal_json_output($output);
}

/**
 * Autocomplete handler for email.
 *
 * @param $email String
 *    The email.
 *
 * @return The autocompleted email.
 */
function dominion_admin_users_autocomplete_email($email = '') {
  $output = array();
  if (!empty($email)) {
    $result = db_select('users', 'u')
    ->fields('u')
    ->condition('u.mail', db_like($email) . '%', 'LIKE')
    ->orderBy('u.mail', 'ASC')
    ->range(0, 25)
    ->execute();

    foreach ($result as $row) {
      $output[$row->mail] = check_plain($row->mail);
    }
  }
  return drupal_json_output($output);
}
