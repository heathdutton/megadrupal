<?php
/**
 * @file
 * Theme settings for Breadcrump and Social Block. 
 */

/**
 * Implements of hook_form_system_theme_settings_alter().
 */
function katturai_form_system_theme_settings_alter(&$form, &$form_state) {
  $libraries_options = array(
    'bootstrapcdn' => t('Bootstrap CDN'),
    'theme' => t('[current_theme]/libraries/bootstrap'),
  );
  if (module_exists('libraries')) {
    $bootstrap_path = libraries_get_path('bootstrap');
    if (!empty($bootstrap_path)) {
      $libraries_options['libraries'] = libraries_get_path('bootstrap');
    }
  }
  $form['libraries'] = array(
    '#type' => 'fieldset',
    '#title' => t('Libraries'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'bootstrap_source' => array(
      '#type' => 'radios',
      '#title' => t('Load Twitter Bootstrap library from:'),
      '#options' => $libraries_options,
      '#default_value' => theme_get_setting('bootstrap_source'),
    ),
    'bootstrap_version' => array(
      '#type' => 'textfield',
      '#title' => t('Twitter Bootstrap version:'),
      '#default_value' => theme_get_setting('bootstrap_version'),
    ),
  );
  return $form;
}
