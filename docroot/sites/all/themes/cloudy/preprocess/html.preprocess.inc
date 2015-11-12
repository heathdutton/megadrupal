<?php

/**
 * Implements hook_preprocess_html().
 */
function cloudy_preprocess_html(&$variables) {
  drupal_add_js(drupal_get_path('theme', 'omega') . '/js/jquery.matchmedia.js', array('group' => JS_THEME));

  if (render($variables['page']['banner'])) {
    $variables['attributes_array']['class'][] = 'with-banner';
  }

  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['attributes_array']['class'][] = 'two-sidebars';
  }
  elseif (!empty($variables['page']['sidebar_first'])) {
    $variables['attributes_array']['class'][] = 'sidebar-first';
  }
  elseif (!empty($variables['page']['sidebar_second'])) {
    $variables['attributes_array']['class'][] = 'sidebar-second';
  }
  else {
    $variables['attributes_array']['class'][] = 'no-sidebars';
  }
}