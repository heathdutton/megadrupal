<?php

/**
 * @file
 * Handles the 'eck view' override task.
 */

/**
 * Implements hook_HOOK_page_manager_tasks().
 */
function panels_eck_eck_view_page_manager_tasks() {
  return array(
    'task type' => 'page',
    'title' => t('Entity Construction Kit'),
    'subtasks' => TRUE,
    'subtask callback' => 'panels_eck_eck_view_subtask',
    'subtasks callback' => 'panels_eck_eck_view_subtasks',
    'hook menu alter' => 'panels_eck_eck_view_menu_alter',
    'handler type' => 'context',
    'get arguments' => 'panels_eck_eck_view_get_arguments',
    'get context placeholders' => 'panels_eck_eck_view_get_contexts',
  );
}

/**
 * Callback defined by panels_eck_eck_view_page_manager_tasks().
 */
function panels_eck_eck_view_menu_alter(&$items, $task) {
  module_load_include('inc', 'eck', 'eck.entity');

  foreach (EntityType::loadAll() as $entity_type) {
    $subtask_id = $entity_type->name;
    if (variable_get("panels_eck_eck_view_disabled_$subtask_id", TRUE)) {
      continue;
    }

    foreach (Bundle::loadByEntityType($entity_type) as $bundle) {
      $crud_info = get_bundle_crud_info($entity_type->name, $bundle->name);
      if (empty($crud_info['view'])) {
        continue;
      }

      $path = $crud_info['view']['path'];
      $callback = $items[$path]['page callback'];

      if ($callback == 'eck__entity__view' || variable_get('page_manager_override_anyway', FALSE)) {
        $items[$path]['page callback'] = 'panels_eck_eck_view_page';
        $items[$path]['file path'] = $task['path'];
        $items[$path]['file'] = $task['file'];
      }
    }
  }
}

/**
 * Entry point for our overridden Entity Construction Kit view page.
 */
function panels_eck_eck_view_page($entity_type, $bundle, $entity) {
  ctools_include('context');
  ctools_include('context-task-handler');

  $task = page_manager_get_task('eck_view');
  $subtask = page_manager_get_task_subtask($task, $entity_type);
  $id = entity_id($entity_type, $entity);
  $contexts = ctools_context_handler_get_task_contexts($task, $subtask, array($entity));
  $output = ctools_context_handler_render($task, $subtask, $contexts, array($id));
  if ($output !== FALSE) {
    return $output;
  }

  $function = 'eck__entity__view';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('eck_view')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  return $function($entity_type, $bundle, $id);
}

/**
 * Callback to get arguments provided by this task handler.
 */
function panels_eck_eck_view_get_arguments($task, $subtask) {
  return array(array(
    'keyword' => $subtask['name'],
    'identifier' => t('Entity being viewed'),
    'id' => 3,
    'name' => "entity_id:{$subtask['name']}",
    'settings' => array(),
  ));
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function panels_eck_eck_view_get_contexts($task, $subtask) {
  return ctools_context_get_placeholders_from_argument(panels_eck_eck_view_get_arguments($task, $subtask));
}

/**
 * Callback to enable/disable the page.
 */
function panels_eck_eck_view_enable($cache, $status) {
  variable_set('panels_eck_eck_view_disabled_' . $cache->subtask_id, $status);
}

/**
 * Task callback to get all subtasks.
 */
function panels_eck_eck_view_subtasks($task) {
  $subtasks = array();
  foreach (EntityType::loadAll() as $entity_type) {
    $subtask_id = $entity_type->name;
    $subtasks[$subtask_id] = panels_eck_eck_view_build_subtask($task, $subtask_id);
  }
  return $subtasks;
}

/**
 * Callback to return a single subtask.
 */
function panels_eck_eck_view_subtask($task, $subtask_id) {
  return panels_eck_eck_view_build_subtask($task, $subtask_id);
}

/**
 * Build a subtask array for a given page.
 */
function panels_eck_eck_view_build_subtask($task, $subtask_id) {
  $entity_info = entity_get_info($subtask_id);
  $replacements = array(
    '@entity_type' => $entity_info['label'],
  );

  $subtask = array(
    'name' => $subtask_id,
    'admin title' => t('Entity Construction Kit: @entity_type', $replacements),
    'admin description' => t('Entity Construction Kit: @entity_type', $replacements),
    'admin type' => t('System'),
    'storage' => t('In code'),
    'disabled' => variable_get("panels_eck_eck_view_disabled_$subtask_id", TRUE),
    'enable callback' => 'panels_eck_eck_view_enable',
  );

  return $subtask;
}
