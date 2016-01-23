<?php

/**
 * @file
 * User page callbacks for the tracker module.
 */

/**
 * Menu callback. Prints a listing of active nodes on the site.
 */
function trackerlite_page($account = NULL, $set_title = FALSE) {
  if ($account) {
    if ($set_title) {
      // When viewed from user/%user/track, display the name of the user
      // as page title -- the tab title remains Track so this needs to be done
      // here and not in the menu definiton.
      drupal_set_title($account->name);
    }
  // TODO: These queries are very expensive, see http://drupal.org/node/105639
    $sql = 'SELECT DISTINCT(n.nid), n.title, n.type, n.changed, n.uid, u.name, n.changed AS last_updated FROM {node} n  INNER JOIN {users} u ON n.uid = u.uid WHERE n.status = 1 AND n.uid = %d ORDER BY last_updated DESC';
    $sql = db_rewrite_sql($sql);
    $sql_count = 'SELECT COUNT(DISTINCT(n.nid)) FROM {node} n WHERE n.status = 1 AND n.uid = %d';
    $sql_count = db_rewrite_sql($sql_count);
    $result = pager_query($sql, 25, 0, $sql_count, $account->uid);
  }
  else {
    $sql = 'SELECT DISTINCT(n.nid), n.title, n.type, n.changed, n.uid, u.name, n.changed AS last_updated FROM {node} n INNER JOIN {users} u ON n.uid = u.uid WHERE n.status = 1 ORDER BY last_updated DESC';
    $sql = db_rewrite_sql($sql);
    $sql_count = 'SELECT COUNT(n.nid) FROM {node} n WHERE n.status = 1';
    $sql_count = db_rewrite_sql($sql_count);
    $result = pager_query($sql, 25, 0, $sql_count);
  }

  $rows = array();
  $types = node_type_get_names();
  while ($node = db_fetch_object($result)) {
    $rows[] = array(
      check_plain($types[$node->type]),
      l($node->title, "node/$node->nid") . ' ' . theme('mark', node_mark($node->nid, $node->changed)),
      theme('username', $node),
      t('!time ago', array('!time' => format_interval(REQUEST_TIME - $node->last_updated)))
    );
  }

  if (!$rows) {
    $rows[] = array(array('data' => t('No posts available.'), 'colspan' => '5'));
  }

  $header = array(t('Type'), t('Post'), t('Author'), t('Last updated'));

  $output = '<div id="tracker">';
  $output .= theme('table', $header, $rows);
  
  #$output .= theme('pager', NULL, 25, 0);
  $output .= theme('pager');
  $output .= '</div>';

  return $output;
}
