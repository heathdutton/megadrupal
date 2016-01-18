<?php
/**
 * @file
 * Provides administrative forms for the Flickrbomb module.
 */

/**
 * Admin settings form for Flickrbomb module.
 */
function flickrbomb_admin_settings_form($form_values) {
  $form['flickrbomb_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr API key'),
    '#description' => t("If you don't already have one you should !apply for a Flickr API key", array('!apply' => l('apply', 'http://www.flickr.com/services/apps/create/apply'))),
    '#default_value' => variable_get('flickrbomb_api_key', FLICKRBOMB_API_KEY),
  );

  return system_settings_form($form);
}
