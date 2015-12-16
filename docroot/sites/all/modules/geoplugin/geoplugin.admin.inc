<?php

/**
 * @file
 * Admin pages for the geoPlugin module.
 *
 */

/**
 * Provides the main settings form for the geoPlugin module.
 */
function geoplugin_settings_form() {
  $form = array();

  $form['settings'] = array(
    '#title' => t('Basic settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['settings']['geoplugin_js_lib_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable the geoPlugin JavaScript library'),
    '#default_value' => variable_get('geoplugin_js_lib_enabled', FALSE),
    '#description' => t('When this setting is enabled, the geoplugin js script will be attached to every page on the site. Activate it only if you want to use the geoPlugin service via JavaScript calls.'),
  );

  return system_settings_form($form);
}
