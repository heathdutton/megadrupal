<?php
/**
 * @file
 * Outdated Browser module administration page.
 */

/**
 * Outdated Browser module configuration form.
 */
function outdatedbrowser_admin() {
  $form = array();

  $form['outdatedbrowser_compression_type'] = array(
    '#title' => t('Outdated Browser library compression type'),
    '#type' => 'select',
    '#options' => array(
      'minified' => t('Minified (production)'),
      'source' => t('Uncompressed (development version)'),
    ),
    '#default_value' => variable_get('outdatedbrowser_compression_type', 'minified'),
    '#required' => TRUE,
  );

  $form['outdatedbrowser_bgcolor'] = array(
    '#title' => t('Outdated Browser message: background color'),
    '#description' => t('The background color of the displayed browser upgrade advice. Must be a valid hexadecimal CSS color string, such as !hex.', array('!hex' => '#f25648')),
    '#type' => 'textfield',
    '#default_value' => variable_get('outdatedbrowser_bgcolor', '#f25648'),
    '#required' => TRUE,
    '#element_validate' => array('outdatedbrowser_element_validate_colour'),
  );

  $form['outdatedbrowser_color'] = array(
    '#title' => t('Outdated Browser message: font color'),
    '#description' => t('The font color of the displayed browser upgrade advice. Must be a valid hexadecimal CSS color string, such as !hex.', array('!hex' => '#ffffff')),
    '#type' => 'textfield',
    '#default_value' => variable_get('outdatedbrowser_color', '#ffffff'),
    '#element_validate' => array('outdatedbrowser_element_validate_colour'),
    '#required' => TRUE,
  );

  $form['outdatedbrowser_lowerthan'] = array(
    '#title' => t('Outdated Browser message: targeting browser'),
    '#description' => t('The targeting browser of the displayed browser upgrade advice, either a CSS property or an Internet Explorer version.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('outdatedbrowser_lowerthan', 'transform'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Element validation callback: validate hexadecimal colour code.
 */
function outdatedbrowser_element_validate_colour($element, &$form_state, $form) {
  if (!outdatedbrowser_valid_hexadecimal_string($element['#value'])) {
    form_error($element, t('The given value is not a valid hexadecimal colour code.'));
  }
}
