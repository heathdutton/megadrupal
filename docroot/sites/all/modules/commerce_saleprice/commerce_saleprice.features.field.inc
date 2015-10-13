<?php
/**
 * @file
 * commerce_saleprice.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function commerce_saleprice_field_default_fields() {
  $fields = array();

  // Exported field: 'commerce_product-product-field_commerce_saleprice_on_sale'
  $fields['commerce_product-product-field_commerce_saleprice_on_sale'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_commerce_saleprice_on_sale',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => '',
          1 => '',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_boolean',
    ),
    'field_instance' => array(
      'bundle' => 'product',
      'commerce_cart_settings' => array(
        'attribute_field' => 0,
        'attribute_widget' => 'select',
      ),
      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
      'deleted' => '0',
      'description' => 'If checked, the <em>Sale price</em> value will be used rather than the normal price.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '1',
        ),
        'line_item' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'node_teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'commerce_product',
      'field_name' => 'field_commerce_saleprice_on_sale',
      'label' => 'On sale',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
        'type' => 'options_onoff',
        'weight' => '4',
      ),
    ),
  );

  // Exported field: 'commerce_product-product-field_commerce_saleprice'
  $fields['commerce_product-product-field_commerce_saleprice'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_commerce_saleprice',
      'foreign keys' => array(),
      'indexes' => array(
        'currency_price' => array(
          0 => 'amount',
          1 => 'currency_code',
        ),
      ),
      'module' => 'commerce_price',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'commerce_price',
    ),
    'field_instance' => array(
      'bundle' => 'product',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'This price will be used if the <em>On sale</em> checkbox is checked.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '2',
        ),
        'line_item' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'node_teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'commerce_product',
      'field_name' => 'field_commerce_saleprice',
      'label' => 'Sale price',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'commerce_price',
        'settings' => array(
          'currency_code' => 'default',
        ),
        'type' => 'commerce_price_simple',
        'weight' => '5',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('If checked, the <em>Sale price</em> value will be used rather than the normal price.');
  t('On sale');
  t('Sale price');
  t('This price will be used if the <em>On sale</em> checkbox is checked.');

  return $fields;
}
