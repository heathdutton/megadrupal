<?php
/**
 * @file
 * openlucius_events.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function openlucius_events_extras_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'flagging-not_attending_event-field_event_reason'
  $field_instances['flagging-not_attending_event-field_event_reason'] = array(
    'bundle' => 'not_attending_event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Why are you not attending?',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'flagging',
    'field_name' => 'field_event_reason',
    'label' => 'Reason',
    'required' => 1,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'ol_full_html' => 'ol_full_html',
          'plain_text' => 'plain_text',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'ol_full_html' => array(
              'weight' => -9,
            ),
            'plain_text' => array(
              'weight' => -10,
            ),
          ),
        ),
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Reason');
  t('Why are you not attending?');

  return $field_instances;
}
