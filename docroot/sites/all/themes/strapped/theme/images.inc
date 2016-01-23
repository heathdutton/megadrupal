<?php
/**
 * @file
 * Image related theme functions.
 */


/**
 * @param $variables
 */
function strapped_preprocess_image_style(&$variables) {
  // @todo make this optional
  if (empty($variables['alt'])) {
    $variables['alt'] = drupal_basename($variables['path']);
  }
}

