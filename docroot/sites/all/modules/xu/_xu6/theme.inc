<?php
/**
 * @file
 * Useful functions for themeing
 */

$plugin = array (
  'title' => t('Theme'),
  'description' => t('Library for working with the Theme API.'),
  'xu_type' => 'library'
);

/**
 * Add inline CSS to the HEAD
 *
 * @param $style
 */
function xu_add_css($style) {
  drupal_set_html_head('<style type="text/css">' . $style . '</style>');
}
