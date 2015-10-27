<?php
// $Id: theme-settings.php,v 1.0.0 2011/05/01 08:18:15 symphonythemes Exp $

/**
 * @file
 * Allow themes to alter the theme-specific settings form.
 *
 * With this hook, themes can alter the theme-specific settings form in any way
 * allowable by Drupal's Forms API, such as adding form elements, changing
 * default values and removing form elements. See the Forms API documentation on
 * api.drupal.org for detailed information.
 *
 * Note that the base theme's form alterations will be run before any sub-theme
 * alterations.
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */ 
 
function conch_form_system_theme_settings_alter(&$form, $form_state) {
  global $base_url;

  // Get theme name from url (admin/.../theme_name)
  $theme_name = arg(count(arg()) - 1);

  // Get default theme settings from .info file
  $theme_data = list_themes();   // get data for all themes
  $defaults = ($theme_name && isset($theme_data[$theme_name]->info['settings'])) ? $theme_data[$theme_name]->info['settings'] : array();

  // Create theme settings form widgets using Forms API
  // ST Fieldset
  $form['st_container'] = array(
    '#type' => 'fieldset',
    '#title' => t('conch theme settings'),
    '#description' => t('Use these settings to enhance the appearance of your SymphonyTheme theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // General Settings
  $form['st_container']['general_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Theme Color
  $form['st_container']['general_settings']['them_color_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Color setting'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['st_container']['general_settings']['them_color_config']['theme_color'] = array(
    '#type' => 'select',
    '#title' => t('Color'),
    '#default_value' => theme_get_setting('theme_color'),
    '#options'  => array(
        'green'     => t('Green'),
        'blue'      => t('Blue'),
        'purple'    => t('Purple'),
        'orange'    => t('Orange'),
        'red'       => t('Red'),
    ),
  );

  // Return theme settings form
  return $form;
}
