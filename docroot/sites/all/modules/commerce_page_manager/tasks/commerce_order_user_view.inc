<?php

/**
 * @file
 * Handles the Commerce Order view task.
 *
 * This plugin overrides Commerce Order UI view path and reroutes
 * it to the page manager, where a list of tasks can be used to service this
 * request based upon criteria supplied by access plugins.
 */

/**
 * Specialized implementation of hook_page_manager_task_tasks().
 */
function commerce_page_manager_commerce_order_user_view_page_manager_tasks() {
  $stop = null;
  return array(
    // This is a 'page' task and will fall under the page user UI
    'task type' => 'page',

    'title' => t('Commerce Order (User) template'),

    'admin title' => t('Commerce Order (User) template'),
    'admin description' => t('When enabled, this overrides the default Drupal behavior for displaying Commerce Orders to administrators. If you add variants, you may use selection criteria such as order type or language or user access to provide different views of commerce_orders. If no variant is selected, the default Drupal commerce order view will be used. This page only affects commerce_orders viewed as pages, it will not affect commerce order\'s viewed in lists or at other locations.'),
    'admin path' => 'user/%user/orders/%commerce_order',

    // Menu hooks so that we can alter the commerce_order/%commerce_order menu entry to point to us.
    'hook menu' => 'commerce_page_manager_commerce_order_user_view_menu',
    'hook menu alter' => 'commerce_page_manager_commerce_order_user_view_menu_alter',

    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',
    'get arguments' => 'commerce_page_manager_commerce_order_user_view_get_arguments',
    'get context placeholders' => 'commerce_page_manager_commerce_order_user_view_get_contexts',

    // Allow this to be enabled or disabled:
    'disabled' => variable_get('commerce_page_manager_commerce_order_user_view_disabled', TRUE),
    'enable callback' => 'commerce_page_manager_commerce_order_user_view_enable',
    'access callback' => 'commerce_page_manager_commerce_order_user_view_access_check',
  );
}

/**
 * Callback defined by commerce_page_manager_commerce_order_user_view_page_manager_tasks().
 *
 * Alter the commerce order view input so that commerce_order view comes to us
 * rather than the normal commerce order view process.
 */
function commerce_page_manager_commerce_order_user_view_menu_alter(&$items, $task) {
  if (variable_get('commerce_page_manager_commerce_order_user_view_disabled', TRUE)) {
    return;
  }

  // Override the commerce_order view handler for our purpose.
  $callback = $items['user/%user/orders/%commerce_order']['page callback'];
  if ($callback == 'commerce_order_ui_order_view' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['user/%user/orders/%commerce_order']['page callback'] = 'commerce_page_manager_commerce_order_user_view_page';
    $items['user/%user/orders/%commerce_order']['file path'] = $task['path'];
    $items['user/%user/orders/%commerce_order']['file'] = $task['file'];
  }
  else {
    // automatically disable this task if it cannot be enabled.
    variable_set('commerce_page_manager_commerce_order_user_view_disabled', TRUE);
    if (!empty($GLOBALS['commerce_page_manager_enabling_commerce_order_user_view'])) {
      drupal_set_message(t('Page manager module is unable to enable user/%user/orders/%commerce_order because some other module already has overridden with %callback.', array('%callback' => $callback)), 'error');
    }
  }
}

/**
 * Entry point for our overridden commerce order view.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to Drupal core's
 * commerce order view, which is commerce_order_ui_order_view().
 */
function commerce_page_manager_commerce_order_user_view_page($commerce_order) {
  // Load my task plugin
  $task = page_manager_get_task('commerce_order_user_view');

  // Load the commerce_order into a context.
  ctools_include('context');
  ctools_include('context-task-handler');

  // We need to mimic Drupal's behavior of setting the commerce_order title here.
  drupal_set_title('Order ' . $commerce_order->order_id);
  $contexts = ctools_context_handler_get_task_contexts($task, '', array($commerce_order));

  $output = ctools_context_handler_render($task, '', $contexts, array($commerce_order->order_id));
  if ($output != FALSE) {
    return $output;
  }

  $function = 'commerce_order_ui_order_view';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . 'page_manager_override';
    if (($rc = $call('commerce_order_view')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // Otherwise, fall back.
  return $function($commerce_order);
}

/**
 * Callback to get arguments provided by this task handler.
 *
 * Since this is the commerce_order view and there is no UI on the arguments, we
 * create dummy arguments that contain the needed data.
 */
function commerce_page_manager_commerce_order_user_view_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => 'commerce_order',
      'identifier' => t('Commerce Order being viewed'),
      'id' => 1,
      'name' => 'entity_id:commerce_order',
      'settings' => array(),
    ),
  );
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function commerce_page_manager_commerce_order_user_view_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(commerce_page_manager_commerce_order_user_view_get_arguments($task, $subtask_id));
}

/**
 * Callback to enable/disable the page from the UI.
 */
function commerce_page_manager_commerce_order_user_view_enable($cache, $status) {
  variable_set('commerce_page_manager_commerce_order_user_view_disabled', $status);

  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['commerce_page_manager_enabling_commerce_order_user_view'] = TRUE;
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
 * @return bool
 *   TRUE if the current user can access the page.
 */
function commerce_page_manager_commerce_order_user_view_access_check($task, $subtask_id, $contexts) {
  $context = reset($contexts);
  return commerce_order_access('view', $context->data);
}
