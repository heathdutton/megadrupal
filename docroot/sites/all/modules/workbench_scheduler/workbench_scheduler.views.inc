<?php

/**
 * @file
 *  Content schedule views integration for Workbench.
 */


/**
 * Implements hook_views_data().
 */
function workbench_scheduler_views_data() {
  $data = array();

  $data['workbench_scheduler_nodes']['table']['group'] = t('Workbench Scheduler');

  $data['workbench_scheduler_nodes']['table']['join'] = array(
    'node' => array(
      'left_table' => 'node_revision',
      'left_field' => 'vid',
      'field' => 'vid',
      'type' => 'LEFT OUTER',
    ),
    'node_revision' => array(
      'left_field' => 'vid',
      'field' => 'vid',
      'type' => 'LEFT OUTER',
    ),
  );

  $data['workbench_scheduler_nodes']['nid'] = array(
    'title' => t('Node'),
    'help' => t('The node according to the Workbench Schedule.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node'),
    ),
  );

  $data['workbench_scheduler_nodes']['vid'] = array(
    'title' => t('Node Revision'),
    'help' => t('The node revision according to the Workbench Schedule.'),
    'relationship' => array(
      'base' => 'node_revision',
      'field' => 'vid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node Revision'),
    ),
  );

  $data['workbench_scheduler_nodes']['sid'] = array(
    'title' => t('Workbench Schedule ID'),
    'help' => t('The workbench schedule ID'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['workbench_scheduler_nodes']['start_date'] = array(
    'title' => t('Start Date'),
    'help' => t('The date to set the start state'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['workbench_scheduler_nodes']['end_date'] = array(
    'title' => t('End Date'),
    'help' => t('The date to set the end state'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['workbench_scheduler_nodes']['completed'] = array(
    'title' => t('Completed'),
    'help' => t('Whether or not this node schedule has been completed'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Completed'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['workbench_scheduler_schedules']['table']['group'] = t('Workbench Scheduler');
  $data['workbench_scheduler_schedules']['table']['join'] = array(
    'workbench_scheduler_nodes' => array(
      'left_table' => 'workbench_scheduler_nodes',
      'left_field' => 'sid',
      'field' => 'sid',
    ),
    'node' => array(
      'left_table' => 'workbench_scheduler_nodes',
      'left_field' => 'sid',
      'field' => 'sid',
    ),
    'node_revision' => array(
      'left_table' => 'workbench_scheduler_nodes',
      'left_field' => 'sid',
      'field' => 'sid',
    ),
  );

  $data['workbench_scheduler_schedules']['sid'] = array(
    'title' => t('Schedule ID'),
    'help' => t('The workbench schedule ID'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['workbench_scheduler_schedules']['name'] = array(
    'title' => t('Machine name'),
    'help' => t('The machine readable name of the schedule'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'workbench_scheduler_handler_filter_name',
    ),
  );

  $data['workbench_scheduler_schedules']['label'] = array(
    'title' => t('Schedule Name'),
    'help' => t('The human readable name of the schedule'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  $data['workbench_scheduler_schedules']['start_state'] = array(
    'title' => t('Start State'),
    'help' => t('The workbench schedule start state'),
    'filter' => array(
      'handler' => 'workbench_scheduler_handler_filter_state',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name table' => 'workbench_moderation_states',
      'name field' => 'label',
    ),
  );

  $data['workbench_scheduler_schedules']['end_state'] = array(
    'title' => t('End State'),
    'help' => t('The workbench schedule end state'),
    'filter' => array(
      'handler' => 'workbench_scheduler_handler_filter_state',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name table' => 'workbench_moderation_states',
      'name field' => 'label',
    ),
  );

  return $data;
}

