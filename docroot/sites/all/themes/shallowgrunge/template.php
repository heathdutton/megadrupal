<?php
// $Id: 

function shallowgrunge_preprocess(&$variables, $hook) {
  global $user;


  // Call and assign Shallow Grunge theme setting variables
  
  $variables['headcolor'] = theme_get_setting('headcolor');
  $variables['navcolor']  = theme_get_setting('navcolor');
  $variables['headingscolor']  = theme_get_setting('headingscolor');
  $variables['linkcolor']  = theme_get_setting('linkcolor');
  $variables['grunge']  = '/' . path_to_theme() . '/images/' . theme_get_setting('grunge') . '.png';

}
