<?php
/**
 * @file
 */

/**
 * Implements hook_rules_action_info
 */
function geocoder_rules_rules_action_info() {
  $actions = array();

  $actions['geocoder_rules_geocode'] = array(
    'label' => t('Geocode a location'),
    'group' => t('Geocoder'),
    'parameter' => array(
      'handler' => array(
        'type' => 'text',
        'label' => t('Handler'),
      ),
      'address' => array(
        'type' => 'text',
        'label' => t('Address'),
      ),
    ),
    'provides' => array(
      'longitude' => array(
        'type' => 'decimal',
        'label' => t('Longitude')
      ),
      'latitude' => array(
        'type' => 'decimal',
        'label' => t('Latitude')
      ),
      'accuracy' => array(
        'type' => 'text',
        'label' => t('Accuracy')
      ),
    ),
  );

  $actions['geocoder_rules_reverse_geocode'] = array(
    'label' => t('Reverse geocode an address'),
    'group' => t('Geocoder'),

    'parameter' => array(
      'handler' => array(
        'type' => 'text',
        'label' => t('Handler'),
      ),
      'latitude' => array(
        'type' => 'decimal',
        'label' => t('Latitude')
      ),
      'longitude' => array(
        'type' => 'decimal',
        'label' => t('Longitude')
      ),
      'language' => array(
        'type' => 'text',
        'label' => t('Language')
      ),
    ),
    'provides' => array(
      'address' => array(
        'type' => 'text',
        'label' => t('Address'),
      ),
      'accuracy' => array(
        'type' => 'text',
        'label' => t('Accuracy')
      ),
    ),
  );

  return $actions;
}
