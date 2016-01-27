<?php

/**
 * @file
 * Contains default fields/instances for commerce bundle invoice.
 */

/**
 * Define the default fields for the Invoice.
 *
 * @return array
 *   Return default fields and instances to be attached to invoice.
 *
 * @see field_create_field()
 * @see field_create_instance()
 */
function erpal_invoice_default_fields() {
  $fields = array();
  $instances = array();

  // Exported field_base: 'field_invoice_credit_period'
  $fields[] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_invoice_credit_period',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'number_integer',
  );

  // Exported field_base: 'field_invoice_due_date'
  $fields[] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_invoice_due_date',
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'timezone_db' => 'UTC',
      'todate' => '',
      'tz_handling' => 'site',
    ),
    'translatable' => 0,
    'type' => 'datestamp',
  );

  // Exported field_base: 'field_invoice_order'
  $fields[] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_invoice_order',
    'foreign keys' => array(
      'commerce_order' => array(
        'columns' => array(
          'target_id' => 'order_id',
        ),
        'table' => 'commerce_order',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
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
          'type' => 'none',
        ),
        'target_bundles' => array(
          'commerce_order' => 'commerce_order',
        ),
      ),
      'target_type' => 'commerce_order',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_instance:
  // 'commerce_order-invoice-field_invoice_credit_period'.
  $instances[] = array(
    'bundle' => 'invoice',
    'default_value' => array(
      0 => array(
        'value' => 10,
      ),
    ),
    'deleted' => 0,
    'description' => 'Please enter integer value in days',
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
        'weight' => -2,
      ),
    ),
    'entity_type' => 'commerce_order',
    'field_name' => 'field_invoice_credit_period',
    'label' => 'Credit period',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => 'day(s)',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => -2,
    ),
  );

  // Exported field_instance: 'commerce_order-invoice-field_invoice_due_date'
  $instances[] = array(
    'bundle' => 'invoice',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Internally set depending on Invoice date and Credit period fields.',
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
        ),
        'type' => 'date_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'commerce_order',
    'field_name' => 'field_invoice_due_date',
    'label' => 'Due date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'field_extrawidgets',
      'settings' => array(),
      'type' => 'field_extrawidgets_hidden',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'commerce_order-invoice-field_invoice_order'
  $instances[] = array(
    'bundle' => 'invoice',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 4,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'commerce_order',
    'field_name' => 'field_invoice_order',
    'label' => 'Order',
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
        'references_dialog_add' => 0,
        'references_dialog_edit' => 0,
        'references_dialog_search' => 0,
        'references_dialog_search_view' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 4,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Credit period');
  t('Please enter integer value in days');
  t('Due date');
  t('Internally set depending on Invoice date and Credit period fields.');

  return array(
    'fields' => $fields,
    'instances' => $instances,
  );
}
