<?php
/**
 * @file
 * cm_show_airings.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function cm_show_airings_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'airing-airing-field_airing_show_ref'
  $field_instances['airing-airing-field_airing_show_ref'] = array(
    'bundle' => 'airing',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 18,
      ),
    ),
    'entity_type' => 'airing',
    'field_name' => 'field_airing_show_ref',
    'label' => 'Show',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 6,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Show');

  return $field_instances;
}
