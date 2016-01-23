<?php
/**
 * @file
 * Entity Panels task plugin.
 */

function entity_panels_entity_panels_page_manager_tasks() {
  return array(
    'task type' => 'entity_panels',
    'title' => t('Entity panels'),
    'subtasks' => TRUE,
    'subtask callback' => 'entity_panels_entity_panels_subtask',
    'subtasks callback' => 'entity_panels_entity_panels_subtasks',
    'page type' => 'entity panels',
    'handler type' => 'context',
    'get arguments' => 'entity_panels_entity_panels_get_arguments',
    'get context placeholders' => 'entity_panels_entity_panels_get_contexts',
    'save subtask callback' => 'entity_panels_entity_panels_save_subtask',
  );
}

function entity_panels_entity_panels_enable($cache, $status) {
  list($entity_type, $bundle, $view_mode) = explode('-', $cache->subtask_id, 3);
  $enabled = variable_get('entity_panels_entity_panels_enabled', array());

  if (empty($status)) {
    $enabled[$entity_type][$bundle][$view_mode] = TRUE;
  }
  else {
    unset($enabled[$entity_type][$bundle][$view_mode]);
    if (empty($enabled[$entity_type][$bundle])) {
      unset($enabled[$entity_type][$bundle]);
      if (empty($enabled[$entity_type])) {
        unset($enabled[$entity_type]);
      }
    }
  }

  variable_set('entity_panels_entity_panels_enabled', $enabled);
}

function entity_panels_entity_panels_subtasks($task) {
  $return = array();

  $entity_types = array_intersect_key(entity_get_info(), array_flip(variable_get('entity_panels_entity_types', array('node'))));
  foreach ($entity_types as $entity_type => $entity_info) {
    if (empty($entity_info['bundles']) || empty($entity_info['view modes'])) {
      continue;
    }
    $view_modes = array('default' => array('label' => t('Default'))) + $entity_info['view modes'];
    foreach (array_keys($entity_info['bundles']) as $bundle) {
      foreach ($view_modes as $view_mode => $view_mode_info) {
        $view_mode_settings = field_view_mode_settings($entity_type, $bundle);
        $visibility = ($view_mode == 'default') || !empty($view_mode_settings[$view_mode]['custom_settings']);
        if (!$visibility) {
          continue;
        }
        $task_id = $entity_type . '-' . $bundle . '-' . $view_mode;
        $return[$task_id] = entity_panels_entity_panels_build_subtask($task, $task_id);
      }
    }
  }

  return $return;
}

function entity_panels_entity_panels_subtask($task, $subtask_id) {
  return entity_panels_entity_panels_build_subtask($task, $subtask_id);
}

function entity_panels_entity_panels_build_subtask($task, $task_id) {
  static $admin_type = NULL;
  static $storage = NULL;

  if (empty($admin_type)) {
    $admin_type = t('Entity panels');
  }
  if (empty($storage)) {
    $storage = t('In code');
  }

  list($entity_type, $bundle, $view_mode) = explode('-', $task_id, 3);

  $info = entity_get_info($entity_type);
  $entity_type_label = isset($info['label']) ? $info['label'] : $entity_type;
  $bundle_label = isset($info['bundles'][$bundle]['label']) ? $info['bundles'][$bundle]['label'] : $bundle;
  $view_mode_label = isset($info['view modes'][$view_mode]['label']) ? $info['view modes'][$view_mode]['label'] : $view_mode;
  $label = $entity_type_label . ' / ' . $bundle_label . ' (' . $view_mode_label . ')';

  $enabled = variable_get('entity_panels_entity_panels_enabled', array());

  $subtask = array(
    'name' => $task_id,
    'admin title' => check_plain($label),
    'admin path' => 'entity_panels/' . check_plain(str_replace('-', '/', $task_id)) . '/%entity',
    'admin description' => check_plain($label),
    'admin type' => $admin_type,
    'storage' => $storage,
    'disabled' => empty($enabled[$entity_type][$bundle][$view_mode]),
    'enable callback' => 'entity_panels_entity_panels_enable',
  );

  return $subtask;
}

function entity_panels_entity_panels_get_arguments($task, $subtask) {
  list($entity_type, $bundle, $view_mode) = explode('-', $subtask['name'], 3);

  return array(
    array(
      'keyword' => $entity_type,
      'identifier' => t('@type being viewed', array('@type' => ucfirst(check_plain($entity_type)))),
      'id' => 1,
      'name' => 'entity_panels_entity_id:' . $entity_type,
      'settings' => array(),
      'bundle' => $bundle,
    ),
  );
}

function entity_panels_entity_panels_get_contexts($task, $subtask) {
  return ctools_context_get_placeholders_from_argument(entity_panels_entity_panels_get_arguments($task, $subtask));
}

function entity_panels_entity_panels_save_subtask($subtask, $cache) {
  if (!empty($cache->handler_info)) {
    foreach ($cache->handler_info as $id => &$info) {
      $handler = $cache->handlers[$id];
      if (isset($handler->conf['name'])) {
        // Existence of conf['name'] makes it impossible to clone variants.
        unset($handler->conf['name']);
        if (!($info['changed'] & PAGE_MANAGER_CHANGED_DELETED)) {
          $info['changed'] |= PAGE_MANAGER_CHANGED_CACHED;
        }
      }
    }
  }
}
