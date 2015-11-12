<?php

/**
 * @file
 * Provides function for form building for baidumap_field_apikey
 */

/**
 *  Form builder for baidumap_field_apikey settings page.
 */
function baidumap_field_apikey_settings_form($form, &$form_state) {
  $form = array();

  $form['baidumap_field_apikey'] = array(
    '#title' => t('Baidu Maps API ak'),
    '#type' => 'textfield',
    '#description' => t('Enter your Baidu Maps API v2.0 ak in this box.'),
    '#default_value' => variable_get('baidumap_field_apikey', ''),
  );

  return system_settings_form($form);
}
