<?php
/**
 * @file
 * Theme settings.
 */

/**
 * Implements theme_settings().
 */
function dts_form_system_theme_settings_alter(&$form, &$form_state) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('theme', 'dts') . '/theme-settings.php';

  // Add theme settings here.
  $form['dts_theme_settings'] = array(
    '#title' => t('Theme Settings'),
    '#type' => 'fieldset',
  );
  $form['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumbs Settings'),
    '#weight' => 0,
    );
  $form['breadcrumb']['breadcrumb_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display breadcrumbs'),
    '#default_value' => theme_get_setting('breadcrumb_title'));
   
  // Copyright.
  $copyright = theme_get_setting('copyright');
  $form['dts_theme_settings']['copyright'] = array(
    '#title' => t('Copyright'),
    '#type' => 'text_format',
    '#format' => $copyright['format'],
    '#default_value' => $copyright['value'] ? $copyright['value'] : t('Drupal is a registered trademark of Dries Buytaert.'),
  );

  // Return the additional form widgets.
  return $form;
}
