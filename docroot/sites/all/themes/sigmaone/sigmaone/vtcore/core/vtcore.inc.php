<?php

/**
 * @file
 *
 * This file is the core of vtcore theme
 * All core function should be included here
 * And this file should be included in template.php
 *
 */

/**
 * Define all global definition
 */
global $vtcore;
if (!defined('BASE_THEME')) {
  define('BASE_THEME', 'sigmaone');
}

/**
 * Invoke vtcore to start
 */
vtcore_start();

/**
 * HARD RESET FUNCTIONS
 * This function will delete everything and clear cache
 * all configuration related to theme will be deleted
 *
 * only invoke this function if the theme is broken
 * and vtcore_reset_theme() function didn't restore the
 * theme to its default state
 *
 * @todo: Expand this with hookable / alterable functions so other plugins
 * can register their special hard reset functions
 */
function vtcore_hard_reset_theme() {
  global $theme_key;
  variable_del('theme_' . $theme_key . '_settings');
  variable_del('theme_' . $theme_key . '_plugin');
  cache_clear_all();
}

/**
 * Emergency reset
 * in case something broken badly invoke this function
 * to reset the theme to its default state
 *
 * @todo: Expand this with hookable / alterable functions so other plugins
 * can register their special hard reset functions
 */
function vtcore_reset_theme() {
  global $theme_key;
  variable_del('theme_' . $theme_key . '_settings');
  variable_del('theme_' . $theme_key . '_plugin');
}

/**
 * Function to load all the included files
 *
 * Instruction :
 *  - create a plugin and place the plugin in the plugins directory
 *    the plugin should contain a .meta and .plugin file
 *    .meta file should include a brief description of what the plugin do
 *    .plugin file is the php file for all the plugin code
 *    .admin file is the php file for storing admin related code, this file
 *    will only be loaded during theme configuration process.
 *
 *    plugin naming should be
 *    	folder - example
 *      .meta - example.meta
 *      .plugin - example.plugin
 *      .admin - example.admin
 *
 */
function vtcore_start() {
  global $vtcore, $theme_key, $user;

  // Non Ajax fix for sane theme key, without this
  // Configuration page will always defaulted to base theme (or admin theme)
  if (arg(1) == 'appearance' && arg(2) == 'settings' && arg(3)) {
    $theme_key = arg(3);
  }

  // Fix for ajax returning wrong theme
  // This is important for sane plugins loading since
  // now VTCore capable to load plugins in subtheme directory.
  // Plugin utilizing Ajax function must change ajax callback
  // path to use system/ajax/theme-THEME_NAME to utilize
  // this function and sane plugin loading.
  if (arg(0) == 'system' && arg(1) == 'ajax' && arg(2)) {
    if (strpos(arg(2), 'theme-') !== FALSE) {
      $theme_key = str_replace('theme-', '', arg(2));
    }
  }

  // build the globally stored vtcore variables
  if (empty($vtcore)) {
    $vtcore = new stdClass();
    $vtcore->frontend_theme_path = drupal_get_path('theme', variable_get('theme_default', NULL));
    $vtcore->base_theme_path = drupal_get_path('theme', BASE_THEME);
    $vtcore->active_theme_path = drupal_get_path('theme', $theme_key);

    // @fixme : Remove this when all plugin doesn't use this anymore
    $vtcore->theme_path = $vtcore->active_theme_path;

    // Only declare the subtheme plugin path if the directory exists
    // This should happen before vtcore_scan_plugin() run.
    if ($theme_key != BASE_THEME && is_dir($vtcore->theme_path . '/vtcore/plugins')) {
      $vtcore->subtheme_plugin_path = $vtcore->theme_path . '/vtcore/plugins';
    }

    $vtcore->plugin_path = $vtcore->base_theme_path . '/vtcore/plugins';
    $vtcore->core_path = $vtcore->base_theme_path . '/vtcore/core';
    $vtcore->all_plugins = vtcore_scan_plugin();

    // Switch between saved theme variable for enabled plugins
    // or fall back to default meta file configuration if
    // user hasn't configured the theme yet
    // @todo with this trim all dependent function that does double
    // checking from all_plugins and enabled_plugins
    $enabled_plugins = variable_get('theme_' . $theme_key . '_plugin', array());
    if (empty($enabled_plugins)) {
      $enabled_plugins = $vtcore->all_plugins;
    }
    $vtcore->enabled_plugins = $enabled_plugins;


    $vtcore->admin_page = path_is_admin(current_path());
    $vtcore->admin_theme = variable_get('admin_theme', 0);

    // This will get rechecked in adminpage.plugin
    // since it is too early to determine the overlay status
    // in this stage.
    $vtcore->overlay = FALSE;

    // Define Drupal cache status
    $vtcore->cache = (bool) variable_get('cache', FALSE);

    $vtcore->user_access_admin_theme = user_access('view the administration theme', $user);

    // Load all the enabled base theme plugin first
    vtcore_load_include($vtcore->plugin_path, 'plugin');

    // Load additional plugin for subtheme
    if (isset($vtcore->subtheme_plugin_path)) {
      vtcore_load_include($vtcore->subtheme_plugin_path, 'plugin');
    }
  }
}

/**
 * Clone of drupal alter
 *
 * This clone will search all enabled plugin for hookable function
 * implementation
 *
 * @param string $type
 * @param array / string / object $data
 * @param array / string / object $context1
 * @param array / string / object $context2
 */
function vtcore_alter_process($type, &$data, &$context1 = NULL, &$context2 = NULL) {
  // Use the advanced drupal_static() pattern, since this is called very often.
  static $drupal_static_fast;
  global $vtcore, $theme_key;

  if (!isset($drupal_static_fast)) {
    $drupal_static_fast['functions'] = &drupal_static(__FUNCTION__);
  }
  $functions = &$drupal_static_fast['functions'];
  $plugins = &$drupal_static_fast['plugins'];

  if (!isset($plugins)) {
    $plugins = $vtcore->all_plugins;
  }

  $cid = $type;

  // Some alter hooks are invoked many times per page request, so statically
  // cache the list of functions to call, and on subsequent calls, iterate
  // through them quickly.
  if (!isset($functions[$cid])) {
    $functions[$cid] = array();
    $hook = $type . '_alter_process';

    foreach ($plugins as $key => $value) {
      if (isset($functions[$cid][$key])) {
        continue;
      }

      if ((isset($vtcore->enabled_plugins[$key]) &&  $vtcore->enabled_plugins[$key]['status'] == 0)) {
        continue;
      }

      if (!isset($vtcore->enabled_plugins[$key]) && $value['status'] == 0) {
        continue;
      }

      $functions[$cid][$key] = $key . '_' . $hook;

    }

    // Add current active theme
    $functions[$cid][$theme_key] = $theme_key . '_' . $hook;

    // Add base theme
    $functions[$cid][BASE_THEME] = BASE_THEME . '_' . $hook;
  }

  foreach ($functions[$cid] as $function) {
    if (function_exists($function)) {
      $function($data, $context1, $context2);
    }
  }
}

/**
 * Function to wrap theme_get_setting()
 *
 * So it can behave like variable_get() and returning the
 * specified default value if no variable is set yet.
 *
 * This function will return the value fetched in this order :
 *  1. theme_get_setting()
 *  2. Fetching data from plugin .meta
 *  3. $default specified by caller
 *  4. FALSE if nothing is available
 *
 *
 * @param string $name
 *   variable name to fetch
 * @param string $default
 *   default value to fallback if no variable set yet
 * @param string $plugin
 *   the plugin machine name
 */
function vtcore_get_plugin_setting($name, $plugin, $default = FALSE, $theme_key = FALSE) {
  // Use the advanced drupal_static() pattern, since this is called very often.
  static $data;
  if (!isset($data)) {
    $data = &drupal_static(__FUNCTION__);
  }

  // try to fetch from theme_get_setting
  if (!isset($data[$plugin][$name])) {
    if ($theme_key == FALSE) {
      global $theme_key;
    }
    $data[$plugin][$name] = theme_get_setting($name, $theme_key);
    vtcore_alter_process('vtcore_default_plugin_settings', $name, $plugin, $data);
  }

  // Default is empty and data is empty too
  // Try to fetch the plugin meta stored value for
  // Default value
  if (empty($default) && !isset($data[$plugin][$name]) && !empty($plugin)) {
    global $vtcore;

    // Search the base theme for meta files
    if (file_exists($vtcore->plugin_path . '/' . $plugin . '/' . $plugin . '.meta')) {
      $settings = drupal_parse_info_file($vtcore->plugin_path . '/' . $plugin . '/' . $plugin . '.meta');
    }

    // Just in case the plugin is on subtheme folder or subtheme has meta override
    if (isset($vtcore->subtheme_plugin_path) && file_exists($vtcore->subtheme_plugin_path . '/' . $plugin . '/' . $plugin . '.meta')) {
      $settings = drupal_parse_info_file($vtcore->subtheme_plugin_path . '/' . $plugin . '/' . $plugin . '.meta');
    }

    if (isset($settings['settings'])) {
      $data[$plugin] = $settings['settings'];
    }
  }

  // Data is empty but user defined a default value, return that value instead
  if (!isset($data[$plugin][$name]) && !empty($default)) {
    $data[$plugin][$name] = $default;
  }

  // Data is still empty return FALSE
  if (!isset($data[$plugin][$name])) {
    $data[$plugin][$name] = FALSE;
  }

  return $data[$plugin][$name];
}

/**
 * Function to load include files in a directory
 *
 * @param string $path
 *   The directory path
 * @param string $type
 *   The file type to load, example 'plugin' or 'admin'
 */
function vtcore_load_include($path, $type) {

  // Break early if no path or type defined
  if (empty($path) || empty($type)) {
    return FALSE;
  }
  static $data;
  if (!isset($data)) {
    $data = &drupal_static(__FUNCTION__);

    global $vtcore;

    foreach ($vtcore->all_plugins as $key => $value) {
      // build the filepath
      $filepath = $path . '/' . $key . '/' . $key . '.' . $type;

      // Skip the loop if file not exists
      if (!file_exists($filepath)) {
        continue;
      }
      // skip if this plugin is disabled
      if (isset($vtcore->enabled_plugins[$key]) && $vtcore->enabled_plugins[$key]['status'] == 0) {
        continue;
      }
      // skip if this plugin is disabled by default
      if (!isset($vtcore->enabled_plugins[$key]) && $value['status'] == 0) {
        continue;
      }

      if (!empty($data[$filepath])) {
        continue;
      }
      $data[$filepath] = $filepath;
    }
  }

  // include all the files
  foreach ($data as $filepath) {
    include_once($filepath);
  }

  return $data;
}

/**
 * Function to return all the plugin in plugin directory.
 *
 * The return value will be array keyed with plugin machine name and boolean
 * value of plugin enabled state configured from its meta.
 *
 */
function vtcore_scan_plugin() {
  global $vtcore, $theme_key;

  static $data;
  if (!isset($data)) {
    $data = &drupal_static(__FUNCTION__);

    // get theme plugin enabled status bypass
    $bypass = theme_get_setting('plugin', $theme_key);

    $paths = array();

    $paths[] = $vtcore->plugin_path;

    if (isset($vtcore->subtheme_plugin_path) && is_dir($vtcore->subtheme_plugin_path)) {
      $paths[] = $vtcore->subtheme_plugin_path;
    }

    foreach ($paths as $path) {
      // Loop into the plugin directory to parse the plugin meta file
      foreach (file_scan_directory($path, '/.*\.meta$/', array('key' => 'filename')) as $file) {
        // Load plugin information from its meta file
        $meta = drupal_parse_info_file($file->uri);
        // set the default weight
        if (!isset($meta['weight'])) {
          $meta['weight'] = 0;
        }

        // check for current theme plugin default status bypass first
        if (isset($bypass[$file->name]['enabled'])) {
          $meta['enabled'] = $bypass[$file->name]['enabled'];
        }


        $data[$file->name] = array('status' => $meta['enabled'] , 'weight' => $meta['weight']);
      }
    }

    uasort($data, 'drupal_sort_weight');
  }

  return $data;
}

/**
 * Function to fetch the default value set in
 * meta files
 *
 * Return value can be an empty array if nothing
 * found or array keyed by setting key
 *
 * @param string $plugin
 *   Plugin name
 */
function vtcore_get_default($plugin) {
  global $vtcore;

  // break early if no plugin specified
  if (empty($plugin)) {
    return array();
  }

  static $default;
  if (!isset($default)) {
    $default = &drupal_static(__FUNCTION__);
  }

  if (!isset($default[$plugin])) {
    if (file_exists($vtcore->plugin_path . '/' . $plugin . '/' . $plugin . '.meta')) {
      $meta = drupal_parse_info_file($vtcore->plugin_path . '/' . $plugin . '/' . $plugin . '.meta');
    }

    if (isset($vtcore->subtheme_plugin_path) && file_exists($vtcore->subtheme_plugin_path . '/' . $plugin . '/' . $plugin . '.meta')) {
      $meta = drupal_parse_info_file($vtcore->subtheme_plugin_path . '/' . $plugin . '/' . $plugin . '.meta');
    }

    if (!empty($meta['settings'])) {
      $default[$plugin] = $meta['settings'];
    }
  }

  return $default[$plugin];
}


/**
 * Hookable function for providing renderable array of a special block
 * This function is called in the core layout plugin when processing
 * a special block.
 *
 * Other plugin may invoke hook_vtcore_special_block_array() to perform
 * add, delete and changing of a certain special block renderable array.
 *
 * @param $options
 *   Array containing special blocks options such as weight, region, etc
 * @param $variables
 *   Drupal Renderable array, specifically vtcore modified page array
 */
function vtcore_special_block_renderable_arrays(&$options, &$variables) {
  $blocks = array();

  // Grab plugins renderable arrays information
  vtcore_alter_process('vtcore_special_block_array', $options, $variables, $blocks);
  return $blocks;
}

/**
 * Function to get all the special blocks (vtcore-block)
 * All plugins that create a special blocks need to register their
 * special blocks in the form of
 * $blocks[block_delta] = block_name;
 *
 * by invoking hook_vtcore_special_block_register_alter_process(). in
 * their .plugin file.
 */
function vtcore_special_block_register() {
  static $blocks;
  if (!isset($blocks)) {
    $blocks = &drupal_static(__FUNCTION__);
    $blocks = array();
    vtcore_alter_process('vtcore_special_block_register', $blocks);
  }
  return $blocks;
}

/**
 * Search for key and return the arrays
 * containing the key recursively
 */
function vtcore_find($needle_key, $array) {
  foreach($array as $key=>$value){
    if ($key == $needle_key) {
      return $value;
    }
    if (is_array($value)){
      if (($result = vtcore_find($needle_key,$value)) !== false) {
        return $result;
      }
    }
  }
  return false;
}

/**
 * Function to merge two arrays and update the value for the first array
 * using the value of the second array.
 */
function vtcore_array_merge_recursive_distinct(array &$array1, array &$array2) {
  $merged = $array1;

  foreach ($array2 as $key => &$value) {
    if (is_array($value) && isset($merged[$key]) && is_array($merged[$key])) {
      $merged[$key] = vtcore_array_merge_recursive_distinct($merged[$key], $value);
    }
    else {
      $merged[$key] = $value;
    }
  }

  return $merged;
}

/**
 * Helper function to print any block region content
 */
function render_block($block_region) {
  $block = block_get_blocks_by_region($block_region);
	if ($block) {
	  return render($block);
	}
}

/**
 * Helper function to load include files in form
 * This function is needed to force load include
 * files when a form is rebuilded, We cannot rely on
 * Drupal form_load_include files if the file
 * included is not a module files.
 */
function vtcore_form_load_includes($file, $form, &$form_state) {
  if (empty($file)) {
    return FALSE;
  }
  if (!isset($form_state['build_info']['files'][$file])) {
    if (is_file($file)) {
      require_once $file;
      $form_state['build_info']['files'][$file] = $file;
    }
  }

  if (isset($form_state['build_info']['files'][$file])) {
    return TRUE;
  }
}

/**
 * Gernerates an info file that can be parsed by drupal_parse_info_file.
 *
 * @param array $array
 *   What is returned from drupal_parse_info_file().
 * @param string $prefix
 *   A string to prefix each entry with, should be used only by the function itself
 *   during recursion.
 *
 * @return string
 *   A string corresponding to $array in the info format. If written into a file, it can be parsed back by drupal_parse_info_file
 *
 */
function vtcore_build_info_file($array, $prefix = FALSE) {
  $info = '';
  if (is_array($array)) {
    foreach ($array as $key => $value) {
      if (is_array($value)) {
        $info .= vtcore_build_info_file($value, (!$prefix ? $key : "{$prefix}[{$key}]"));
        $info .= "\n";
      }
      else {
        $info .= $prefix ? ("{$prefix}[" . (is_int($key) ? '' : $key) .']') : $key;
        $info .= " = '" . str_replace("'", "\'", $value) . "'\n";
      }
    }
  }
  return $info;
}

/**
 * Helper function to determine the default value
 */
function vtcore_get_default_value($key, $array, $default) {
  if (isset($array[$key])) {
    $default = $array[$key];
  }

  return $default;
}

/**
 * Helper function to load all the js files
 * found in the specified directory
 */
function vtcore_load_all_js($dir, $weight = 90, $regex = '/.*\.js$/') {
  // Break on no valid dir
  if (empty($dir) || !is_dir($dir)) {
    return FALSE;
  }

  $files = &drupal_static(__FUNCTION__);
  if (!isset($files[$dir])) {
    $options = array('key' => 'uri');
    $files[$dir] = file_scan_directory($dir, '/.*\.js$/', $options);

    // Attach all jQueryUI related js files
    foreach ($files[$dir] as $uri => $file) {
      drupal_add_js($uri, array('weight' => $weight));
      $weight++;
    }
  }
}

/**
 * Helper function to load all the css files
 * found in the specified directory
 */
function vtcore_load_all_css($dir, $weight = 90, $regex = '/.*\.css$/') {
  // Break on no valid dir
  if (empty($dir) || !is_dir($dir)) {
    return FALSE;
  }

  $files = &drupal_static(__FUNCTION__);

  if (!isset($files[$dir])) {
    $options = array('key' => 'uri');
    $files[$dir] = file_scan_directory($dir, $regex, $options);

    // Attach all jQueryUI related js files
    foreach ($files[$dir] as $uri => $file) {
      drupal_add_css($uri, array('weight' => $weight));
      $weight++;
    }
  }
}

/**
 * Helper function to attach all the js files
 * found in the specified directory into form array
 */
function vtcore_attach_all_js($dir, $weight = 90, &$form, $regex = '/.*\.js$/') {
  // Break on no valid dir
  if (empty($dir) || !is_dir($dir)) {
    return FALSE;
  }

  $js = &drupal_static(__FUNCTION__);
  if (!isset($js[$dir])) {
    $js[$dir] = array();
    $options = array('key' => 'uri');
    $files[$dir] = file_scan_directory($dir, $regex, $options);

    // Attach all jQueryUI related js files
    foreach ($files[$dir] as $uri => $file) {
      $data = array(
        'type' => 'file',
        'data' => $uri,
        'weight' => $weight,
        'group' => CSS_THEME,
      );

      $js[$dir][$uri] = $data;
      $weight++;
    }
  }

  if (!isset($form['#attached']['js'])) {
    $form['#attached']['js'] = array();
  }

  $form['#attached']['js'] += $js[$dir];

}

/**
 * Helper function to attach all the css files
 * found in the specified directory into form array
 */
function vtcore_attach_all_css($dir, $weight = 90, &$form, $regex = '/.*\.css$/') {
  // Break on no valid dir
  if (empty($dir) || !is_dir($dir)) {
    return FALSE;
  }

  $css = &drupal_static(__FUNCTION__);

  if (!isset($css[$dir])) {
    $css[$dir] = array();
    $options = array('key' => 'uri');
    $files[$dir] = file_scan_directory($dir, $regex, $options);

    // Attach all jQueryUI related js files
    foreach ($files[$dir] as $uri => $file) {
      $data = array(
        'type' => 'file',
        'data' => $uri,
        'weight' => $weight,
      );
      $css[$dir][$uri] = $data;
      $weight++;
    }
  }

  if (!isset($form['#attached']['css'])) {
    $form['#attached']['css'] = array();
  }

  $form['#attached']['css'] += $css[$dir];
}

/**
 * Clone of _block_load_blocks().
 * Except this one can fetch all blocks registered
 * to specified theme instead of relying on $theme_key
 */
function vtcore_block_load_blocks($theme_key) {

  $query = db_select('block', 'b');
  $result = $query
    ->fields('b')
    ->condition('b.theme', $theme_key)
    ->condition('b.status', 1)
    ->orderBy('b.region')
    ->orderBy('b.weight')
    ->orderBy('b.module')
    ->addTag('block_load')
    ->addTag('translatable')
    ->execute();

  $block_info = $result->fetchAllAssoc('bid');
  // Allow modules to modify the block list.
  drupal_alter('block_list', $block_info);

  $blocks = array();
  foreach ($block_info as $block) {
    $blocks[$block->region]["{$block->module}_{$block->delta}"] = $block;
  }
  return $blocks;
}


/**
 * Checks the page, user role, and user-specific visibilty settings.
 * Return (boolean) false if block should not visible;
 */
function vtcore_block_check_visibility(&$blocks, $theme_key) {
  global $user;

  if (empty($theme_key)) {
    global $theme_key;
  }

  $block_roles = drupal_static(__FUNCTION__);

  // Query once to save database hit
  if (empty($block_roles)) {
    // Build an array of roles for each block.
    $block_roles = array();
    $result = db_query('SELECT module, delta, rid FROM {block_role}');
    foreach ($result as $record) {
      $block_roles[$record->module][$record->delta][] = $record->rid;
    }
  }

  foreach ($blocks as $key => $block) {
    if (!isset($block->theme) || !isset($block->status) || $block->theme != $theme_key || $block->status != 1) {
      unset($blocks[$key]);
      continue;
    }

    // If a block has no roles associated, it is displayed for every role.
    // For blocks with roles associated, if none of the user's roles matches
    // the settings from this block, remove it from the block list.
    if (isset($block_roles[$block->module][$block->delta]) && array_intersect($block_roles[$block->module][$block->delta], array_keys($user->roles))) {
      // No match.
      unset($blocks[$key]);
      continue;
    }

    // Use the user's block visibility setting, if necessary.
    if ($block->custom != BLOCK_CUSTOM_FIXED) {
      if ($user->uid && isset($user->data['block'][$block->module][$block->delta])) {
        $enabled = $user->data['block'][$block->module][$block->delta];
      }
      else {
        $enabled = ($block->custom == BLOCK_CUSTOM_ENABLED);
      }
    }
    else {
      $enabled = TRUE;
    }

    // Limited visibility blocks must list at least one page.
    if ($block->visibility == BLOCK_VISIBILITY_LISTED && empty($block->pages)) {
      $enabled = FALSE;
    }

    if (!$enabled) {
      unset($blocks[$key]);
      continue;
    }

    // Match path if necessary.
    if ($block->pages) {
      // Convert path to lowercase. This allows comparison of the same path
      // with different case. Ex: /Page, /page, /PAGE.
      $pages = drupal_strtolower($block->pages);
      if ($block->visibility < BLOCK_VISIBILITY_PHP) {
        // Convert the Drupal path to lowercase
        $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
        // Compare the lowercase internal and lowercase path alias (if any).
        $page_match = drupal_match_path($path, $pages);
        if ($path != $_GET['q']) {
          $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
        }
        // When $block->visibility has a value of 0 (BLOCK_VISIBILITY_NOTLISTED),
        // the block is displayed on all pages except those listed in $block->pages.
        // When set to 1 (BLOCK_VISIBILITY_LISTED), it is displayed only on those
        // pages listed in $block->pages.
        $page_match = !($block->visibility xor $page_match);
      }
      elseif (module_exists('php')) {
        $page_match = php_eval($block->pages);
      }
      else {
        $page_match = FALSE;
      }
    }
    else {
      $page_match = TRUE;
    }
    if (!$page_match) {
      unset($blocks[$key]);
    }
  }
}

/**
 * Helper function to grab any field type from certain bundles
 *
 * @return
 *   An array of elements containing machine_name => human name.
 *   if nothing exists will return empty array;
 */
function vtcore_get_field_type_name($field_type, $bundles) {
  $fields = array();
  if (module_exists('field') && module_exists('text')) {
    foreach (field_info_fields() as $key => $field) {
      if ($field['type'] == $field_type && isset($field['bundles'][$bundles])) {
        $fields[$field['field_name']] = $field['field_name'];
      }
    }
  }
  return $fields;
}