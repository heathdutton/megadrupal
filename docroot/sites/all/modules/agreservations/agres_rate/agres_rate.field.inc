<?php

/**
 * Helper function
 *
 * @todo Documentation.
 *
 * @return array
 */
function _agres_rate_contenttypes() {
  $types = array(
    array(
      'type' => 'agres_rate',
      'name' => t('Rate'),
      'base' => 'node_content',
      'description' => t("A Rate overriding a Unit Types Product Price if the attached Timeframe applies."),
      'custom' => 1,
      'modified' => 1,
      'locked' => 0,
    ),
  );
  return $types;
}

/**
 * return fields array.
 */

/**
 * Helper function that returns the fields array.
 *
 * @return array The fields array.
 */
function _agres_rate_fields() {
  $fields = array();
  // Exported field: 'node-agreservation-field_agres_ref_unit'
  $fields['node-agreservation_unittype-field_agres_ref_rate'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_agres_ref_rate',
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
          '0' => 'nid',
        ),
      ),
      'module' => 'node_reference',
      'settings' => array(
        'field_permissions' => array(
          'create' => 0,
          'edit' => 0,
          'edit own' => 0,
          'view' => 0,
          'view own' => 0,
        ),
        'referenceable_types' => array(
          'agres_rate' => 'agres_rate',
          'agres_category' => 0,
          'agres_timeframe' => 0,
          'agreservation' => 0,
          'agreservations_unit' => 0,
          'agreservations_unittype' => 0,
          'article' => 0,
          'page' => 0,
        ),
      ),
      'translatable' => '1',
      'type' => 'node_reference',
    ),
    'field_instance' => array(
      'bundle' => 'agreservations_unittype',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 2,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_agres_ref_rate',
      'label' => 'Reference Rate',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '-2',
      ),
    ),
  );

  // Exported field: 'node-agres_rate-field_agres_ref_timeframe'
  $fields['node-agres_rate-field_agres_ref_timeframe'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_agres_ref_timeframe',
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
        'field_permissions' => array(
          'create' => 0,
          'edit' => 0,
          'edit own' => 0,
          'view' => 0,
          'view own' => 0,
        ),
        'referenceable_types' => array(
          'agres_category' => 0,
          'agres_timeframe' => 'agres_timeframe',
          'agreservation' => 0,
          'agreservations_unit' => 0,
          'agreservations_unittype' => 0,
          'article' => 0,
          'page' => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '1',
      'type' => 'node_reference',
    ),
    'field_instance' => array(
      'bundle' => 'agres_rate',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 2,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_agres_ref_timeframe',
      'label' => 'Reference Timeframe',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '-2',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('If this field is filled, not this products price will be used, but the base unit types price modified by the percent rate entered here.
Use - and + to indicate if the baseprice is increased or decreased by the entered percent number.');
  t('Percent Price Modifier');
  t('Reference Timeframe');

  return $fields;
}