<?php
/**
 * @file
 * commerce_sp.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function commerce_sp_field_default_fields() {
  $fields = array();

  // Exported field: 'commerce_product-commerce_sp_subscription-commerce_price'
  $fields['commerce_product-commerce_sp_subscription-commerce_price'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'commerce_product',
      ),
      'field_name' => 'commerce_price',
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
      'bundle' => 'commerce_sp_subscription',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'commerce_line_item_display' => array(
          'label' => 'hidden',
          'module' => 'commerce_price',
          'settings' => array(
            'calculation' => 'calculated_sell_price',
          ),
          'type' => 'commerce_price_formatted_amount',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'hidden',
          'module' => 'commerce_price',
          'settings' => array(
            'calculation' => 'calculated_sell_price',
          ),
          'type' => 'commerce_price_formatted_amount',
          'weight' => '3',
        ),
        'full' => array(
          'label' => 'hidden',
          'module' => 'commerce_price',
          'settings' => array(
            'calculation' => 'calculated_sell_price',
          ),
          'type' => 'commerce_price_formatted_amount',
          'weight' => 0,
        ),
        'line_item' => array(
          'label' => 'hidden',
          'module' => 'commerce_price',
          'settings' => array(
            'calculation' => 'calculated_sell_price',
          ),
          'type' => 'commerce_price_formatted_amount',
          'weight' => 0,
        ),
        'node_teaser' => array(
          'label' => 'hidden',
          'module' => 'commerce_price',
          'settings' => array(
            'calculation' => 'calculated_sell_price',
          ),
          'type' => 'commerce_price_formatted_amount',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'commerce_product',
      'field_name' => 'commerce_price',
      'label' => 'Price',
      'required' => TRUE,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'module' => 'commerce_price',
        'settings' => array(
          'currency_code' => 'default',
        ),
        'type' => 'commerce_price_full',
        'weight' => '3',
      ),
    ),
  );

  // Exported field: 'commerce_product-commerce_sp_subscription-field_commerce_sp_time'
  $fields['commerce_product-commerce_sp_subscription-field_commerce_sp_time'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_commerce_sp_time',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(
        'field_permissions' => NULL,
        'profile2_private' => FALSE,
      ),
      'translatable' => '0',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'commerce_sp_subscription',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The validity of the subscription time in weeks. In case multiple subscription products are bought, the subscription time of all bought products is aggregated. In case there is already a valid subscription, its validity is extended by the given time period.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => '2',
        ),
        'line_item' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'commerce_product',
      'field_name' => 'field_commerce_sp_time',
      'label' => 'Subscription time',
      'required' => 1,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => 'weeks',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'user-user-field_commerce_sp_validity'
  $fields['user-user-field_commerce_sp_validity'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_commerce_sp_validity',
      'field_permissions' => array(
        'type' => 2,
      ),
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'date',
      'settings' => array(
        'field_permissions' => NULL,
        'granularity' => array(
          'day' => 'day',
          'hour' => 'hour',
          'minute' => 'minute',
          'month' => 'month',
          'second' => 'second',
          'year' => 'year',
        ),
        'profile2_private' => FALSE,
        'repeat' => 0,
        'timezone_db' => 'UTC',
        'todate' => '',
        'tz_handling' => 'user',
      ),
      'translatable' => '0',
      'type' => 'datestamp',
    ),
    'field_instance' => array(
      'bundle' => 'user',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'long',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => '',
            'show_repeat_rule' => 'show',
          ),
          'type' => 'date_default',
          'weight' => 1,
        ),
      ),
      'entity_type' => 'user',
      'field_name' => 'field_commerce_sp_validity',
      'label' => 'Subscription validity',
      'required' => 0,
      'settings' => array(
        'default_value' => 'blank',
        'default_value2' => 'blank',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => 0,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => 15,
          'input_format' => 'Y-m-d H:i:s',
          'input_format_custom' => '',
          'label_position' => 'above',
          'repeat_collapsed' => 0,
          'text_parts' => array(),
          'year_range' => '-3:+3',
        ),
        'type' => 'date_text',
        'weight' => '8',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Price');
  t('Subscription time');
  t('Subscription validity');
  t('The validity of the subscription time in weeks. In case multiple subscription products are bought, the subscription time of all bought products is aggregated. In case there is already a valid subscription, its validity is extended by the given time period.');

  return $fields;
}
