<?php

/**
 * @file
 *
 * Administrative functions for flipboardrss integration.
 */

/**
 * Menu callback for flipboardrss admin settings.
 */
function flipboardrss_admin_settings() {
  $form = array();

  $form['ping_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact your hub'),
  );

  $form['ping_set']['ping'] = array(
    '#type' => 'submit',
    '#value' => t('Ping Hub'),
    '#submit' => array('flipboardrss_ping'),
  );

  $form['flipboardrss_set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Publishing Settings'),
  );

  $form['flipboardrss_set']['flipboardrss_hub_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Publishing address to your hub'),
    '#default_value' => variable_get('flipboardrss_hub_url', 'http://your-hub-name.superfeedr.com/'),
    '#required' => TRUE,
    '#description' => t("Enter the publishing URL to your hub."),
  );

  $form['flipboardrss_set']['flipboardrss_feed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of your feed'),
    '#default_value' => variable_get('flipboardrss_feed_url', ''),
    '#required' => TRUE,
    '#description' => t('Enter the URL of the feed you want to publish.'),
  );

  return system_settings_form($form);
}
