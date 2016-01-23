<?php

/**
 * Implements hook_views_data_alter()
 */
function views_string_select_views_data_alter(&$data) {
  foreach ($data as $table_name => $table_info) {
    foreach ($table_info as $field_name => $field_info) {
      if ((isset($field_info['filter']['handler'])) && (($field_info['filter']['handler'] == 'views_handler_filter_string') || ($field_info['filter']['handler'] == 'views_autocomplete_filters_handler_filter_string'))) {
        if (strpos($field_info['title'], ':format') !== FALSE) {
          continue;
        }
        $filter = $field_info;
        $filter['title'] = (!empty($field_info['filter']['title']) ? $field_info['filter']['title'] : $field_info['title']) . ' (string select)';
        $filter['filter']['handler'] = 'views_string_select_filter';
        $filter['filter']['field'] = $field_name;
        unset($filter['argument'], $filter['field'], $filter['relationship'], $filter['sort'], $filter['filter']['title'], $filter['aliases']);
        $data[$table_name][$field_name . '_string_select'] = $filter;
      }
    }
  }
}
