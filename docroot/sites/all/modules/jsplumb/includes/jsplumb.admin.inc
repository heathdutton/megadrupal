<?php
/**
 * @file
 *
 *
 *
 * @author Kálmán Hosszu - hosszu.kalman@gmail.com
 */

/**
 * Build jsplumb_settings_form form.
 *
 * @param array $form_state
 * @return
 *   Array the created system settings form.
 */
function jsplumb_settings_form($form_state) {
  $form = array();

  $form['jsplumb_always_add_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always include JavaScript file to the site.'),
    '#default_value' => variable_get('jsplumb_always_add_js'),
  );

  return system_settings_form($form);
}