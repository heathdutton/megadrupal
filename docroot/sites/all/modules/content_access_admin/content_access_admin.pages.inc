<?php
/**
 * @file content_access_admin.pages.inc
 */

/**
 * Callback that provides the access list and filter.
 *
 * @return
 *   An array of form elements.
 */
function content_access_admin_page($form, $form_state) {
  $form['filter'] = content_access_admin_filter();
  $form['#submit'][] = 'content_access_admin_form_submit';
  $form['admin'] = content_access_admin_form();

  return $form;
}

/**
 * Create the filter form for the content access list.
 *
 * @return
 *   An array of form elements.
 */
function content_access_admin_filter() {
  $filters = isset($_SESSION['content_access_admin_filters']) ? $_SESSION['content_access_admin_filters'] : array();

  // Turns out we can simply steal the node admin theme functions :-)
  $form['filters'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Show ACLs with'),
    '#collapsed' => TRUE,
    '#theme' => 'exposed_filters__node'
  );

  $form['filters']['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#description' => t('Limit results to nodes whose titles contain'),
    '#default_value' => !empty($filters['title']) ?$filters['title'] : '',
  );

  $form['filters']['user'] = array(
    '#title' => t('User'),
    '#type' => 'textfield',
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#description' => t('Limit results to ACLs for this user'),
    '#default_value' => !empty($filters['user']) ? $filters['user'] : '',
  );

  $types = array('' => t(' any'));
  $types += node_type_get_names();
  $form['filters']['type'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' => $types,
    '#description' => t('Limit results to this node type'),
    '#default_value' => !empty($filters['type']) ? $filters['type'] : '',
  );

  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Filter'),
  );
  $form['filters']['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset'),
  );

  return $form;
}

/**
 * Submit handler for the filter form.
 *
 * Pop the filter selections in a session variable or empty said variable
 * when the user clicks the `Reset' button.
 */
function content_access_admin_form_submit($form, &$form_state) {
  $_SESSION['content_access_admin_filters'] = ($form_state['values']['op'] == 'Reset') ? array() : $form_state['values']['filters'];
}

/**
 * Return a paged table containing an overview of the acl table entries.
 *
 * Mainly build the query, then return the data in form elements.
 *
 * @return
 *   An array of form elements.
 */
function content_access_admin_form() {
  $filters = isset($_SESSION['content_access_admin_filters']) ? $_SESSION['content_access_admin_filters'] : array();

  // Build the sortable table header
  $header = array(
    'title' => array(
      'data' => t('Title'),
      'field' => 'n.title',
    ),
    'user' => t('User'),
    'view' => array(
      'data' => t('View'),
      'field' => 'v',
    ),
    'update' => array(
      'data' => t('Update'),
      'field' => 'u',
    ),
    'delete' => array(
      'data' => t('Delete'),
      'field' => 'd',
    ),
    'type' => array(
      'data' => t('Type'),
      'field' => 'n.type',
    ),
    'status' => array(
      'data' => t('Status'),
      'field' => 'n.status',
    ),
    'changed' => array(
      'data' => t('Updated'),
      'field' => 'n.changed',
      'sort' => 'desc',
    ),
  );

  // Put together the select query.
  $query = db_select('acl_user', 'au');
  $query->join('acl_node', 'an', 'au.acl_id = an.acl_id');
  $query->join('node', 'n', 'an.nid = n.nid');

  // Add fields!
  $query->fields('an', array('nid'))
    ->fields('au', array('uid'))
    ->fields('n', array('title', 'type', 'status', 'changed'));

  // Calculate the permissions...
  $query->addExpression('MAX(an.grant_view)', 'v');
  $query->addExpression('MAX(an.grant_update)', 'u');
  $query->addExpression('MAX(an.grant_delete)', 'd');
  $query->groupBy('au.uid, an.nid');

  // And pagerify it.
  $query->extend('PagerDefault')->extend('TableSort')->limit(50)->orderByHeader($header);

  // Process filters.
  if (!empty($filters['user'])) {
    $query->join('users', 'u', 'au.uid = u.uid');
    $query->condition('u.name', check_plain($filters['user']));
  }

  if (!empty($filters['title'])) {
    $query->condition('n.title', db_like($filters['title']) . '%', 'LIKE');
  }

  if (!empty($filters['type'])) {
    $query->condition('n.type', check_plain($filters['type']));
  }

  // Execute.
  $result = $query->execute();

  $options = array();
  while ($row = $result->fetchAssoc()) {
    $option = array(
      'title' => array(
        'data' => array(
          '#type' =>'link',
          '#title' => $row['title'],
          '#href' => 'node/' . $row['nid'] .'/access',
          '#suffix' => ' ' . theme('mark', array('type' => node_mark($row['nid'], $row['changed']))),
        ),
      ),
      'user' => theme('username', array('account' => user_load($row['uid']))),
      'view' => $row['v'] ? t('Yes') : t('No'),
      'update' => $row['u'] ? t('Yes') : t('No'),
      'delete' => $row['d'] ? t('Yes') : t('No'),
      'type' => $row['type'],
      'status' => $row['status'] ? t('published') : t('not published'),
      'updated' => format_date($row['changed'], 'short'),
    );
    $options[] = $option;
  }

  $form['access'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No ACLs available.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));
  return $form;
}
