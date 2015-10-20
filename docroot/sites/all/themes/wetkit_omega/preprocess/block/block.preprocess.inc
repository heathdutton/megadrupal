<?php
/**
 * @file
 * block.preprocess.inc
 */


/**
 * Implements hook_preprocess_block().
 */
function wetkit_omega_preprocess_block(&$variables) {
  // Add support for css classes and ids from panels.
  if (!empty($variables['elements']['#block']->css_class)) {
    $classes = explode(' ', $variables['elements']['#block']->css_class);
    $variables['classes_array'] = array_merge($variables['classes_array'], $classes);
  }
}
