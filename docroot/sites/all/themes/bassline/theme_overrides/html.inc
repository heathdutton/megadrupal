<?php
/**
 * @file
 * HTML theme overrides.
 */

/**
 * Implements template_preprocess_html().
 */
function bassline_preprocess_html(&$vars) {
  // Add mobile meta tags.
  $mobile_meta = array(
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1, maximum-scale=1',
    ),
  );
  drupal_add_html_head($mobile_meta, 'bassline_mobile_meta');
}
