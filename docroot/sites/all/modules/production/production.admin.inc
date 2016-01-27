<?php

/**
 * @file
 * Settings.
 */

/**
 * Settings form.
 *
 * @return array
 *   Drupal form.
 */
function production_settings_form() {
  $form['production_hostname'] = array(
    '#type' => 'textfield',
    '#title' => t('Host name'),
    '#default_value' => variable_get('production_hostname', ''),
    '#description' => t('Provide website production version hostname.'),
  );

  return system_settings_form($form);
}
