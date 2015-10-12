<?php
/**
 * @file
 * 
 * 
 *
 * @author Kálmán Hosszu - hosszu.kalman@gmail.com - http://www.kalman-hosszu.com
 */

function theme_imagelens_formatter($variables) {
  drupal_add_library('imagelens', 'imagelens');
  drupal_add_js(drupal_get_path('module', 'imagelens') . '/imagelens.js');
  $item = $variables['item'];
  $image = array(
    'path' => $item['uri'],
    'alt' => $item['alt'],
  );
  
  // Do not output an empty 'title' attribute.
  if (drupal_strlen($item['title']) > 0) {
    $image['title'] = $item['title'];
  }
  
  if ($variables['imagelens_style']) {
    $image['attributes'] = array(
      'class' => 'imagelens',
    );
    if (!empty($variables['imagelens_style']['height'])) {
      $image['attributes']['height'] = $variables['imagelens_style']['height'];
    }
    if (!empty($variables['imagelens_style']['width'])) {
      $image['attributes']['width'] = $variables['imagelens_style']['width'];
    }
  }
  
  return theme('image', $image);
}