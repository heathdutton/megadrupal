<?php
/**
 * @file theme.inc
 * Prepares theme variables for the templates.
 */

/**
 * Implements template_preprocess_THEME_ELEMENT();
 * @param $vars
 *   array of theme variables
 */
function template_preprocess_wiredocs_applet(&$vars) {
  global $user, $is_https;
  $file_path = drupal_get_path('module', 'wiredocs');
  if ($vars['file'] && $vars['file']->fid) {
    $vars['archiveJar'] = url('wiredocs/jar/wiredocs-1.0', array('absolute' => TRUE));
    
    $vars['downloadURL'] = url('wiredocs/' . $vars['file']->fid . '/download', array('absolute' => TRUE));
    $vars['uploadURL'] = url('wiredocs/' . $vars['file']->fid . '/upload', array('absolute' => TRUE));
    
    $vars['cookie'] = session_name() . '=' . ($is_https ? $user->ssid : $user->sid);
  }
}