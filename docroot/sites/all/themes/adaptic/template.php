<?php
/**
 * @file
 * adaptIC's template.php file.
 * File is currently empty, nothing to see here for now.
 */

 /**
  * Implements hook_preprocess_html().
  */
function adaptic_preprocess_html(&$variables) {

  /* Path variables */
  $variables['base_path'] = base_path();
  $variables['adaptic_path'] = drupal_get_path('theme', 'adaptic');

  /* add necessary sidebar classes to the body tag and remove no-sidebars */

  if (isset($variables['page']['sidebar_right'])) {
    if (isset($variables['page']['sidebar_left'])) {
      $sidebar = ' double-sidebar';
    }
    else {
      $sidebar = ' right-sidebar';
    }
  }
  elseif (isset($variables['page']['sidebar_left'])) {
    $sidebar = ' left-sidebar';
  }
  if (!empty($variables['is_admin'])) {
    $variables['classes_array'][] = ' admin';
  }
  if (!empty($sidebar)) {
    $variables['classes_array'] = array_diff($variables['classes_array'], array('no-sidebars'));
    $variables['classes_array'][] = $sidebar;
  }
}
