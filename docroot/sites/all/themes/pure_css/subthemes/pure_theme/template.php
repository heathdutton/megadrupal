<?php


/**
 * HTML preprocessing
 */
function pure_theme_preprocess_html(&$vars) {
  global $theme_key, $user;

// Build array of additional body classes and retrieve custom theme settings
  if(theme_get_setting('roundcorners')) {
    $vars['classes_array'][] = 'rnd';
  }
  if(theme_get_setting('pageicons')) {
    $vars['classes_array'][] = 'pi';
  }

}
