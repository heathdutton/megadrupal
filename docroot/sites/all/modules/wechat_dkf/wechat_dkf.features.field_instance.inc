<?php
/**
 * @file
 * wechat_dkf.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function wechat_dkf_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'wechat_response_message-transfer_customer_service-field_kfaccount'
  $field_instances['wechat_response_message-transfer_customer_service-field_kfaccount'] = array(
    'bundle' => 'transfer_customer_service',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'wechat_response_message',
    'field_name' => 'field_kfaccount',
    'label' => 'kfaccount',
    'required' => 0,
    'settings' => array(
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
  t('kfaccount');

  return $field_instances;
}
