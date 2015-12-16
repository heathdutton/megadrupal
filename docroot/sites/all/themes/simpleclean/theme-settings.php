<?php
/**
* @file
* Implementation of THEMEHOOK_settings() function.
*
* @param $saved_settings
*   array An array of saved settings for this theme.
* @return
*   array A form array.
*/

function simpleclean_form_system_theme_settings_alter(&$form, $form_state) {
  $form['simpleclean_mission'] = array(
    '#type' => 'textfield',
    '#title' => t('Mission statement'),
    '#default_value' => theme_get_setting('simpleclean_mission'),
    '#size' => 128,
    '#description' => t('Specify the text for the mission statement visable on frontpage. Leave it empty if you dont want a mission statement or if you want to use blocks instead.'),
    '#weight' => -2,
  );
}
