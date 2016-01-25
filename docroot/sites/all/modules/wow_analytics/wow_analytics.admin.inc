<?php
/**
 * @file
 * Administrative page callbacks for the WOW analytics module.
 */

/**
 * Implements hook_admin_settings().
 */
function wow_analytics_admin_settings_form($form_state) {
  $form['wow_analytics_client_id'] = array(
    '#title' => t('Web Property ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('wow_analytics_client_id', ''),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => TRUE,
    '#description' => t(
      'This ID is unique to each site you want to track separately. <a href="@analytics">Find more information in the documentation</a>.',
      array('@analytics' => 'http://www.wowanalytics.co.uk/documentation')),
  );

  $form['wow_analytics_js_scope'] = array(
    '#type' => 'select',
    '#title' => t('JavaScript scope'),
    '#description' => t('WOW analytics recommends adding the external JavaScript files to the header.'),
    '#options' => array(
      'footer' => t('Footer'),
      'header' => t('Header'),
    ),
    '#default_value' => variable_get('wow_analytics_js_scope', 'header'),
  );

  return system_settings_form($form);
}
