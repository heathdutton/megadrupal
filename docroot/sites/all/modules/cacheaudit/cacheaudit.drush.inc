<?php

/**
 * Implementation of hook_drush_command().
 */
function cacheaudit_drush_command() {
  $items['cacheaudit'] = array(
    'description' => 'Show a report on configured cache settings.',
    'arguments' => array(
      'module' => '(Optional) Show only the output from this module.',
    ),
  );
  return $items;
}

/**
 * Drush command callback for Cache Audit.
 */
function drush_cacheaudit($module = '') {
  $results = array();

  // Check if a specific module was requested.
  if (!empty($module)) {
    if (!($results = module_invoke($module, 'cacheaudit'))) {
      drush_set_error(DRUSH_FRAMEWORK_ERROR, "The " . $module . " module does not exist or does not appear to support cacheaudit.");
      return;
    }
  }
  else if (drush_get_context('DRUSH_PIPE')) {
    drush_set_error(DRUSH_FRAMEWORK_ERROR, "A module argument is required when using --pipe.");
    return;
  }
  else {
    $results = module_invoke_all('cacheaudit');
  }

  foreach ($results as $cat => $result) {
    if (drush_get_context('DRUSH_PIPE')) {
      drush_print_pipe(drush_format($result, NULL, 'csv'));
    }
    else {
      drush_print($cat);
      drush_print(str_pad('', 100, "-"));
      drush_print_table($result, TRUE);
    }
  }
}

/**
 * Implements hook_cacheaudit().
 *
 * Implemented on behalf of system.module.
 */
function system_cacheaudit() {
  global $conf;

  $vars = array(
    'block_cache',
    'cache',
    'cache_lifetime',
  );

  $block_cache_values = $cache_values = array(
    dt('Off'),
    dt('On'),
  );

  $cache_lifetime_values = $page_cache_maximum_age_values = $page_cache_max_age_values = array(
    0 => dt('<none>'),
    60 => dt('1 min'),
    180 => dt('3 min'),
    300 => dt('5 min'),
    600 => dt('10 min'),
    900 => dt('15 min'),
    1800 => dt('30 min'),
    2700 => dt('45 min'),
    3600 => dt('1 hour'),
    10800 => dt('3 hours'),
    21600 => dt('6 hours'),
    32400 => dt('9 hours'),
    43200 => dt('12 hours'),
    86400 => dt('1 day'),
  );

  if (drush_drupal_major_version() == 7) {
    $vars[] = 'cache_default_class';
    $vars[] = 'page_cache_maximum_age';
    $vars[] = 'cache_backends';
    $vars = array_merge($vars, preg_grep('/^cache_class_/', array_keys($conf)));
  }
  else if (drush_drupal_major_version() == 6) {
    $vars[] = 'cache_inc';
    $cache_values[1] = dt('Normal');
    $cache_values[2] = dt('Aggressive');
    // Pressflow
    if (function_exists("drupal_page_cache_header_external")) {
      $vars[] = 'page_cache_max_age';
      $cache_values[3] = dt('External');
    }
  }
  sort($vars);

  $results = array();
  $results[] = array(
    'Variable',
    'Value',
  );

  foreach ($vars as $var) {
    $value = variable_get($var, '<not set>');

    // Add check for ability to use block caching
    if ($var == 'block_cache' && $value == 0) {
      if (count(module_implements('node_grants')) > 0) {
        $value .= t(' (not available)');
      }
    }

    // Check for multiple backends
    if ($var == 'cache_backends' && is_array($value)) {
      foreach ($value as $key => $backend) {
        $value[$key] = basename($backend);
      }
      $value = implode(', ', $value);
    }

    if (isset(${$var . '_values'})) {
      $var_values = ${$var . '_values'};
      $value = isset($var_values[$value]) ? $var_values[$value] : $value;
    }

    $results[] = array($var, $value);
  }

  return array('system' => $results);
}

/**
 * Implements hook_cacheaudit().
 *
 * Implemented on behalf of block.module.
 */
function block_cacheaudit() {
  $theme = variable_get('theme_default', NULL);
  if (!$theme) {
    return;
  }

  $results = array();
  $results[] = array(
    'Module-Delta',
    'Info',
    'Cache',
  );

  $blocks = _block_rehash($theme);
  usort($blocks, 'block_cacheaudit_sort');
  foreach ($blocks as $block) {
    $results[] = array(
      $block['module'] .'-'. $block['delta'],
      $block['info'],
      cacheaudit_get_cache_constant($block['cache']),
    );
  }

  return array('block' => $results);
}

/**
 * Custom sorting function for block names.
 */
function block_cacheaudit_sort($a, $b) {
  $a_full = $a['module'] .'-'. $a['delta'];
  $b_full = $b['module'] .'-'. $b['delta'];
  return strcmp($a_full, $b_full);
}

/**
 * Implements hook_cacheaudit().
 *
 * Implemented on behalf of Views module.
 */
function views_cacheaudit() {
  $defaults = array(
    'name' => '',
    'display' => 'default',
    'status' => 'Enabled',
    'type' => 'none',
    'results_lifespan' => '0',
    'output_lifespan' => '0',
    'block_caching' => 'N/A',
  );
  $results = array();
  $results[] = array('View', 'Display', 'Status', 'Plugin', 'Results', 'Output', 'Block cache');

  foreach (views_get_all_views() as $name => $view) {
    // Get the default display first.
    $default = isset($view->display['default']->display_options['cache']) ? $view->display['default']->display_options['cache'] : array();
    $status = !isset($view->disabled) || !$view->disabled ? 'Enabled' : 'Disabled';
    $results[] = array_merge($defaults, array('name' => $name, 'display' => 'default', 'status' => $status), $default);

    foreach ($view->display as $display_name => $display) {
      // Skip default display since we already got it above.
      if ($display_name === 'default') continue;

      $display_results = array();
      if (isset($display->display_options['cache'])) {
        $display_results = $display->display_options['cache'];
      }

      if ($display->display_plugin === 'block') {
        if (empty($display_results)) {
          $display_results = $default;
        }
        if (isset($display->display_options['block_caching'])) {
          $display_results['block_caching'] = cacheaudit_get_cache_constant($display->display_options['block_caching']);
        }
        else {
          $display_results['block_caching'] = 'Off';
        }
      }

      if ($display_results) {
        $results[] = array_merge($defaults, array('name' => $name, 'display' => $display_name, 'status' => $status), $display_results);
      }
    }
  }
  return array('views' => $results);
}

/**
 * Implements hook_cacheaudit().
 *
 * Implemented on behalf of the Page Manager module.
 */
function page_manager_cacheaudit() {
  $results = array();

  $cache_lifetime_values = array(
    0 => dt('<none>'),
    15 => dt('15s'),
    30 => dt('30s'),
    60 => dt('1 min'),
    120 => dt('2 min'),
    180 => dt('3 min'),
    240 => dt('4 min'),
    300 => dt('5 min'),
    600 => dt('10 min'),
    900 => dt('15 min'),
    1200 => dt('20 min'),
    1800 => dt('30 min'),
    2700 => dt('45 min'),
    3600 => dt('1 hour'),
    7200 => dt('2 hour'),
    10800 => dt('3 hours'),
    14400 => dt('4 hours'),
    21600 => dt('6 hours'),
    28800 => dt('8 hours'),
    32400 => dt('9 hours'),
    43200 => dt('12 hours'),
    86400 => dt('1 day'),
    172800 => dt('2 days'),
    259200 => dt('3 days'),
    345600 => dt('4 days'),
    604800 => dt('1 week'),
  );

  foreach (_cacheaudit_get_panels() as $name => $panes) {
    foreach ($panes as $pane) {
      $cache = isset($pane->cache['method']) ? $pane->cache['method'] : '';
      $lifetime = isset($pane->cache['method']) ? $cache_lifetime_values[$pane->cache['settings']['lifetime']] : '';
      $granularity = isset($pane->cache['method']) ? var_export($pane->cache['settings']['granularity'], TRUE) : '<not set>';
      $results[] = array($name, $pane->subtype, $pane->panel, $cache, $lifetime, $granularity);
    }
  }

  if (!empty($results)) {
    array_unshift($results, array('Panel', 'Pane', 'Region', 'Cache', 'Lifetime', 'Granularity'));
    return array('panels' => $results);
  }

  return array();
}

/**
 * @see http://drupalcode.org/project/cache_actions.git/blob/refs/heads/7.x-2.x:/cache_actions.rules.inc#l138
 */
function _cacheaudit_get_panels() {
  $available_handlers = array();
  // First, get all tasks. This corresponds to all types of page manager pages
  // that can be used, for for instance pages, node view, node edit...
  $tasks = page_manager_get_tasks();
  foreach ($tasks as $task) {
    // Subtasks are tasks that are created under the primary tasks, for instance
    // a custom page the user has created.
    $subtasks = page_manager_get_task_subtasks($task);
    // If we have subtasks, then that's what we're after.
    if (count($subtasks)) {
      foreach ($subtasks as $subtask) {
        // Subtasks have handlers. These can for instance correspond to a panel
        // variant.
        $handlers = page_manager_load_task_handlers($task, $subtask['name']);
        foreach ($handlers as $handler) {
          // Handlers have plugins, in this case we need to get the plugin for
          // this handler.
          $plugin = page_manager_get_task_handler($handler->handler);
          $title = page_manager_get_handler_title($plugin, $handler, $task, $subtask['name']);
          $key = 'task:' . $handler->name . ':' . $handler->task;
          // Fetch available panes.
          $handler_panes = _cacheaudit_load_panes($handler, $title);
          foreach ($handler_panes as $pane_key => $handler_pane) {
            $available_handlers[$handler->conf['title']][$pane_key] = $handler_pane;
          }
        }
      }
    }
    else {
      // Otherwise let's use the task.
      $handlers = page_manager_load_task_handlers($task);
      if (count($handlers)) {
        foreach ($handlers as $handler) {
          $plugin = page_manager_get_task_handler($handler->handler);
          $title = page_manager_get_handler_title($plugin, $handler, $task, $task['name']);
          // If not, then we have an in-code display. Save off the name, so we
          // can get it.
          // Fetch available panes.
          $handler_panes = _cacheaudit_load_panes($handler, $title);
          foreach ($handler_panes as $pane_key => $handler_pane) {
            $available_handlers[$handler->conf['title']][$pane_key] = $handler_pane;
          }
        }
      }
    }
  }
  // Otherwise just return the handlers.
  return $available_handlers;
}

/**
 * @see http://drupalcode.org/project/cache_actions.git/blob/refs/heads/7.x-2.x:/cache_actions.rules.inc#l231
 */
function _cacheaudit_load_panes($handler) {
  ctools_include('plugins', 'panels');
  ctools_include('content');

  $available_panes = array();
  if (isset($handler->conf['did'])) {
    $display = panels_load_display($handler->conf['did']);
  }
  else if (!empty($handler->conf['display'])) {
    $display = $handler->conf['display'];
  }
  if (isset($display->content)) {
    foreach($display->content as $content) {
      if (!count($content->cache)) {
        $content->cache = $display->cache;
      }
    }
  }
  return isset($display->content) ? $display->content : array();
}

/**
 * Convert a caching constant to a printable string.
 */
function cacheaudit_get_cache_constant($constant_value) {
  switch ($constant_value) {
  case DRUPAL_CACHE_CUSTOM:
    $constant_name = 'DRUPAL_CACHE_CUSTOM';
    break;
  case DRUPAL_NO_CACHE:
    $constant_name = 'DRUPAL_NO_CACHE';
    break;
  case DRUPAL_CACHE_CUSTOM:
    $constant_name = 'DRUPAL_CACHE_CUSTOM';
    break;
  case DRUPAL_CACHE_GLOBAL:
    $constant_name = 'DRUPAL_CACHE_GLOBAL';
    break;
  case DRUPAL_CACHE_PER_PAGE:
    $constant_name = 'DRUPAL_CACHE_PER_PAGE';
    break;
  case DRUPAL_CACHE_PER_ROLE:
    $constant_name = 'DRUPAL_CACHE_PER_ROLE';
    break;
  case DRUPAL_CACHE_PER_USER:
    $constant_name = 'DRUPAL_CACHE_PER_USER';
    break;
  }
  return $constant_name;
}
