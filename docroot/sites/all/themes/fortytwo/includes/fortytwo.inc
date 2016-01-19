<?php

/**
 * @file
 * Contains functions needed for the theme.
 */

global $_fortytwo_path_to_core;
$_fortytwo_path_to_core = drupal_get_path('theme', 'fortytwo');

/**
 * Get theme functionalities.
 */
function fortytwo_get_theme() {
  fortytwo_add_libraries();
  fortytwo_add_head_items();
}

/**
 * Get the path to the static files folder in the theme.
 */
function fortytwo_get_staticpath($addprefix = FALSE, $theme_key = 'fortytwo') {
  $ret = '';
  if ($addprefix) {
    $ret = '/';
  }
  $ret .= drupal_get_path('theme', $theme_key) . '/static/';

  return $ret;
}

/**
 * Add libraries to the theme.
 */
function fortytwo_add_libraries() {
  $staticpath = fortytwo_get_staticpath();

  $lodash = theme_get_setting('ft_lodash_js');
  if (!empty($lodash)) {
    drupal_add_js($staticpath . 'js/lib/lodash.js', array('every_page' => TRUE));
  }

  $enquire = theme_get_setting('ft_enquire_js');
  if (!empty($enquire)) {
    drupal_add_js($staticpath . 'js/lib/matchMedia.js', array('every_page' => TRUE));
    drupal_add_js($staticpath . 'js/lib/matchMedia.addListener.js', array('every_page' => TRUE));
    drupal_add_js($staticpath . 'js/lib/enquire.js', array('every_page' => TRUE));
  }

  $transit = theme_get_setting('ft_transit_js');
  if (!empty($transit)) {
    drupal_add_js($staticpath . 'js/lib/transit.js', array('every_page' => TRUE));
  }

  $placeholder = theme_get_setting('ft_placeholder_js');
  if (!empty($placeholder)) {
    drupal_add_js($staticpath . 'js/lib/polyfills/placeholder.js', array('every_page' => TRUE));
  }

  $selectbox = theme_get_setting('ft_prettify_js');
  if (!empty($selectbox)) {
    drupal_add_js(array(
      'fortytwo' => array(
        'prettify_hide_upload' => theme_get_setting('ft_prettify_js_hide_upload'),
        'prettify_add_remove' => theme_get_setting('ft_prettify_js_add_remove'),
      ),
    ), 'setting');
    drupal_add_js($staticpath . 'js/plugins/prettify.js', array('every_page' => TRUE));
  }
}

/**
 * Add meta tags to the html header.
 */
function fortytwo_add_head_items() {
  $viewport = theme_get_setting('ft_viewport');
  if (!empty($viewport)) {
    $user_scalable = theme_get_setting('ft_viewport_user_scaleable');
    $initial_scale = theme_get_setting('ft_viewport_initial_scale');
    $minimum_scale = theme_get_setting('ft_viewport_minimum_scale');
    $maximum_scale = theme_get_setting('ft_viewport_maximum_scale');
    $content = "width=device-width, initial-scale=" . $initial_scale . ", minimum-scale=" . $minimum_scale . ", maximum-scale=" . $maximum_scale . ", user-scalable=" . $user_scalable;
    $element = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' => $content,
      ),
    );
    drupal_add_html_head($element, 'ft-viewport');
  }

  $apple = theme_get_setting('ft_apple_touch');
  if (!empty($apple)) {
    fortytwo_apple_icons();
  }

  $cleartype = theme_get_setting('ft_cleartype');
  if (!empty($cleartype)) {
    $meta = array(
      '#tag' => 'meta',
      '#attributes' => array('http-equiv' => 'cleartype', 'content' => 'on'),
    );

    drupal_add_html_head($meta, 'ft-cleartype');
  }
}

/**
 * Load debugging helper files.
 */
function fortytwo_load_debuggers() {
  $debug = FALSE;
  // Load window size bookmarklet.
  if (theme_get_setting('ft_show_window_size') === 1) {
    $debug = TRUE;
    drupal_add_js(fortytwo_get_staticpath() . 'js/plugins/window-size.js', array(
        'type' => 'file',
        'scope' => 'footer',
        'group' => JS_THEME,
        'preprocess' => TRUE,
        'cache' => TRUE,
      )
    );
  }
  if (theme_get_setting('ft_show_grid') === 1) {
    $debug = TRUE;
    drupal_add_js(fortytwo_get_staticpath() . 'js/plugins/gridbackgroundview.js', array(
        'type' => 'file',
        'group' => JS_THEME,
        'preprocess' => TRUE,
        'cache' => TRUE,
      )
    );
  }
  if ($debug) {
    drupal_set_message(t('Please turn off Forty Two debuggers on production websites.'), 'warning');
  }
}

/**
 * Add apple touch icons.
 */
function fortytwo_apple_icons() {
  // The regex finds all files following certain naming conventions.
  $mask = '/^apple-touch-icon(-precomposed)?(-([0-9]+x[0-9]+))?\.png$/';

  // Loop over all themes in the trail in reverse (starting with the current
  // theme) and use the touch icons of the first theme we find. Note: There
  // is no inheritance, meaning that as soon as a theme includes a touch icon
  // any potential touch icons from the base theme are ignored entirely.
  foreach (array_reverse(fortytwo_theme_trail()) as $theme => $name) {
    $path = drupal_get_path('theme', $theme) . '/static/images';

    // Apple touch icons have to be in the root directory of the theme for
    // them to be discoverable.
    if ($files = file_scan_directory($path, $mask, array('recurse' => FALSE))) {
      foreach ($files as $file) {
        $matches = array();

        // Run the filename through the regex once more picking up the
        // sub-matches in order to find out the dimensions of the touch icon
        // and whether it's preprocessed or not.
        preg_match($mask, $file->filename, $matches);

        // Set the html headers.
        $element = array(
          '#tag' => 'link',
          '#attributes' => array(
            'rel' => 'apple-touch-icon',
            'href' => url($path . '/' . $file->filename),
          ),
        );
        $key = 'ft-apple-touch-icon';
        if (isset($matches[3])) {
          $element['#attributes']['#sizes'] = $matches[3];
          $key .= '-' . $matches[3];
        }
        drupal_add_html_head($element, $key);
      }

      // Break out of the loop because we found at least one touch icon.
      break;
    }
  }
}

/**
 * Builds the full theme trail (deepest theme first, subtheme last) for a theme.
 *
 * @param string $theme
 *   (Optional) The key (machine-readable name) of a theme. Defaults to the key
 *   of the current theme.
 *
 * @return array
 *   An array of all themes in the trail, keyed by theme key.
 */
function fortytwo_theme_trail($theme = NULL) {
  $theme = isset($theme) ? $theme : $GLOBALS['theme_key'];

  if (($cache = &drupal_static(__FUNCTION__)) && isset($cache[$theme])) {
    return $cache[$theme];
  }

  $cache[$theme] = array();

  if ($theme == $GLOBALS['theme'] && isset($GLOBALS['theme_info']->base_themes)) {
    $cache[$theme] = $GLOBALS['theme_info']->base_themes;
  }

  $themes = list_themes();
  if (empty($cache[$theme]) && isset($themes[$theme]->info['base theme'])) {
    $cache[$theme] = system_find_base_themes($themes, $theme);
  }

  // Add our current subtheme ($key) to that array.
  $cache[$theme][$theme] = $themes[$theme]->info['name'];

  return $cache[$theme];
}
