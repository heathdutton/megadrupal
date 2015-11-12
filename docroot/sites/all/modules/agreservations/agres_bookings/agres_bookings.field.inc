<?php

/**
 * @file
 * @todo Fill this in. 
 */

/**
 * Return fields array.
 * 
 * @todo Documentation
 */
function _agres_bookings_fields() {
  $fields = array();
   // Exported field: 'uc_order-uc_order-field_agr_associated_res'
  $fields['uc_order-uc_order-field_agr_associated_res'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_agr_associated_res',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid',
        ),
      ),
      'module' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'agres_category' => 0,
          'agres_rate' => 0,
          'agres_service' => 0,
          'agres_timeframe' => 0,
          'agreservation' => 'agreservation',
          'agreservation_unit_info' => 0,
          'agreservations_unit' => 0,
          'agreservations_unittype' => 0,
          'article' => 0,
          'page' => 0,
          'product' => 0,
          'product_kit' => 0,
          'service_product' => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'node_reference',
    ),
    'field_instance' => array(
      'bundle' => 'uc_order',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'uc_order',
      'field_name' => 'field_agr_associated_res',
      'label' => 'associated reservation',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'node_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'node_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'node_reference_autocomplete',
        'weight' => '10',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('associated reservation');

  return $fields;
}