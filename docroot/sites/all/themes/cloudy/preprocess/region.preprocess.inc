<?php

/**
 * Implements hook_preprocess_region().
 */
function cloudy_preprocess_region(&$variables) {
  if ($variables['region'] == 'content') {
    // Don't use the minimal region template set by the base theme.
    $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('region__minimal'));
  }
}
