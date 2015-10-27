<?php
/**
 * Process function for navigation pane.
 */
function versatile_process_pane_navigation(&$vars) {
  $vars['main_menu'] = theme('links__system_main_menu', array(
    'links' => menu_main_menu(),
    'attributes' => array('id' => 'main-menu', 'class' => array('clearfix')),
  ));
  $vars['secondary_menu'] = theme('links__system_secondary_menu', array(
    'links' => menu_secondary_menu(),
    'attributes' => array('id' => 'secondary-menu', 'class' => array('clearfix')),
  ));
}

/**
 * Preprocess function for site template layout.
 */
function versatile_preprocess_versatile_site_template(&$vars) {
  $vars['header_outside'] = theme_get_setting('versatile_header_outside');
  $vars['footer_outside'] = theme_get_setting('versatile_footer_outside');
  
  $responsive = theme_get_setting('versatile_responsive');
  if ($responsive) {
    $module_path = drupal_get_path('theme', 'versatile');
    drupal_add_css($module_path . '/css/versatile_w640.css', array('media' => 'screen and (max-width:640px) and (min-width:481px)'));
    drupal_add_css($module_path . '/css/versatile_w480.css', array('media' => 'screen and (max-width:480px)'));
  }
}