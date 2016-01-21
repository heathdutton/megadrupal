<?php

/**
 * @file
 *
 * Administration interface for the Book access module.
 */

/**
 * Form builder for the book access configuration page.
 *
 * @see book_access_ui_user_add_submit()
 * @see book_access_ui_grants_form_submit()
 * @see theme_book_access_ui_grants_form()
 * @ingroup forms
 */
function book_access_ui_grants_form($form, &$form_state, $node) {
  if (!empty($node->book['bid'])) {
    $bid = $node->book['bid'];
    $context = array('bid' => $bid, 'node' => clone $node);
    $form = array(
      '#bid' => $bid,
      '#author' => $node->uid,
      '#rids' => array(),
      '#uids' => array(),
      '#tree' => TRUE,
    );
    // Build the role access permissions for the book.
    $grants = BookAccess::getAuthorGrants($bid, $node->uid, variable_get('book_access_default_roles_access', array()));
    $grants_copy = $grants;

    drupal_alter('book_access_author_grants', $grants, $context);

    if ($grants != $grants_copy) {
      _book_access_ui_permissions_warning();
    }

    $author = user_load($node->uid);

    if ($author) {
      $form['author']['name'] = array(
        '#markup' => t('!author-href (author)', array('!author-href' => theme('username', array('account' => $author)))),
      );
    }
    else {
      $form['author']['name'] = array(
        '#markup' => t('author'),
      );
    }

    $form['author']['grant_view'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_view']),
    );
    $form['author']['grant_update'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_update']),
    );
    $form['author']['grant_delete'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_delete']),
    );
    $form['author']['grant_admin_access'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_admin_access']),
    );
    $form['author']['grant_add_child'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_add_child']),
    );
    $form['author']['grant_edit_outline'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_edit_outline']),
    );

    $grants = BookAccess::getRoleGrants($bid, $roles, variable_get('book_access_default_roles_access', array()));
    $grants_copy = $grants;
    drupal_alter('book_access_roles_grants', $roles, $grants, $context);

    if ($grants != $grants_copy) {
      _book_access_ui_permissions_warning();
    }

    foreach ($roles as $rid => $name) {
      $form['#rids'][] = $rid;
      $form['roles']['names'][$rid] = array(
        '#markup' => check_plain($name),
      );
      $form['roles']['grant_view'][$rid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_view'][$rid]),
      );
      $form['roles']['grant_update'][$rid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_update'][$rid]),
      );
      $form['roles']['grant_delete'][$rid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_delete'][$rid]),
      );
      $form['roles']['grant_admin_access'][$rid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_admin_access'][$rid]),
      );
      $form['roles']['grant_add_child'][$rid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_add_child'][$rid]),
      );
      $form['roles']['grant_edit_outline'][$rid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_edit_outline'][$rid]),
      );
    }

    $grants = BookAccess::getUserGrants($bid, $uids);
    $grants_copy = $grants;
    drupal_alter('book_access_users_grants', $uids, $grants, $context);

    if ($grants != $grants_copy) {
      _book_access_ui_permissions_warning();
    }

    foreach ($uids as $uid) {
      $form['#uids'][] = $uid;
      $form['users']['grant_view'][$uid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_view'][$uid]),
      );
      $form['users']['grant_update'][$uid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_update'][$uid]),
      );
      $form['users']['grant_delete'][$uid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_delete'][$uid]),
      );
      $form['users']['grant_admin_access'][$uid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_admin_access'][$uid]),
      );
      $form['users']['grant_add_child'][$uid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_add_child'][$uid]),
      );
      $form['users']['grant_edit_outline'][$uid] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($grants['grant_edit_outline'][$uid]),
      );
    }

    $form['user']['username'] = array(
      '#type' => 'textfield',
      '#size' => 20,
      '#autocomplete_path' => 'user/autocomplete',
    );

    $grants = array_filter(variable_get('book_access_default_users_access', array()));

    $form['user']['grant_view'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_view']),
    );
    $form['user']['grant_update'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_update']),
    );
    $form['user']['grant_delete'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_delete']),
    );
    $form['user']['grant_admin_access'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_admin_access']),
    );
    $form['user']['grant_add_child'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_add_child']),
    );
    $form['user']['grant_edit_outline'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($grants['grant_edit_outline']),
    );
    $form['user']['add_user'] = array(
      '#type'  => 'submit',
      '#value' => t('Add user'),
      '#submit' => array('book_access_ui_user_add_submit'),
    );

    $form['submit'] = array(
      '#type'  => 'submit',
      '#value' => t('Save configuration'),
    );

    // the ability to reset just this book to defaults
    $form['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
      '#submit' => array('book_access_ui_reset_one_to_defaults_submit'),
    );

  }

  return $form;
}

/**
 * Form submissions callback for to reset one book node's defaults.
 *
 * @see book_access_ui_grants_form_access()
 */
function book_access_ui_reset_one_to_defaults_submit($form, &$form_state) {
  $bid = $form['#bid'];
  BookAccess::resetToDefault( $bid);

  // rebuild permissions
  $node = node_load( $bid, NULL, TRUE);
  if ($node) {
    node_access_acquire_grants( $node);
  }
  else {
    drupal_set_message( "Could not rebuild permissions for this book", "error");
  }

  cache_clear_all();
}

/**
 * Simple form handler to reset all book access to defaults.
 */
function book_access_ui_reset_all_to_defaults($form, &$form_state) {
  $form['destination'] = array(
    '#type' => 'hidden',
    '#value' => drupal_get_destination(),
  );

  $form['book_access_rebuild']['heading'] = array(
    '#markup' => t('<h1>Reset all to default</h1>'),
  );

  $form['book_access_rebuild']['content'] = array(
    '#markup' => t('<p>This will reset <em>all</em> book permissions to their defaults.  This will leave user-specific permissions alone, but otherwise everything will get clobbered.</p><p>Be very sure this is what you want to do, this is irreversible!</p>'),
  );

  $form['return'] = array(
    '#markup' => t('<p><a href=\'/admin/content/book/settings\'>Never mind, go back!</a></p>'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('I\'m sure, reset all (non-user-specific) book permissions to their defaults!'),
  );

  return $form;
}

/**
 * Form submissions callback for book_access_ui_reset_all_to_defaults().
 *
 * @see book_access_ui_reset_all_to_defaults()
 */
function book_access_ui_reset_all_to_defaults_submit($form, &$form_state) {
  $allBids = array();
  $resultSet =
    db_select( 'book', 'book')
    ->fields( 'book', array( 'bid'))
    ->distinct()
    ->execute();
  while ($row = ($resultSet->fetchAssoc())) {
    $allBids[] = $row['bid'];
  }
  $batch = array(
    'title' => t('Resetting all book access permissions'),
    'operations' => array(
      array('_book_access_ui_reset_batch_operation', array($allBids)),
    ),
    'finished' => '_book_access_ui_reset_batch_finished',
    'progress_message' => '',
    'file' => drupal_get_path('module', 'book_access') . '/book_access_ui.admin.inc',
  );
  batch_set( $batch);
}

/**
 * Form submission callback for book_access_ui_grants_form().
 *
 * @see book_access_ui_grants_form()
 */
function book_access_ui_user_add_submit($form, &$form_state) {
  $bool = (
    ($username = trim($form_state['values']['user']['username'])) &&
    ($user = user_load_by_name($username))
  );

  if ($bool) {
    $grants = array();
    $uid = $user->uid;
    $grants['grant_view'][$uid] = $form_state['values']['user']['grant_view'];
    $grants['grant_update'][$uid] = $form_state['values']['user']['grant_update'];
    $grants['grant_delete'][$uid] = $form_state['values']['user']['grant_delete'];
    $grants['grant_admin_access'][$uid] = $form_state['values']['user']['grant_admin_access'];
    $grants['grant_add_child'][$uid] = $form_state['values']['user']['grant_add_child'];
    $grants['grant_edit_outline'][$uid] = $form_state['values']['user']['grant_edit_outline'];

    BookAccess::addUserGrants($form['#bid'], array($uid), $grants);
    _book_access_ui_permissions_warning();
  }
  elseif ($username) {
    drupal_set_message(t("User %user has not been found.", array('%user' => $username)), 'error');
  }
}

/**
 * Form submission callback for book_access_ui_grants_form().
 *
 * @see book_access_ui_grants_form()
 */
function book_access_ui_grants_form_submit($form, &$form_state) {
  $bid = $form['#bid'];

  BookAccess::setAuthorGrants($bid, $form['#author'], $form_state['values']['author']);
  BookAccess::setRoleGrants($bid, $form['#rids'], $form_state['values']['roles']);

  if (!empty($form['#uids'])) {
    BookAccess::setUserGrants($bid, $form['#uids'], $form_state['values']['users']);
  }

  $batch = array(
    'title' => t('Rebuilding book access permissions'),
    'operations' => array(
      array('_book_access_ui_rebuild_batch_operation', array($bid)),
    ),
    'finished' => '_book_access_ui_rebuild_batch_finished',
    'progress_message' => '',
    'file' => drupal_get_path('module', 'book_access') . '/book_access_ui.admin.inc',
  );

  batch_set($batch);
}

/**
 * Form builder for the remove permissions confirmation form.
 *
 * @see book_access_ui_user_permissions_remove_submit()
 * @ingroup forms
 */
function book_access_ui_user_permissions_remove($form, $form_state, $node, $user) {
  $form['#bid'] = $node->book['bid'];
  $form['#uid'] = $user->uid;

  $top_node = node_load($form['#bid']);
  $title = $top_node ? $top_node->title : ('#' . $node->book['bid']);

  return confirm_form(
    $form,
    t('Are you sure you want to remove the user %user permissions for the book %title?', array('%user' => $user->name, '%title' => $title)),
    'node/' . $form['#bid'] . '/book_access',
     t('This action cannot be undone.'), t('Remove')
  );
}

/**
 * Form submission handler for book_access_ui_user_permissions_remove().
 *
 * @see book_access_ui_user_permissions_remove()
 */
function book_access_ui_user_permissions_remove_submit($form, &$form_state) {
  db_delete('book_access_user')
    ->condition('nid', $form['#bid'])
    ->condition('uid', $form['#uid'])
    ->execute();

  _book_access_ui_permissions_warning();

  $form_state['redirect'] = 'node/' . $form['#bid'] . '/book_access';
}

/**
 * Simple form handler to launch a rebuild-all pipeline.
 */
function book_access_ui_rebuild_all($form, &$form_state) {
  $form['book_access_rebuild']['heading'] = array(
    '#markup' => t('<h1>Rebuild all book access permissions</h1>'),
  );

  $form['book_access_rebuild']['content'] = array(
    '#markup' => t('<p>This will rebuild <em>all</em> book permissions, refreshing node_access with values from book_access_author, book_access_role, book_access_user.</p><p><strong>Only</strong> do this if you have a very good reason to suspect that your node_access and book_access permissions are out of sync, as if you do this while there is a bug with your book_access permissions, you will propagate them to <em>all</em> books.</p>'),
  );

  $form['return'] = array(
    '#markup' => t('<p><a href=\'/admin/content/book/settings\'>Never mind, go back!</a></p>'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('I\'m sure, rebuild permissions!'),
  );

  return $form;
}

/**
 * Form submissions handler for book_access_ui_rebuild_all().
 *
 * @see book_access_ui_rebuild_all()
 */
function book_access_ui_rebuild_all_submit($form, &$form_state) {
  $allBids = array();
  $resultSet =
    db_select( 'book', 'book')
    ->fields( 'book', array( 'bid'))
    ->distinct()
    ->execute();
  while ($row = ($resultSet->fetchAssoc())) {
    $allBids[] = $row['bid'];
  }
  $batch = array(
    'title' => t('Rebuilding all book access permissions'),
    'operations' => array(
      array('_book_access_ui_rebuild_batch_operation', array($allBids)),
    ),
    'finished' => '_book_access_ui_rebuild_batch_finished',
    'progress_message' => '',
    'file' => drupal_get_path('module', 'book_access') . '/book_access_ui.admin.inc',
  );
  batch_set( $batch);
}

function _book_access_ui_reset_batch_operation($bid, &$context) {
  if (! is_array( $bid)) {
    $bid = array( $bid);
  }

  if (empty($context['sandbox'])) {
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = db_select('book', 'book')
      ->condition('book.bid', $bid, 'IN')
      ->fields('book', array('nid'))
      ->distinct()
      ->countQuery()
      ->execute()
      ->fetchField();
    $context['sandbox']['progress'] = 0;
  }

  $result = db_select('book', 'b')
    ->fields('b', array('bid', 'nid'))
    ->condition('b.nid', $context['sandbox']['current_node'], '>')
    ->condition('b.bid', $bid, 'IN')
    ->orderBy('b.nid')
    ->distinct()
    ->range(0, 20)
    ->execute();

  $list = '';
  foreach ($result as $row) {
    $node = node_load( $row->nid);
    BookAccess::resetToDefault( $row->nid);
    $list .= t('<li>%title</li>', array( '%title' => $node->title));

    $context['sandbox']['progress']++;
    $context['sandbox']['current_node'] = $row->nid;
  }
  $context['message'] = t('Resetting permissions for:<ul>!list</ul>',
    array('!list' => $list));

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function _book_access_ui_reset_batch_finished($success, $results, $operations) {
  global $base_url;

  if ($success) {
    drupal_set_message(t('Book permissions have been reset to their defaults.'));
    drupal_set_message(t('You will need to <a href=\'!base/book_access/rebuild_all\'>rebuild all permissions</a> to propagate the reset.', array('!base' => $base_url)));
    node_access_needs_rebuild(FALSE);
  }
  else {
    drupal_set_message(t('Book access permissions have not been properly reset (uh oh).'), 'error');
  }
  cache_clear_all();
}

/**
 * Themes the permission tab added to each book.
 *
 * @param $variables
 *   An associative array containing:
 *     - form: A render element representing the form.
 *
 * @see book_access_ui_grants_form()
 * @ingroup themeable
 */
function theme_book_access_ui_grants_form($variables) {
  $form = $variables['form'];
  $rows = array();

  if ($form['#author']) {
    $rows[] = array(
      drupal_render($form['author']['name']),
      drupal_render($form['author']['grant_admin_access']),
      drupal_render($form['author']['grant_update']),
      drupal_render($form['author']['grant_delete']),
      drupal_render($form['author']['grant_add_child']),
      drupal_render($form['author']['grant_edit_outline']),
      drupal_render($form['author']['grant_view']),
      ' ',
    );
  }

  if (isset($form['#rids'])) {
    foreach ($form['#rids'] as $key) {
      $row = array();

      $row[] = drupal_render($form['roles']['names'][$key]);
      $row[] = drupal_render($form['roles']['grant_admin_access'][$key]);
      $row[] = drupal_render($form['roles']['grant_update'][$key]);
      $row[] = drupal_render($form['roles']['grant_delete'][$key]);
      $row[] = drupal_render($form['roles']['grant_add_child'][$key]);
      $row[] = drupal_render($form['roles']['grant_edit_outline'][$key]);
      $row[] = drupal_render($form['roles']['grant_view'][$key]);
      $row[] = ' ';

      $rows[] = $row;
    }
  }

  if (isset($form['#uids'])) {
    foreach ($form['#uids'] as $key) {
      $row = array();
      $user = user_load($key);

      $row[] = theme('username', array('account' => $user));
      $row[] = drupal_render($form['users']['grant_admin_access'][$key]);
      $row[] = drupal_render($form['users']['grant_update'][$key]);
      $row[] = drupal_render($form['users']['grant_delete'][$key]);
      $row[] = drupal_render($form['users']['grant_add_child'][$key]);
      $row[] = drupal_render($form['users']['grant_edit_outline'][$key]);
      $row[] = drupal_render($form['users']['grant_view'][$key]);
      $row[] = l(t('delete'), 'book_access/delete/user_permission/' . $form['#bid'] . "/$key");

      $rows[] = $row;
    }
  }

  $rows[] = array(
    drupal_render($form['user']['username']),
    drupal_render($form['user']['grant_admin_access']),
    drupal_render($form['user']['grant_update']),
    drupal_render($form['user']['grant_delete']),
    drupal_render($form['user']['grant_add_child']),
    drupal_render($form['user']['grant_edit_outline']),
    drupal_render($form['user']['grant_view']),
    drupal_render($form['user']['add_user']),
  );

  $header = array(
    t('Role/user'),
    t('Administer access'),
    t('Edit pages'),
    t('Delete pages'),
    t('Add child pages'),
    t('Edit outline'),
    t('View this book'),
    t('Operations'),
  );

  return theme('table', array('header' => $header, 'rows' => $rows)) .
    drupal_render_children($form);
}

/**
 * Themes the settings fieldset added to the book settings page.
 *
 * @param $variables
 *   An associative array containing:
 *     - form: A render element representing the form.
 *
 * @see book_access_ui_form_book_admin_settings_alter()
 * @ingroup themeable
 */
function theme_book_access_ui_settings($variables) {
  $form = $variables['form'];
  $header = array(t('Access type'));
  $options = $form['#options'];
  $rows = array();

  foreach ($options as $option => $title) {
    $header[] = $title;
  }

  foreach (element_children($form['grants']) as $key) {
    $row = array();

    $row[] = $form['grants'][$key]['#title'];
    unset($form['grants'][$key]['#title']);

    foreach ($options as $option => $title) {
      $row[] = drupal_render($form['grants'][$key][$option]);
    }

    $rows[] = $row;
  }

  return drupal_render($form['info']) .
    theme('table', array('header' => $header, 'rows' => $rows)) .
    drupal_render_children($form);
}

/**
 * Shows the warning message that reminds the users with the permissions to
 * edit the book permissions that they need to save the settings.
 */
function _book_access_ui_permissions_warning() {
  drupal_set_message(t("The changes will not be saved until the <em>Save configuration</em> button is clicked."), 'warning', FALSE);
}

function _book_access_ui_rebuild_batch_operation($bid, &$context) {
  if (! is_array( $bid)) {
    $bid = array( $bid);
  }

  if (empty($context['sandbox'])) {
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = db_select('book', 'book')
      ->condition('book.bid', $bid, 'IN')
      ->fields('book', array('nid'))
      ->distinct()
      ->countQuery()
      ->execute()
      ->fetchField();
    $context['sandbox']['progress'] = 0;
  }

  $result = db_select('book', 'b')
    ->fields('b', array('bid', 'nid'))
    ->condition('b.nid', $context['sandbox']['current_node'], '>')
    ->condition('b.bid', $bid, 'IN')
    ->orderBy('b.nid')
    ->distinct()
    ->range(0, 20)
    ->execute();

  $list = '';
  foreach ($result as $row) {
    if ($node = node_load($row->nid, NULL, TRUE)) {
      node_access_acquire_grants($node);
      $list .= t('<li>%title</li>', array( '%title' => $node->title));
    }

    $context['sandbox']['progress']++;
    $context['sandbox']['current_node'] = $row->nid;
  }
  $context['message'] = t('Rebuilding access permissions for:<ul>!list</ul>',
    array('!list' => $list));

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function _book_access_ui_rebuild_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The book access permissions have been rebuilt.'));
    node_access_needs_rebuild(FALSE);
  }
  else {
    drupal_set_message(t('The book access permissions have not been properly rebuilt.'), 'error');
  }
  cache_clear_all();
}
