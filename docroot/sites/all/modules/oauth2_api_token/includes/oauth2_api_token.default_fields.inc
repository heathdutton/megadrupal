<?php
/**
 * @file
 * Default fields and instances for the OAuth2 API Token module.
 */

/**
 * Install default fields.
 *
 * This is invoked on cache clear.
 */
function oauth2_api_token_install_fields() {
  $field = field_info_field('api_token_app');
  // Create the application field if it's missing.
  if (!$field) {
    $field = array(
      'field_name' => 'api_token_app',
      'type' => 'text',
    );
    field_create_field($field);
  }

  $needed_instances = array(
    'oauth2_server_token' => array('api_token'),
  );
  foreach ($needed_instances as $entity_type => $bundles) {
    $existing = array();
    if (!empty($field['bundles'][$entity_type])) {
      $existing = $field['bundles'][$entity_type];
    }
    $diff = array_diff($bundles, $existing);
    foreach ($diff as $new_bundle) {
      $instance = array(
        'label' => 'Application',
        'required' => TRUE,
        'field_name' => 'api_token_app',
        'entity_type' => $entity_type,
        'bundle' => $new_bundle,
      );
      field_create_instance($instance);
    }
  }
}
