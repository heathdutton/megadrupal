<?php


/**
 * HTML preprocessing
 */
function pure_theme_preprocess_html(&$vars) {
  global $theme_key, $user;

  if(theme_get_setting('roundcorners')) {
    $vars['classes_array'][] = 'rnd';
  }

  if(theme_get_setting('pageicons')) {
    $vars['classes_array'][] = 'pi';
  }

}

