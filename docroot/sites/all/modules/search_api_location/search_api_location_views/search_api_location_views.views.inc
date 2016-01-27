<?php

/**
 * @file
 * Provide views data for Search API location
 */

/**
 * Implements hook_views_data_alter().
 */
function search_api_location_views_views_data_alter(&$data) {
  foreach (search_api_index_load_multiple(FALSE) as $id => $index) {
    $table = &$data['search_api_index_' . $id];
    foreach (search_api_location_get_location_fields($index) as $key => $field) {
      $key = _entity_views_field_identifier($key, $table);
      if (isset($table[$key])) {
        $table[$key]['field']['handler'] = 'SearchApiViewsHandlerFieldLocation';
        $table[$key]['field']['type'] = $field['real_type'];
        $table[$key]['filter']['handler'] = 'SearchApiViewsHandlerFilterLocation';
        $table[$key]['filter']['type'] = $field['real_type'];
        $table[$key]['argument']['handler'] = 'SearchApiViewsHandlerArgumentLocationGeofilt';
        $table[$key]['argument']['type'] = $field['real_type'];
        // Add additional pseudo field to set point separately by an argument.
        $table[$key . '_point']['group'] = $table[$key]['group'];
        $table[$key . '_point']['title'] = $table[$key]['title'] . ' ' . t('point');
        $table[$key . '_point']['help'] = t('The central point for a distance filter. Also requires the radius.');
        $table[$key . '_point']['real field'] = $table[$key]['real field'];
        $table[$key . '_point']['argument']['handler'] = 'SearchApiViewsHandlerArgumentLocationPoint';
        $table[$key . '_point']['argument']['type'] = $field['real_type'];
        // Add additional pseudo field to set radius separately by an argument.
        $table[$key . '_radius']['group'] = $table[$key]['group'];
        $table[$key . '_radius']['title'] = $table[$key]['title'] . ' ' . t('radius');
        $table[$key . '_radius']['help'] = t('The radius for a distance filter. Also requires the central point.');
        $table[$key . '_radius']['real field'] = $table[$key]['real field'];
        $table[$key . '_radius']['argument']['handler'] = 'SearchApiViewsHandlerArgumentLocationRadius';
        $table[$key . '_radius']['argument']['type'] = 'decimal';
      }
    }
  }
}
