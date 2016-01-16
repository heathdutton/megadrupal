<?php
/**
 * @file
 * Implementation of THEMEHOOK_settings() function.
 *
 * @param $form_settings
 *
 * array An array of saved settings for this theme.
 * @return
 * array A form array.
 */

/**
 * Form system theme settings alter for mission statement.
 */
function acta_html5_form_system_theme_settings_alter(&$form, $form_state) {
  // Function for mission statement.
  $form['acta_html5_mission'] = array(
    '#type' => 'textfield',
    '#title' => t('Mission statement'),
    '#default_value' => theme_get_setting('Acta_Html5_mission'),
    '#size' => 128,
    '#description' => t('Specify the text for the mission statement visable on frontpage. Leave it empty if you dont want a mission statement or if you want to use blocks instead.'),
    '#weight' => -2,
  );
}
