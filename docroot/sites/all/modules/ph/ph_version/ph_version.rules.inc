<?php
/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_data_info().
 */
function ph_version_rules_data_info() {
  return array(
    'ph_version' => array(
      'label' => t('version'),
      'wrap' => TRUE,
      'creation callback' => 'ph_version_rules_create_array',
      'property info' => ph_version_data_property_info(),
    ),
  );
}
