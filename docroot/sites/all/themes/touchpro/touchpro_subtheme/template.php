<?php

global $theme_path;

/**
 * If color module is not enabled we omit the color_module.css file that was
 * registired in the .info file. Color module required that stylesheets it acts
 * on a registered in the .info file.
 */

if (!module_exists('color')) {
  drupal_add_css($theme_path . 'color/color_module.css');
} else {
  /**
   * @Code
   * In every configuration the color_module.css file must load after style.css
   * so it needs the same logical add_css with a higher weight
   */
  drupal_add_css(
    $theme_path . '/color/color_module.css', array(
      'preprocess' => variable_get('preprocess_css', '') == 1 ? TRUE : FALSE,
      'group' => CSS_THEME,
      'media' => 'all',
      'every_page' => TRUE,
      'weight' => (CSS_THEME+1)
    )
  );
}

/**
 * Override or insert variables into the page template for HTML output.
 */
function touchpro_subtheme_process_html(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

/**
 * Override or insert variables into the page template.
 */
function touchpro_subtheme_process_page(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}