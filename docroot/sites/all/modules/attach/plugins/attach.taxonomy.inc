<?php
/**
 * Renders attached taxonomy.
 */
function attach_taxonomy_process($params) {
  $result = db_query_range("
  SELECT n.title, n.nid FROM {node} n
  INNER JOIN {node_revisions} nv ON n.vid = nv.vid
  INNER JOIN {taxonomy_term_node} tn ON n.vid = tn.vid
  WHERE tn.tid = :tid
  ORDER BY n.changed DESC
  ", 0, $params['count'], array(':tid' => $params['tid']));

  $items = array();
  while ($row = db_fetch_object($result)) {
    $items[] = l($row->title, 'node/' . $row->nid);
  }
  if ($items) {
    return theme_item_list($items);
  }
}
