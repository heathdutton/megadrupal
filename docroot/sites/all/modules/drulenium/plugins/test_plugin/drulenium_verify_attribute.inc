<?php

/**
 * @file drulenium_verify_attribute.inc
 *
 * Main (ctools) plugin file for "verify_attribute" plugin type
 */

$plugin = array(
    'title' => t('Verify Attribute'),
    'description' => t('Drulenium Verify Attribute plugin'),
);

function drulenium_verify_attribute_defaults() {
  return array(
      'value' => '',
      'locator' => '',
  );
}

function drulenium_verify_attribute_fb_settings($options) {

  $form = array();
  $form['locator'] = array(
      '#type' => 'textfield',
      '#title' => t('Element Locator'),
      '#description' => t('The Locator of the element you want to Verify.')
  );
  $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#description' => t('The Attribute Value you want to Verify.')
  );

  $defaults = drulenium_verify_attribute_defaults();

  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }

  return $form;

}

function drulenium_verify_attribute_drupal_settings($options) {
  return array();
}
