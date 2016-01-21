<?php

/**
 * @file
 * Interacts with the Advanced Theme Settings module.
 */

function forest_floor_settings($saved_settings) {
  // Set the default values for the theme variables
  $defaults = array(
    'forest_floor_width' => '800',
  );

  // Merge the variables and their default values
  $settings = array_merge($defaults, $saved_settings);

  // Layout Type
  $form['forest_floor_width'] = array(
    '#type' => 'select',
    '#title' => t('Fixed Width'),
    '#default_value' => $settings['forest_floor_width'],
    '#options' => array(
      '800' => t('800px'),
      '1024' => t('1024px'),
    ),
  );

  return $form;

}
