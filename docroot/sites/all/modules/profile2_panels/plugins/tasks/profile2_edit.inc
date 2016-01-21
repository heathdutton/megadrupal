<?php

/**
 * Specialized implementation of hook_page_manager_task_tasks(). See api-task.html for
 * more information.
 */
function profile2_panels_profile2_edit_page_manager_tasks() {
  return array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',
    'title' => t('Profile2 add/edit template'),
    'admin title' => t('Profile2 add/edit template'),
    'admin description' => t('When enabled, this overrides the default Drupal behavior for adding or edit profile2s at <em>%profile-type/%profile2_by_uid/edit</em> and <em>profile2/add/%profile2_type</em>. If you add variants, you may use selection criteria such as profile2 type or language or user access to provide different edit forms for profile2s. If no variant is selected, the default Drupal profile2 edit will be used.'),
    'admin path' => '%profile-type/%profile2_by_uid/edit',
    // Menu hooks so that we can alter the %profile-type/%profile2_by_uid menu entry to point to us.
    'hook menu alter' => 'profile2_panels_profile2_edit_menu_alter',
    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',
    'get arguments' => 'profile2_panels_profile2_edit_get_arguments',
    'get context placeholders' => 'profile2_panels_profile2_edit_get_contexts',
    // Allow this to be enabled or disabled:
    'disabled' => variable_get('profile2_panels_profile2_edit_disabled', TRUE),
    'enable callback' => 'profile2_panels_profile2_edit_enable',
    'access callback' => 'profile2_panels_profile2_edit_access_check',
  );
}

/**
 * Callback defined by profile2_panels_edit_page_manager_tasks().
 *
 * Alter the profile2 edit input so that profile2 edit comes to us rather than the
 * normal profile2 edit process.
 */
function profile2_panels_profile2_edit_menu_alter(&$items, $task) {
  if (variable_get('profile2_panels_profile2_edit_disabled', TRUE)) {
    return;
  }

  // Override the profile2 edit handler for our purpose.
  foreach (profile2_get_types() as $type_name => $type) {
    if (!empty($type->data['use_page'])) {
      $path = profile2_page_get_base_path($type);

      $callback = $items[$path . '/%profile2_by_uid/edit']['page callback'];
      if ($callback == 'entity_ui_get_form' || variable_get('page_manager_override_anyway', FALSE)) {
        $items[$path . '/%profile2_by_uid/edit']['page callback'] = 'profile2_panels_profile2_edit';
        $items[$path . '/%profile2_by_uid/edit']['file path'] = $task['path'];
        $items[$path . '/%profile2_by_uid/edit']['file'] = $task['file'];
      }
    }
  }
}

/**
 * Entry point for our overridden profile2 edit.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to Drupal core's
 * profile2 edit, which is entity_ui_get_form().
 */
function profile2_panels_profile2_edit($entity_type, $profile2) {

  // Load my task plugin
  $task = page_manager_get_task('profile2_edit');

  // Load the profile2 into a context.
  ctools_include('context');
  ctools_include('context-task-handler');
  $contexts = ctools_context_handler_get_task_contexts($task, '', array($profile2));

  $arg = array(isset($profile2->pid) ? $profile2->pid : $profile2->type);
  $output = ctools_context_handler_render($task, '', $contexts, $arg);
  if ($output === FALSE) {
    // Fall back!
    // We've already built the form with the context, so we can't build it again, or
    // form_clean_id will mess up our ids. But we don't really need to, either:
    $context = reset($contexts);
    $output = $context->form;
  }

  return $output;
}

/**
 * Callback to get arguments provided by this task handler.
 *
 * Since this is the profile2 edit and there is no UI on the arguments, we
 * create dummy arguments that contain the needed data.
 */
function profile2_panels_profile2_edit_get_arguments($task, $subtask_id) {

  return array(
    array(
      'keyword' => 'profile2',
      'identifier' => t('Profile2 being edited'),
      'id' => 1,
      'name' => 'profile2_edit',
      'settings' => array(),
    ),
  );
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function profile2_panels_profile2_edit_get_contexts($task, $subtask_id) {

  return ctools_context_get_placeholders_from_argument(profile2_panels_profile2_edit_get_arguments($task, $subtask_id));
}

/**
 * Callback to enable/disable the page from the UI.
 */
function profile2_panels_profile2_edit_enable($cache, $status) {
  variable_set('profile2_panels_profile2_edit_disabled', $status);
  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['page_manager_enabling_profile2_edit'] = TRUE;
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
function profile2_panels_profile2_edit_access_check($task, $subtask_id, $contexts) {
  $context = reset($contexts);
  return profile2_access('update', $context->data);
}
