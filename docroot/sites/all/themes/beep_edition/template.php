<?php
/*
  Preprocess
*/


/**
 * Implements theme_menu_local_tasks().
 */
// Add button-group class to tabs so they can be styled better
function beep_edition_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="button-group">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="button-group secondary">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

// Remove Height and Width Inline Styles from Drupal Images
function rwtio_preprocess_image(&$variables) {
  foreach (array('width', 'height') as $key) {
    unset($variables[$key]);
  }
}