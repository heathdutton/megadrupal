<?php
/**
 * @file
 * Comment theme overrides.
 */

/**
 * Implements template_preprocess_page().
 */
function bassline_preprocess_comment(&$vars) {
  $vars['title_attributes_array']['class'][] = 'comment-title';
}
