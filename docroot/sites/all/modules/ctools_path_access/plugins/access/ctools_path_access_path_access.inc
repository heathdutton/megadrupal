<?php
/**
 * @file
 * Plugin to provide access control based upon the access callback of an
 * existing menu item.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Access to another path'),
  'description' => t('Control access according to the access on another path.'),
  'callback' => 'ctools_path_access_path_access_ctools_access_check',
  'default' => array('path' => ''),
  'settings form' => 'ctools_path_access_path_access_ctools_access_settings',
  'settings form submit' => 'ctools_path_access_path_access_ctools_access_settings_submit',
  'summary' => 'ctools_path_access_path_access_ctools_access_summary',
  'all contexts' => TRUE,
);

/**
 * Settings form for the path_access access plugin
 */
function ctools_path_access_path_access_ctools_access_settings($form, &$form_state, $conf) {
  $form['settings']['path'] = array(
    '#title' => t('Path to determine access'),
    '#type' => 'textfield',
    '#description' => t('Use the access rules for this path to determine access. <strong>Leave blank to use the original access rules for the path that this page is at</strong> (if no page has been overridden, access is granted). Enter any valid Drupal path, or %front for the front page. You may use context keywords. Please do not use a path that itself uses this path access check.', array('%front' => '<front>')),
    '#default_value' => $conf['path'],
  );
  return $form;
}

/**
 * Validation for the settings form for the path_access access plugin
 */
function ctools_path_access_path_access_ctools_access_settings_validate($form, &$form_state, $conf) {
  if (url_is_external($form_state['values']['path'])) {
    form_set_error('path', t('Please supply an internal Drupal path'));
  }
}

/**
 * Check for access.
 */
function ctools_path_access_path_access_ctools_access_check($conf, $context) {
  if (!empty($conf['path'])) {
    $path = $conf['path'];
  }
  else {
    $path = $_GET['q'];
  }

  // Replace any context keywords in $path.
  if (!empty($context)) {
    $path = ctools_context_keyword_substitute($path, array(), $context);
  }

  $path = drupal_get_normal_path($path);

  // A cache is useful because we often check the access in multiple places.
  $cache = &drupal_static(__FUNCTION__);
  if (isset($cache[$path])) {
    // Handle circular loop of access checks...
    if ($cache[$path] == MENU_NOT_FOUND) {
      // Instead, deal with access in the same way as custom pages - let any
      // other access checks make the decision.

      // We want to try and get hold of the and/or setting for access checks, if
      // available.
      // We can't use menu_get_item because it computes access, so would get
      // stuck in a loop back here. We just need the page_callback,
      // access_callback and access_arguments.
      $menu_item = _ctools_path_access_menu_get_item($path);

      // Create a dummy custom page router item.
      static $custom_page_item;
      if (!isset($custom_page_item) && function_exists('page_manager_get_task')) {
        $task = page_manager_get_task('page');
        require_once DRUPAL_ROOT . '/' . $task['hook menu']['path'] . '/' . $task['hook menu']['file'];
        if (function_exists('page_manager_page_menu_item')) {
          $custom_page_item = page_manager_page_menu_item(FALSE, array(),array(), array(), array());
        }
        else {
          // Something has changed in page_manager. We can't check if we are a
          // custom page page task, we're just going to have to treat the page
          // as if it doesn't contain any other access plugins.
        }
      }

      // If our $menu_item is a standard custom page page task, with callbacks
      // and argument formats that match, we can detect the logic needed, and
      // whether we need to return FALSE/TRUE to avoid affecting the outcome.
      if (!empty($custom_page_item) && isset($custom_page_item['access callback']) && !empty($menu_item) && isset($menu_item['access_callback']) && isset($menu_item['access_arguments']) && $menu_item['access_callback'] == $custom_page_item['access callback']) {
        $access_arguments = unserialize($menu_item['access_arguments']);
        if (empty($access_arguments[0]['plugins'])) {
          $cache[$path] = TRUE;
        }
        elseif (isset($access_arguments[0]['logic'])) {
          if ($access_arguments[0]['logic'] == 'or') {
            $cache[$path] = FALSE;
          }
          elseif ($access_arguments[0]['logic'] == 'and') {
            $cache[$path] = TRUE;
          }
        }
      }

      // It's possible that we are on a page that does use CTools access
      // plugins, but not in the same way as the CTools custom page page task.
      // If this is the case, we can't really know how to return something that
      // won't affect the logic like we do above. Let's go with the default...

      // No other plugins exist, or have set access yet, so assume we should
      // allow access (which is the CTools default behaviour too).
      if ($cache[$path] == MENU_NOT_FOUND) {
        $cache[$path] = TRUE;
      }
    }
    return $cache[$path];
  }
  else {
    // Set a flag in the cache that we can use to pick up on whether we're
    // trying to establish access for this path. Use MENU_NOT_FOUND because it
    // corresponds pretty closely to what we're after, and doesn't clash with
    // TRUE/FALSE.
    $cache[$path] = MENU_NOT_FOUND;
  }

  $access = drupal_valid_path($path, TRUE);
  $cache[$path] = $access;
  return $access;
}

/**
 * Loads menu item instead of menu_get_item with minimal information.
 *
 * @see menu_get_item()
 */
function _ctools_path_access_menu_get_item($path) {
  $cached_items = drupal_static('menu_get_item');  // Borrows menu_get_item cache, but read-only.
  $router_items = $cached_items;
  if (!isset($router_items[$path])) {
    $original_map = arg(NULL, $path);

    // Since there is no limit to the length of $path, use a hash to keep it
    // short yet unique.
    $cid = 'menu_item:' . hash('sha256', $path);
    if ($cached = cache_get($cid, 'cache_menu')) {
      $router_item = $cached->data;
    }
    else {
      $parts = array_slice($original_map, 0, MENU_MAX_PARTS);
      $ancestors = menu_get_ancestors($parts);
      $router_item = db_query_range('SELECT * FROM {menu_router} WHERE path IN (:ancestors) ORDER BY fit DESC', 0, 1, array(':ancestors' => $ancestors))->fetchAssoc();
      // We don't really want to mess with the menu cache. We could be helpful
      // and set it (since it wasn't already), but probably best not to.
      //cache_set($cid, $router_item, 'cache_menu');
    }
    if ($router_item) {
      // Allow modules to alter the router item before it is translated and
      // checked for access.
      drupal_alter('menu_get_item', $router_item, $path, $original_map);
      // We should by now have everything we need, so we don't do anything more
      // like menu_get_item() does.
    }
    $router_items[$path] = $router_item;
  }
  return $router_items[$path];
}

/**
 * Provide a summary description based upon the checked path_accesss.
 */
function ctools_path_access_path_access_ctools_access_summary($conf, $context) {
  if (!empty($conf['path'])) {
    $path = '/' . $conf['path'];
  }
  else {
    $path = t('the path that has been overridden. (Always allowed if not an override.)');
  }

  return t('Logged in user has access to !path', array('!path' => $path));
}
