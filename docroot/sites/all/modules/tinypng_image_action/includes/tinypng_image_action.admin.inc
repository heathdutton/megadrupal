<?php
/**
 * @file
 * Admin form for module.
 */

/**
 * TinyPNG Optimize form callback.
 */
function tinypng_image_action_settings_form($form, &$form_state) {
  $form = array();

  $settings = variable_get('tinypng_image_action', array(
    'api_key' => '',
  ));

  $form['tinypng_image_action'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  $form['tinypng_image_action']['api_key'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Yor API key'),
    '#description'      => t('You can get you key <a href="https://tinypng.com/developers">here</a>'),
    '#default_value'    => $settings['api_key'],
    '#size'             => 36,
    '#maxlength'        => 32,
    '#required'         => FALSE,
    '#element_validate' => array('tinypng_image_action_validate_api_key'),
  );

  return system_settings_form($form);
}

/**
 * Validation callback for tinyPNG API key.
 */
function tinypng_image_action_validate_api_key($element, &$form_state, $form) {
  if (strlen($element['#value']) != 32) {
    form_set_error($element['#name'], t('TinyPNG API key must have 32-char length.'));
    return FALSE;
  }
  return TRUE;
}
