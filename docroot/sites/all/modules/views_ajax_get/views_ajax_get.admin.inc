<?php

/**
 * @file
 * Provide the admin form to configure views_ajax_get.
 */

/**
 * Menu callback for admin/config/system/views_ajax_get.
 */
function views_ajax_get_admin_form() {
  $views = views_get_all_views();
  $form['views_ajax_get_exemptions'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exempt the following views from using GET ajax.'),
    '#description' => t('Use this if you are having issues with certain views using GET ajax.
      (They will revert to using POST).
      Note this will only apply to Views that have ajax enabled.'),
    '#options' => drupal_map_assoc(array_keys($views)),
    '#default_value' => variable_get('views_ajax_get_exemptions', array()),
  );

  return system_settings_form($form);
}
