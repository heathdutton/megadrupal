<?php

/**
 * @file
 * Handle CSS & JS inclusion or exclusion.
 */


/**
 * Implements hook_js_alter().
 *
 * This function checks all CSS files currently added via drupal_add_js() and
 * checks to see if any of them are disabled via theme settings.
 */
function sasson_js_alter(&$js) {
  // Replace core's jQuery with latest version from Google's CDN
  if (theme_get_setting('sasson_latest_jquery')) {
    $path = '//ajax.googleapis.com/ajax/libs/jquery/1/jquery.min.js';
    $js[$path] = $js['misc/jquery.js'];
    // Update necessary settings
    $js[$path]['version'] = 'latest';
    $js[$path]['data'] = $path;
    // Remove core's jQuery
    unset($js['misc/jquery.js']);
  }

  // Disable core and modules JS via theme settings
  if (theme_get_setting('sasson_disable_js')) {
    $disable_files = array();
    // remove selected JS files
    if ($modules = theme_get_setting('sasson_disable_js_modules')) {
      $disable_files = array_merge($disable_files, sasson_get_assets_list('js', $modules));
    }
    if (is_array(theme_get_setting('sasson_disable_js_files')) && $files = array_filter(theme_get_setting('sasson_disable_js_files'))) {
      $disable_files = array_merge($disable_files, $files);
    }
    if (is_array(theme_get_setting('sasson_disable_jquery_files')) && $files = array_filter(theme_get_setting('sasson_disable_jquery_files'))) {
      $disable_files = array_merge($disable_files, $files);
    }
    if (theme_get_setting('sasson_disable_sasson_js') && $files = sasson_get_assets_list('js', 'sasson')) {
      // remove all JS files added by Sasson
      $disable_files = array_merge($disable_files, $files);
    }
    $js = array_diff_key($js, $disable_files);
  }

  // Move all scripts to the footer.
  // To force a file to go in the header do -
  // drupal_add_js('path/to/js', array('force_header' => TRUE));
  if (theme_get_setting('sasson_js_footer')) {
    foreach ($js as &$item) {
      if (empty($item['force_header'])) {
        $item['scope'] = 'footer';
      }
    }
  }
}


/**
 * Implements hook_css_alter().
 *
 * This function checks all CSS files currently added via drupal_add_css() and
 * checks to see if any of them are disabled via theme settings and if a
 * direction-specific file should be included.
 */
function sasson_css_alter(&$css) {
  global $language;

  // Disable core and modules CSS via theme settings
  if (theme_get_setting('sasson_disable_css')) {
    $disable_files = array();
    if (theme_get_setting('sasson_disable_css_all')) {
      // remove all css files from contrib and core modules
      $disable_files = sasson_get_assets_list();
    }
    else {
      // remove selected css files
      if ($modules = theme_get_setting('sasson_disable_css_modules')) {
        $disable_files = array_merge($disable_files, sasson_get_assets_list('css', $modules));
      }
      if (is_array(theme_get_setting('sasson_disable_css_files')) && $files = array_filter(theme_get_setting('sasson_disable_css_files'))) {
        $disable_files = array_merge($disable_files, $files);
      }
    }

    $css = array_diff_key($css, $disable_files);
  }

  foreach ($css as $data => &$item) {

    // The CSS_SYSTEM aggregation group doesn't make any sense. Therefore, we are
    // pre-pending it to the CSS_DEFAULT group. This has the same effect as giving
    // it a separate (low-weighted) group but also allows it to be aggregated
    // together with the rest of the CSS.
    if ($item['group'] == CSS_SYSTEM) {
      $item['group'] = CSS_DEFAULT;
      $item['weight'] = $item['weight'] - 100;
    }

    // Include direction-specific stylesheets
    if ($item['type'] == 'file') {
      // If the current language is LTR, add the file with the LTR overrides.
      // Core already takes care of RTL css files.
      if ($language->direction == LANGUAGE_LTR) {
        $dir_path = str_replace(".css", "-ltr.css", $item['data']);
      }
      // If the file exists, add the file with the LTR overrides.
      if (isset($dir_path) && file_exists($dir_path) && !isset($css[$dir_path])) {
        // Replicate the same item, but with the LTR path and a little larger
        // weight so that it appears directly after the original CSS file.
        $newitem = $item;
        $newitem['data'] = $dir_path;
        $newitem['weight'] += 0.0001;
        $css[$dir_path] = $newitem;
      }
    }
  }
}


/**
 * Includes all custom style sheets for the current theme.
 */
function sasson_css_include() {

  $dynasty = sasson_theme_dynasty();

  foreach ($dynasty as $theme) {
    $info = drupal_parse_info_file($theme->filename);

    if (isset($info['styles']) && !empty($info['styles'])) {
      foreach ($info['styles'] as $file => $style) {
        $file = drupal_get_path('theme', $theme->name) . "/{$file}";
        $style['options']['theme'] = $theme->name;
        drupal_add_css($file, $style['options']);
      }
    }
  }
}


/**
 * Build the theme tree from base theme to active theme.
 */
function sasson_theme_dynasty() {
  global $theme_key;
  $themes = list_themes();
  $dynasty = array();
  $dynasty[] = $obj = $themes[$theme_key];

  while (isset($obj->base_theme) && isset($themes[$obj->base_theme]) && !empty($themes[$obj->base_theme])) {
    $dynasty[] = $obj = $themes[$obj->base_theme];
  }

  return $dynasty;
}

/**
 * Returns a list of modules
 */
function sasson_get_modules_list($options = FALSE) {
  $modules = $options ? system_list('module_enabled') : module_list();
  asort($modules);

  return $modules;
}

/**
 * Returns a list of assets (js/css)
 */
function sasson_get_assets_list($type = 'css', $filter = array()) {
  $files = array();
  $modules = array();
  $enabled_modules = sasson_get_modules_list(TRUE);

  if (is_array($filter) && count($filter)) {
    $filter = array_combine($filter, $filter);
    $modules = array_intersect_key($enabled_modules, $filter);
  }
  elseif ($filter == 'jquery') {
    $modules['jquery'] = (object) array('filename' => 'misc/jquery.js');
  }
  elseif ($filter == 'sasson') {
    $modules['sasson'] = (object) array('filename' => drupal_get_path('theme', 'sasson') . '/scripts/sasson.js');
  }
  else {
    $modules = $enabled_modules;
  }

  foreach ($modules as $name => $module) {
    $module_path = pathinfo($module->filename, PATHINFO_DIRNAME);
    $module_assets_files = file_scan_directory($module_path, '/.*\.' . $type . '$/');
    foreach ($module_assets_files as $path => $file) {
      $files[$path] = $path;
    }
  }

  return $files;
}
