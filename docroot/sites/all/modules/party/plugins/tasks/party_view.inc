<?php

/**
 * @file
 * Handle the 'party view' overrride task
 *
 * This plugin overrides party/%party/party/view and rerroutes it to the page
 * manager, where a list of tasks can be used to service this request based
 * upon criteria supplied by acess plugins
 */

/**
 * Implements hook_page_manager_task_tasks()
 */
function party_party_view_page_manager_tasks() {
  return array(
    // This is a 'page' task and will fall under the pade admin ui
    'task type' => 'page',
    'title' => t('Party Template'),
    'admin title' => t('Party Template'),
    'admin description' => t('When enabled this overides the default behaviour for <em>party/%party/party/view</em>. If you add varients you may use selection criteria based on hats or anything else to provide different views of parties. If no variant is selected the default Party view will be used'),
    'admin path' => 'party/%party/party/view',

    // Menu hooks so that we can alrer the entry point to us
    'hook menu' => 'party_view_page_manager_menu',
    'hook menu alter' => 'party_view_page_manager_menu_alter',

    // This task uses context handlers and must implement these to give th handler data it needs
    'handler type' => 'context',
    'get arguments' => 'party_view_page_manager_get_arguments',
    'get context placeholders' => 'party_view_page_manager_get_contexts',

    // Allow this to be enabled or disabled
    'disabled' => variable_get('party_view_page_manager_disabled', TRUE),
    'enable callback' => 'party_view_page_manager_enable',
  );
}

/**
 * Alter the menu to put us in charge of party_view
 */
function party_view_page_manager_menu_alter(&$items, $task) {
  if (variable_get('party_view_page_manager_disabled', TRUE)) {
    return;
  }

  // Override the party view handler for our purpose
  $callback = $items['party/%party/party/view']['page callback'];
  if ($callback == 'party_page_view' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['party/%party/party/view']['page callback'] = 'party_view_page_manager_page';
    $items['party/%party/party/view']['file path'] = $task['path'];
    $items['party/%party/party/view']['file'] = $task['file'];

    $items['party/%party/party']['page callback'] = 'party_view_page_manager_page';
    $items['party/%party/party']['file path'] = $task['path'];
    $items['party/%party/party']['file'] = $task['file'];

    $items['party/%party']['page callback'] = 'party_view_page_manager_page';
    $items['party/%party']['file path'] = $task['path'];
    $items['party/%party']['file'] = $task['file'];
  }
  else {
    // Automatically disable
    variable_set('party_view_page_manager_disabled', TRUE);
    drupal_set_message(t('Page manager module is unable to enable party/%party/party/view because some other module already has overridden with %callback.', array('%callback' => $callback)), 'error');
  }
}

/**
 * Entry point for our overridden Party View
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to Party's
 * party_page_view()
 */
function party_view_page_manager_page($party) {
  // Load task plugin
  $task = page_manager_get_task('party_view');

  // Load the party into a context
  ctools_include('context');
  ctools_include('context-task-handler');
  $contexts = ctools_context_handler_get_task_contexts($task, '', array($party));

  $output = ctools_context_handler_render($task, '', $contexts, array($party->pid));
  if ($output != FALSE) {
    return $output;
  }

  $function = 'party_page_view';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('party_view')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // If we're falling back to the standard, make sure party.pages.inc is
  // included
  if ($function == 'party_page_view') {
    module_load_include('inc', 'party', 'party.pages');
  }

  // Otherwise, fall back.
  return $function($party);
}

/**
 * Callback to get arguments provided by this task handler.
 *
 * Since this is the party view and there is no UI on the arguments, we
 * create dummy arguments that contain the needed data.
 */
function party_view_page_manager_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => 'party',
      'identifier' => t('Party being viewed'),
      'id' => 1,
      'name' => 'entity_id:party',
      'settings' => array(),
    ),
  );
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function party_view_page_manager_get_contexts($task, $subtask_id) {
  $args = party_view_page_manager_get_arguments($task, $subtask_id);
  return ctools_context_get_placeholders_from_argument($args);
}

/**
 * Callback to enable/disable the page from the UI.
 */
function party_view_page_manager_enable($cache, $status) {
  variable_set('party_view_page_manager_disabled', $status);
}