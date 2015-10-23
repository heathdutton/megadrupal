<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Manage all OpenX Managers.
 */

/**
 * Implementation of hook_access().
 *
 * For Managers
 */
function _openx_node_manager_access($op, $node, $account) {
  if ($op == 'view') {
    return user_access('view openx managers', $account);
  }
  elseif ($op == 'create') {
    return user_access('create openx managers', $account);
  }
  elseif ($op == 'update' or $op == 'delete') {
    if (user_access('edit own openx managers', $account) and ($account->uid == $node->uid)) {
      return TRUE;
    }
  }
}

/**
 * Implementation of hook_delete().
 *
 * For Managers
 */
function _openx_node_manager_delete(&$node) {
  _openx_manager_api('deleteManager', (int) $node->oaid);
  db_query('DELETE FROM {openx_manager_manager} WHERE nid = %d', $node->nid);
}

/**
 * Implementation of hook_form().
 *
 * For Managers
 */
function _openx_node_manager_form(&$node, $form_state) {
  $form['title'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Manager'),
    '#required'       => TRUE,
    '#default_value'  => $node->title,
    '#weight'         => -5,
  );
  $form['omid'] = array(
    '#value'          => $node->omid,
    '#type'           => 'value',
  );
  $form['contact'] = array(
    '#default_value'  => $node->contact,
    '#required'       => TRUE,
    '#title'          => t('Contact name'),
    '#type'           => 'textfield',
  );
  $form['email'] = array(
    '#default_value'  => $node->email,
    '#title'          => t('Email'),
    '#type'           => 'textfield',
  );
  $form['username'] = array(
    '#default_value'  => $node->username,
    '#title'          => t('Username'),
    '#type'           => 'textfield',
  );
  $form['password'] = array(
    '#default_value'  => $node->password,
    '#title'          => t('Password'),
    '#type'           => 'password',
  );

  $form['#submit'][] = '_openx_node_manager_submit';
  return $form;
}

/**
 * Implementation of hook_insert().
 *
 * For Managers
 */
function _openx_node_manager_insert(&$node) {
  drupal_write_record('openx_manager_manager', $node);
}

/**
 * Implementation of hook_load().
 *
 * For Managers
 */
function _openx_node_manager_load(&$node) {
  $sql = db_query('SELECT * FROM {openx_manager_manager} WHERE nid = %d', $node->nid);
  while ($field = db_fetch_array($sql)) {
    foreach ($field as $key => $value) {
      $node->$key = $value;
    }
  }
}

/**
 * Unify the hook_insert and hook_update common parts.
 */
function _openx_node_manager_submit(&$form, &$form_state) {
  if ($form_state['values']['op'] != t('Preview')) {
    // Increase the code readability
    $node = &$form_state['values'];

    $data = array(
      'agencyName'      => $node['title'],
      'contactName'     => $node['contact'],
    );
    if (!empty($node['email'])) {
      $data['emailAddress'] = $node['email'];
    }
    if (!empty($node['username'])) {
      $data['username'] = $node['username'];
    }
    if (!empty($node['password'])) {
      $data['password'] = $node['password'];
    }

    // Update or insert
    if (empty($node['nid'])) {
      $node['omid'] = _openx_manager_api('addAgency', $data);
    }
    else {
      $data['agencyId'] = (int) $node['omid'];
      _openx_manager_api('modifyAgency', $data);
    }
  }
}

/**
 * Implementation of hook_update().
 *
 * For Managers
 */
function _openx_node_manager_update(&$node) {
  drupal_write_record('openx_manager_manager', $node, 'nid');
}

/**
 * Implementation of hook_form().
 *
 * For Managers
 */
function _openx_node_manager_validate(&$node) {
  if (!variable_get('openx_manager_server', '')) {
    form_set_error('title', t('OpenX integration not configured'));
  }
  if (!empty($node->email) and !valid_email_address($node->email)) {
    form_set_error('email', t('Invalid email'));
  }
}

/**
 * Implementation of hook_load().
 *
 * For Managers
 */
function _openx_node_manager_view(&$node, $teaser = FALSE, $page = FALSE) {
  $node->content['openx_manager'] = array(
    '#value'  => theme('openx_manager', $node),
    '#weight' => 1,
  );

  return $node;
}

/**
 * View the correspondent Manager node from a given user.
 *
 * @param $user
 *   Object. The user object. Its received from the hook_menu().
 * @return
 *   HTML. The node view.
 */
function _openx_user_manager($user) {
  if ($nid = db_result(db_query("SELECT nid FROM {openx_manager_manager}
      WHERE duid = %d", $user->uid))) {
    $node = node_load($nid);
  }
  else {
    _openx_user_manager_update($user, (array) $user);
  }
  return node_view($node);
}

/**
 * Delete the Manager node linked to a user.
 *
 * @param $user
 *   Object. The user object.
 */
function _openx_user_manager_delete(&$user) {
  if ($nid = db_result(db_query("SELECT nid FROM {openx_manager_manager}
        WHERE duid = %d", $user->uid))) {
    $node = node_delete($nid);
  }
}

/**
 * Create or update a Manager node linked to a user.
 *
 * @param $user
 *   Object. The user object.
 */
function _openx_user_manager_update(&$user, $user_new) {
  $node = array(
    'type'          => 'openx_manager',
    'title'         => $user_new['name'],
    'uid'           => $user->uid,
    'name'          => $user_new['name'],
    'duid'          => $user->uid,
    'contact'       => $user_new['name'],
    'email'         => $user_new['mail'],
  );
  if ($manager = db_fetch_array(db_query("SELECT nid, omid FROM {openx_manager_manager}
        WHERE duid = %d", $user->uid))) {
    $node['nid']  = $manager['nid'];
    $node['omid'] = $manager['omid'];
  }
  $form = $form_state = array();
  $form_state['values'] = &$node;
  _openx_node_manager_submit($form, $form_state);
  $node = node_submit($node);
  node_save($node);

  return $node;
}

/**
 * Display the Managers fields
 */
function theme_openx_manager(&$node) {
  $fields['contact'] = array(
    '#value'          => $node->contact,
    '#title'          => t('Name'),
    '#type'           => 'item',
  );
  $fields['email'] = array(
    '#value'          => $node->email,
    '#title'          => t('Email'),
    '#type'           => 'item',
  );

  // Print all field in the right order
  $output = '';
  foreach ($fields as $field) {
    $output .= drupal_render($field);
  }
  return $output;
}
