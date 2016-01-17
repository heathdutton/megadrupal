<?php

/**
 * @file
 * Admin page callbacks for the User-Agent Cache Bypass module.
 */


/**
 * Defines the User-Agent Cache Bypass settings form.
 */
function ua_cache_bypass_settings($form_state) {
  $form = array();

  $form['ua_cache_bypass_crawler_names'] = array(
    '#type' => 'textarea',
    '#title' => t('User Agents'),
    '#description' => t("A list of case-sensitive user-agents, delimited by newlines, that are allowed to bypass the page cache. The full user-agent string is required."),
    '#default_value' => implode("\n", variable_get('ua_cache_bypass_crawler_names', array('MyUserAgent/1.0'))),
    '#required' => TRUE,
  );

  $form['ua_cache_bypass_log_interval'] = array(
    '#type' => 'select',
    '#title' => t('Log Interval'),
    '#description' => t("This is the interval at which cache bypasses via user-agent should be logged to watchdog."),
    '#default_value' => variable_get('ua_cache_bypass_log_interval', 0),
    '#options' => array(
      0 => t('Never'),
      60 => t('1 minute'),
      300 => t('5 minutes'),
      600 => t('10 minutes'),
      900 => t('15 minutes'),
      1800 => t('30 minutes'),
      3600 => t('1 hour'),
      86400 => t('1 day'),
      604800 => t('1 week'),
    ),
    '#required' => TRUE,
  );

  $form['#validate'][] = 'ua_cache_bypass_settings_validate';

  return system_settings_form($form);
}


/**
 * Validation function for ua_cache_bypass_settings().
 */
function ua_cache_bypass_settings_validate($form, &$form_state) {
  // It's handier to store these as an array.
  $form_state['values']['ua_cache_bypass_crawler_names'] = explode("\n", $form_state['values']['ua_cache_bypass_crawler_names']);
}
