<?php

/**
 * Given an integer tid, returns an array of tid and all children tids
 * @param  [type] $tid [description]
 * @return [type]      [description]
 */
function tid_merge_children($tid) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $tids = array();
    foreach(taxonomy_get_children($tid) as $child) {
      $tids[] = $child->tid;
    }

    // add origin tid;
    $tids[] = $tid;

    return $tids;
  }
}