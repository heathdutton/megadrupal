<?php
/**
 * @file
 */

/**
 * Settings form.
 */
function headerlet_settings_form() {
  $form['headerlet_banner'] = array(
    '#title' => t('Banner text'),
    '#description' => t('Enter the figlet banner (or other text) you want to output in the HTTP headers.'),
    '#type' => 'textarea',
    '#rows' => 8,
    '#default_value' => variable_get('headerlet_banner', ''),
  );

  $form['headerlet_header'] = array(
    '#title' => t('Header'),
    '#description' => t('Enter the name of the HTTP header to output. This name will be prefixed with <em>X-</em>. Leave blank to use the default.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('headerlet_header', 'Headerlet'),
  );

  return system_settings_form($form);
}
