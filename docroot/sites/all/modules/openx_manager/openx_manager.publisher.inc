<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Manage all OpenX Publishers.
 */

/**
 * Implementation of hook_access().
 *
 * For Publishers
 */
function _openx_node_publisher_access($op, $node, $account) {
  if ($op == 'view') {
    return user_access('view openx publishers', $account);
  }
  elseif ($op == 'create') {
    return user_access('create openx publishers', $account);
  }
  elseif ($op == 'update' or $op == 'delete') {
    if (user_access('edit own openx publishers', $account) and ($account->uid == $node->uid)) {
      return TRUE;
    }
  }
}

/**
 * Implementation of hook_delete().
 *
 * For Publishers
 */
function _openx_node_publisher_delete(&$node) {
  _openx_manager_api('deletePublisher', (int) $node->opid);
  db_query('DELETE FROM {openx_manager_publisher} WHERE nid = %d', $node->nid);
}

/**
 * Implementation of hook_form().
 *
 * For Publishers
 */
function _openx_node_publisher_form(&$node, $form_state) {
  $form['title'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Publisher'),
    '#required'       => TRUE,
    '#default_value'  => $node->title,
    '#weight'         => -5,
  );
  $form['opid'] = array(
    '#value'          => $node->opid,
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

  $form['#submit'][] = '_openx_node_publisher_submit';
  return $form;
}

/**
 * Implementation of hook_insert().
 *
 * For Publishers
 */
function _openx_node_publisher_insert(&$node) {
  drupal_write_record('openx_manager_publisher', $node);
}

/**
 * Implementation of hook_load().
 *
 * For Publishers
 */
function _openx_node_publisher_load(&$node) {
  $sql = db_query('SELECT * FROM {openx_manager_publisher} WHERE nid = %d', $node->nid);
  while ($field = db_fetch_array($sql)) {
    foreach ($field as $key => $value) {
      $node->$key = $value;
    }
  }
}

/**
 * Unify the hook_insert and hook_update common parts.
 */
function _openx_node_publisher_submit(&$form, &$form_state) {
  if ($form_state['values']['op'] != t('Preview')) {
    // Increase the code readability
    $node = &$form_state['values'];

    $data = array(
      'publisherName'   => $node['title'],
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
      $node['opid'] = _openx_manager_api('addPublisher', $data);
    }
    else {
      $data['publisherId'] = (int) $node['opid'];
      _openx_manager_api('modifyPublisher', $data);
    }
  }
}

/**
 * Implementation of hook_update().
 *
 * For Publishers
 */
function _openx_node_publisher_update(&$node) {
  drupal_write_record('openx_manager_publisher', $node, 'nid');
}

/**
 * Implementation of hook_form().
 *
 * For Publishers
 */
function _openx_node_publisher_validate(&$node) {
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
 * For Publishers
 */
function _openx_node_publisher_view(&$node, $teaser = FALSE, $page = FALSE) {
  $node->content['openx_publisher'] = array(
    '#value'  => theme('openx_publisher', $node),
    '#weight' => 1,
  );

  return $node;
}

/**
 * View the correspondent Publisher node from a given user.
 *
 * @param $user
 *   Object. The user object. Its received from the hook_menu().
 * @return
 *   HTML. The node view.
 */
function _openx_user_publisher($user) {
  if ($nid = db_result(db_query("SELECT nid FROM {openx_manager_publisher}
      WHERE duid = %d", $user->uid))) {
    $node = node_load($nid);
  }
  else {
    _openx_user_publisher_update($user, (array) $user);
  }
  return node_view($node);
}

/**
 * Delete the Publisher node linked to a user.
 *
 * @param $user
 *   Object. The user object.
 */
function _openx_user_publisher_delete(&$user) {
  if ($nid = db_result(db_query("SELECT nid FROM {openx_manager_publisher}
        WHERE duid = %d", $user->uid))) {
    $node = node_delete($nid);
  }
}

/**
 * Create or update a Publisher node linked to a user.
 *
 * @param $user
 *   Object. The user object.
 */
function _openx_user_publisher_update(&$user, $user_new) {
  $node = array(
    'type'          => 'openx_publisher',
    'title'         => $user_new['name'],
    'uid'           => $user->uid,
    'name'          => $user_new['name'],
    'duid'          => $user->uid,
    'contact'       => $user_new['name'],
    'email'         => $user_new['mail'],
  );
  if ($publisher = db_fetch_array(db_query("SELECT nid, opid FROM {openx_manager_publisher}
        WHERE duid = %d", $user->uid))) {
    $node['nid']  = $publisher['nid'];
    $node['opid'] = $publisher['opid'];
  }
  $form = $form_state = array();
  $form_state['values'] = &$node;
  _openx_node_publisher_submit($form, $form_state);
  $node = node_submit($node);
  node_save($node);

  return $node;
}

/**
 * Display the Publisher fields
 */
function theme_openx_publisher(&$node) {
  $fields['contact'] = array(
    '#value'          => $node->contact,
    '#title'          => t('Contact name'),
    '#type'           => 'item',
  );
  $fields['email'] = array(
    '#value'          => $node->email,
    '#title'          => t('Contact Email'),
    '#type'           => 'item',
  );

  // Zones list
  $fields['zones'] = array(
    '#collapsible'    => TRUE,
    '#title'          => t('Zones'),
    '#type'           => 'fieldset',
    '#weight'         => 10,
  );
  $view = views_get_view('openx_manager_zones');
  $fields['zones'][] = array(
    '#value'          => $view->execute_display(NULL, array($node->opid)),
    '#type'           => 'item',
  );

  // Print all field in the right order
  $output = '';
  foreach ($fields as $field) {
    $output .= drupal_render($field);
  }
  return $output;
}
