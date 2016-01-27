<?php

/**
 * Implements hook_views_data().
 */
function state_flow_schedule_views_data() {
  $data = array();

  $data['state_flow_schedule']['table']['group'] = t('State Flow Schedule');
  $data['state_flow_schedule']['date'] = array(
    'title' => t('Scheduled Transition Date'),
    'help' => t('The date of the scheduled transition'),
    'field' => array('handler' => 'views_handler_field_date', 'click sortable' => TRUE),
    'filter' => array('handler' => 'views_handler_filter_date'),
    'sort' => array('handler' => 'views_handler_sort_date'),
  );
  $data['state_flow_schedule']['event'] = array(
    'title' => t('Scheduled Transition Event'),
    'help' => t('The event type of the scheduled transition'),
    'field' => array('handler' => 'state_flow_schedule_handler_field_state_machine_event', 'click sortable' => TRUE),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'sort' => array('handler' => 'views_handler_sort_string'),
  );

  $data['state_flow_schedule']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'node_revision' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  return $data;
}
