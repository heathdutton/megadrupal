<?php


/*
 * These things are all copied from the original admin_menu module.
 */


/**
 * Menu callback; Enable/disable developer modules.
 *
 * This can save up to 150ms on each uncached page request.
 */
function dqx_adminmenu_toggle_modules() {

  if (!dqx_adminmenu_allow_actions()) {
    return 'This url is not supposed to be called directly.';
  }

  $rebuild = FALSE;
  $saved_state = variable_get('dqx_adminmenu_devel_modules_enabled', NULL);
  if (isset($saved_state)) {
    // Re-enable modules that were enabled before.
    module_enable($saved_state);
    variable_del('dqx_adminmenu_devel_modules_enabled');
    drupal_set_message(t('Enabled these modules: !module-list.', array('!module-list' => implode(', ', $saved_state))));
    $rebuild = TRUE;
  }
  else {
    // Allow site admins to override this variable via settings.php.
    $devel_modules = variable_get('dqx_adminmenu_devel_modules', _dqx_adminmenu_developer_modules());
    // Store currently enabled modules in a variable.
    $devel_modules = array_intersect(module_list(FALSE, FALSE), $devel_modules);
    $devel_modules = array_diff($devel_modules, variable_get('dqx_adminmenu_devel_modules_skip', array()));
    if (!empty($devel_modules)) {
      variable_set('dqx_adminmenu_devel_modules_enabled', $devel_modules);
      // Disable developer modules.
      module_disable($devel_modules);
      drupal_set_message(t('Disabled these modules: !module-list.', array('!module-list' => implode(', ', $devel_modules))));
      $rebuild = TRUE;
    }
    else {
      drupal_set_message(t('No developer modules are enabled.'));
    }
  }
  if ($rebuild) {
    // Make sure everything is rebuilt, basically a combination of the calls
    // from system_modules() and system_modules_submit().
    drupal_rebuild_theme_registry();
    menu_rebuild();
    cache_clear_all('schema', 'cache');
    cache_clear_all();
    drupal_clear_css_cache();
    drupal_clear_js_cache();
    // Synchronize to catch any actions that were added or removed.
    actions_synchronize();
    // Get an up-to-date version of dqx_adminmenu
    variable_set('dqx_adminmenu_timestamp', time());
  }
}

/**
 * Helper function to return a default list of developer modules.
 */
function _dqx_adminmenu_developer_modules() {
  return array(
    'cache_disable',
    'coder',
    'content_copy',
    'debug',
    'delete_all',
    'demo',
    'devel',
    'devel_node_access',
    'devel_themer',
    'macro',
    'form_controller',
    'imagecache_ui',
    'journal',
    'rules_admin',
    'stringoverrides',
    'trace',
    'upgrade_status',
    'user_display_ui',
    'util',
    'views_ui',
    'views_theme_wizard',
  );
}



/**
 * Flush all caches or a specific one.
 *
 * @param $name
 *   (optional) Name of cache to flush.
 */
function dqx_adminmenu_flush_cache($name = NULL) {

  if (!dqx_adminmenu_allow_actions()) {
    return 'This url is not supposed to be called directly.';
  }

  switch ($name) {

    case 'menu':
      menu_rebuild();
      break;

    case 'cache':
      // Don't clear cache_form - in-progress form submissions may break.
      // Ordered so clearing the page cache will always be the last action.
      // @see drupal_flush_all_caches()
      $core = array('cache', 'cache_block', 'cache_filter', 'cache_page');
      $cache_tables = array_merge(module_invoke_all('flush_caches'), $core);
      foreach ($cache_tables as $table) {
        cache_clear_all('*', $table, TRUE);
      }
      break;

    case 'cssjs':
    case 'css+js':
    case 'css-js':
    case 'requisites':
      // Change query-strings on css/js files to enforce reload for all users.
      _drupal_flush_css_js();
      drupal_clear_css_cache();
      drupal_clear_js_cache();
      // Cached html might still link to old css and js aggregates.
      // Even if page cache is currently disabled, we flush it preemptively,
      // for the time when it will be enabled again.
      // See http://drupal.org/node/902860
      cache_clear_all('*', 'cache_page', TRUE);
      break;

    case 'theme':
      module_invoke('system', 'theme_data');
      drupal_rebuild_theme_registry();
      break;

    default:
      // Flush all caches; no need to re-implement this.
      module_load_include('inc', 'system', 'system.admin');
      $form = $form_state = array();
      system_clear_cache_submit($form, $form_state);
      return;
  }

  drupal_set_message("Caches '$name' cleared");
}




