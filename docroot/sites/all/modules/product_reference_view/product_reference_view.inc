<?php

/**
 * @file
 * Commonly used functions product_reference_view module.
 */

/**
 * Utility function to return viewfield field_rsrvd_prdref_srcview
 * Reserved Product Reference Source View. viewfield field strictly for product_reference_view
 * @link http://drupal.org/node/1260046#comment-4907274
 * @return array $field
 */
function _product_reference_view_get_reserved_viewfield() {
  return array(
    'field_rsrvd_prdref_srcview' => array(
      'field_name' => 'field_rsrvd_prdref_srcview',
      'type' => 'viewfield',
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
    ),
  );  
}

/**
 * Utility function to return viewfield field_rsrvd_prdref_srcview instance array
 * @link http://drupal.org/node/1260046#comment-4907274
 * @return array $field
 */
function _product_reference_view_viewfield_scaffold_instance($entity_type, $bundle, $commerce_product_reference_field_label) {
  return array(
    'field_rsrvd_prdref_srcview' => array('field_name' => 'field_rsrvd_prdref_srcview',
      'entity_type' =>  $entity_type,
      'bundle' => $bundle,        
      'label' => t("View Source of $commerce_product_reference_field_label"),
      'display' => array( 'default' => array('label' => 'hidden', 'type' => 'hidden') ),
    ),
  );  
}