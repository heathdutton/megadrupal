<?php

/**
 * @file
 * Evanced Registration admin pages
 */


/**
 * Admin settings form
 */
function evanced_registration_settings() {
  $form = array();
  
  $form['evanced_registration_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Evanced URL'),
    '#description' => t('Include the full URL minus the individual .asp page name.<br />e.g., http://host1.evanced.info/library/evanced/'),
    '#default_value' => variable_get('evanced_registration_url'),
    '#required' => TRUE
  );
  $form['evanced_registration_assistance_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Web address for assistance with events'),
    '#description' => t('If errors occur during registration, the messages displayed to the customers/patrons will contain this web address to find more help.'),
    '#default_value' => variable_get('evanced_registration_assistance_url')
  );

  $form = system_settings_form($form);
  return $form;
}
