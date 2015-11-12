<?php
/**
 * @file
 * commerce_couponprodref.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function commerce_couponprodref_field_default_fields() {
  $fields = array();

  // Exported field: 'commerce_coupon-commerce_coupon_pct-field_commerce_couponprodref'.
  $fields['commerce_coupon-commerce_coupon_pct-field_commerce_couponprodref'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_commerce_couponprodref',
      'foreign keys' => array(
        'commerce_product' => array(
          'columns' => array(
            'target_id' => 'product_id',
          ),
          'table' => 'commerce_product',
        ),
      ),
      'indexes' => array(
        'target_id' => array(
          0 => 'target_id',
        ),
      ),
      'locked' => '0',
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'behaviors' => array(
            'views-select-list' => array(
              'status' => 0,
            ),
          ),
          'sort' => array(
            'direction' => 'ASC',
            'field' => 'commerce_price:amount',
            'property' => 'product_id',
            'type' => 'none',
          ),
          'target_bundles' => array(),
        ),
        'handler_submit' => 'Change handler',
        'target_type' => 'commerce_product',
      ),
      'translatable' => '0',
      'type' => 'entityreference',
    ),
    'field_instance' => array(
      'bundle' => 'commerce_coupon_pct',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'link' => FALSE,
          ),
          'type' => 'entityreference_label',
          'weight' => 9,
        ),
      ),
      'entity_type' => 'commerce_coupon',
      'field_name' => 'field_commerce_couponprodref',
      'label' => 'Products',
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
          'size' => '60',
        ),
        'type' => 'entityreference_autocomplete',
        'weight' => '9',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Products');

  return $fields;
}
