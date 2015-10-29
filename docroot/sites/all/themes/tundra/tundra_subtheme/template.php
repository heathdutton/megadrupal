<?php
global $theme, $base_path, $theme_path, $tundra_subtheme_theme_path, $abs_tundra_subtheme_theme_path, $files_path;
/* Store theme paths in php and javascript variables */
$tundra_subtheme_theme_path = drupal_get_path('theme', 'tundra');
$abs_tundra_subtheme_theme_path = $base_path . $tundra_subtheme_theme_path;

/**
 * Load Features
 */
foreach (file_scan_directory($tundra_subtheme_theme_path . '/features', '/controller.inc/i') as $file) {
  require_once($file->uri);
}

/**
 * Color module integration
 */

/**
 * Returns HTML for a theme's color form.
 * Removes the mini-preview
 * @todo rework and implement the fullsize live preview from tundra 1.0
 */
function tundra_subtheme_color_scheme_form($variables) {
  $form = $variables['form'];

  $theme = $form['theme']['#value'];
  $info = $form['info']['#value'];

  $output  = '';
  $output .= '<div class="color-form clearfix">';
  // Color schemes
  $output .= drupal_render($form['scheme']);
  // Palette
  $output .= '<div id="palette" class="clearfix">';
  foreach (element_children($form['palette']) as $name) {
    $output .= drupal_render($form['palette'][$name]);
  }
  $output .= '</div>';
  // Preview
  $output .= drupal_render_children($form);
  // Close the wrapper div.
  $output .= '</div>';

  return $output;
}

/**
 * Override or insert variables into the page template for HTML output.
 */
function tundra_subtheme_process_html(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

/**
 * Override or insert variables into the page template.
 */
function tundra_subtheme_process_page(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}