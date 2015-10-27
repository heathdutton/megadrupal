<?php

/**
 * Menu callback; prints a page listing a glossary of Drupal terminology.
 */
function twbs_menu_help_main_alter() {
  $empty_arg = drupal_help_arg();
  $module_info = system_rebuild_module_data();

  $modules = array();
  foreach (module_implements('help', TRUE) as $module) {
    if (module_invoke($module, 'help', "admin/help#$module", $empty_arg)) {
      $modules[$module] = $module_info[$module]->info['name'];
    }
  }
  asort($modules);

  $output = '<h2>' . t('Help topics') . '</h2><p>' . t('Help is available on the following items:') . '</p>';
  $output .= '<ul>';
  foreach ($modules as $module => $name) {
    $output .= '<li>' . l($name, 'admin/help/' . $module) . '</li>';
  }
  $output .= '</ul>';

  return $output;
}
