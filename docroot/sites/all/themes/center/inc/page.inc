<?php
/**
 * @file
 * Page template functions.
 */

/**
 * Implements hook_preprocess_page().
 */
function center_preprocess_page(&$vars) {
  $vars['title_attributes_array']['class'][] = 'page-title';
}
