<?php

/**
 * @file
 * Administrative page callbacks for the StatHat module.
 */

/**
 * Form constructor for the StatHat admin settings form.
 */
function stathat_admin_settings_form($form, $form_state) {
	$form = array();
  $form['stathat_ezkey'] = array(
    '#type' => 'textfield',
    '#title' => t('StatHat EZ Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('stathat_ezkey'),
    '#description' => t('The ezkey for your StatHat account.'),
  );
  return system_settings_form($form);
}
