<?php

function page_manager_pathauto_drush_command() {
  $items = array();

  $items['subtask-list'] = array(
    'description' => "List all the available tasks.",
    'drupal dependencies' => array('page_manager', 'pathauto'),
    'aliases' => array('stl'),
  );
  $items['subtask-alias-generate'] = array(
    'description' => "Generate url aliases for this subtask.",
    'arguments' => array(
      'subtask_id' => 'The subtask_id for which we are going to generate aliases.',
    ),
    'drupal dependencies' => array('page_manager', 'pathauto'),
    'aliases' => array('stag'),
  );

  return $items;
}

/**
 * Get a list of all subtasks.
 */
function drush_page_manager_pathauto_subtask_list() {
  $rows = array(array(
    t('Subtask'),
    t('Subtask ID'),
    t('Handler Paths'),
  ));
  $tasks = page_manager_get_tasks();
  if ($tasks) {
    foreach ($tasks as $task_id => $task) {
      $subtasks = page_manager_get_task_subtasks($task);
      foreach ($subtasks as $subtask_id => $subtask) {
        $default_handlers = isset($subtask['subtask']->default_handlers) ? $subtask['subtask']->default_handlers : NULL;
        $handlers = page_manager_load_task_handlers($task, $subtask_id, $default_handlers);
        if ($handlers) {
          $row_handlers = '';
          foreach ($handlers as $handler_id => $handler) {
            $conf = $handler->conf;
            $conf['pathalias'] = isset($conf['pathalias']) ? $conf['pathalias'] : 'no path alias';
            $row_handlers .= dt('@handler: @alias', array('@handler' => $conf['title'], '@alias' => $conf['pathalias']));
          }
          $rows[] = array(
            $subtask['admin title'],
            $subtask_id,
            $row_handlers,
          );
        }
      }
    }
  }
  drush_print_table($rows, TRUE);
}

function drush_page_manager_pathauto_subtask_alias_generate($subtask_id) {
  $drush_task = &drupal_static(__FUNCTION__);
  $drush_task = TRUE;
  ctools_include('plugins');
  ctools_get_plugins('page_manager', 'tasks', 'page');
  $page = page_manager_page_load($subtask_id);
  $task = page_manager_get_task($page->task);
  $subtask = page_manager_get_task_subtask($task, $subtask_id);

  $contexts = array();
  if ($function = ctools_plugin_get_function($task, 'get context placeholders')) {
    $contexts = $function($task, $subtask);
  }
  $default_handlers = isset($subtask['subtask']->default_handlers) ? $subtask['subtask']->default_handlers : NULL;
  $handlers = page_manager_load_task_handlers($task, $subtask_id, $default_handlers);
  $context_indexes = array();
  ctools_include('plugins');
  $num = 0;
  foreach ($contexts as $context_id => $context) {
    if (empty($context->data)) {
      $plugin = ctools_get_plugins('page_manager_pathauto', 'context_indexes', $context->plugin);
      if ($function = ctools_plugin_get_function($plugin, 'callback')) {
        $context_indexes[$context->plugin . '--' . $context_id] = $function($plugin['context']);
      }
    }
  }
  $filled_contexts = page_manager_pathauto_build_contexts($context_indexes, array(), $contexts);
  page_manager_pathauto_recursive_contexts_process($subtask_id, $filled_contexts);
}

function page_manager_pathauto_recursive_contexts_process($subtask_id, $contexts, $arguments = array(), $count = 0) {
  $page = page_manager_page_load($subtask_id);
  $i = 0;
  foreach ($contexts as $context) {
    $i++;
    if (is_array($context) && isset($context['data'])) {
      $arguments[] = $context['data'];
      if (isset($context['contexts'])) {
        $count = count($context['contexts']);
        page_manager_pathauto_recursive_contexts_process($subtask_id, $context['contexts'], $arguments, $count);
      }
      else {
        $page = page_manager_page_load($subtask_id);
        $path = $page->path;
        $path_components = explode('/', $path);
        $argument_list = $arguments;
        foreach ($path_components as &$component) {
          if (substr($component, 0, 1) == '%') {
            $argument = array_shift($argument_list);
            $component = $argument->original_argument;
          }
        }
        $path = implode('/', $path_components);
        $_GET['q'] = $path;
        $access_arguments = $arguments;
        array_unshift($access_arguments, $page->access);
        if (call_user_func_array('ctools_access_menu', $access_arguments)) {
          $final_args = $arguments;
          array_unshift($final_args, $subtask_id);
          call_user_func_array('page_manager_page_execute', $final_args);
        }
      }
      array_pop($arguments);
    }
  }
}

function page_manager_pathauto_build_contexts($indexes, $contexts = array(), $base_contexts) {
  ctools_include('context');
  $keys = array_keys($indexes);
  list($plugin_name, $context_name) = explode('--', array_shift($keys));
  $index = array_shift($indexes);
  $temp_context = array();
  foreach ($index as $id => $info) {
    $temp_contexts[] = array('data' => ctools_context_get_context_from_argument($base_contexts[$context_name]->placeholder['conf'], $id));
  }
  $contexts = page_manager_pathauto_merge_context($contexts, $temp_contexts);
  if ($indexes) {
    $contexts = page_manager_pathauto_build_contexts($indexes, $contexts, $base_contexts);
  }
  return $contexts;
}

function page_manager_pathauto_merge_context($contexts = NULL, $temp_contexts) {
  if (!$contexts) {
    return $temp_contexts;
  }
  else {
    foreach ($contexts as $context_id => $context) {
      if (is_array($context) && isset($context['data'])) {
        $contexts[$context_id]['contexts'] = $temp_contexts;
      }
      else {
        $contexts = page_manager_pathauto_merge_context($context[$context_id], $temp_contexts);
      }
    }
  }
  return $contexts;
}
