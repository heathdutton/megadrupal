<?php

/**
 * @file
 * Functionality for Grid field formatter administration page.
 */

/**
 * Menu callback: configuration of Grid Field Formatter field types.
 */
function grid_field_formatter_settings($form, &$form_state) {

  $form['#prefix'] = t('Configure the Grid Field Formatter settings below:');
  $form['grid_field_formatter_field_types'] = array(
    '#title' => t('Please select the field type for which Grid Field Formatter should be available.'),
    '#type' => 'checkboxes',
    '#options' => grid_field_formatter_field_types_options_list(),
    '#default_value' => variable_get('grid_field_formatter_field_types', array()),
  );

  $form['#submit'][] = 'grid_field_formatter_settings_submit';

  return system_settings_form($form);
}

/**
 * Flush field cache.
 */
function grid_field_formatter_settings_submit($form, &$form_state) {
  // Flush field cache.
  field_cache_clear();
}
