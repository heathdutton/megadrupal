<?php
/**
 * @file Admin functions of the Premium Access module.
 */


/**
 * Settings options.
 */
function premium_access_admin_settings($form_state) {
  $form = array(
    'premium_access_time' => array(
      '#type' => 'textfield',
      '#title' => t('Length of premium content access.'),
      '#default_value' => variable_get('premium_access_time', 30),
      '#description' => t('How long should access of a piece of content be recognised before requiring a button again. Zero will use the session lifetime. Time in minutes.'),
    ),
    'premium_access_reg_delay_flood_window' => array(
      '#type' => 'textfield',
      '#title' => t('Registration delay flood window'),
      '#size' => 5,
      '#default_value' => variable_get('premium_access_reg_delay_flood_window', 1440),
      '#description' => t('How long should flood records be held against reg delay abuse. Time in minutes.'),
    ),
  );

  return system_settings_form($form);
}
