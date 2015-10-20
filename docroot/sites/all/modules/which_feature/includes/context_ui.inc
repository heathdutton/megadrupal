<?php
/**
 * @file
 * Context UI module integration.
 */

/**
 *
 */
function context_ui_which_feature_ctools_table_row(&$delta, &$row) {
  if (count($row['data']) == 1) {
    $row['data'][0]['colspan']++;
    return FALSE;
  }
  else {
    list($tag, $delta) = explode(':', $delta);
    return TRUE;
  }
}
