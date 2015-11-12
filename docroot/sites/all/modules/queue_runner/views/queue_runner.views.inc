<?php
/**
 * @file
 * Provides Views integration for the Queue Runner module.
 */

/**
 * Implements hook_views_data().
 */
function queue_runner_views_data() {
  // Queue Runner Queue.
  $data['queue_runner_failures']['table']['group']  = t('Queue Runner Failures');
  $data['queue_runner_failures']['table']['base'] = array(
    'field' => 'item_id',
    'title' => t('Queue failure'),
    'help' => t('A queued item.'),
  );
  $data['queue_runner_failures']['item_id'] = array(
    'title' => t('Item ID'),
    'help' => t('The unique identifier of this item.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['queue_runner_failures']['title'] = array(
    'title' => t('Item title'),
    'help' => t('The title of a queue item'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['queue_runner_failures']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the queued item was created.'),
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

  // nid
  $data['queue_runner_failures']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
    ),
  );
  $data['queue_runner_failures']['task_operations'] = array(
    'title' => t('Task operations'),
    'help' => t('Operations to perform on tasks.'),
    'field' => array(
      'handler' => 'queue_runner_handler_field_task_operations',
      'real field' => 'item_id',
      'additional fields' => array(
        'item_id',
      ),
    ),
  );


  // Queue Runner Collectors.
  $data['queue_runner_collector']['table']['group']  = t('Queue Runner Collectors');
  $data['queue_runner_collector']['table']['base'] = array(
    'field' => 'coll_id',
    'title' => t('Queue Collector'),
    'help' => t('Information about a collector item.'),
  );
  $data['queue_runner_collector']['coll_id'] = array(
    'title' => t('Collector ID'),
    'help' => t('The unique identifier of this collector.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['queue_runner_collector']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of a queue collector'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['queue_runner_collector']['status'] = array(
    'title' => t('Status'),
    'help' => t('The status of a queue collector'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'queue_runner_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['queue_runner_collector']['total'] = array(
    'title' => t('Total sub tasks'),
    'help' => t('count of sub tasks collector.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['queue_runner_collector']['queued'] = array(
    'title' => t('Sub tasks in queue'),
    'help' => t('Number of sub tasks collector added to the queue.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['queue_runner_collector']['remaining'] = array(
    'title' => t('Sub tasks remaining'),
    'help' => t('Number of sub tasks collector added ot the queue.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // nid
  $data['queue_runner_collector']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
    ),
  );
  $data['queue_runner_collector']['collector_operations'] = array(
    'title' => t('Collector operations'),
    'help' => t('Operations to perform on collectors.'),
    'field' => array(
      'handler' => 'queue_runner_handler_field_collector_operations',
      'real field' => 'coll_id',
      'additional fields' => array(
        'coll_id',
      ),
    ),
  );

  $data['queue_runner_collector']['table']['join'] = array();
  $data['queue_runner_collector']['table']['join']['queue_runner_rel_collector'] = array(
    'left_field' => 'coll_id',
    'field' => 'coll_id',
  );
  $data['queue_runner_collector']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );


  // Queue Runner Log.
  $data['queue_runner_dblog']['table']['group']  = t('Queue Runner Log');
  $data['queue_runner_dblog']['table']['base'] = array(
    'field' => 'wid',
    'title' => t('Queue log event'),
    'help' => t('A log event relateding to a queued item.'),
  );
  $data['queue_runner_dblog']['message'] = array(
    'title' => t('Log message'),
    'help' => t('The log message'),
    'field' => array(
      'handler' => 'queue_runner_handler_field_log_message',
      'click sortable' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['queue_runner_dblog']['timestamp'] = array(
    'title' => t('Event date'),
    'help' => t('The date the event happened.'),
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
  $data['queue_runner_dblog']['severity'] = array(
    'title' => t('Severity'),
    'help' => t('The severity of a log message'),
    'field' => array(
      'handler' => 'queue_runner_handler_field_log_level',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'queue_runner_handler_filter_log_level',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['queue_runner_dblog']['hostname'] = array(
    'title' => t('hostname'),
    'help' => t('The name of the server where the log message was triggered'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // nid
  $data['queue_runner_dblog']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
    ),
  );

  $data['queue_runner_dblog']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  return $data;
}

