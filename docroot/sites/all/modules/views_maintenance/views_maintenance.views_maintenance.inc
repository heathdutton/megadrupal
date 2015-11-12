<?php

/**
 * @file
 * Provides displays use cases info to Views Maintenance.
 */

/**
 * Returns array with all blocks provided by views.
 *
 * Format is the same as of _block_rehash(), except deltas in array keys are
 * restored from hashes.
 *
 * @return array
 */
function _views_maintenance_all_views_blocks() {
  $blocks = _block_rehash();
  $hashes = variable_get('views_block_hashes', array());
  $views_blocks = array();
  foreach ($blocks as $block) {
    if ($block['module'] == 'views') {
      $delta = $block['delta'];

      // Un-hash deltas to its originals. See views_block('list') for details.
      if (isset($hashes[$delta])) {
        $delta = $hashes[$delta];
      }

      $views_blocks[$delta] = $block;
    }
  }
  return $views_blocks;
}

/**
 * Returns settings of passed display blocks.
 *
 * @param array $blocks
 *   List of blocks fetched from display.
 *
 * @return array
 *   Two dimensional array with passed blocks settings. Keys of first array are
 *   deltas (non-hashed).
 */
function _views_maintenance_display_blocks_settings($blocks) {
  static $all_blocks;
  if (!isset($all_blocks)) {
    $all_blocks = _views_maintenance_all_views_blocks();
  }

  $blocks_settings = array();
  if (is_array($blocks)) {
    foreach ($blocks as $delta => $block) {
      if (!empty($all_blocks[$delta])) {
        $blocks_settings[$delta] = $all_blocks[$delta];
      }
    }
  }
  return $blocks_settings;
}

/**
 * Returns use cases as blocks for passed display.
 *
 * @param object $display
 *
 * @return array
 */
function _views_maintenance_display_blocks_use_cases($display) {
  $blocks_settings = array();

  // Fetch ordinary blocks from display
  if (isset($display->handler) && !empty($display->handler->definition['uses hook block'])) {
    $display_blocks = $display->handler->execute_hook_block_list();
    $blocks_settings += _views_maintenance_display_blocks_settings($display_blocks);
  }

  // Fetch exposed form blocks from display
  if (isset($display->handler) && $display->handler->get_option('exposed_block')) {
    $exposed_blocks = $display->handler->get_special_blocks();
    $blocks_settings += _views_maintenance_display_blocks_settings($exposed_blocks);
  }

  // Build use cases array.
  $result = array();
  foreach ($blocks_settings as $delta => $block_info) {
    // Block delta is converted to hash if it is too long.
    // See views_block('list') for details.
    if (strlen($delta) >= 32) {
      $delta = md5($delta);
    }

    $links = array();
    if (user_access('administer blocks')) {
      $links[] = l(t('Block settings'), 'admin/build/block/configure/views/' . $delta);
    }

    if (empty($block_info['status'])) {
      $status = 'unused';
      $enabled = t('disabled');
    }
    else {
      $status = 'ok';
      $enabled = t('enabled');
    }

    $result[] = array(
      'type' => t('Block'),
      'status' => $status,
      'description' => t('Block %admin_title is !enabled.', array(
        '%admin_title' => $block_info['info'],
        '!enabled' => $enabled,
      )),
      'links' => $links,
    );
  }

  return $result;
}

/**
 * Returns use cases as paths for passed display and fills menu items array.
 *
 * Also reports about path used by two or more displays with "broken" status
 * for second and later displays.
 *
 * @param object $display
 * @param object $view
 * @param array $paths
 *   Filled with list of paths provided by display.
 *
 * @return array
 *   Use cases for paths (not menu items) provided by passed display.
 */
function _views_maintenance_display_paths_use_cases($display, $view, &$paths) {
  static $occupied_paths = array();

  $result = array();
  if (isset($display->handler) && !empty($display->handler->definition['uses hook menu'])) {
    $callbacks = array();
    $menu_items = $display->handler->execute_hook_menu($callbacks);
    if (!empty($menu_items)) {
      foreach ($menu_items as $path => $item) {
        // Ignore items without callback (local tasks and other).
        if (empty($item['page callback'])) {
          continue;
        }

        // Format path into link if it's without placeholders.
        if (strpos($path, '%') !== FALSE) {
          $formatted_path = check_plain($path);
        }
        else {
          $formatted_path = l($path, $path);
        }

        // If display provides path it is marked as probably used.
        $result[] = array(
          'type' => t('Path provided'),
          'status' => 'maybe',
          'description' => $formatted_path
        );

        // Check for paths provided by multiple displays.
        if (isset($occupied_paths[$path])) {
          $occupant = $occupied_paths[$path];
          $arguments = array(
            '!path' => $formatted_path,
            '%view' => $occupant['view'],
            '%display_title' => $occupant['display_title'],
          );

          // Path is provided by another view, this should be reported as broken.
          $result[] = array(
            'type' => t('Path is in use'),
            'status' => 'broken',
            'description' => array(
              t('!path is already used by another display.', $arguments),
              t('View: %view.', $arguments),
              t('Display: %display_title.', $arguments),
            ),
            'links' => array(
              l(t('Edit display of first view'), 'admin/structure/views/view/' . $occupant['view'], array(
                'fragment' => 'views-tab-' . $occupant['display_id'],
              )),
            ),
          );
        }
        else {
          $occupied_paths[$path] = array(
            'view' => $view->name,
            'display_id' => $display->id,
            'display_title' => $display->display_title,
          );
        }

        // Add path to list no matter if it was occupied by multiple displays.
        $paths[$path][$view->name][$display->id] = TRUE;
      }
    }
  }

  return $result;
}

/**
 * Returns use cases as menu items for passed menu items fetched from displays.
 *
 * Searches for paths provided by displays in menus.
 *
 * @param array $items
 *   Nested array:
 *   - 1st: keys are paths
 *   - 2nd: keys are views names
 *   - 3rd: keys are display ids
 *   - values are TRUE
 *
 * @return string
 *
 * @see _views_maintenance_display_paths_use_cases()
 *
 * @todo Search for disabled items if path doesn't have enabled items.
 */
function _views_maintenance_menu_items_use_cases($items) {
  $result = array();

  if (!empty($items)) {
    // Search for enabled menu items with passed paths and build use cases.
    $query = db_select('menu_links', 'ml');
    $query->addField('ml', 'router_path','path');
    $query->addExpression('COUNT(*)', 'count');
    $query->condition('ml.hidden', 0);
    $query->condition('ml.router_path', array_keys($items));
    $query->groupBy('ml.router_path');

    foreach ($query->execute() as $row) {
      foreach ($items[$row->path] as $view_id => $displays) {
        foreach ($displays as $display_id => $item) {
          $result[$view_id][$display_id][] = array(
            'type' => t('Menu items'),
            'status' => 'ok',
            'description' => format_plural($row->count, '@count item is enabled', '@count items are enabled', array(
              '@count' => $row->count,
            )),
          );
        }
      }
    }
  }

  return $result;
}

/**
 * Returns use cases for broken display and its plugins/handlers if any.
 *
 * Most part of code was copied from views_ui_import_validate().
 *
 * @param object $display
 *
 * @return array
 */
function _views_maintenance_display_broken_use_cases($display) {
  $result = array();

  // Check display handler.
  if (empty($display->handler) || !empty($display->handler->broken)) {
    $result[] = array(
      'type' => t('Broken display plugin'),
      'status' => 'broken',
      'description' => t('Plugin %plugin is not available.', array('%plugin' => $display->display_plugin)),
    );

    // Return immediately, there is no sense to continue.
    return $result;
  }

  // Check style plugin and row plugin if it is used.
  $plugin = views_get_plugin('style', $display->handler->get_option('style_plugin'));
  if (!$plugin) {
    $result[] = array(
      'type' => t('Broken style plugin'),
      'status' => 'broken',
      'description' => t('Plugin %plugin is not available.', array('%plugin' => $display->handler->get_option('style_plugin'))),
    );
  }
  elseif ($plugin->uses_row_plugin()) {
    $plugin = views_get_plugin('row', $display->handler->get_option('row_plugin'));
    if (!$plugin) {
      $result[] = array(
        'type' => t('Broken row plugin'),
        'status' => 'broken',
        'description' => t('Plugin %plugin is not available.', array('%plugin' => $display->handler->get_option('row_plugin'))),
      );
    }
  }

  // Check field, argument, sort, filter and relationship handlers.
  foreach (views_object_types() as $type => $info) {
    $handlers = $display->handler->get_handlers($type);
    if ($handlers) {
      foreach ($handlers as $id => $handler) {
        if ($handler->broken()) {
          $result[] = array(
            'type' => t('Broken handler'),
            'status' => 'broken',
            'description' => t('@type handler @table.@field is not available.', array(
              '@type' => $info['stitle'],
              '@table' => $handler->table,
              '@field' => $handler->field,
            )),
          );
        }
      }
    }
  }

  return $result;
}

/**
 * Returns use cases of attached displays (feed, for example).
 *
 * @param object $display
 * @param object $view
 *
 * @return array
 * 
 * @todo Probably we should check display type here instead of trying to get
 *   displays list directly.
 */
function _views_maintenance_display_attachment_use_cases($display, $view) {
  $result = array();

  if (!empty($display->handler)) {
    $attachers = $display->handler->get_option('displays');
    if (!empty($attachers)) {
      foreach ($attachers as $attacher_id) {
        if (!empty($view->display[$attacher_id])) {
          $result[] = array(
            'type' => t('Display attachment'),
            'status' => 'ok',
            'description' => t('Attached to display %display', array(
              '%display' => $view->display[$attacher_id]->display_title,
            )),
          );
        }
      }
    }
  }

  return $result;
}

/**
 * Implements hook_views_maintenance_use_cases().
 */
function views_maintenance_views_maintenance_use_cases($views) {
  // Init blocks cache before any views init, because _block_rehash()
  // resets views.
  _views_maintenance_display_blocks_settings(NULL);

  // Paths provided by displays. It is filled by multiple calls to
  // _views_maintenance_display_paths_use_cases().
  $paths = array();

  // Collect displays use cases in blocks, attachments, as a menu item and
  // broken displays.
  $result = array();
  foreach ($views as $view_id => $view) {
    $view->init_display();
    foreach ($view->display as $display_id => $display) {
      $use_cases = array();

      // Fetch different types of use cases.
      $use_cases[] = _views_maintenance_display_paths_use_cases($display, $view, $paths);
      $use_cases[] = _views_maintenance_display_blocks_use_cases($display);
      $use_cases[] = _views_maintenance_display_broken_use_cases($display);
      $use_cases[] = _views_maintenance_display_attachment_use_cases($display, $view);

      // Merge use cases together and add them to result.
      $use_cases = call_user_func_array('array_merge', $use_cases);
      if (!empty($use_cases)) {
        $result[$view_id][$display_id] = $use_cases;
      }
    }
  }

  // Retrieve all use cases as menu items at once for paths provided by all
  // displays.
  $menu_use_cases = _views_maintenance_menu_items_use_cases($paths);
  $result = array_merge_recursive($result, $menu_use_cases);

  if (module_exists('views_content') && !function_exists('views_content_views_maintenance_use_cases')) {
    module_load_include('inc', 'views_maintenance', 'includes/ctools');

    // Fetch views use cases in page manager pages.
    if (module_exists('page_manager')) {
      $page_use_cases = _views_maintenance_page_use_cases();
      $result = array_merge_recursive($result, $page_use_cases);
    }

    // Get views use cases in mini panels.
    if (module_exists('panels_mini')) {
      $mini_panel_use_cases = _views_maintenance_mini_panel_use_cases();
      $result = array_merge_recursive($result, $mini_panel_use_cases);
    }

    // Collect use cases from panel nodes.
    if (module_exists('panels_node')) {
      $panel_node_use_cases = _views_maintenance_panel_node_use_cases();
      $result = array_merge_recursive($result, $panel_node_use_cases);
    }
  }
  return $result;
}
