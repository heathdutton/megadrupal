<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Manage all OpenX Users.
 */

/**
 * Implementation of hook_access().
 *
 * For Users
 */
function _openx_node_user_access($op, $node, $account) {
  if ($op == 'view') {
    return user_access('view openx users', $account);
  }
  elseif ($op == 'create') {
    return user_access('create openx users', $account);
  }
  elseif ($op == 'update' or $op == 'delete') {
    if (user_access('edit own openx users', $account) and ($account->uid == $node->uid)) {
      return TRUE;
    }
  }
}

/**
 * Implementation of hook_delete().
 *
 * For Users
 */
function _openx_node_user_delete(&$node) {
  _openx_manager_api('deleteUser', (int) $node->ouid);
  db_query('DELETE FROM {openx_manager_user} WHERE nid = %d', $node->nid);
}

/**
 * Implementation of hook_form().
 *
 * For Users
 */
function _openx_node_user_form(&$node, $form_state) {
  $form['title'] = array(
    '#type'           => 'textfield',
    '#title'          => t('User'),
    '#required'       => TRUE,
    '#default_value'  => $node->title,
    '#weight'         => -5,
  );
  $form['ouid'] = array(
    '#value'          => $node->ouid,
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

  $form['#submit'][] = '_openx_node_user_submit';
  return $form;
}

/**
 * Implementation of hook_insert().
 *
 * For Users
 */
function _openx_node_user_insert(&$node) {
  drupal_write_record('openx_manager_user', $node);
}

/**
 * Implementation of hook_load().
 *
 * For Users
 */
function _openx_node_user_load(&$node) {
  $sql = db_query('SELECT * FROM {openx_manager_user} WHERE nid = %d', $node->nid);
  while ($field = db_fetch_array($sql)) {
    foreach ($field as $key => $value) {
      $node->$key = $value;
    }
  }
}

/**
 * Unify the hook_insert and hook_update common parts.
 */
function _openx_node_user_submit(&$form, &$form_state) {
  if ($form_state['values']['op'] != t('Preview')) {
    // Increase the code readability
    $node = &$form_state['values'];

    $data = array(
      'userName'        => $node['title'],
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
      $node['ouid'] = _openx_manager_api('addUser', $data);
    }
    else {
      $data['userId'] = (int) $node['ouid'];
      _openx_manager_api('modifyAgency', $data);
    }
  }
}

/**
 * Implementation of hook_update().
 *
 * For Users
 */
function _openx_node_user_update(&$node) {
  drupal_write_record('openx_manager_user', $node, 'nid');
}

/**
 * Implementation of hook_form().
 *
 * For Users
 */
function _openx_node_user_validate(&$node) {
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
 * For Users
 */
function _openx_node_user_view(&$node, $teaser = FALSE, $page = FALSE) {
  $node->content['openx_user'] = array(
    '#value'  => theme('openx_user', $node),
    '#weight' => 1,
  );

  return $node;
}

/**
 * View the correspondent User node from a given user.
 *
 * @param $user
 *   Object. The user object. Its received from the hook_menu().
 * @return
 *   HTML. The node view.
 */
function _openx_user_user($user) {
  if ($nid = db_result(db_query("SELECT nid FROM {openx_manager_user}
      WHERE duid = %d", $user->uid))) {
    $node = node_load($nid);
  }
  else {
    _openx_user_user_update($user, (array) $user);
  }
  return node_view($node);
}

/**
 * Delete the User node linked to a user.
 *
 * @param $user
 *   Object. The user object.
 */
function _openx_user_user_delete(&$user) {
  if ($nid = db_result(db_query("SELECT nid FROM {openx_manager_user}
        WHERE duid = %d", $user->uid))) {
    $node = node_delete($nid);
  }
}

/**
 * Create or update a User node linked to a user.
 *
 * @param $user
 *   Object. The user object.
 */
function _openx_user_user_update(&$user, $user_new) {
  $node = array(
    'type'          => 'openx_user',
    'title'         => $user_new['name'],
    'uid'           => $user->uid,
    'name'          => $user_new['name'],
    'duid'          => $user->uid,
    'contact'       => $user_new['name'],
    'email'         => $user_new['mail'],
  );
  if ($ouser = db_fetch_array(db_query("SELECT nid, ouid FROM {openx_manager_user}
        WHERE duid = %d", $user->uid))) {
    $node['nid']  = $ouser['nid'];
    $node['ouid'] = $ouser['ouid'];
  }
  $form = $form_state = array();
  $form_state['values'] = &$node;
  _openx_node_user_submit($form, $form_state);
  $node = node_submit($node);
  node_save($node);

  return $node;
}

/**
 * Display the User fields
 */
function theme_openx_user(&$node) {
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
