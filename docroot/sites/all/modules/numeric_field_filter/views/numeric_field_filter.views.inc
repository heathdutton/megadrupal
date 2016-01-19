<?php

/**
 * @file
 * Views integration for Numeric Field Filter module.
 */

/**
 * Constant denotes a part of filtering expression that will be substituted
 * with the selected Field API field.
 *
 * @var string
 */
define('NUMERIC_FIELD_FILTER_VIEWS_FIELD_TOKEN', '[field]');

/**
 * Constant denotes filtering in the regular way, when a field is compared
 * against a constant value.
 *
 * @var string
 */
define('NUMERIC_FIELD_FILTER_VIEWS_FIELD_CONSTANT', 'field_constant');

/**
 * Constant denotes that no views relationship was selected for the field, i.e.
 * that the field is a field attached to the entity of base table.
 */
define('NUMERIC_FIELD_FILTER_VIEWS_FIELD_RELATIONSHIP_BASE', 'base');

/**
 * Implements hook_views_data_alter().
 */
function numeric_field_filter_views_data_alter(&$data) {
  foreach (field_info_instances() as $entity_type => $bundles) {
    foreach ($bundles as $bundle => $instances) {
      foreach ($instances as $instance) {
        $field = field_info_field($instance['field_name']);

        if (in_array($field['type'], _numeric_field_filter_field_types())) {
          // This field is eligble for use with Number Field Filter.
          $views_data = field_views_field_default_views_data($field);
          foreach ($views_data as $table_name => $table_data) {
            // Adding our filter into the views data.
            if (isset($table_data[$field['field_name']])) {
              $column = numeric_field_filter_converter_method_invoke($field, 'columnName', $field);
              $data[$table_name][$field['field_name'] . '_' . $column]['filter']['handler'] = 'numeric_field_filter_handler_filter_numeric';
            }
            // @todo: add support for revisions, currently we operate only on
            // the base table of field.
          }
        }
      }
    }
  }
}
