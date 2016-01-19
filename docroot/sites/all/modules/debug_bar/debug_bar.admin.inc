<?php

/**
 * @file
 * Administrative pages for Debug bar module.
 */

/**
 * Debug bar settings form.
 */
function debug_bar_settings_form($form, &$form_state) {

  $form['debug_bar_float'] = array(
    '#type' => 'checkbox',
    '#title' => t('Float'),
    '#description' => t('Make bar draggable'),
    '#default_value' => variable_get('debug_bar_float'),
  );
  $form['debug_bar_position'] = array(
    '#type' => 'radios',
    '#title' => t('Position'),
    '#options' => array(
      'top_left' => t('Top left'),
      'top_right' => t('Top right'),
      'bottom_left' => t('Bottom left'),
      'bottom_right' => t('Bottom right'),
    ),
    '#default_value' => variable_get('debug_bar_position', 'bottom_right'),
    '#states' => array(
      'visible' => array(
        'input[name="debug_bar_float"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['debug_bar_appearance'] = array(
    '#type' => 'radios',
    '#title' => t('Appearance'),
    '#options' => array(
      'both' => t('Icons and text'),
      'icons' => t('Icons only'),
      'text' => t('Text only'),
    ),
    '#default_value' => variable_get('debug_bar_appearance', 'both'),
  );

  return system_settings_form($form);
}
