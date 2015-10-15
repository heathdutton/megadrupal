<?php

/**
 * @file
 * Administration page.
 */

/**
 * Configuration form render.
 */
function custom_add_content_admin_settings() {

  $form['custom_add_content_renderer'] = array(
    '#type' => 'select',
    '#title' => t('Menu renderer'),
    '#options' => array(
      0 => t("Drupal's core renderer"),
      1 => t("Module's custom renderer"),
    ),
    '#default_value' => variable_get('custom_add_content_renderer', 1),
    '#description' => t('Select your preferred method'),
  );

  return system_settings_form($form, TRUE);
}
