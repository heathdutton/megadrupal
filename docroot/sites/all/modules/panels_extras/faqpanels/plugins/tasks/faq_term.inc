<?php
// $Id$ faq_term.inc,v 1.0 2011/07/25 16:34:07 chriscalip Exp $

/**
 * @file
 * Provides additional page manager tasks FAQ page for panels use
 */

/**
 * Specialized implementation of hook_page_manager_task_tasks(). See api-task.html for
 * more information.
 */
function faqpanels_faq_term_page_manager_tasks() {
  if (!module_exists('faq')) {
    return;
  }
  return array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',
    'title' => t('FAQ Page'),
    'admin title' => t('FAQ Page with categories'),
    'admin description' => t('when enabled, this overrides the default Drupal behavior of the FAQ page at <em>faq/%tid/%faq_display</em>'),
    'admin path' => 'faq/%tid/%faq_display',
    // Menu hooks so that we can alter the faq/%tid/%faq_display menu entry to point to us.
    'hook menu' => 'faqpanels_faq_term_menu',
    'hook menu alter' => 'faqpanels_faq_term_menu_alter',
    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',
    'get arguments' => 'faqpanels_faq_term_get_arguments',
    'get context placeholders' => 'faqpanels_faq_term_get_contexts',
    // Allow this to be enabled or disabled:
    'disabled' => variable_get('faqpanels_faq_term_disabled', TRUE),
    'enable callback' => 'faqpanels_faq_term_enable',
  );
}

/**
 * Callback defined by faqpanels_faq_term_page_manager_tasks().
 *
 * Alter the FAQ page with terms so that the FAQ page comes to us rather than the
 * normal FAQ process.
 * Menus to be altered are only the list faq display (faq,faq/%,faq/%/list,faq/list).
 */
function faqpanels_faq_term_menu_alter(&$items, $task) {
  if (variable_get('faqpanels_faq_term_disabled', TRUE)) {
    return;
  }

  $callback = $items['faq']['page callback'];
  // Override the faq term handler for our purpose.
  if ($callback == 'faq_page' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['faq']['page callback'] = 'faqpanels_faq_term';
    $items['faq']['file path'] = $task['path'];
    $items['faq']['file'] = $task['file'];
  }
  else {
    variable_set('faqpanels_faq_term_disabled', TRUE);
    if (!empty($GLOBALS['faqpanels_enabling_faq_term'])) {
      drupal_set_message(t('Page manager module is unable to enable faq because some other module already has overridden with %callback.', array('%callback' => $callback)), 'warning');
    }

  }

  $callback = $items['faq/%']['page callback'];
  // Override the faq term handler for our purpose.
  if ($callback == 'faq_page' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['faq/%']['page callback'] = 'faqpanels_faq_term';
    $items['faq/%']['file path'] = $task['path'];
    $items['faq/%']['file'] = $task['file'];
  }
  else {
    variable_set('faqpanels_faq_term_disabled', TRUE);
    if (!empty($GLOBALS['faqpanels_enabling_faq_term'])) {
      drupal_set_message(t('Page manager module is unable to enable faq/%tid/%faq_display because some other module already has overridden with %callback.', array('%callback' => $callback)), 'warning');
    }

  }

  $callback = $items['faq/%/list']['page callback'];
  // Override the faq term handler for our purpose.
  if ($callback == 'faq_page' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['faq/%/list']['page callback'] = 'faqpanels_faq_term';
    $items['faq/%/list']['file path'] = $task['path'];
    $items['faq/%/list']['file'] = $task['file'];
  }
  else {
    variable_set('faqpanels_faq_term_disabled', TRUE);
    if (!empty($GLOBALS['faqpanels_enabling_faq_term'])) {
      drupal_set_message(t('Page manager module is unable to enable faq/%tid/%faq_display because some other module already has overridden with %callback.', array('%callback' => $callback)), 'warning');
    }

  }

  $callback = $items['faq/list']['page callback'];
  // Override the faq term handler for our purpose.
  if ($callback == 'faq_page' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['faq/list']['page callback'] = 'faqpanels_faq_term';
    $items['faq/list']['file path'] = $task['path'];
    $items['faq/list']['file'] = $task['file'];
  }
  else {
    variable_set('faqpanels_faq_term_disabled', TRUE);
    if (!empty($GLOBALS['faqpanels_enabling_faq_term'])) {
      drupal_set_message(t('Page manager module is unable to enable faq/%tid/%faq_display because some other module already has overridden with %callback.', array('%callback' => $callback)), 'warning');
    }

  }

  return;
}

/**
 * Entry point for our overridden cck email generated contact form page.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to CCK email field, which is email_mail_page().
 */
function faqpanels_faq_term($tid = 0, $faq_display = '') {
  // Load my task plugin
  $task = page_manager_get_task('faq_term');

  ctools_include('context');
  ctools_include('context-task-handler');

  // @todo add ability to accept the node and field name context; well maybe just the node context

  // is it worth the performance hit to include node as a context? if so switch
  //$contexts = faqpanels_faq_term_get_contexts($task, '', array($nid));
  $contexts = faqpanels_faq_term_get_contexts($task, '', array());

  $output = ctools_context_handler_render($task, '', $contexts, array('tid' => $tid, 'faq_display' => $faq_display));
  if ($output !== FALSE) {
    return $output;
  }

  // fallback to the default email contact form page handler
  // assume the cck email module is on.
  $function = 'faq_page';

  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('faq_term')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }
  // Otherwise, fall back.
  return $function($tid, $faq_display);
}

/**
 * Callback to get arguments provided by this task handler.
 *
 * Since this is the node edit and there is no UI on the arguments, we
 * create dummy arguments that contain the needed data.
 */
function faqpanels_faq_term_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => 'term',
      'identifier' => t('Term'),
      'id' => 1,
      'name' => 'tid',
      'settings' => array(),
    ),
  );
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function faqpanels_faq_term_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(faqpanels_faq_term_get_arguments($task, $subtask_id));
}

/**
 * Callback to enable/disable the page from the UI.
 */
function faqpanels_faq_term_enable($cache, $status) {
  variable_set('faqpanels_faq_term_disabled', $status);
  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['faqpanels_enabling_faq_term'] = TRUE;
  }
}
