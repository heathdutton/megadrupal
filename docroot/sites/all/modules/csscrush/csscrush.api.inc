<?php

/**
 * @file
 * CSS-Crush module API functions.
 */

/**
 * Adds a CSS-Crush preprocessed CSS file to the stylesheet queue.
 *
 * A wrapper for drupal_add_css().
 *
 * @param mixed $data
 *   The stylesheet data to be added. If the data type is a file will accept
 *   filepaths in the following form in addition to regular paths:
 *     - theme://themename/styles.css
 *     - module://modulename/styles.css
 *
 *   See drupal_add_css().
 *
 * @param array $options
 *   (optional) See drupal_add_css().
 *
 * @return array
 *   An array of queued cascading stylesheets.
 *
 * @see drupal_add_css()
 */
function csscrush_add_css($data = NULL, $options = NULL, $csscrush_options = array()) {
  $result = _csscrush_process($data, $options, $csscrush_options);
  return drupal_add_css($result['data'], $result['options']);
}


/**
 * Link library files and prepare output directory.
 *
 * The result is cached.
 *
 * @return bool
 *   TRUE on success. FALSE on failure.
 */
function csscrush_bootstrap($suppress_error_message = FALSE) {

  static $status_bool = NULL;
  if (is_bool($status_bool)) {
    return $status_bool;
  }

  // Link the library.
  // Library may be autoload-able by a tool like composer so testing first.
  if (!_csscrush_available(TRUE)) {
    if ($csscrush_include_path = _csscrush_include_path()) {
      require_once $csscrush_include_path;
    }
    else {
      $status_bool = FALSE;
      if (!$suppress_error_message) {
        drupal_set_message(t('CSS-Crush library files not found'), 'warning');
      }
      return $status_bool;
    }
  }

  // Link the adapter.
  if (csscrush_version()->compare('2.1.0') >= 0) {
    module_load_include('inc', 'csscrush', 'csscrush.adapter');
  }
  else {
    module_load_include('inc', 'csscrush', 'csscrush.adapter-legacy');
  }
  csscrush_set('config', array('io' => 'CssCrushDrupalIo'));

  // Overriding option defaults, and applying options set in admin page.
  csscrush_set('options', array(
    'rewrite_import_urls' => 'absolute',
    'trace' => variable_get('csscrush_trace', FALSE),
    'source_map' => variable_get('csscrush_source_map', FALSE),
  ));

  // Global library settings.
  csscrush_set('config', '_csscrush_config');

  // Create output file directory and check it's writable.
  $dir = CSSCRUSH_OUT_DIR;
  file_prepare_directory($dir, FILE_CREATE_DIRECTORY);

  $status_bool = TRUE;
  return $status_bool;
}


/**
 * Low level handler for preprocessing files and strings.
 *
 * @see csscrush_add_css()
 */
function _csscrush_process($data = NULL, $options = NULL, $csscrush_options = array()) {

  // Accept a string for type.
  if (is_null($options)) {
    $options = array('type' => 'file');
  }
  elseif (!is_array($options)) {
    $options = array('type' => $options);
  }
  $type = isset($options['type']) ? $options['type'] : 'file';

  // Resolve pseudo stream paths.
  $pseudo_stream = NULL;
  if ($type === 'file' && preg_match('~^(theme|module)://([\w-]+)/~', $data, $m)) {
    list($full_match, $pseudo_stream, $base) = $m;
    $path = drupal_get_path($pseudo_stream, $base);
    $data = str_replace($full_match, "$path/", $data);
  }

  if (!csscrush_bootstrap()) {
    return array('data' => $data, 'options' => $options);
  }

  // Resolve csscrush options.
  $csscrush_options += array(
    'cache' => TRUE,
    'debug' => FALSE,
  );

  // These options must be set this way for things to work correctly in drupal.
  $csscrush_options['versioning'] = FALSE;
  $csscrush_options['rewrite_import_urls'] = 'absolute';

  if ($type === 'file') {

    // Set some defaults if pseudo streams are used.
    if ($pseudo_stream) {

      // If group is not explicitly set, set it by the pseudo stream.
      if (!isset($options['group'])) {
        switch ($pseudo_stream) {
          case 'theme':
            $options['group'] = CSS_THEME;
            break;

          case 'module':
            $options['group'] = CSS_DEFAULT;
            break;
        }
      }
    }

    // Create the file.
    $created_file = csscrush_file(DRUPAL_ROOT . '/' . $data, $csscrush_options);
    $options['csscrush_processed'] = TRUE;

    // Since the operation is expensive set file to display on every page by
    // default, allow override by passing in the option manually.
    if (!isset($options['every_page'])) {
      $options['every_page'] = TRUE;
    }

    $data = CSSCRUSH_OUT_DIR . '/' . basename($created_file);
  }
  elseif ($type === 'inline') {

    $data = csscrush_string($data, $csscrush_options);
    $options['csscrush_processed'] = TRUE;
  }

  return array('data' => $data, 'options' => $options);
}


/**
 * Library settings callback.
 *
 * @param object $config
 *   The library config object.
 */
function _csscrush_config($config) {
  // Adding drupal specific global variables.
  // For performance reasons only variables that rarely change should be
  // applied globally.
  $config->vars += array(
    'base-path' => $GLOBALS['base_path'],
  );
}


/**
 * Determine whether the library is available.
 *
 * @param bool $autoload
 *   (optional) Whether to attempt autoloading if library is not found.
 *
 * @return bool
 *   TRUE on success. FALSE on failure.
 */
function _csscrush_available($autoload = FALSE) {

  $available = class_exists('CssCrush', $autoload);
  if (version_compare(PHP_VERSION, '5.3.0', '>=') && !$available) {
    $available = class_exists('CssCrush\CssCrush', $autoload);
  }

  return $available;
}


/**
 * Finds a path to the library seed file.
 *
 * The result is cached.
 *
 * @return string
 *   Path to the library seed file or null on failure.
 */
function _csscrush_include_path() {

  static $csscrush_include_path = NULL;
  if ($csscrush_include_path) {
    return $csscrush_include_path;
  }

  $target_file = 'CssCrush.php';

  // Create a stack of search paths, first to last.
  $search_paths = array();

  // First try the libraries API.
  if (module_exists('libraries') && ($csscrush_library_path = libraries_get_path('csscrush'))) {
    $search_paths[] = "$csscrush_library_path/$target_file";
  }
  // Then common locations.
  $search_paths[] = "sites/all/libraries/CssCrush/$target_file";
  $search_paths[] = "sites/all/libraries/csscrush/$target_file";

  // Run search.
  foreach ($search_paths as $search) {
    if (file_exists($search)) {
      $csscrush_include_path = $search;
      break;
    }
  }

  return $csscrush_include_path;
}
