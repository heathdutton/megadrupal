<?php

/**
 * @file
 * Admin page callbacks for the Clear Cache Remotely module.
 */

/**
 * Clear Cache Remotely administration settings.
 *
 * @return form
 *   Forms for administrator to set configuration options.
 */
function clear_cache_remotely_admin() {

  $form['clear_cache_remotely_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret Key'),
    '#default_value' => variable_get('clear_cache_remotely_key', ''),
    '#size' => 60,
    '#weight' => -10,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#description' => t('The secret key is added to the Clear Cache Remotely URL as a safety measure, for example: http://yourdomain.com/clear_cache_remotely/all/[secret key]'),
  );

  $form['clear_cache_remotely_ips'] = array(
    '#type' => 'textarea',
    '#title' => t('IP Whitelist'),
    '#default_value' => variable_get('clear_cache_remotely_ips', ''),
    '#description' => t('Comma-separated list of IP addresses. You can use wildcards. No spaces. Example:127.0.0.1,127.0.0.2'),
  );
  return system_settings_form($form);
}
