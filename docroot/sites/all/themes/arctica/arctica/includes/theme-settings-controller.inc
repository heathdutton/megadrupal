<?php

/**
 * Load Features
 */
foreach (file_scan_directory($arctica_theme_path . '/features', '/controller.inc/i') as $file) {
  require_once($file->uri);
}

/**
 * CSS File Cache
 */
if (!is_file($files_path . '/arctica-cache-' . $theme . '.css')) {
  arctica_css_cache_build($theme);
}

/**
 * Thx @ JeffBurnz for this trick
 * @code makes sure sylesheet is never loaded via @import. @import loading prevents respondjs from doing it's job.
 * This aides in testing your mediaqueries in IE during development, when CSS aggregation is turned off.
 */
if (is_file($files_path . '/arctica-cache-' . $theme . '.css')) {
  drupal_add_css(
    $files_path . '/arctica-cache-' . $theme . '.css', array(
      'preprocess' => variable_get('preprocess_css', '') == 1 ? TRUE : FALSE,
      'group' => CSS_THEME,
      'media' => 'all',
      'every_page' => TRUE,
      'weight' => 100
    )
  );
}

/**
 * @code we 'add' a stylesheet with the name of the core stylesheet to a location
 * where no such file exists, effectively disabling the stylesheet.
 */
$css_overrides = theme_get_setting('css_overrides');
if (is_array($css_overrides)) {
  foreach ($css_overrides as $key => $core_css) {
    if ($core_css) {
      drupal_add_css(str_replace('_', '.', $key));
    }
  }
}