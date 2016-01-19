<?php
/**
 * @file
 * Fancybox functions
 */

/**
 * Fancybox initialisation function
 */
function janrain_capture_fancybox_init() {
  $js_css = janrain_capture_js_css();

  foreach ($js_css as $type => $files) {
    foreach ($files as $name => $filename) {
      // Find the location of the library file and add it using the appropriate
      // function, i.e. drupal_add_js or drupal_add_css.
      $full_path = janrain_capture_get_library_path($name) . '/' . $filename;
      $drupal_add = 'drupal_add_' . $type;
      $drupal_add($full_path, array('every_page' => TRUE));
    }
  }
}

/**
 * Helper function as a centralized list of JS and CSS files.
 */
function janrain_capture_js_css() {
  $js_css = array(
    'js' => array(
      'fancybox' => 'jquery.fancybox-1.3.4.pack.js',
    ),
    'css' => array(
      'fancybox' => 'jquery.fancybox-1.3.4.css',
    )
  );
  return $js_css;
}

/**
 * Find the location of library JS and CSS files.
 */
function janrain_capture_get_library_path($name) {
  $lib_paths = &drupal_static(__FUNCTION__, array());
  if (!isset($lib_paths[$name])) {
    $cid = 'janrain_capture_libraries';
    $cache = cache_get($cid);

    if (!empty($cache->data)) {
      $lib_paths = $cache->data;
      if (isset($lib_paths[$name])) {
        return $lib_paths[$name];
      }
    }

    // Use Libraries module, if available, to find the correct path.
    if (function_exists('libraries_get_path')) {
      $lib_path = libraries_get_path($name);
      if (!empty($lib_path)) {
        $lib_paths[$name] = $lib_path;
      }
    }

    // If we still haven't found a path, assume it's at sites/all/libraries
    if (!isset($lib_paths[$name])) {
      $lib_paths[$name] = 'sites/all/libraries/' . $name;
    }
    cache_set($cid, $lib_paths);
  }
  return $lib_paths[$name];
}
