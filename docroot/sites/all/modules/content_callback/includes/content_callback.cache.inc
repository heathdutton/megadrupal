<?php

/**
 * Wrapper around cache_get() to make it easier to pull different
 * datastores from a single cache row.
 */
function content_callback_cache_get($key, $reset = FALSE) {
  static $cache;
  if (!isset($cache) || $reset) {
    $cache = cache_get('content_callback', 'cache');
    $cache = $cache ? $cache->data : array();
  }
  return !empty($cache[$key]) ? $cache[$key] : FALSE;
}

/**
 * Wrapper around cache_set() to make it easier to write different
 * datastores to a single cache row.
 */
function content_callback_cache_set($key, $value) {
  $cache = cache_get('content_callback', 'cache');
  $cache = $cache ? $cache->data : array();
  $cache[$key] = $value;
  cache_set('content_callback', $cache);
}

/**
 * Return all callbacks registered by hook_content_callback_info
 */
function content_callback_callbacks_get() {
  $callbacks = content_callback_cache_get('callbacks');
  if (!$callbacks) {
    $callbacks = _content_callback_callbacks_build();
  }
  return $callbacks;
}

/**
 * Build an array of all callbacks and add them to the cache
 */
function _content_callback_callbacks_build() {
  $callbacks = array();
  $modules = module_implements('content_callback_info');

  // Get all callbacks for the current entity_type
  foreach ($modules as $module) {
    $module_callbacks = array();
    $function = $module . '_' . 'content_callback_info';
    if (function_exists($function)) {
      $module_callbacks = $function();

      if (count($module_callbacks)) {
        foreach ($module_callbacks as $key => &$callback) {
          $callback['module'] = $module;
          if (!isset($callback['callback'])) {
            $callback['callback'] = $key . '_callback';
          }
        }
      }
    }
    $callbacks = array_merge($callbacks, $module_callbacks);
  }

  // Get all Content Callback Views Displays
  if (module_exists('content_callback_views')) {
    $views_all = views_get_all_views();

    $displays = array();
    foreach ($views_all as $view) {
      foreach ($view->display as $id => $display) {
        if ($display->display_plugin == 'content_callback') {
          $row = new stdClass();
          $row->vid = (isset($view->vid) && $view->vid) ? $view->vid : $view->name;
          $row->id = $id;
          $row->name = $view->name;
          $row->display_title = $display->display_title;
          $row->display_options = $display->display_options;
          $displays[] = $row;
        }
      }
    }

    $view_callbacks = array();
    foreach ($displays as $row) {
      $display_options = $row->display_options;

      $view_callbacks['view#' . $row->vid . '_' . $row->id] = array(
        'title' => isset($display_options['title']) ? $display_options['title'] : $row->display_title,
        'module' => 'content_callback_views',
        'callback' => 'content_callback_views_content_callback_views_callback',
        'views_display_id' => $row->id,
        'views_view_name' => $row->name,
      );
    }
    $callbacks = array_merge($callbacks, $view_callbacks);
  }
  content_callback_cache_set('callbacks', $callbacks);

  return $callbacks;
}

/**
 * Implements hook_flush_caches().
 */
function content_callback_flush_caches() {
  content_callback_invalidate_cache();
}

/**
 * Invalidates all content_callback caches().
 */
function content_callback_invalidate_cache() {
  cache_clear_all('content_callback', 'cache', TRUE);
}
