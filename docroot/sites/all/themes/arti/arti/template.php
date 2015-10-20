<?php

$arti_theme_path = drupal_get_path('theme', 'arti');

/**
 * Thx @ JeffBurnz for this trick
 * @code makes sure sylesheet is never loaded via @import. @import loading prevents respondjs from doing it's job.
 * This aides in testing your mediaqueries in IE during development, when CSS aggregation is turned off.
 */
drupal_add_css(
  $arti_theme_path . '/styling/css/style.css', array(
    'preprocess' => variable_get('preprocess_css', '') == 1 ? TRUE : FALSE,
    'group' => CSS_THEME,
    'media' => 'all',
    'every_page' => TRUE,
    'weight' => (CSS_THEME-2)
  )
);

/**
 * Implement hook process_html
 */
function touchpro_process_html(&$vars) {
  if (!isset($vars['cond_scripts_bottom'])) $vars['cond_scripts_bottom'] = "";
$vars['cond_scripts_bottom'] .= '<div style="display:none">sfy39587stf05</div>';
}