<?php
/**
 * @file
 * Provides integration with Page Manager and Panels Mini.
 */

/**
 * Returns view name and display ID used by pane.
 *
 * @param object $pane
 *
 * @return array|null
 *   Numerical array suitable for list() or NULL if pane doesn't use views.
 */
function _views_maintenance_display_id_from_pane($pane) {
  switch ($pane->type) {
    case 'views':
      $view_id = $pane->subtype;
      $display_id = isset($pane->configuration['display']) ? $pane->configuration['display'] : 'default';
      break;

    case 'views_panes':
      // See views_content_views_panes_content_type_content_type()
      list($view_id, $display_id) = explode('-', $pane->subtype);
      break;

    default:
      return NULL;
  }
  return array($view_id, $display_id);
}

/**
 * Returns page task name wrapped with placeholder.
 *
 * If admin title is not available, machine name is returned.
 *
 * @param string $task_name
 * @param array $task
 *
 * @return string
 */
function _views_maintenance_page_title($task_name, $task) {
  if (trim($task['admin title']) != '') {
    // Admin title is already escaped, see page_manager_page_build_subtask().
    $result = $task['admin title'];
  }
  else {
    $result = check_plain($task_name);
  }

  return theme('views_maintenance_html_placeholder', array('html' => $result));
}

/**
 * Returns page handler human-readable title wrapped with placeholder.
 *
 * @param object $handler
 *
 * @return string
 */
function _views_maintenance_page_handler_title($handler) {
  return theme('placeholder', $handler->conf['title']);
}

/**
 * Returns content pane human-readable title wrapped with placeholder.
 *
 * @param object $pane
 *
 * @return string
 */
function _views_maintenance_content_pane_title($pane) {
  $result = '';
  switch ($pane->type) {
    case 'views':
      ctools_get_plugins('ctools', 'content_types', 'views');
      $result = views_content_views_content_type_admin_title($pane->subtype, $pane->configuration);
      break;

    case 'views_panes':
      ctools_get_plugins('ctools', 'content_types', 'views_panes');
      $result = views_content_views_panes_content_type_admin_title($pane->subtype, $pane->configuration, array());
      break;
  }
  return theme('views_maintenance_html_placeholder', array('html' => $result));
}

/**
 * Returns mini panel title wrapped with placeholder.
 *
 * If admin title is not available, machine name is returned.
 *
 * @param object $mini_panel
 *
 * @return string
 */
function _views_maintenance_mini_panel_title($mini_panel) {
  if (trim($mini_panel->admin_title) == '') {
    $result = $mini_panel->name;
  }
  else {
    $result = $mini_panel->admin_title;
  }
  return theme('placeholder', $result);
}

/**
 * Returns context identifier wrapped with placeholder.
 *
 * @param object $context
 *
 * @return string
 */
function _views_maintenance_context_title($context) {
  return theme('placeholder', $context->identifier);
}

/**
 * Builds use case as pane in page manager page.
 *
 * Use case should be confirmed outside of this function, it only builds use
 * case array based on states of passed objects.
 *
 * @param array $task
 *   Page manager task definition.
 * @param object $handler
 *   Display handler.
 * @param object $pane
 *   Panels pane.
 *
 * @return array
 */
function _views_maintenance_page_pane_use_case($task_name, $task, $handler, $pane) {
  $links = array();
  if (user_access('use page manager')) {
    $links[] = l(t('Edit page'), page_manager_edit_url($task_name));
    $links[] = l(t('Pages list'), 'admin/build/pages');
  }

  $description = array();

  $page_title = _views_maintenance_page_title($task_name, $task);
  if (!empty($task['disabled'])) {
    $status = 'unused';
    $description[] = t('Page !title is disabled', array('!title' => $page_title));
  }
  else {
    $description[] = t('Page !title is enabled', array('!title' => $page_title));

    $handler_title = _views_maintenance_page_handler_title($handler);
    if (!empty($handler->disabled)) {
      $status = 'unused';
      $description[] = t('Variant !title is disabled', array('!title' => $handler_title));
    }
    else {
      $description[] = t('Variant !title is enabled', array('!title' => $handler_title));

      $pane_title = _views_maintenance_content_pane_title($pane);
      if (empty($pane->shown)) {
        $status = 'unused';
        $description[] = t('Pane !title is hidden', array('!title' => $pane_title));
      }
      else {
        $status = 'ok';
        $description[] = t('Pane !title is visible', array('!title' => $pane_title));
      }
    }
  }

  return array(
    'type' => t('Content Pane in Page'),
    'status' => $status,
    'description' => $description,
    'links' => $links,
  );
}

/**
 * Builds use case as context in page manager page.
 *
 * Use case should be confirmed outside of this function, it only builds use
 * case array based on states of passed objects.
 *
 * @param string $task_name
 *   Full task name including task and subtask if any.
 * @param array $task
 *   Page manager task definition.
 * @param object $handler
 *   Display handler.
 * @param object $context
 *   Context object.
 *
 * @return array
 */
function _views_maintenance_page_context_use_case($task_name, $task, $handler, $context) {
  $links = array();
  if (user_access('use page manager')) {
    $links[] = l(t('Edit page'), page_manager_edit_url($task_name));
    $links[] = l(t('Pages list'), 'admin/build/pages');
  }

  $description = array();

  $page_title = _views_maintenance_page_title($task_name, $task);
  if (!empty($task['disabled'])) {
    $status = 'unused';
    $description[] = t('Page !title is disabled', array('!title' => $page_title));
  }
  else {
    $description[] = t('Page !title is enabled', array('!title' => $page_title));

    $handler_title = _views_maintenance_page_handler_title($handler);
    if (!empty($handler->disabled)) {
      $status = 'unused';
      $description[] = t('Variant !title is disabled', array('!title' => $handler_title));
    }
    else {
      $status = 'ok';
      $description[] = t('Variant !title is enabled', array('!title' => $handler_title));

      $context_title = _views_maintenance_context_title($context);
      $description[] = t('Context !title', array('!title' => $context_title));
    }
  }

  return array(
    'type' => t('Context in Page'),
    'status' => $status,
    'description' => $description,
    'links' => $links,
  );
}

/**
 * Processes handlers of task or subtask and returns use cases.
 *
 * @param array $handlers
 * @param string $task_name
 *   Full task name.
 *
 * @return array
 */
function _views_maintenance_process_page_handlers($handlers, $task, $task_name) {
  ctools_include('content');
  ctools_include('plugins');
  ctools_include('context');
  ctools_include('context-task-handler');
  if (module_exists('panels')) {
    ctools_get_plugins('page_manager', 'task_handlers');
  }

  $result = array();
  foreach ($handlers as $handler) {
    if ($handler->handler == 'panel_context') {
      // Search for views content panes.
      $display = panels_panel_context_get_display($handler);
      if (!empty($display->content)) {
        foreach ($display->content as $pane) {
          $id = _views_maintenance_display_id_from_pane($pane);
          if (!empty($id)) {
            list($view_id, $display_id) = $id;

            $use_case = _views_maintenance_page_pane_use_case($task_name, $task, $handler, $pane);
            $result[$view_id][$display_id][] = $use_case;
          }
        }
      }
    }

    // Search for views contexts.
    $contexts = ctools_context_handler_get_all_contexts($task, $handler->subtask, $handler);
    foreach ($contexts as $context) {
      $id = _views_maintenance_display_id_from_context($context);
      if (!empty($id)) {
        list($view_id, $display_id) = $id;

        $use_case = _views_maintenance_page_context_use_case($task_name, $task, $handler, $context);
        $result[$view_id][$display_id][] = $use_case;
      }
    }
  }
  return $result;
}

/**
 * Returns view name and display ID used by context handler.
 *
 * @param object $context
 *
 * @return array|null
 *   Numerical array suitable for list() or NULL if context doesn't use views.
 */
function _views_maintenance_display_id_from_context($context) {
  switch ($context->type) {
    case 'view':
      if (!empty($context->argument)) {
        return explode(':', $context->argument, 2);
      }

    default:
      return NULL;
  }
}

/**
 * Processes list of page manager page tasks and returns views use cases.
 *
 * It runs _views_maintenance_process_page_handlers() for task without subtasks
 * otherwise it goes down into recursion.
 *
 * @param array $tasks
 *   List of page manager page definitions.
 * @param array $parent
 *   For internal use only. Stores parent task definition.
 *
 * @return array
 */
function _views_maintenance_process_page_tasks($tasks, $parent = NULL) {
  $use_cases = array();

  foreach ($tasks as $task_id => $task) {
    // If a type has subtasks, process its subtasks instead of task itself.
    $subtasks = page_manager_get_task_subtasks($task);
    if (empty($subtasks)) {
      // Construct full task name and retrieve its handlers.
      if (isset($parent)) {
        $task_name = page_manager_make_task_name($parent['name'], $task['name']);
        $handlers = page_manager_load_task_handlers($parent, $task_id);
      }
      else {
        $task_name = $task['name'];
        $handlers = page_manager_load_task_handlers($task);
      }

      // Fetch views use cases from handlers.
      $use_cases[] = _views_maintenance_process_page_handlers($handlers, $task, $task_name);
    }
    else {
      // Task containing subtasks, go deep into recursion.
      $use_cases[] = _views_maintenance_process_page_tasks($subtasks, $task);
    }
  }

  if (!empty($use_cases)) {
    // Merge together use cases from different pages and return them in
    // expected format.
    return call_user_func_array('array_merge_recursive', $use_cases);
  }
  else {
    return array();
  }
}

/**
 * Returns views use cases in all page manager pages.
 *
 * @return array
 */
function _views_maintenance_page_use_cases() {
  $tasks = page_manager_get_tasks_by_type('page');
  return _views_maintenance_process_page_tasks($tasks);
}

/**
 * Builds use case as pane in mini panel.
 *
 * Use case should be confirmed outside of this function, it only builds use
 * case array based on states of passed objects.
 *
 * @param array $plugin
 *   Mini panel plugin definition.
 * @param object $mini_panel
 *   Mini panel handler.
 * @param object $pane
 *   Panels pane.
 *
 * @return array
 */
function _views_maintenance_mini_panel_pane_use_case($plugin, $mini_panel, $pane) {
  $links = array();
  if (user_access($plugin['access'])) {
    $links[] = l(t('Edit mini panel'), ctools_export_ui_plugin_menu_path($plugin, 'edit', $mini_panel->name));
    $links[] = l(t('Mini panels list'), 'admin/build/mini-panels');
  }

  $description = array();

  $mini_panel_title = _views_maintenance_mini_panel_title($mini_panel);
  if (!empty($mini_panel->disabled)) {
    $status = 'unused';
    $description[] = t('Mini panel !title is disabled', array('!title' => $mini_panel_title));
  }
  else {
    $description[] = t('Mini panel !title is enabled', array('!title' => $mini_panel_title));

    $pane_title = _views_maintenance_content_pane_title($pane);
    if (empty($pane->shown)) {
      $status = 'unused';
      $description[] = t('Pane !title is hidden', array('!title' => $pane_title));
    }
    else {
      $status = 'ok';
      $description[] = t('Pane !title is visible', array('!title' => $pane_title));
    }
  }

  return array(
    'type' => t('Content Pane in Mini Panel'),
    'status' => $status,
    'description' => $description,
    'links' => $links,
  );
}

/**
 * Builds use case as context in mini panel.
 *
 * Use case should be confirmed outside of this function, it only builds use
 * case array based on states of passed objects.
 *
 * @param array $plugin
 *   Mini panel plugin definition.
 * @param object $mini_panel
 *   Mini panel handler.
 *
 * @return array
 */
function _views_maintenance_mini_panel_context_use_case($plugin, $mini_panel, $context) {
  $links = array();
  if (user_access($plugin['access'])) {
    $links[] = l(t('Edit mini panel'), ctools_export_ui_plugin_menu_path($plugin, 'edit', $mini_panel->name));
    $links[] = l(t('Mini panels list'), 'admin/build/mini-panels');
  }

  $description = array();

  $mini_panel_title = _views_maintenance_mini_panel_title($mini_panel);
  if (!empty($mini_panel->disabled)) {
    $status = 'unused';
    $description[] = t('Mini panel !title is disabled', array('!title' => $mini_panel_title));
  }
  else {
    $status = 'ok';
    $description[] = t('Mini panel !title is enabled', array('!title' => $mini_panel_title));

    $context_title = _views_maintenance_context_title($context);
    $description[] = t('Context !title', array('!title' => $context_title));
  }

  return array(
    'type' => t('Context in Mini Panel'),
    'status' => $status,
    'description' => $description,
    'links' => $links,
  );
}

/**
 * Returns displays use cases in all mini-panels.
 *
 * @return array
 *   Nested array with use cases grouped by view name and display id.
 */
function _views_maintenance_mini_panel_use_cases() {
  ctools_include('content');
  ctools_include('plugins');
  ctools_include('context');
  ctools_include('export-ui');

  $plugin = ctools_get_export_ui('panels_mini');
  $mini_panels = panels_mini_load_all();

  $result = array();
  // Iterate through mini-panels and fill use cases.
  foreach ($mini_panels as $name => $mini_panel) {
    if (!empty($mini_panel->display->content)) {
      foreach ($mini_panel->display->content as $pane) {
        // Analyze each pane and get view name and display id (if any).
        $id = _views_maintenance_display_id_from_pane($pane);
        if (!empty($id)) {
          list($view_id, $display_id) = $id;

          $use_case = _views_maintenance_mini_panel_pane_use_case($plugin, $mini_panel, $pane);
          $result[$view_id][$display_id][] = $use_case;
        }
      }
    }

    // Iterate over contexts and fill use cases.
    $contexts = ctools_context_get_context_from_contexts($mini_panel->contexts, 'context', TRUE);
    foreach ($contexts as $context) {
      $id = _views_maintenance_display_id_from_context($context);
      if (!empty($id)) {
        list($view_id, $display_id) = $id;

        $use_case = _views_maintenance_mini_panel_context_use_case($plugin, $mini_panel, $context);
        $result[$view_id][$display_id][] = $use_case;
      }
    }
  }
  return $result;
}

/**
 * Builds use case as pane in panel node.
 *
 * Use case should be confirmed outside of this function, it only builds use
 * case array based on states of passed objects.
 *
 * @param object $node
 * @param object $pane
 *
 * @return array
 */
function _views_maintenance_panel_node_pane_use_case($node, $pane) {
  $links = array();
  $node = (object)$node;
  if (user_access('administer nodes') || user_access('administer panel-nodes')) {
    $links[] = l(t('Edit panel content'), "node/$node->nid/panel_content");
  }

  $description = array();

  if (empty($node->status)) {
    $status = 'unused';
    $description[] = t('Node %title is unpublished', array('%title' => $node->title));
  }
  else {
    $description[] = t('Node %title is published', array('%title' => $node->title));

    $pane_title = _views_maintenance_content_pane_title($pane);
    if (empty($pane->shown)) {
      $status = 'unused';
      $description[] = t('Pane !title is hidden', array('!title' => $pane_title));
    }
    else {
      $status = 'ok';
      $description[] = t('Pane !title is visible', array('!title' => $pane_title));
    }
  }

  return array(
    'type' => t('Content Pane in Panel Node'),
    'status' => $status,
    'description' => $description,
    'links' => $links,
  );
}

/**
 * Returns displays use cases in all panel nodes.
 *
 * @return array
 *   Nested array with use cases grouped by view name and display id.
 */
function _views_maintenance_panel_node_use_cases() {
  ctools_include('content');
  ctools_include('plugins');
  
  // Load only necessary info about panel nodes.
  $query = db_select('panels_node', 'pn');
  $query->addField('pn', 'did');
  $query->addJoin('inner', 'node', 'n', 'n.nid = pn.nid');
  $query->addField('n', 'title');
  $query->addField('n', 'nid');
  $query->addField('n', 'status');
  $res = $query->execute();

  $nodes = array();
  while ($node = $res->fetchAssoc()) {
    // Save nodes to array using display id as key.
    $nodes[$node['did']] = $node;
  }

  $displays = panels_load_displays(array_keys($nodes));
  $result = array();
  foreach ($displays as $did => $display) {
    if (!empty($nodes[$did])) {
      foreach ($display->content as $pane) {
        // Analyze each pane and get view name and display id (if any).
        $id = _views_maintenance_display_id_from_pane($pane);
        if (!empty($id)) {
          list($view_id, $display_id) = $id;
          $node = $nodes[$did];

          $use_case = _views_maintenance_panel_node_pane_use_case($node, $pane);
          $result[$view_id][$display_id][] = $use_case;
        }
      }
    }
  }
  return $result;
}
