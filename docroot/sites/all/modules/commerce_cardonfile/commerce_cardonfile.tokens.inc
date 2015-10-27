<?php

/**
 * @file
 * Provides token integration.
 */

/**
 * Implements hook_token_info().
 *
 * Provides commerce-cardonfile-charge-card-response tokens.
 */
function commerce_cardonfile_token_info() {
  $properties = commerce_cardonfile_charge_card_response_property_info_callback();
  // Remove unsupported properties.
  unset($properties['card_chosen']);

  // Translate properties to tokens.
  $tokens = array();
  foreach ($properties as $property_name => $property_info) {
    $name = str_replace('_', '-', $property_name);
    $description = $property_info['label'];
    if (!empty($property_info['description'])) {
      $description = $property_info['description'];
    }

    $tokens['commerce-cardonfile-charge-card-response'][$name] = array(
      'name' => $property_info['label'],
      'description' => $description,
      'type' => str_replace('_', '-', $property_info['type']),
    );
  }

  return array(
    'types' => array(
      'commerce-cardonfile-charge-card-response' => array(
        'name' => t('Charge Card on File Response'),
        'description' => t('Tokens related to the Card on File charge response.'),
        'needs-data' => 'commerce-cardonfile-charge-card-response',
      ),
    ),
    'tokens' => $tokens,
  );
}

/**
 * Implements hook_tokens().
 */
function commerce_cardonfile_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $token_info = array(
    'commerce-cardonfile-charge-card-response' => array(
      'property_type' => 'commerce_cardonfile_charge_card_response',
      'property_callback' => 'commerce_cardonfile_charge_card_response_property_info_callback',
    ),
  );
  // Wrong type or no data provided.
  if (empty($token_info[$type]) || empty($data[$type])) {
    return array();
  }

  // Make a pseudo wrapper for response data.
  $wrapper_info = array();
  if (isset($token_info[$type]['property_callback'])) {
    $property_info = $token_info[$type]['property_callback']();
    $wrapper_info = array('property info' => $property_info);
  }
  $wrapper = new EntityStructureWrapper($token_info[$type]['property_type'], $data[$type], $wrapper_info);

  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  foreach ($tokens as $name => $original) {
    $replacement_value = NULL;
    $property_name = str_replace('-', '_', $name);
    if (isset($wrapper->{$property_name})) {
      $property_value = $wrapper->{$property_name}->value();
      if (!is_object($property_value)) {
        $replacement_value = $property_value;
      }
    }

    if (isset($replacement_value)) {
      $replacements[$original] = $sanitize ? check_plain($replacement_value) : $replacement_value;
    }
  }

  return $replacements;
}
