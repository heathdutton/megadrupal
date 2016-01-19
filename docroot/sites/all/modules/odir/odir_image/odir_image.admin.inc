<?php
/**
 * @file
 * Admin form of odir_image module
 */

/**
 * Form builder. Configure odir_image.
 * Invokes odir_image_supported_slideshow_modes().
 *
 * @ingroup forms
 */
function odir_image_admin_settings() {
  $options = array();
  $options = module_invoke_all('odir_image_supported_slideshow_modes');
  $form['odir_image_selected_slideshow_module'] = array(
    '#type' => 'radios',
    '#title' => t('Select a slideshow module for directory based galleries'),
    '#options' => $options,
    '#default_value' => variable_get('odir_image_selected_slideshow_module', 'none'),
  );
  return system_settings_form($form);
}
