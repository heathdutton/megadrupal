<?php

/**
 * @file
 * Provides Rules integration for the MaPS Import module.
 */

/**
 * Implements hook_rules_event_info().
 */
function maps_import_rules_event_info() {
  return array(
    'maps_import_mapping_completed' => array(
      'label' => t('Mapping completed'),
      'module' => 'maps_import',
      'group' => 'MaPS Import' ,
      'variables' => array(
        'args' => array(
          'type' => 'array',
          'label' => t('Additional arguments.'),
          'optional' => TRUE,
        ),
      ),
    ),
  );
}
