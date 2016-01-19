<?php
/**
 * @file
 * Hooks and preprocess functions for the Slick views module.
 */

/**
 * Implements hook_preprocess_slick_views().
 */
function template_preprocess_slick_views(&$variables) {
  $settings = $variables['options'];

  // Prepare the Slick wrappers for asNavFor.
  $variables['wrapper_prefix'] = '';
  $variables['wrapper_suffix'] = '';

  // Build the Slick wrappers for asNavFor.
  if (count($variables['rows']) > 1) {
    // Mimick the Slick wrapper attributes.
    $attributes['class'] = array('slick-wrapper', 'slick-wrapper--asnavfor');
    if ($settings['skin'] && $settings['skin'] != 'asnavfor') {
      $attributes['class'][] = drupal_clean_css_identifier('slick-wrapper--' . $settings['skin']);
    }

    $variables['wrapper_prefix'] = '<div' . drupal_attributes($attributes) . '>';
    $variables['wrapper_suffix'] = '</div>';
  }
}
