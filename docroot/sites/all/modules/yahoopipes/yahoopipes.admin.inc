<?php
// $Id: yahoopipes.admin.inc,v 1.1 2009/04/03 16:42:12 robloach Exp $

/**
 * @file
 * Provides the administration settings for the Yahoo Pipes Drupal module.
 */

/**
 * The general Yahoo! Pipes administration page.
 */
function yahoopipes_admin_form() {
  $form = array();
  $form['yahoopipes_cache_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache Duration'),
    '#size' => 4,
    '#description' => t('An amount of time, in minutes, describing how long pipe information should be cached before a new update is made. Leaving blank will store until next cache dump.'),
    '#default_value' => variable_get('yahoopipes_cache_time', 10),
  );
  $form['yahoopipes_curl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use cURL'),
    '#description' => t('When enabled, will use <a href="!curl">cURL</a> to make the HTTP request instead of <a href="!drupal_http_request">drupal_http_request</a>. If unsure, leave unchecked.', array('!drupal_http_request' => 'http://api.drupal.org/api/function/drupal_http_request', '!curl' => 'http://php.net/curl')),
    '#default_value' => variable_get('yahoopipes_curl', FALSE),
  );
  return system_settings_form($form);
}
