<?php

/**
 * @file
 * Handle the 'while view' override task.
 *
 * This plugin overrides while/%entity_object and reroutes it to the page manager, where
 * a list of tasks can be used to service this request based upon criteria
 * supplied by access plugins.
 */

/**
 * Specialized implementation of hook_page_manager_task_tasks(). See api-task.html for
 * more information.
 */
function while_while_view_page_manager_tasks() {
  $path = while_while_view_path();

  return array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',

    'title' => t('@while template', array('@while' => while_get_string('entity label'))),

    'admin title' => t('@while template', array('@while' => while_get_string('entity label'))),
    'admin description' => t('When enabled, this overrides the default Drupal behavior for displaying @whiles at <em>@path</em>. If you add variants, you may use selection criteria such as @while type or language or user access to provide different views of @whiles. If no variant is selected, the default Drupal @while view will be used. This page only affects @whiles viewed as pages, it will not affect @whiles viewed in lists or at other locations. Also please note that if you are using pathauto, aliases may make a while to be somewhere else, but as far as Drupal is concerned, they are still at @path.', array('@path' => $path, '@while' => while_get_string('entity label'), '@whiles' => while_get_string('entity plural label'))),
    'admin path' => $path,

    // Menu hooks so that we can alter the while/%entity_object menu entry to point to us.
    'hook menu alter' => 'while_while_view_menu_alter',

    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',
    'get arguments' => 'while_while_view_get_arguments',
    'get context placeholders' => 'while_while_view_get_contexts',

    // Allow this to be enabled or disabled:
    'disabled' => variable_get('while_while_view_disabled', TRUE),
    'enable callback' => 'while_while_view_enable',
    'access callback' => 'while_while_view_access_check',
  );
}

/**
 * Callback defined by while_while_view_page_manager_tasks().
 *
 * Alter the while view input so that while view comes to us rather than the
 * normal while view process.
 */
function while_while_view_menu_alter(&$items, $task) {
  if (variable_get('while_while_view_disabled', TRUE)) {
    return;
  }

  $path = while_while_view_path();

  // Override the while view handler for our purpose.
  $callback = $items[$path]['page callback'];
  if ($callback == 'entity_ui_entity_page_view' || variable_get('while_override_anyway', FALSE)) {
    $items[$path]['page callback'] = 'while_while_view_page';
    $items[$path]['file path'] = $task['path'];
    $items[$path]['file'] = $task['file'];
  }
  else {
    // automatically disable this task if it cannot be enabled.
    variable_set('while_while_view_disabled', TRUE);
    if (!empty($GLOBALS['while_enabling_while_view'])) {
      drupal_set_message(t('Page manager module is unable to enable %path because some other module already has overridden with %callback.', array('%path' => $path, '%callback' => $callback)), 'error');
    }
  }

  // @todo override while revision handler as well?
}

/**
 * Entry point for our overridden while view.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to Drupal core's
 * while view, which is while_page_view().
 */
function while_while_view_page($while) {
  // Load my task plugin
  $task = page_manager_get_task('while_view');

  // Load the while into a context.
  ctools_include('context');
  ctools_include('context-task-handler');

  // We need to mimic Drupal's behavior of setting the while title here.
  drupal_set_title($while->label());
  $uri = entity_uri('while', $while);
  // Set the while path as the canonical URL to prevent duplicate content.
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);
  $contexts = ctools_context_handler_get_task_contexts($task, '', array($while));

  $output = ctools_context_handler_render($task, '', $contexts, array($while->id));
  if ($output != FALSE) {
    // todo
    //    node_tag_new($node);
    return $output;
  }

  $function = 'entity_ui_entity_page_view';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('entity_view')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // Otherwise, fall back.
  return $function($while);
}

/**
 * Callback to get arguments provided by this task handler.
 *
 * Since this is the while view and there is no UI on the arguments, we
 * create dummy arguments that contain the needed data.
 */
function while_while_view_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => 'while',
      'identifier' => t('@while being viewed', array('@while' => while_get_string('entity label'))),
      'id' => 1,
      'name' => 'entity_id:while',
      'settings' => array(),
    ),
  );
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function while_while_view_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(while_while_view_get_arguments($task, $subtask_id));
}

/**
 * Callback to enable/disable the page from the UI.
 */
function while_while_view_enable($cache, $status) {
  variable_set('while_while_view_disabled', $status);

  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['while_enabling_while_view'] = TRUE;
  }
}

/**
 * Callback to determine if a page is accessible.
 *
 * @param $task
 *   The task plugin.
 * @param $subtask_id
 *   The subtask id
 * @param $contexts
 *   The contexts loaded for the task.
 * @return
 *   TRUE if the current user can access the page.
 */
function while_while_view_access_check($task, $subtask_id, $contexts) {
  $context = reset($contexts);
  return while_access('view', $context->data);
}

/**
 * Helper function that provides the override path.
 */
function while_while_view_path() {
  $info = entity_get_info('while');
  $wildcard = isset($info['admin ui']['menu wildcard']) ? $info['admin ui']['menu wildcard'] : '%entity_object';
  return while_get_string('base path') . '/' . $wildcard;
}