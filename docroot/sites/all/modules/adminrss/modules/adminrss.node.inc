<?php
/**
 * @file
 * Node feed related functions.
 *
 * @see adminrss_adminrss_feed_info()
 */

/**
 * Feed builder: output a node feed for nodes under moderation.
 *
 * @return void
 *   Builder emits data and does not return any content.
 */
function adminrss_feed_node() {
  $sql = <<<EOT
  SELECT n.nid
  FROM {node} n
  INNER JOIN {users} u ON n.uid = u.uid
  WHERE n.status = 0
  ORDER BY n.changed DESC
EOT;

  // No db_rewrite_sql: this feed is for admins only.
  $results = db_query_range($sql, 0, 15);
  $nids = array();
  foreach ($results as $result) {
    $nids[] = $result->nid;
  }

  // Node_feed() does not include Atom support, so we cannot add <atom:self>.
  node_feed($nids, array(
    'description' => t('Unapproved Nodes for Administration'),
    'title' => t('@site - AdminRSS Nodes Feed', array('@site' => variable_get('site_name', 'drupal'))),
  ));
}
