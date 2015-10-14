<?php

/**
 * Implements hook_preprocess_page().
 */
function semantic_ui_omega_preprocess_page(&$vars) {
  // Hook into color.module
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
  
  // Move secondary tabs into a separate variable.
  $vars['tabs2'] = array(
    '#theme' => 'menu_local_tasks',
    '#secondary' => $vars['tabs']['#secondary'],
  );
  unset($vars['tabs']['#secondary']);

  if (isset($vars['main_menu'])) {
    $vars['primary_nav'] =  menu_tree('main-menu');
  }
  else {
    $vars['primary_nav'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_nav'] = menu_tree('secondary-menu');
  }
  else {
    $vars['secondary_nav'] = FALSE;
  }
  // Prepare header.
  $site_fields = array();
  if (!empty($vars['site_name'])) {
    $site_fields[] = $vars['site_name'];
  }
  if (!empty($vars['site_slogan'])) {
    $site_fields[] = $vars['site_slogan'];
  }
  $vars['site_title'] = implode(' ', $site_fields);
  if (!empty($site_fields)) {
    $site_fields[0] = '<span>' . $site_fields[0] . '</span>';
  }
  $vars['site_html'] = implode(' ', $site_fields);

  // Set a variable for the site name title and logo alt attributes text.
  $slogan_text = $vars['site_slogan'];
  $site_name_text = $vars['site_name'];
  $vars['site_name_and_slogan'] = $site_name_text . ' ' . $slogan_text;
  // You can use preprocess hooks to modify the variables before they are passed
  // to the theme function or template file.
}
