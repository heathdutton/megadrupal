<?php
/**
 * @file
 * Provide views data for Search API location
 */

/**
 * Implements hook_views_data_alter().
 */
function search_api_location_map_views_data_alter(&$data) {
  foreach ($data as $table => $fields) {
    if (substr($table, 0, 17) === 'search_api_index_') {
      foreach ($fields as $field => $handlers) {
        if (isset($handlers['filter']['type']) && $handlers['filter']['type'] == 'location') {
          $data[$table][$field]['filter']['handler'] = 'SearchApiLocationMapViewsHandlerFilter';
        }
      }
    }
  }
}
