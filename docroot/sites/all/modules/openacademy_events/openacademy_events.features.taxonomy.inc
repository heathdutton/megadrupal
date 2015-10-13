<?php
/**
 * @file
 * openacademy_events.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function openacademy_events_taxonomy_default_vocabularies() {
  return array(
    'openacademy_event_type' => array(
      'name' => 'Event Type',
      'machine_name' => 'openacademy_event_type',
      'description' => 'A categorization of events',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
