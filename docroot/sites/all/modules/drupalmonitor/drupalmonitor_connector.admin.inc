<?php
/**
 * @file
 * Administrative functionality for the DrupalMonitor Connector.
 */

/**
 * Implements hook_form().
 */
function drupalmonitor_connector_settings_form() {
  $form['drupalmonitor_hash'] = array(
    '#type' => 'textfield',
    '#title' => t('Security Hash (Password)'),
    '#default_value' => variable_get('drupalmonitor_hash'),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Set the "password" to protect access to !url1. This security hash should be the same string as in your site settings on !url2.',
     array(
       '!url1' => l(t('/drupalmonitor'), 'drupalmonitor', array('query' => array('hash' => variable_get('drupalmonitor_hash')))),
       '!url2' => l(t('drupalmonitor.com/websites'), "http://www.drupalmonitor.com/websites"),
     )),
  );

  $form['drupalmonitor_performance_monitoring'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable performance monitoring'),
    '#default_value' => variable_get('drupalmonitor_performance_monitoring', 1),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('If enabled, the drupalmonitor.com module will log all page requests on your Drupal site. Logs are needed to generate load graphs on !url. On high traffic websites, logging can be time consuming and slow your website.', array('!url' => l(t('drupalmonitor.com/websites'), "http://www.drupalmonitor.com/websites"))),
  );

  $form['drupalmonitor_server_monitoring'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable server data'),
    '#default_value' => variable_get('drupalmonitor_server_monitoring', 1),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('If enabled, the drupalmonitor.com module will monitor server statistics.'),
  );

  $form['drupalmonitor_user_monitoring'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable user data'),
    '#default_value' => variable_get('drupalmonitor_user_monitoring', 1),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('If enabled, the drupalmonitor.com module will monitor user activity.'),
  );

  $form['drupalmonitor_files_monitoring'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable disk usage data'),
    '#default_value' => variable_get('drupalmonitor_files_monitoring', 1),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('If enabled, the drupalmonitor.com module will monitor user disk usage.'),
  );

  $form['drupalmonitor_node_monitoring'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable content data'),
    '#default_value' => variable_get('drupalmonitor_node_monitoring', 1),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('If enabled, the drupalmonitor.com module will monitor content activity.'),
  );

  $form['drupalmonitor_modules_monitoring'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable module update status'),
    '#default_value' => variable_get('drupalmonitor_modules_monitoring', 1),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('If enabled, the drupalmonitor.com module will monitor module updates.'),
  );

  $form['drupalmonitor_variables_monitoring'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable variables monitoring'),
    '#default_value' => variable_get('drupalmonitor_variables_monitoring', 1),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('If enabled, the drupalmonitor.com module will monitor variables and Drupal settings.'),
  );

  $form['drupalmonitor_custom_monitoring'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable custom metrics'),
    '#default_value' => variable_get('drupalmonitor_custom_monitoring', 1),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('If enabled, the drupalmonitor.com module will run hook_drupalmonitor() to monitor custom metrics.'),
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_validate().
 */
function drupalmonitor_connector_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['drupalmonitor_hash'] == '') {
    form_set_error('drupalmonitor_hash', t('You must set a security hash.'));
  }
}
