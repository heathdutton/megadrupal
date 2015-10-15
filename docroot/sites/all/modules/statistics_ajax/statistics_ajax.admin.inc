<?php
/**
 * @file
 * Provide all the administration pages
 */

/**
 * Implements hook_settings().
 */
function statistics_ajax_settings($form, &$form_state) {
  $form = array();
  $form['statistics_ajax_limit_requests_xmlhttprequest'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow ONLY xmlhttprequest method (AJAX)'),
    '#default_value' => variable_get('statistics_ajax_limit_requests_xmlhttprequest', 1),
    '#description' => t('Restricts the node counter statistics update to
       AJAX xmlhttprequests (through javascript)'),
  );
  $form['statistics_ajax_limit_requests_get'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow HTTP GET method'),
    '#default_value' => variable_get('statistics_ajax_limit_requests_get', 1),
    '#description' => t('Allows HTTP GET requests to update the node counter
      statistics'),
  );
  $form['statistics_ajax_limit_requests_post'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow HTTP POST method'),
    '#default_value' => variable_get('statistics_ajax_limit_requests_post', 1),
    '#description' => t('Allows HTTP POST requests to update the node counter
      statistics'),
  );
  return system_settings_form($form);
}
