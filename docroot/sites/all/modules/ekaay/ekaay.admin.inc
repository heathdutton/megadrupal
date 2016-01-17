<?php

/**
 * @file
 * Provides admin settings for ekaay module.
 */
function ekaay_admin_settings() {
  $form = array();
  $form['ekaay_ekaay_url'] = array(
    '#title' => t('Ekaay path'),
    '#default_value' => variable_get('ekaay_ekaay_url', ''),
    '#type' => 'textfield',
    '#description' => t('The path to the ekaay installation. So something like https://www.meinserver.com/ekaay.'),
  );

  $form = system_settings_form($form);
  $form['#validate'][] = 'ekaay_admin_settings_validate';
  return $form;
}

/**
 * Form validation handler for ekaay_admin_settings().
 */
function ekaay_admin_settings_validate(&$form, &$form_state) {
  // @todo: Check whether the installation of ekaay is done right.
}
