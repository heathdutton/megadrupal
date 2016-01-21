<?php

/**
 * @file
 * Module admin settings functions for the Pushretail For Ubercart module.
 */

/**
 * Module settings form.
 * @param array $form
 * @param array $form_state
 * @return array 
 */
function uc_pushretail_settings_form($form, &$form_state) {
  $form = array();
  $form['uc_pushretail_site_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => variable_get('uc_pushretail_site_key', ''),
    '#description' => t('To receive push notifications you need to specify api key. You could get it free at pushretail.com'),
  );
  return system_settings_form($form);
}
