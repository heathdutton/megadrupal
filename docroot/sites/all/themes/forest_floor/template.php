<?php

/**
 * @file
 * Initialises theme settings and adds body tags to aid theming.
 */

/**
 * Initialise theme settings
 */
if (is_null(theme_get_setting('forest_floor_width'))) {
  global $theme_key;
  $defaults = array(
    'forest_floor_width' => '800',
  );

  $settings = theme_get_settings($theme_key);
  if (module_exists('node')) {
    foreach (node_get_types() as $type => $name) {
      unset($settings['toggle_node_info_' . $type]);
    }
  }
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, $settings)
  );
  theme_get_setting('', TRUE);
}

/**
 * Sets the body tag class and id attributes.
 *
 * @param $is_front
 *   boolean Whether or not the current page is the front page.
 * @param $layout
 *   string Which sidebars are being displayed.
 * @return
 *   string The rendered id and class attributes.
 */
function phptemplate_body_attributes($is_front = false, $layout = 'none') {

  if ($is_front) {
    $body_id = $body_class = 'homepage';
  }
  else {
    global $base_path;
    list(,$path) = explode($base_path, $_SERVER['REQUEST_URI'], 2);
    list($path,) = explode('?', $path, 2);
    $path = rtrim($path, '/');
    $body_id = str_replace('/', '-', $path);
    list($body_class,) = explode('/', $path, 2);
  }
  $body_id = 'page-'. $body_id;
  $body_class = 'section-'. $body_class;

  $sidebar_class = ($layout == 'both') ? 'sidebars' : "sidebar-$layout";

  return " id=\"$body_id\" class=\"$body_class $sidebar_class\"";
}

/**
 * Adds the 1024.css stylesheet if enabled with Advanced Theme Settings.
 */
if (theme_get_setting('forest_floor_width') == '1024') {
  drupal_add_css(drupal_get_path('theme', 'forest_floor')  .'/1024.css', 'theme');
}
