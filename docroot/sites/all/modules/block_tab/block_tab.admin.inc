<?php
/**
 * @file
 * Admin interface functions.
 */

/**
 * Admin form.
 * 
 * @return array
 */
function block_tab_admin_settings() {
  $regions = system_region_list(variable_get('theme_default', 'garland'));

  $form = array();
  $form['block_tab-regions'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Regions should be tabified'),
    '#options' => $regions,
    '#default_value' => variable_get('block_tab-regions', array()),
  );

  return system_settings_form($form);
}
