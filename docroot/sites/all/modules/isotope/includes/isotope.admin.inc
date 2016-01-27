<?php
/**
 * @file
 * The admin form include file
 *
 * Defines the FAPI object and renders the system settings form for
 * the History.js Library Module
 */

/**
 * Implements hook_admin().
 */
function isotope_admin() {
  $form = array();
  $form['isotope_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add CSS3 transitions for Isotope items'),
    '#description' => t('Add default transition CSS for Isotope items.'),
    '#default_value' => variable_get('isotope_css', TRUE),
  );
  $form['isotope_libraries'] = array(
    '#type'  => 'checkboxes',
    '#title' => t('Select additional libraries to include.'),
    '#description' => t('Adds the selected .js libraries to your theme if the .js files are located in the sites/&lt;yoursitenamehere&gt;/libraries/isotope/js folder.'),
    '#options' => array(
      'jquery-1.7.1'          => t('Include jQuery thats packaged with isotope (Minimum Version 1.7.1 recommended)'),
      'jquery.ba-bbq'         => t('Include jQuery BBQ'),
      'jquery.infinitescroll' => t('Include Infinite Scroll'),
    ),
    '#default_value' => variable_get('isotope_libraries', array()),
  );

  return system_settings_form($form);
}
