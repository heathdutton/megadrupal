<?php
/**
 * @file
 * le_gate.pages.inc
 */

/**
 * Page callback for Le Gate page.
 */
function le_gate_page() {
  $content = '';
  $content .= theme('le_gate_gate');
  return $content;
}


function le_gate_eval_path_is_gated() {
  // Check path.
  if (!isset($_GET['le-gate-request-path'])) {
    return drupal_json_output('error:no request-path specified');
  }
  $current_path = ltrim($_GET['le-gate-request-path'], '/');
  $return = 1;
  // If this is the le-gate path return.
  if (($current_path == LE_GATE_PATH) || ($current_path == LE_GATE_ADMIN_PATH)) {
    $return = 0;
  }
  // If admin page and configured to skip admin pages, return.
  $skip = variable_get('le_gate_skip_admin', 1);
  if ($skip && path_is_admin($current_path)) {
    $return = 0;
  }
  $paths = variable_get('le_gate_paths', '');
  // If excluded just return FALSE.

  if (($paths == '~') || drupal_match_path("~$current_path", $paths)) {
    $return = 0;
  }
  // Check if included, if not return FALSE.
  if (variable_get('le_gate_select_paths', FALSE) && (!($paths == '')) && !(drupal_match_path($current_path, $paths))) {
    $return = 0;
  }
  drupal_json_output($return);
}

function le_gate_eval_date() {
  // Get user entered date.
  if (!($check_date = le_gate_get_date_stamp(array('month' => $_GET['month'], 'day' => $_GET['day'], 'year' => $_GET['year'])))) {
    return drupal_json_output('error:no date specified');
  }
  // Get type of date comparision.
  $type = variable_get('le_gate_date_type', 'before');
  // Get the redirect url for after check.
  $redirect_url = variable_get('le_gate_date_redirect', '');
  // Get timestamp for configured date 1.
  $date1 = le_gate_get_date_stamp(variable_get('le_gate_date_1'));
  $return = 0;
  switch ($type) {
    case 'before':
      if ($check_date < $date1) {
        $return = 1;
      }
      break;
    case 'after':
      if ($check_date > $date1) {
        $return = 1;
      }
      break;
    case 'between':
      // Get timestamp for configured date 2.
      $date2 = le_gate_get_date_stamp(variable_get('le_gate_date_2'));
      if (($date1 < $check_date) && ($check_date < $date2)) {
        $return = 1;
      }
      break;
  }
  return drupal_json_output($return);
}
