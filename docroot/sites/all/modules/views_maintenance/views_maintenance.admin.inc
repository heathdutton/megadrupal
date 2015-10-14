<?php
/**
 * @file
 * Provides administrative UI for Views Maintenance.
 */

/**
 * Sorts by status, the order is "broken", "ok", "maybe", "unused".
 *
 * Callback for usort() within views_maintenance_page().
 *
 * @param array $a
 * @param array $b
 */
function _views_maintenance_use_cases_sort_by_status($a, $b) {
  return _views_maintenance_compare_by_order($a['status'], $b['status']);
}

/**
 * Returns view status based on its displays statuses.
 *
 * @param array $displays_statuses
 *   Flat array containing statuses from most important use case of each
 *   view display.
 *
 * @return string
 *   Status of view. Value is one of:
 *   - "broken"
 *   - "ok"
 *   - "has-maybe"
 *   - "has-unused"
 *   - "unused".
 */
function _views_maintenance_view_status($displays_statuses = array()) {
  // Check "broken" status existence.
  if (array_search('broken', $displays_statuses) !== FALSE) {
    return 'broken';
  }

  // Move 'default' display status from array
  $default_status = $displays_statuses['default'];
  unset($displays_statuses['default']);

  if (empty($displays_statuses)) {
    // Default display is the only display in view, use its status for view
    // status.
    switch ($default_status) {
      case 'ok':
        return 'ok';

      case 'maybe':
        return 'has-maybe';

      default: // unused
        return 'unused';
    }
  }

  $counts = array_count_values($displays_statuses);
  if (!empty($counts['unused'])) {
    // View has unused displays.
    if (empty($counts['ok']) && empty($counts['maybe']) && $default_status == 'unused') {
      // View contains only unused displays.
      return 'unused';
    }
    else {
      // View has unused displays among others.
      return 'has-unused';
    }
  }
  elseif (!empty($counts['maybe'])) {
    // View doesn't have unused displays, but has "maybe" non-default displays.
    // Default display status is ignored here.
    return 'has-maybe';
  }
  else {
    // All non-default displays have "ok" status, ignore default display status.
    return 'ok';
  }
}

/**
 * Returns array of administration links for view passed.
 *
 * Most of code copied from template_preprocess_views_ui_list_views(). Don't
 * need to check access for links, because views maintenance menu item already
 * has permissions check.
 *
 * @param object $view
 *
 * @return array
 */
function _views_maintenance_view_admin_links($view) {
  static $tokens = array();

  $query = drupal_get_destination();

  // Fill tokens static cache.
  if (empty($tokens)) {
    $tokens['enable'] = drupal_get_token('enable');
    $tokens['disable'] = drupal_get_token('disable');
  }

  $links = array(
    l(t('Edit'), "admin/structure/views/view/$view->name", array('query' => $query)),
    l(t('Export'), "admin/structure/views/view/$view->name/export", array('query' => $query)),
  );

  // Add Delete/Revert/Enable/Disable link.
  if ($view->type != t('Default')) {
    $links[] = ($view->type == t('Overridden'))
      ? l(t('Revert'), "admin/structure/views/view/$view->name/revert", array('query' => $query))
      : l(t('Delete'), "admin/structure/views/view/$view->name/delete", array('query' => $query));
  }
  else {
    if (empty($view->disabled)) {
      $query['token'] = $tokens['disable'];
      $links[] = l(t('Disable'), "admin/structure/views/view/$view->name/disable", array('query' => $query));
    }
    else {
      $query['token'] = $tokens['enable'];
      $links[] = l(t('Enable'), "admin/structure/views/view/$view->name/enable", array('query' => $query));
    }
  }

  return $links;
}

/**
 * Page callback for views maintenance page.
 *
 * @return string
 */
function views_maintenance_page() {
  $views = views_get_all_views();

  // Filter-out disabled views from list.
  foreach ($views as $view_id => $view) {
    if (!empty($view->disabled)) {
      // Exclude disabled views
      $view->destroy();
      unset($views[$view_id]);
      continue;
    }
  }

  $views_info = array();
  if (!empty($views)) {
    // Include necessary files. But first of all include views/includes/handlers.inc,
    // because hook_views_api() implementations for core modules are there and
    // after we call module_implements('views_api') implementations static cache
    // will miss those implementations and all core handlers will be broken.
    views_include('base');
    views_include('handlers');
    views_module_include('views_maintenance.inc');

    // Run hooks to get status from all modules
    $views_use_cases = module_invoke_all('views_maintenance_use_cases', $views);

    // Init order for sorting use cases by level of importance.
    _views_maintenance_compare_by_order(NULL, NULL, array('broken', 'ok', 'maybe', 'unused'));

    // Build views info.
    views_include('admin');
    $base_tables = views_fetch_base_tables();
    foreach ($views as $view_id => $view) {
      $displays_info = array();
      $displays_statuses = array();
      foreach ($view->display as $display_id => $display) {
        if (!empty($views_use_cases[$view_id][$display_id])) {
          // At least one use case was returned, sort use cases by status. This
          // brings use case with most important status to the first position.
          // Order was initialized earlier.
          $display_use_cases = $views_use_cases[$view_id][$display_id];
          usort($display_use_cases, '_views_maintenance_use_cases_sort_by_status');
        }
        else {
          // No use cases were returned for this display, use default use case
          // with "unused" status and without description.
          $display_use_cases = array(
            array(
              'status' => 'unused',
            ),
          );
        }

        // Save first use case status, it is used for determining view status.
        $display_use_cases_keys = array_keys($display_use_cases);
        $first = reset($display_use_cases_keys);
        $displays_statuses[$display_id] = $display_use_cases[$first]['status'];

        // Build display info.
        $displays_info[$display_id] = array(
          'name' => $display->display_title,
          'type' => $display->handler->definition['title'],
          'use_cases' => $display_use_cases,
        );
      }

      // Determine view status based on most important status of each display.
      $view_status = _views_maintenance_view_status($displays_statuses);

      $views_info[$view_id] = array(
        'name' => $view->name,
        'type' => !empty($base_tables[$view->base_table]['title']) ? $base_tables[$view->base_table]['title'] : t('Broken'),
        'storage' => $view->type,
        'status' => $view_status,
        'links' => _views_maintenance_view_admin_links($view),
        'description' => $view->description,
        'displays' => $displays_info,
      );
    }
  }

  return theme('views_maintenance_views_table', array('views' => $views_info));
}
