<?php
/**
 * @file
 * Dev field implementation.
 */

/**
 * Views field handler for version:is_dev.
 */
class ph_version_views_handler_field_dev extends views_handler_field_boolean {
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function get_value($values, $field = NULL) {
    // Prepare column names.
    $field_name = $this->definition['field_name'];
    $column_patch_name = $field_name . '_patch';
    $column_increment_name = $field_name . '_increment';
    $alias_patch = $this->aliases[$column_patch_name];
    $alias_increment = $this->aliases[$column_increment_name];

    // Return a combination of 'patch' and 'increment'.
    return !isset($values->{$alias_patch}) || isset($values->{$alias_increment});
  }
}
