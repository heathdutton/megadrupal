<?php
/**
 * @file
 * Variable module hook implementations.
 */

/**
 * Implements hook_variable_group_info().
 */
function outdatedbrowser_variable_group_info() {
  $groups['outdatedbrowser'] = array(
    'title' => 'Outdated Browser library',
    'access' => 'administer outdatedbrowser',
  );
  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function outdatedbrowser_variable_info($options) {
  $variables = array();

  $variables['outdatedbrowser_compression_type'] = array(
    'title' => t('Outdated Browser library compression type', array(), $options),
    'type' => 'select',
    'options' => array(
      'minified' => t('Minified (production)'),
      'source' => t('Uncompressed (development version)'),
    ),
    'default' => 'minified',
    'group' => 'outdatedbrowser',
  );

  $variables['outdatedbrowser_bgcolor'] = array(
    'title' => t('Outdated Browser message: background color', array(), $options),
    'type' => 'string',
    'description' => t('The background color of the displayed browser upgrade advice. Must be a valid hexadecimal CSS color string, such as !hex.', array('!hex' => '#f25648'), $options),
    'default' => '#f25648',
    'validate callback' => 'outdatedbrowser_variable_validate_colour',
    'group' => 'outdatedbrowser',
  );

  $variables['outdatedbrowser_color'] = array(
    'title' => t('Outdated Browser message: font color', array(), $options),
    'type' => 'string',
    'description' => t('The font color of the displayed browser upgrade advice. Must be a valid hexadecimal CSS color string, such as !hex.', array('!hex' => '#ffffff'), $options),
    'default' => '#ffffff',
    'validate callback' => 'outdatedbrowser_variable_validate_colour',
    'group' => 'outdatedbrowser',
  );

  $variables['outdatedbrowser_lowerthan'] = array(
    'title' => t('Outdated Browser message: targeting browser', array(), $options),
    'type' => 'string',
    'description' => t('The targeting browser of the displayed browser upgrade advice, either a CSS property or an Internet Explorer version.', array(), $options),
    'default' => 'transform',
    'group' => 'outdatedbrowser',
  );

  return $variables;
}

/**
 * Validate hexadecimal colour code.
 */
function outdatedbrowser_variable_validate_colour($variable) {
  if (!outdatedbrowser_valid_hexadecimal_string($variable['value'])) {
    return t('The given value is not a valid hexadecimal colour code.');
  }
}
