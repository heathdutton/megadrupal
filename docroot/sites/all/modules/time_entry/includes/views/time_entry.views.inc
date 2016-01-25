<?php

/**
 * Implements hook_views_data_alter()
 */
function time_entry_views_data_alter(&$data) {
  foreach (array('field', 'sort', 'filter', 'argument') as $type) {
    $data['time_entry']['time'][$type]['handler'] = 'views_handler_' . $type . '_date';
    $data['time_entry']['time'][$type]['is date'] = TRUE;
    $data['time_entry']['time'][$type]['field_info'] = array();
    $data['time_entry']['duration'][$type]['handler'] = 'views_handler_' . $type . '_time_interval';
    $data['time_entry']['duration'][$type]['is date'] = TRUE;
  }
}
