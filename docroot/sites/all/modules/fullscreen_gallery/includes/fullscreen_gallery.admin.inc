<?php

/**
 * @file
 * Administrative page callbacks for the fullscreen_gallery module.
 */

/**
 * Fullscreen gallery settings form.
 *
 * Callback function for admin/config/media/fullscreen_gallery page.
 */
function fullscreen_gallery_admin() {
  $form = array();

  // Load css file for configuration form.
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'fullscreen_gallery') . '/css/config_form.css',
  );

  // Right sidebar width value.
  $form['fullscreen_gallery_rs_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of the right side bar'),
    '#default_value' => variable_get('fullscreen_gallery_rs_width', ''),
    '#size' => 4,
    '#maxlength' => 4,
    '#required' => FALSE,
    '#element_validate' => array('element_validate_integer_positive'),
  );

  // Right sidebar width type (px or %).
  $form['fullscreen_gallery_rs_width_type'] = array(
    '#type' => 'select',
    '#title' => '',
    '#default_value' => variable_get('fullscreen_gallery_rs_width_type', 'px'),
    '#description' => t('Right sidebar appears if there is any content on <em>Full screen gallery right sidebar</em> region, and a valid sidebar width is given.'),
    '#options' => array(
      'px' => t('pixels'),
      'pe' => t('percent'),
    ),
  );

  // Checkbox for disabling title display.
  $form['fullscreen_gallery_disable_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable image titles'),
    '#default_value' => variable_get('fullscreen_gallery_disable_titles', FALSE),
    '#description' => t("Hide image titles in fullscreen gallery."),
    '#required' => FALSE,
  );

  return system_settings_form($form);
}
