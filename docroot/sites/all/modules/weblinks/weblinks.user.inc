<?php
/**
 * @file
 * Displays a Drupal page containing weblinks submitted by a given user.
 * Drupal 7 Version
 */

/**
 * Callback function for user/%user/weblinks
 *
 * @param object $account
 *   The account of the user page being viewed.
 */
function weblinks_user_page($account) {
  $save_hp = @$account->homepage;
  unset($account->homepage);
  $trail = array(l(t('Home'), ''), theme('username', array('account' => $account, 'levels' => FALSE)));
  if ($save_hp) {
    $account->homepage = $save_hp;
  }
  drupal_set_breadcrumb($trail);

  $form = drupal_get_form('weblinks_user_form', $account);
  $output = theme('weblinks_user_form', array('form' => $form));
  return $output;
}

/**
 * Helper function to return if user should get the edit and delete links.
 *
 * @param object $user
 *   The current user.
 * @param object $account
 *   The account of the user page being viewed.
 * @param string $op
 *   The operation to check for, either 'edit' or 'delete'.
 */
function _weblinks_user_access_check($user, $account, $op) {
  // Allow access if the user has the $op permission for any weblinks nodes.
  if (user_access($op . ' any weblinks content', $user)) {
    return TRUE;
  }
  // If the above failed, but the user is viewing their own links then give
  // access if they have the $op permission for their own weblinks.
  if (user_access($op . ' own weblinks content', $user) && ($account->uid == $user->uid)) {
    return TRUE;
  }
  // Weblinks administrators can edit or delete any weblinks content.
  return user_access('administer weblinks', $user);
}

/**
 * Return form for user page.
 */
function weblinks_user_form($form, &$form_state, $account) {
  global $user;
  $vocid = _weblinks_get_vocid();
  $unclassed = _weblinks_unclassed();
  $destination = drupal_get_destination();
  $options = array();
  $now = $_SERVER['REQUEST_TIME'];
  $limit = 20;
  $node_ops = module_invoke_all('node_operations');

  // Get the table and field name of the vocabulary being used for weblinks.
  $vocab_info = _weblinks_vocabulary_field_info();

  // Get all Web Links owned by this user.
  $query = db_select('node', 'n');
  $query->extend('PagerDefault')
    ->limit($limit);
  $query->condition('n.uid', $account->uid, '=')
    ->condition('n.type', 'weblinks', '=')
    ->fields('n', array('nid', 'title', 'status', 'changed', 'promote', 'sticky'))
    ->orderBy('n.changed', 'DESC');
  $query_result = $query->execute();

  foreach ($query_result as $node) {
    $status = array();
    $status[] = $node->status ? t('published') : t('not published');
    if ($node->promote) {
      $status[] = t('promoted');
    }
    if ($node->sticky == 1) {
      $status[] = t('sticky');
    }

    // We don't want to "mark" our own content.
    if ($account->uid == $user->uid) {
      $mark = NULL;
    }
    else {
      $mark = ' ' . theme('mark', array('type' => node_mark($node->nid, $node->changed)));
    }
    $form['title'][$node->nid] = array('#markup' => l($node->title, 'node/' . $node->nid) . $mark);
    $form['status'][$node->nid] =  array('#markup' => implode(', ', $status));
    $form['update'][$node->nid] =  array('#markup' => format_interval($now - $node->changed));

    $tid_list = array();
    // If $vocid is not empty, we know that the taxonomy module is available
    // and the field tables will exist.
    if ($vocid) {
      // Get the term names associated with this node.
      $query = db_select($vocab_info['table_name'], 't');
      $query->fields('t', array('entity_id', $vocab_info['tid']));
      $query->join('taxonomy_term_data', 'tt', 'tt.tid = t.' . $vocab_info['tid']);
      $query->fields('tt', array('name'));
      $query->condition('entity_id', $node->nid);
      $result = $query->execute();

      if ($result->rowCount() > 0) {
        foreach ($result as $row) {
          $tid_list[] = check_plain($row->name);
        }
      }
      else {
        $tid_list[] = $unclassed->name;
      }
    }
    $form['group'][$node->nid] =  array('#markup' => implode(', ', $tid_list));

    // Provide 'edit' and 'delete' operations if the user has the authority.
    static $edit_link, $delete_link;
    if (!isset($edit_link)) {
      $edit_link = _weblinks_user_access_check($user, $account, 'edit');
      $delete_link = _weblinks_user_access_check($user, $account, 'delete');
    }
    if ($edit_link) {
      $form['operationse'][$node->nid] = array('#markup' => l(t('edit'), 'node/' . $node->nid . '/edit', array('query' => $destination)));
    }
    if ($delete_link) {
      $form['operationsd'][$node->nid] = array('#markup' => l(t('delete'), 'node/' . $node->nid . '/delete', array('query' => $destination)));
    }
  }
  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}

/**
 * Theme node administration overview.
 */
function theme_weblinks_user_form($variables) {
  $form = $variables['form'];
  // Overview table:
  $header = array(t('Title'), t('Status'), t('Group'), t('Last updated'), array('data' => t('Operations'), 'colspan' => '2'));
  $rows =array();

  if (isset($form['title']) && is_array($form['title'])) {
    foreach (element_children($form['title']) as $key) {
      $rows[] = array(
        drupal_render($form['title'][$key]),
        drupal_render($form['status'][$key]),
        drupal_render($form['group'][$key]),
        drupal_render($form['update'][$key]),
        drupal_render($form['operationse'][$key]),
        drupal_render($form['operationsd'][$key]),
        );
    }
  }
  else  {
    $rows[] = array(array('data' => t('No posts available.'), 'colspan' => '6'));
  }
  $output = theme('table', array(
                    'header'     => $header,
                    'rows'       => $rows,
                    'attributes' => array('class' => array('pagerer'))));
  if ($form['pager']['#markup']) {
    $output .= drupal_render($form['pager']);
  }
  // $output .= drupal_render($form);  // Not needed in D7!
  return $output;
}

