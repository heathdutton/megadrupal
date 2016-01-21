<?php

/**
 * @file
 * Handle the entity view.
 */

/**
 * Specialized implementation of hook_page_manager_task_tasks().
 *
 * @see api-task.html for more information.
 */
function eck_pm_eck_pm_entity_view_page_manager_tasks() {

  return array(
    // This is a 'page' task and will fall under the page admin UI.
    'task type' => 'page',
    'title' => t('Entity Template'),

    // There are multiple pages.
    'subtasks' => TRUE,
    'subtask callback' => 'eck_pm_eck_pm_entity_view_subtask',
    'subtasks callback' => 'eck_pm_eck_pm_entity_view_subtasks',
    'hook menu alter' => 'eck_pm_eck_pm_entity_view_menu_alter',
    'handler type' => 'context',
    'get arguments' => 'eck_pm_eck_pm_entity_view_get_arguments',
    'get context placeholders' => 'eck_pm_eck_pm_entity_view_get_contexts',
  );
}


/**
 * Callback defined by eck_pm_eck_pm_entity_view_page_manager_tasks().
 *
 * Overrides the page callbacks for custom tasks.
 */
function eck_pm_eck_pm_entity_view_menu_alter(&$items, $task) {
  $eck_tasks = eck_pm_get_eck_pages();
  foreach ($eck_tasks as $task_id => $task_info) {
    if (variable_get('eck_pm_pages_disabled_' . $task_id, TRUE)) {
      continue;
    }

    $paths = $task_info->paths;

    foreach ($paths as $path_view) {
      if (!isset($items[$path_view])) {
        drupal_set_message(t('%path has not been overridden because the menu item does not exist.', array('%path' => $path_view)));
        continue;
      }
      if (empty($items[$path_view]['page callback'])) {
        continue;
      }

      $callback = $items[$path_view]['page callback'];
      if ($callback == 'eck__entity__view') {
        $items[$path_view]['page callback'] = 'eck_pm_eck_pm_entity_view_page';
        $items[$path_view]['page arguments'][] = $task_id;
        $items[$path_view]['file path'] = $task['path'];
        $items[$path_view]['file'] = $task['file'];
      }
      else {
        variable_set('eck_pm_pages_disabled_' . $task_id, TRUE);
        if (!empty($GLOBALS['eck_pm_pages_disabled_' . $task_id])) {
          drupal_set_message(t('Page manager module is unable to enable %path because some other module already has overridden with %callback.', array('%path' => $path_view,'%callback' => $callback)), 'error');
        }
      }
    }
  }
}


/**
 * Entry point for our overridden task page.
 */
function eck_pm_eck_pm_entity_view_page($entity_type_name, $bundle_name, $entity, $task_id) {
  if (!is_object($entity) && is_numeric($entity)) {
    $entity = entity_load_single($entity_type_name, $entity);
  }

  if (is_object($entity) && $entity->bundle() == $bundle_name) {
    $task = page_manager_get_task('eck_pm_entity_view');
    $subtask = page_manager_get_task_subtask($task, $task_id);

    ctools_include('context');
    ctools_include('context-task-handler');

    $contexts = ctools_context_handler_get_task_contexts($task, $subtask, array($entity));

    $output = ctools_context_handler_render($task, $subtask, $contexts, array($entity_type_name, $bundle_name, $entity->id));

    if ($output !== FALSE) {
      $view_mode = 'full'; // @TODO: full?
      $langcode = NULL; // @TODO: review this.
      // Copy from buildContent method from EntityAPIController
      if (module_exists('rules')) {
        rules_invoke_all($entity_type_name . '_view', $entity, $view_mode, $langcode);
      }else {
        module_invoke_all($entity_type_name . '_view', $entity, $view_mode, $langcode);
      }
      module_invoke_all('entity_view', $entity, $entity_type_name, $view_mode, $langcode);

      return $output;
    }
  }


  $function = 'eck__entity__view';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('eck_pm_entity_view')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  return $function($entity_type_name, $bundle_name, $entity);
}


/**
 * Callback to get arguments provided by this task handler.
 */
function eck_pm_eck_pm_entity_view_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => $subtask_id['entity_type'],
      'identifier' => t('Entity being viewed'),
      'id' => 1,
      'name' => 'entity_id:' . $subtask_id['entity_type'],
      'settings' => array(),
    ),
  );
}


/**
 * Callback to get context placeholders provided by this handler.
 */
function eck_pm_eck_pm_entity_view_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(eck_pm_eck_pm_entity_view_get_arguments($task, $subtask_id));
}


/**
 * Callback to enable/disable the page from the UI.
 */
function eck_pm_eck_pm_entity_view_enable($cache, $status) {
  variable_set('eck_pm_pages_disabled_' . $cache->subtask_id, $status);

  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['eck_pm_pages_disabled_' . $cache->subtask_id] = TRUE;
  }
}


/**
 * Task callback to get all subtasks.
 */
function eck_pm_eck_pm_entity_view_subtasks($task) {
  $return = array();

  $eck_tasks = eck_pm_get_eck_pages();
  foreach ($eck_tasks as $task_id => $info) {
    $return[$task_id] = eck_pm_eck_pm_entity_view_build_subtask($task, $task_id);
  }

  return $return;
}


/**
 * Callback to return a single subtask.
 */
function eck_pm_eck_pm_entity_view_subtask($task, $subtask_id) {
  return eck_pm_eck_pm_entity_view_build_subtask($task, $subtask_id);
}


/**
 * Builds a subtask array for a given page.
 */
function eck_pm_eck_pm_entity_view_build_subtask($task, $task_id) {
  $info = eck_pm_get_eck_pages($task_id);

  // No bundles created.
  if (empty($info->paths)) {
    $admin_path = 'No bundles created.';
  }
  elseif (count($info->paths) == 1) {
    $admin_path = array_shift($info->paths);
  }
  else {
    // Multi paths.
    $admin_path = $info->paths[0] . "<br>/";
    unset($info->paths[0]);
    $admin_path .= implode("<br>/", $info->paths);
  }

  $subtask = array(
    'name' => $task_id,
    'admin title' => check_plain($info->label),
    'admin description' => check_plain($info->label),
    'admin type' => t('ECK'),
    'admin path' => $admin_path,
    'storage' => t('In code'),
    'disabled' => variable_get('eck_pm_pages_disabled_' . $task_id, TRUE),
    'enable callback' => 'eck_pm_eck_pm_entity_view_enable',
    'entity_type' => $info->entity_type,
    'paths' => $info->paths,
  );

  return $subtask;
}

