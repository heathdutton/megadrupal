<?php
/**
 * @file
 * Features module integration.
 */

/**
 * Implements hook_features_pipe_field_alter().
 */
function fbphotosync_features_pipe_field_alter(&$pipe, $data, &$export) {
  foreach ($data as $identifier) {
    list($entity_type, $bundle_name, $field_name) = explode('-', $identifier);
    $instance = field_info_instance($entity_type, $field_name, $bundle_name);
    if (isset($instance['settings']['fbphotosync']['status']) && $instance['settings']['fbphotosync']['status'] == TRUE) {
      $export['dependencies']['fbphotosync'] = 'fbphotosync';
    }
  }
}
