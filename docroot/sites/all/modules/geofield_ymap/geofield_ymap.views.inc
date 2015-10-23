<?php

/**
 * Implementation of hook_views_plugins().
 */
function geofield_ymap_views_plugins() {
  return array(
    'module' => 'geofield_ymap',
    'style' => array(
      'geofield_ymap' => array(
        'title' => t('Yandex Map'),
        'handler' => 'geofield_ymap_plugin_style_geofield_ymap',
        'type' => 'normal',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'even empty' => TRUE,
      ),
    ),
  );
}

/**
 * Implements hook_field_views_data_alter().
 */
function geofield_ymap_field_views_data_alter(&$data, $field, $module) {
  if ($module == 'geofield') {
    foreach ($data as $table_name => $table_data) {
      foreach ($table_data as $field_name => $field_data) {
        if (isset($field_data['field']['handler']) && $field_data['field']['handler'] == 'views_handler_field_field') {
          $field_data['field']['handler'] = 'views_handler_field';
          $field_data['title'] .= ' (GeoObject)';
          $field_data['title short'] .= ' (GeoObject)';

          $extra_additional_fields = array(
            $field_data['field']['field_name'] . '_geo_type',
            $field_data['field']['field_name'] . '_lat',
            $field_data['field']['field_name'] . '_lon',
            $field_data['field']['field_name'] . '_left',
            $field_data['field']['field_name'] . '_top',
            $field_data['field']['field_name'] . '_right',
            $field_data['field']['field_name'] . '_bottom',
            $field_data['field']['field_name'] . '_geohash',
          );
          $field_data['field']['additional fields'] = array_diff($field_data['field']['additional fields'], $extra_additional_fields);

          $data[$table_name][$field_name . '_geoobject'] = $field_data;
        }
      }
    }
  }
}
