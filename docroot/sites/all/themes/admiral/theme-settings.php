<?php
/**
 * @file
 * Theme settings.
 */

/**
 * Implements theme_settings().
 */
function admiral_form_system_theme_settings_alter(&$form, &$form_state) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('theme', 'admiral') . '/theme-settings.php';

  // Add theme settings here.
  $form['admiral_theme_settings'] = array(
    '#title' => t('Theme Settings'),
    '#type' => 'fieldset',
  );

  $menus =  menu_get_menus();

  $selected_menu = theme_get_setting('admiral_admin_menu');

  $form['admiral_theme_settings']['admiral_admin_menu'] = array(
    '#title' => t('Admin Menu'),
    '#description' => t('Select a menu to show as Admin Menu'),
    '#type' => 'select',
    '#options' => menu_get_menus(),
    '#default_value' => $selected_menu ? $selected_menu : 'management',
  );

  // Copyright.
  $copyright = theme_get_setting('copyright');
  $form['admiral_theme_settings']['copyright'] = array(
    '#title' => t('Copyright'),
    '#type' => 'text_format',
    '#format' => $copyright['format'],
    '#default_value' => $copyright['value'] ? $copyright['value'] : t('Drupal is a registered trademark of Dries Buytaert.'),
  );

  // Return the additional form widgets.
  return $form;
}
