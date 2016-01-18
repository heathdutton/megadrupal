<?php
/**
 * @file
 * Theme and preprocess functions for panels and panes.
 */

/**
 * Preprocess function for fieldable_panels_pane.
 */
function startuply_preprocess_fieldable_panels_pane(&$vars) {
  $vars['classes_array'][] = 'clearfix';
}
