<?php
/**
 * @file
 * Override of the hook theme
 */


function tb_events_pager($vars) {
  $element = $vars['element'];
  global $pager_page_array, $pager_total;
  $pager_max = $pager_total[$element];
  if($pager_max > 5) {
    $vars['quantity'] = 4;
  }
  else {
    $vars['quantity'] = 5;
  }
  $vars['tags'] = array('first', 'previous', '', 'next', 'last');
  // End of marker calculations.
  return theme_pager($vars);
}
