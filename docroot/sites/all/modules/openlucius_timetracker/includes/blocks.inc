<?php
/**
 * @file
 * This file contains all blocks for the time-tracker.
 */

/**
 * Implements hook_block_info().
 */
function openlucius_timetracker_block_info() {

  // Initialize empty array.
  $blocks = array();

  // OL Time tracker block.
  $blocks['openlucius_timetracker_timer'] = array(
    'info' => t('Openlucius time tracker timer'),
  );

  // OL Time tracker active timers.
  $blocks['openlucius_active_timers'] = array(
    'info' => t('Openlucius time tracker active timers'),
  );
  // OL Time tracker block.
  $blocks['openlucius_filter'] = array(
    'info' => t('Openlucius time tracker filter'),
  );

  // OL Time tracker block.
  $blocks['openlucius_budget_remaining'] = array(
    'info' => t('Openlucius time tracker budget remaining'),
  );
  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function openlucius_timetracker_block_view($delta = '') {

  // Initialize empty array.
  $block = array();

  switch ($delta) {

    // OL group heading block with buttons.
    case 'openlucius_timetracker_timer':
      $block['title']   = '<none>';
      $block['subject'] = '<none>';
      $block['content'] = drupal_get_form('openlucius_timetracker_timer_form');
      break;

    case 'openlucius_filter':
      $block['title']   = 'Filter';
      $block['subject'] = '<none>';
      $block['content'] = drupal_get_form('openlucius_timetracker_filter_form');
      break;

    case 'openlucius_budget_remaining':
      $block['title']   = 'Budget remaining';
      $block['subject'] = '<none>';
      $block['content'] = openlucius_timetracker_budget_remaining_block();
      break;

    case 'openlucius_active_timers':
      $block['title']   = '<none>';
      $block['subject'] = '<none>';
      $block['content'] = openlucius_timetracker_active_timers_block();
      break;
  }

  return $block;
}

/**
 * Function for rendering the active timer block.
 *
 * @return string|FALSE
 *   Returns either the html for the timer block or FALSE.
 */
function openlucius_timetracker_active_timers_block() {
  // Load the functions for the time-tracker.
  module_load_include('inc', 'openlucius_timetracker', 'includes/functions');
  module_load_include('inc', 'openlucius_timetracker', 'includes/processing');

  $menu_object = menu_get_object();
  // Check if the user has access and whether the user is not on a group node.
  if (user_access('create time entries') && !(isset($menu_object->type) && $menu_object->type == 'ol_group')) {

    $output = '<h3 class="block-title">' . t('Active timers') . '</h3>';
    $output .= '<span class="other-users">' . _openlucius_timetracker_activity() . '</span>';
    return $output;
  }

  return FALSE;
}

/**
 * Function for generating the budget remaining block.
 *
 * @return bool|string
 *   Returns HTML chart for a pretty overview or FALSE.
 */
function openlucius_timetracker_budget_remaining_block() {

  // Only show if the user has rights to create time entries.
  if (!user_access('create time entries')) {
    return FALSE;
  }

  // Load reports functions for pages.
  module_load_include('inc', 'openlucius_timetracker', 'includes/page');

  // Load the node.
  $node = menu_get_object();
  if (!empty($node) && $node->nid) {

    // If this is an ol_todo load the todolist this node is attached to.
    if ($node->type == 'ol_todo') {
      $node = node_load($node->field_todo_list_reference[LANGUAGE_NONE][0]['nid']);
    }

    // Check if the todolist has a budget set.
    if (!empty($node->field_todolist_budget)) {

      // Load processing functions.
      module_load_include('inc', 'openlucius_timetracker', 'includes/processing');

      $budget            = check_plain($node->field_todolist_budget[LANGUAGE_NONE][0]['value']);
      $total             = openlucius_timetracker_todolist_reports($node, OPENLUCIUS_TIMETRACKER_RAW);
      $data[t('Budget')] = $budget;
      $data[t('Total')]  = openlucius_timetracker_format_time($total, '.');

      // Load the javascript required for generating the charts.
      _openlucius_timetracker_add_chart_js();

      // Make the data required for the charts available for javascript.
      drupal_add_js(array(
        'openlucius_timetracker' => array(
          'data'       => $data,
          'zero_start' => TRUE,
        ),
      ), 'setting');

      // Return Canvas for rendering the charts.
      return '<canvas id="timeTrackerOverview" width="200" height="200"></canvas>';
    }
  }

  // Reports page.
  else {
    $result = db_query("SELECT n.nid FROM {node} n
    INNER JOIN {field_data_field_todolist_budget} b
    ON b.entity_id = n.nid
    WHERE n.type = 'ol_todo_list' AND n.status = 1");

    // Initialize array.
    $rows = array();

    foreach ($result as $item) {
      $node   = node_load($item->nid);
      $budget = check_plain($node->field_todolist_budget[LANGUAGE_NONE][0]['value']) * 3600;
      $total  = openlucius_timetracker_todolist_reports($node, OPENLUCIUS_TIMETRACKER_BLOCK);

      $rows[] = array(
        l($node->title, 'node/' . $node->nid . '/time'),
        date('d M', $node->created),
        openlucius_timetracker_format_time($budget),
        openlucius_timetracker_calculate_progress_raw($total['sec'], $budget, openlucius_timetracker_format_time($total)),
      );
    }
    $header = array(t('Project'), t('Start'), t('Budget'), t('Clocked'));
    return theme('table', array('header' => $header, 'rows' => $rows));
  }

  return FALSE;
}
