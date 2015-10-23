<?php

/**
 * @file selenium.inc
 *
 * Main (ctools) plugin file for Remote "selenium" plugin type
 */

$plugin = array(
    'title' => t('Remote Selenium Test'),
    'description' => t('Remote Selenium Test plugin'),
);

function drulenium_remote_selenium_defaults() {
  return array(
      'selenium_remote_file' => '',
  );
}

function drulenium_remote_selenium_fb_settings($options) {

  $form = array();
  $form['selenium_remote_file'] = array(
      '#title' => t('Remote Selenium Test Case URL'),
      '#type' => 'textfield',
      '#description' => t('Put the URL of the selenium test case here'),
      '#default_value' => variable_get('selenium_remote_file', ''),
  );
  $defaults = drulenium_remote_selenium_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }
  return $form;

}

function drulenium_remote_selenium_drupal_settings($options) {
  return array();
}
