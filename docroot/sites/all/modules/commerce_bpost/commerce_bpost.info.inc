<?php

/**
 * @file
 * Provides metadata for the commerce_bpost_rate entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_bpost_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_order properties.
  $properties = &$info['commerce_bpost_rate']['properties'];

  $properties['rates'] = array(
    'label' => t('Rates'),
    'description' => t('Rates array keyed by country code.'),
    'getter callback' => 'commerce_bpost_get_properties',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'rates',
  );

  return $info;
}
