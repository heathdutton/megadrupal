<?php

/**
 * @file
 * Displays a Drupal page containing quotes submitted by a given user.
 */

/**
 * Displays listing of quotes a user has created on their account page.
 */
function quotes_user_page($account) {
  $output = drupal_get_form('quotes_user_form', $account);
  return $output;
}

/**
 * Checks user access permissions.
 */
function _quotes_user_access_check($user, $account) {
  if (user_access('edit own quotes', $user) && ($account->uid == $user->uid)) {
    return TRUE;
  }
  return user_access('administer quotes', $user);
}

/**
 * Display quotes the user has created with edit/delete options.
 */
function quotes_user_form($form, $form_state, $account) {
  global $user;
  $destination = drupal_get_destination();
  $options = array();
  $now = $_SERVER['REQUEST_TIME'];
  $limit = variable_get('quotes_per_page', 10);
  $node_ops = module_invoke_all('node_operations');

  // Overview table:
  $header = array(
    t('Title'),
    t('Status'),
    t('Tags'),
    t('Last updated'),
    array('data' => t('Operations'), 'colspan' => '2'),
  );
  $rows = array();

  $query = db_select('node', 'n')->extend('PagerDefault');
  $query
    ->fields('n')
    ->condition('n.uid', $user->uid)
    ->condition('n.type', 'quotes')
    ->orderBy('n.changed', 'DESC')
    ->addTag('node_access');

  $result = $query->limit($limit)->execute()->fetchCol();
  $nodes = node_load_multiple($result);

  foreach ($nodes as $node) {
    $status = array();
    $status[] = $node->status ? t('published') : t('not published');
    if (isset($node->promoted) && $node->promoted) {
      $status[] = t('promoted');
    }
    if ($node->sticky > 0) {
      $status[] = t('sticky');
    }
    if (isset($node->moderated) && $node->moderated) {
      $status[] = t('moderated');
    }
    $form['title'][$node->nid] = array('#markup' => l($node->title, 'node/' . $node->nid) . ' ' . theme('mark', array('type' => node_mark($node->nid, $node->changed))));
    $form['status'][$node->nid] = array('#markup' => implode(', ', $status));
    $form['updated'][$node->nid] = array('#markup' => format_interval($now - $node->changed));

    $tid_list = array();

    $terms = db_query("SELECT tid, name FROM {taxonomy_index} LEFT JOIN {taxonomy_term_data} USING (tid) WHERE nid = :nid", array(':nid' => $node->nid))->fetchAll();

    if ($terms) {
      foreach ($terms as $row) {
        $tid_list[$row->tid] = check_plain($row->name);
      }

      $form['group'][$node->nid] = array('#markup' => implode(', ', $tid_list));
    }
    if (_quotes_user_access_check($user, $account)) {
      $form['operationse'][$node->nid] = array(
        '#markup' => l(t('edit'), 'node/' . $node->nid . '/edit', array(
          'query' => $destination,
          )
        ),
      );
      $form['operationsd'][$node->nid] = array(
        '#markup' => l(t('delete'), 'node/' . $node->nid . '/delete', array(
          'query' => $destination,
          )
        ),
      );
    }
  }
  foreach (element_children($form['title']) as $key) {
    $rows[] = array(
      drupal_render($form['title'][$key]),
      drupal_render($form['status'][$key]),
      drupal_render($form['group'][$key]),
      drupal_render($form['updated'][$key]),
      drupal_render($form['operationse'][$key]),
      drupal_render($form['operationsd'][$key]),
    );
  }

  $build['pager_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('You have not created any quotes'),
  );

  // Attach the pager theme.
  $build['pager_pager'] = array('#theme' => 'pager');

  return $build;
}
