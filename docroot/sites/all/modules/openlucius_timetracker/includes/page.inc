<?php
/**
 * @file
 * This file contains the functions for the page callbacks.
 */

/**
 * Callback function for the reports page.
 *
 * @param \stdClass|NULL $node
 *   The node to be processed.
 *
 * @return string|FALSE
 *   Either the page containing the reports or FALSE.
 */
function openlucius_timetracker_reports_page($node = NULL) {
  // Overview so load all items.
  if ($node === NULL) {

    // Return the user reports page.
    return openlucius_timetracker_reports();
  }
  // Detail view load only the required items.
  elseif (isset($node->type)) {

    // Prevent access if the user is not in the group.
    if (!openlucius_core_access_to_current_group()) {
      drupal_access_denied();
    }

    // Switch on node type.
    switch ($node->type) {

      case 'ol_todo':
        return openlucius_timetracker_todo_reports($node);

      case 'ol_todo_list':
        return openlucius_timetracker_todolist_reports($node);

      // If a type does not exist in the tracker allow other modules
      // to return the page output.
      default:
        $output = module_invoke_all('openlucius_timetracker_reports_page', $node);
        return (!isset($output) || empty($output)) ? FALSE : $output;
    }
  }
}

/**
 * Function to render the output for a todolist.
 *
 * @param \stdClass $node
 *   The node to be processed.
 *
 * @param string $type
 *   Whether a rendered or a raw version should be returned.
 *
 * @return mixed
 *   Either an array containing the data or a rendered html version.
 */
function openlucius_timetracker_todolist_reports(\stdClass $node, $type = OPENLUCIUS_TIMETRACKER_DEFAULT) {

  // Load timetracker functions.
  module_load_include('inc', 'openlucius_timetracker', 'includes/functions');
  module_load_include('inc', 'openlucius_timetracker', 'includes/processing');

  // Get the advanced query and add the node id as a condition.
  $query = openlucius_timetracker_advanced_query();
  $query->condition('field_todo_list_reference_nid', $node->nid, '=');

  // Execute query.
  $results = $query->execute();

  // Process the results.
  $processed = openlucius_timetracker_process_results($results, $type);

  // Return raw values when raw amount is requested.
  if ($type === OPENLUCIUS_TIMETRACKER_RAW || $type === OPENLUCIUS_TIMETRACKER_BLOCK) {
    return $processed['total']['sec'];
  }

  // Build table options.
  $table_options = openlucius_timetracker_build_list_table($processed['list'], $processed['total']['sec']);

  // Append link back to node.
  $output = l(t('back to !node', array('!node' => $node->title)), 'node/' . $node->nid);

  // Finally build the table.
  $output .= theme('table', $table_options);

  // Allow other modules to change the output if needed.
  drupal_alter('timetracker_todolist_output', $output, $processed);

  return $output;
}

/**
 * Function to render the output for a todo.
 *
 * @param \stdClass $node
 *   The node to be processed.
 *
 * @param string $type
 *   Whether a rendered or a raw version should be returned.
 *
 * @return mixed
 *   Either an array containing the data or a rendered html version.
 */
function openlucius_timetracker_todo_reports(\stdClass $node, $type = OPENLUCIUS_TIMETRACKER_DEFAULT) {

  // Load timetracker functions.
  module_load_include('inc', 'openlucius_timetracker', 'includes/functions');
  module_load_include('inc', 'openlucius_timetracker', 'includes/processing');

  // Get the basic query for the timetracker.
  $query = openlucius_timetracker_basic_query();
  $query->condition('e.nid', $node->nid, '=');

  // Join for user names.
  $query->join('users', 'u', 'e.uid = u.uid');
  $query->fields('u', array('name'));

  // Execute query.
  $results = $query->execute();

  // Process the results.
  $processed = openlucius_timetracker_process_results($results, $type);

  // Return raw if requested.
  if ($type === OPENLUCIUS_TIMETRACKER_RAW) {
    return $processed;
  }

  // Build the table options.
  $table_options = openlucius_timetracker_build_todo_table($processed['list'], $processed['total']['sec']);

  // Append link back to node.
  $output = l(t('back to !node', array('!node' => $node->title)), 'node/' . $node->nid);

  // Finally build the table.
  $output .= theme('table', $table_options);

  // Append the extra time form.
  $output .= drupal_render(drupal_get_form('openlucius_timetracker_extra_time_form'));

  // Allow other modules to change the output if needed.
  drupal_alter('timetracker_output', $output, $processed);

  // Default reports.
  return $output;
}

/**
 * Function to render the output reports output.
 *
 * @return mixed
 *   Either an array containing the data or a rendered html version.
 */
function openlucius_timetracker_reports() {

  // Load timetracker functions.
  module_load_include('inc', 'openlucius_timetracker', 'includes/functions');
  module_load_include('inc', 'openlucius_timetracker', 'includes/processing');

  // Get the advanced query and add the node id as a condition.
  $query = openlucius_timetracker_advanced_query();

  // Get any filters from the url.
  $filter = openlucius_timetracker_get_filter();

  // Extend query.
  $end_of_day = strtotime("tomorrow", $filter['end']) - 1;
  $query->condition('start', array($filter['start'], $end_of_day), 'BETWEEN');

  // State can happen on both versions and have the same query attached to it.
  if (isset($filter['state'])) {
    $query->condition('field_todo_label_tid', $filter['state'], 'IN');
  }

  // Execute query.
  $results = $query->execute();

  // Process the results.
  if (isset($filter['user'])) {
    $processed = openlucius_timetracker_process_results($results, OPENLUCIUS_TIMETRACKER_USER);
  }
  else {
    $processed = openlucius_timetracker_process_results($results, OPENLUCIUS_TIMETRACKER_DEFAULT);
  }

  // Theme processed data.
  return openlucius_timetracker_reports_processing($processed, $filter);
}
