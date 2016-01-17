<?php
/**
 * @file
 *
 *
 *
 * @author Kálmán Hosszu - hosszu.kalman@gmail.com - http://www.kalman-hosszu.com
 */

/**
 * Build ahm_settings_form form.
 *
 * @param array $form_state
 * @return array The created form.
 */
function ahm_settings_form($form_state) {
  $form = array();

  $form['ahm_always_add_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always include JavaScript file to the site.'),
    '#default_value' => variable_get('ahm_always_add_js'),
  );


  return system_settings_form($form);
}