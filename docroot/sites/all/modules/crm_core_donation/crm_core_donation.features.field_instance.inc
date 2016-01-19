<?php
/**
 * @file
 * crm_core_donation.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function crm_core_donation_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'crm_core_activity-cmcd_donation-field_cmcd_amount'
  $field_instances['crm_core_activity-cmcd_donation-field_cmcd_amount'] = array(
    'bundle' => 'cmcd_donation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'crm_core_donation',
        'settings' => array(),
        'type' => 'crm_core_donation_amount',
        'weight' => 13,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcd_amount',
    'label' => 'Amount',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcd_donation-field_cmcd_order'
  $field_instances['crm_core_activity-cmcd_donation-field_cmcd_order'] = array(
    'bundle' => 'cmcd_donation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'commerce_order_reference',
        'settings' => array(),
        'type' => 'commerce_order_reference_link',
        'weight' => 12,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcd_order',
    'label' => 'Order',
    'required' => FALSE,
    'settings' => array(
      'field_injection' => 1,
      'referenceable_types' => array(
        'commerce_order' => 'commerce_order',
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'commerce_order_reference',
      'settings' => array(),
      'type' => 'commerce_order_reference_hidden',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcd_donation-field_cmcd_page'
  $field_instances['crm_core_activity-cmcd_donation-field_cmcd_page'] = array(
    'bundle' => 'cmcd_donation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'This field is used to record the donation page that is the source of a donation.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcd_page',
    'label' => 'Donation Page',
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
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcd_donation-field_cmcd_payment_type'
  $field_instances['crm_core_activity-cmcd_donation-field_cmcd_payment_type'] = array(
    'bundle' => 'cmcd_donation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'This field is used to record the payment type for a donation.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 8,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcd_payment_type',
    'label' => 'Payment Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcd_donation-field_cmcd_receive_date'
  $field_instances['crm_core_activity-cmcd_donation-field_cmcd_receive_date'] = array(
    'bundle' => 'cmcd_donation',
    'deleted' => 0,
    'description' => 'This field is used to record the time and date of a donation.',
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
        'weight' => 4,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcd_receive_date',
    'label' => 'Receive Date',
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
      'module' => 'date',
      'settings' => array(
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array(),
        'year_range' => '-50:+50',
      ),
      'type' => 'date_popup',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcd_donation-field_cmcd_source'
  $field_instances['crm_core_activity-cmcd_donation-field_cmcd_source'] = array(
    'bundle' => 'cmcd_donation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'This field is used to capture the source of a donation.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcd_source',
    'label' => 'Source',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-cmcd_page-field_cmcd_body'
  $field_instances['node-cmcd_page-field_cmcd_body'] = array(
    'bundle' => 'cmcd_page',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'masonry' => 0,
          'masonry_animated' => 1,
          'masonry_animated_duration' => 500,
          'masonry_center' => 0,
          'masonry_gutter' => 0,
          'masonry_resizable' => 1,
          'masonry_rtl' => 0,
          'masonry_width' => 200,
          'masonry_width_unit' => 'px',
        ),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_cmcd_body',
    'label' => 'Body',
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => -4,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Amount');
  t('Body');
  t('Donation Page');
  t('Order');
  t('Payment Type');
  t('Receive Date');
  t('Source');
  t('This field is used to capture the source of a donation.');
  t('This field is used to record the donation page that is the source of a donation.');
  t('This field is used to record the payment type for a donation.');
  t('This field is used to record the time and date of a donation.');

  return $field_instances;
}
