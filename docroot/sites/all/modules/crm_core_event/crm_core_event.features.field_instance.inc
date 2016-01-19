<?php
/**
 * @file
 * crm_core_event.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function crm_core_event_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'crm_core_activity-cmcev_event_registration-field_cmcev_amount'
  $field_instances['crm_core_activity-cmcev_event_registration-field_cmcev_amount'] = array(
    'bundle' => 'cmcev_event_registration',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Stores the total amount paid for the registration.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'crm_core_event',
        'settings' => array(),
        'type' => 'crm_core_event_amount',
        'weight' => 12,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcev_amount',
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
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcev_event_registration-field_cmcev_event'
  $field_instances['crm_core_activity-cmcev_event_registration-field_cmcev_event'] = array(
    'bundle' => 'cmcev_event_registration',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Refers to the event being registered for.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcev_event',
    'label' => 'Event',
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
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcev_event_registration-field_cmcev_order'
  $field_instances['crm_core_activity-cmcev_event_registration-field_cmcev_order'] = array(
    'bundle' => 'cmcev_event_registration',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'commerce_order_reference',
        'settings' => array(),
        'type' => 'commerce_order_reference_link',
        'weight' => 11,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcev_order',
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
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcev_event_registration-field_cmcev_public'
  $field_instances['crm_core_activity-cmcev_event_registration-field_cmcev_public'] = array(
    'bundle' => 'cmcev_event_registration',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Check this box to indicate whether or not to show this registration publicly.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 9,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcev_public',
    'label' => 'View Publicly',
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
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcev_event_registration-field_cmcev_reg_date'
  $field_instances['crm_core_activity-cmcev_event_registration-field_cmcev_reg_date'] = array(
    'bundle' => 'cmcev_event_registration',
    'deleted' => 0,
    'description' => 'Stores the actual date an event registration was received.',
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
        'weight' => 10,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcev_reg_date',
    'label' => 'Registration Date',
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
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcev_event_registration-field_cmcev_source'
  $field_instances['crm_core_activity-cmcev_event_registration-field_cmcev_source'] = array(
    'bundle' => 'cmcev_event_registration',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Select the source of the registration.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcev_source',
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
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcev_event_registration-field_cmcev_status'
  $field_instances['crm_core_activity-cmcev_event_registration-field_cmcev_status'] = array(
    'bundle' => 'cmcev_event_registration',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Stores the status of the registration.',
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
    'field_name' => 'field_cmcev_status',
    'label' => 'Status',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-cmcev_event-body'
  $field_instances['node-cmcev_event-body'] = array(
    'bundle' => 'cmcev_event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => 0,
    'settings' => array(
      'display_summary' => 1,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-cmcev_event-field_cmcev_date'
  $field_instances['node-cmcev_event-field_cmcev_date'] = array(
    'bundle' => 'cmcev_event',
    'deleted' => 0,
    'description' => 'The date the event will occur.',
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
        'weight' => 0,
      ),
      'teaser' => array(
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
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_cmcev_date',
    'label' => 'Date',
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
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-cmcev_event-field_cmcev_location'
  $field_instances['node-cmcev_event-field_cmcev_location'] = array(
    'bundle' => 'cmcev_event',
    'default_value' => array(
      0 => array(
        'element_key' => 'node|cmcev_event|field_cmcev_location|und|0',
        'thoroughfare' => '',
        'premise' => '',
        'locality' => '',
        'administrative_area' => '',
        'postal_code' => '',
        'country' => 'US',
      ),
    ),
    'deleted' => 0,
    'description' => 'The location for the event.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'addressfield',
        'settings' => array(
          'format_handlers' => array(
            0 => 'address',
          ),
          'use_widget_handlers' => 1,
        ),
        'type' => 'addressfield_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'addressfield',
        'settings' => array(
          'format_handlers' => array(
            0 => 'address',
          ),
          'use_widget_handlers' => 1,
        ),
        'type' => 'addressfield_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_cmcev_location',
    'label' => 'Location',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'addressfield',
      'settings' => array(
        'available_countries' => array(
          'US' => 'US',
        ),
        'format_handlers' => array(
          'address' => 'address',
          'address-hide-country' => 'address-hide-country',
          'organisation' => 0,
          'name-full' => 0,
          'name-oneline' => 0,
        ),
      ),
      'type' => 'addressfield_standard',
      'weight' => 5,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Amount');
  t('Body');
  t('Check this box to indicate whether or not to show this registration publicly.');
  t('Date');
  t('Event');
  t('Location');
  t('Order');
  t('Refers to the event being registered for.');
  t('Registration Date');
  t('Select the source of the registration.');
  t('Source');
  t('Status');
  t('Stores the actual date an event registration was received.');
  t('Stores the status of the registration.');
  t('Stores the total amount paid for the registration.');
  t('The date the event will occur.');
  t('The location for the event.');
  t('View Publicly');

  return $field_instances;
}
