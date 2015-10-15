<?php
/**
 * @file
 * Contains the callback for the VR test "Node user is author".
 */

/**
 * Callback defined by node_virtual_roles() in core.inc
 */
function node_user_is_author($op, $account = NULL) {
  if (is_null($account)) {
    global $user;
    $account = $user;
  }
  switch ($op) {
    case 'cache' :
      if (arg(0) == 'node' && is_numeric(arg(1))) {
        $nid = arg(1);
        return "node_user_is_author:$account->uid:$nid";
      }
      return NULL;
      break;
    case 'process' :
      $nid = arg(1);
      $result = db_query_range("SELECT n.uid FROM {node} n WHERE n.nid = :nid", 0, 1, array(':nid' => $nid));
      foreach ($result as $record) {
        return $record->uid == $account->uid;
      }
      return FALSE;
      break;
  }
}
