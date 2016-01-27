<?php
/**
 * @file
 * Acta HTML5 template file.
 */

/**
 * Add conditional CSS for IE6.
 */
function acta_html5_preprocess_html(&$vars) {
  drupal_add_css(path_to_theme() . '/ie6.css', array(
  'group' => CSS_THEME,
  'browsers' => array('IE' => 'lt IE 7', '!IE' => FALSE),
  'preprocess' => FALSE));
}
