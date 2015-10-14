<?php

/**
 * @file
 * Admin functionality for cachewarmer_connect module.
 */

/**
 * Page callback for CacheWarmer configuration form.
 */
function cachewarmer_connect_config_form($form, &$form_state) {

  $form = array();

  $form['cachewarmer_connect_project_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Project ID'),
    '#required' => TRUE,
    '#default_value' => variable_get('cachewarmer_connect_project_id', ''),
    '#description' => t('The project ID from your CacheWarmer project. You can find the project ID on your project page on cache-warmer.com'),
  );

  $form['cachewarmer_connect_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#required' => TRUE,
    '#default_value' => variable_get('cachewarmer_connect_secret', ''),
    '#description' => t('The secret for your CacheWarmer project. You can find the secret on your project page on cache-warmer.com'),
  );

  $form['cachewarmer_connect_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable CacheWarmer integration'),
    '#default_value' => variable_get('cachewarmer_connect_status', FALSE),
    '#description' => t('When enabled your CacheWarming job will be called when the Drupal cache is cleared.'),
  );

  return system_settings_form($form);
}
