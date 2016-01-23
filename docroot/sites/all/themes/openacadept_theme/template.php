<?php

/**
 * @file
 * This file is empty by default because the base theme chain (Alpha & Omega) provides
 * all the basic functionality. However, in case you wish to customize the output that Drupal
 * generates through Alpha & Omega this file is a good place to do so.
 * 
 * Alpha comes with a neat solution for keeping this file as clean as possible while the code
 * for your subtheme grows. Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
*/

/**
 * Implements hook_process_region().
 */
function openacadept_theme_alpha_process_region(&$vars) {
  switch ($vars['elements']['#region']) {
    case 'menu':
      $menu_function = module_exists('i18n_menu') ? 'i18n_menu_translated_tree' : 'menu_tree';
      $vars['main_menu_tree'] = $menu_function(variable_get('menu_main_links_source', 'main-menu'));
      break;
   }
}