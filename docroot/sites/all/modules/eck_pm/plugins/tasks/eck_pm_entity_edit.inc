<?php

/**
 * @file
 * Handle the entity edit/add.
 */

/**
 * Specialized implementation of hook_page_manager_task_tasks().
 *
 * @see api-task.html for more information.
 */
function eck_pm_eck_pm_entity_edit_page_manager_tasks() {

  return array(
    // This is a 'page' task and will fall under the page admin UI.
    'task type' => 'page',
    'title' => t('Entity add/edit form'),

    // There are multiple pages.
    'subtasks' => TRUE,
    'subtask callback' => 'eck_pm_eck_pm_entity_edit_subtask',
    'subtasks callback' => 'eck_pm_eck_pm_entity_edit_subtasks',
    'hook menu alter' => 'eck_pm_eck_pm_entity_edit_menu_alter',
    'handler type' => 'context',
    'get arguments' => 'eck_pm_eck_pm_entity_edit_get_arguments',
    'get context placeholders' => 'eck_pm_eck_pm_entity_edit_get_contexts',
  );
}



/**
 * Callback defined by eck_pm_eck_pm_entity_edit_page_manager_tasks().
 *
 * Overrides the page callbacks for custom tasks.
 */
function eck_pm_eck_pm_entity_edit_menu_alter(&$items, $task) {
  $eck_tasks = eck_pm_get_eck_form();
  foreach ($eck_tasks as $task_id => $task_info) {
    if (variable_get('eck_pm_pages_disabled_' . $task_id, TRUE)) {
      continue;
    }

    $paths = $task_info->paths_type;

    eck_pm_eck_pm_entity_edit_menu_alter_path_type($items, $task, $task_id, $paths['add'], 'eck__entity__add', 'eck_pm_eck_pm_entity_add_form');

    eck_pm_eck_pm_entity_edit_menu_alter_path_type($items, $task, $task_id, $paths['edit'], 'eck__entity__edit', 'eck_pm_eck_pm_entity_edit_form');
  }
}


function eck_pm_eck_pm_entity_edit_menu_alter_path_type(&$items, $task, $task_id, $paths, $old_callback, $new_callback) {

  foreach ($paths as $path) {
    if (!isset($items[$path])) {
      drupal_set_message(t('%path has not been overridden because the menu item does not exist.', array('%path' => $path)));
      continue;
    }
    if (empty($items[$path]['page callback'])) {
      continue;
    }

    $callback = $items[$path]['page callback'];
    if ($callback == $old_callback) {
      $items[$path]['page callback'] = $new_callback;
      $items[$path]['page arguments'][] = $task_id;
      $items[$path]['file path'] = $task['path'];
      $items[$path]['file'] = $task['file'];
    }
    else {
      variable_set('eck_pm_pages_disabled_' . $task_id, TRUE);
      if (!empty($GLOBALS['eck_pm_pages_disabled_' . $task_id])) {
        drupal_set_message(t('Page manager module is unable to enable %path because some other module already has overridden with %callback.', array('%path' => $path,'%callback' => $callback)), 'error');
      }
    }
  }
}


/**
 * Entry point for our overridden task add form.
 */
function eck_pm_eck_pm_entity_add_form($entity_type_name, $bundle_name, $task_id) {
  $entity_info = entity_get_info($entity_type_name);

  $options = array('type' => $bundle_name);

  if (!empty($entity_info['entity keys']['label'])) {
    $options[$entity_info['entity keys']['label']] = '';
  }

  $entity = entity_create($entity_type_name, $options);

  return eck_pm_eck_pm_entity_edit_form($entity_type_name, $bundle_name, $entity, $task_id);
}


/**
 * Entry point for our overridden task page.
 */
function eck_pm_eck_pm_entity_edit_form($entity_type_name, $bundle_name, $entity, $task_id) {
  if (!is_object($entity) && is_numeric($entity)) {
    $entity = entity_load_single($entity_type_name, $entity);
  }

  if (is_object($entity) && $entity->bundle() == $bundle_name) {
    $task = page_manager_get_task('eck_pm_entity_edit');
    $subtask = page_manager_get_task_subtask($task, $task_id);

    ctools_include('context');
    ctools_include('context-task-handler');

    $file = drupal_get_path('module', 'eck') . '/eck.entity.inc';
    require_once DRUPAL_ROOT . '/' . $file;
    $contexts = ctools_context_handler_get_task_contexts($task, $subtask, array($entity));
    // Attach the form to the context.
    if (!isset($entity->id)) {
      $form_id = "eck__entity__form_add_{$entity_type_name}_{$bundle_name}";
    }
    else {
      $form_id = "eck__entity__form_edit_{$entity_type_name}_{$bundle_name}";
    }
    $form_state = array('want form' => TRUE, 'build_info' => array('args' => array($entity)));
    $form = drupal_build_form($form_id, $form_state);

    $context = reset($contexts);
    $context->form       = $form;
    $context->form_state = &$form_state;
    $context->form_id    = $form_id;
    $context->restrictions['form'] = array('form');
    $context->restrictions['type'] = array($bundle_name);

    $args = array($entity_type_name, $bundle_name);
    if (isset($entity->id)) {
      $args[] = $entity->id;
    }
    $output = ctools_context_handler_render($task, $subtask, $contexts, $args);

    if ($output === FALSE) {
      $context = reset($contexts);
      $output = $context->form;
    }

    return $output;
  }

  return '';
}


/**
 * Callback to get arguments provided by this task handler.
 */
function eck_pm_eck_pm_entity_edit_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => $subtask_id['entity_type'],
      'identifier' => t('Entity being edited'),
      'name' => 'entity_id:' . $subtask_id['entity_type'],
      'id' => 1,
      'settings' => array(),
    ),
  );
}


/**
 * Callback to get context placeholders provided by this handler.
 */
function eck_pm_eck_pm_entity_edit_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(eck_pm_eck_pm_entity_edit_get_arguments($task, $subtask_id));
}


/**
 * Callback to enable/disable the page from the UI.
 */
function eck_pm_eck_pm_entity_edit_enable($cache, $status) {
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
function eck_pm_eck_pm_entity_edit_subtasks($task) {
  $return = array();

  $eck_tasks = eck_pm_get_eck_form();
  foreach ($eck_tasks as $task_id => $info) {
    $return[$task_id] = eck_pm_eck_pm_entity_edit_build_subtask($task, $task_id);
  }

  return $return;
}


/**
 * Callback to return a single subtask.
 */
function eck_pm_eck_pm_entity_edit_subtask($task, $subtask_id) {
  return eck_pm_eck_pm_entity_edit_build_subtask($task, $subtask_id);
}


/**
 * Builds a subtask array for a given page.
 */
function eck_pm_eck_pm_entity_edit_build_subtask($task, $task_id) {
  $info = eck_pm_get_eck_form($task_id);

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
    'enable callback' => 'eck_pm_eck_pm_entity_edit_enable',
    'entity_type' => $info->entity_type,
    'paths' => $info->paths,
  );

  return $subtask;
}

