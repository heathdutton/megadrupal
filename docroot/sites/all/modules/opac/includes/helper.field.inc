<?php

/**
 * @file
 * Helper functions related to field.
 */

/**
 * Retrieves field info.
 *
 * We need to have static variable when harvest
 * because this function could be called for each
 * field of each node.
 *
 * @param string $field_name
 *   Field name.
 *
 * @param string $bundle
 *   Content type.
 *
 * @return array
 *   Information about the field and the instance.
 */
function _opac_field_info($field_name, $bundle) {
  $data = &drupal_static(__FUNCTION__ . "$field_name::$bundle");

  if (!isset($data)) {
    // We can access to field settings on
    // $data['key_setting'][..] and to instance
    // settings on $data['instance']['key_setting'][...].
    $data = field_info_field($field_name);
    $data['instance'] = field_info_instance('node', $field_name, $bundle);
  }
  return $data;
}

/**
 * Checks if an entity has the given field name.
 *
 * @param string $entity_type
 *   Entity type name.
 *
 * @param string $field_name
 *   Field name.
 *
 * @param string $bundle
 *   Entity bundle.
 *
 * @return bool
 *   The entity has or hasn't the field.
 */
function _opac_entity_has_field($entity_type, $field_name, $bundle) {
  $has_field = &drupal_static(__FUNCTION__ . "$entity_type::$field_name::$bundle");

  if (!isset($has_field)) {
    $has_field = FALSE;
    $instance = field_info_instance($entity_type, $field_name, $bundle);
    if (isset($instance)) {
      $has_field = TRUE;
    }
  }
  return $has_field;
}
