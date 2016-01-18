<?php

/**
 * Specialized implementation of hook_page_manager_task_tasks(). See api-task.html for
 * more information.
 */
function openid_provider_page_page_manager_tasks() {
dsm('openid_provider_page_page_manager_tasks');
  return array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',

    'title' => t('OpenID provider page'),
    'admin title' => t('OpenID provider page'),
    'admin description' => t('When enabled, this overrides the default Drupal behavior for the site OpenID provider page at <em>/openid/provider</em>.'),
    'admin path' => 'openid/provider',

    // Menu hooks so that we can alter the node/%node menu entry to point to us.
    'hook menu alter' => 'openid_provider_page_menu_alter',

    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',

    // Allow this to be enabled or disabled:
    'disabled' => variable_get('openid_provider_page_disabled', TRUE),
    'enable callback' => 'openid_provider_page_enable',
    'access callback' => TRUE,
  );
}

/**
 * Callback defined by openid_provider_page_page_manager_tasks().
 *
 * Alter the node edit input so that node edit comes to us rather than the
 * normal node edit process.
 */
function openid_provider_page_menu_alter(&$items, $task) {
  if (variable_get('openid_provider_page_disabled', TRUE)) {
    return;
  }

  $callback = $items['openid/provider']['page callback'];
  if ($callback == 'drupal_get_form') {
    $callback = $items['contact']['page arguments'][0];
  }

  // Override the node edit handler for our purpose.
  if ($callback == 'openid_provider_endpoint' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['openid/provider']['page callback'] = 'page_manager_openid_provider_page';
    $items['openid/provider']['file path'] = $task['path'];
    $items['openid/provider']['file'] = $task['file'];
  }
  else {
    variable_set('openid_provider_page_disabled', TRUE);
    if (!empty($GLOBALS['page_manager_enabling_openid_provider_page'])) {
      drupal_set_message(t('Page manager module is unable to enable contact because some other module already has overridden with %callback.', array('%callback' => $callback)), 'warning');
    }
    return;
  }

}

/**
 * Entry point for our overridden site contact.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it.
 */
function page_manager_openid_provider_page() {
  // Load my task plugin
  $task = page_manager_get_task('openid_provider_page');

  ctools_include('context');
  ctools_include('context-task-handler');
  $output = ctools_context_handler_render($task, '', array(), array());
  if ($output !== FALSE) {
    return $output;
  }

  module_load_include('inc', 'openid_provider', 'openid_provider.pages');
  $function = 'openid_provider_endpoint';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('openid_provider_page')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // Otherwise, fall back.
  $function == 'openid_provider_endpoint';
  return $function();
}

/**
 * Callback to enable/disable the page from the UI.
 */
function openid_provider_page_enable($cache, $status) {
  variable_set('openid_provider_page_disabled', $status);
  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['page_manager_enabling_openid_provider_page'] = TRUE;
  }
}
