<?php
/**
 * @file
 * Alters the Geofield field, filter and sort to support polygon distances.
 */

/**
 * Implements hook_field_views_data_alter().
 *
 * Swaps out Geofield's proximity field for one that supports polygons.
 */
function geofield_proximity_polygon_field_views_data_alter(&$data, $field) {

  if (isset($field['type']) && $field['type'] == 'geofield') {
    // E.g., field_name == 'field_geo_coords'
    $field_name = $field['field_name'];
    foreach ($data as $table_name => $table_data) {
      if (isset($table_data[$field_name])) {
        // E.g., table_name == 'field_data_field_geo_coords'
        if (isset($data[$table_name]['field_geofield_distance'])) {
          $distance_field = &$data[$table_name]['field_geofield_distance'];
          if (isset($distance_field['field']['handler'])) {
            // Change from 'geofield_handler_field' to our version
            $distance_field['field']['handler'] = 'geofield_proximity_polygon_handler_field';
          }
          if (isset($distance_field['filter']['handler'])) {
            // Change from 'geofield_handler_filter' to our version
            $distance_field['filter']['handler'] = 'geofield_proximity_polygon_handler_filter';
          }
          if (isset($distance_field['sort']['handler'])) {
            // Change from 'geofield_handler_sort' to our version
            $distance_field['sort']['handler'] = 'geofield_proximity_polygon_handler_sort';
          }

          if (isset($distance_field['title short'])) {
            $distance_field['title short'] .= ' ' . t('to point or polygon');
          }
        }
      }
    }
  }
}
