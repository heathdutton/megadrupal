<?php

class menupoly_AccessChecker {

  /**
   * This function is much more innocent than _menu_tree_check_access().
   * It does not actually remove anything,
   * it just sets the $item['access'] to either TRUE or FALSE.
   */
  function itemsCheckAccess(&$items) {

    // Collect node links
    $node_links = array();
    foreach ($items as $mlid => &$item) {
      if ($item['router_path'] === 'node/%') {
        $nid = substr($item['link_path'], 5);
        if (is_numeric($nid)) {
          $node_links[(int)$nid][$mlid] = $mlid;
        }
      }
    }

    // Adapted from menu_tree_check_access()
    if (!empty($node_links)) {
      $nids = array_keys($node_links);
      $select = db_select('node', 'n');
      $select->addField('n', 'nid');
      $select->condition('n.status', 1);
      $select->condition('n.nid', $nids, 'IN');
      $select->addTag('node_access');
      $nids = $select->execute()->fetchCol();
      foreach ($nids as $nid) {
        foreach ($node_links[$nid] as $mlid) {
          $items[$mlid]['access'] = TRUE;
        }
      }
    }

    // Process all items that are not node links.
    foreach ($items as $mlid => &$item) {
      _menu_link_translate($item);
    }
  }
}
