<?php

/**
 * @file
 * Provide additional Views content.
 */


/**
 * Implements hook_views_data().
 */
function event_data_store_views_data() {
  $data = array();

  // Hook up the non aggregated data store.
  $data['event_data_store'] = array(
    'table' => array(
      'base' => array(
        'field' => 'eds_id',
        'title' => t('Event Data Store records'),
        'help' => t('Query stored data relating to events.'),
      ),
      'group' => t('Event Data Store'),
    ),
    'eds_id' => array(
      'title' => t('Record ID'),
      'help' => t('The unique ID of this specific record.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'event_name' => array(
      'title' => t('Event name'),
      'help' => t('The name of the data store event.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'event_data_store_views_handler_filter_event_name',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'uid' => array(
      'title' => t('Trigger User'),
      'help' => t('The user who triggered the event.'),
      'relationship' => array(
        'title' => t('Trigger User'),
        'help' => t('Relate the record to the user who triggered it.'),
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'field' => 'uid',
        'label' => t('trigger user'),
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_user_name',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'field' => array(
        'handler' => 'views_handler_field_user',
      ),
    ),
    'timestamp' => array(
      'title' => t('Event date'),
      'help' => t('The date the event occurred.'),
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
    ),
    'user_agent' => array(
      'title' => t('Record User Agent data'),
      'help' => t('The triggering users user agent data.'),
      'field' => array(
        'handler' => 'views_handler_field_xss',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'ip_address' => array(
      'title' => t('IP Address'),
      'help' => t('The hostname or IP address from where the event was triggered.'),
      'field' => array(
        'handler' => 'views_handler_field_xss',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
  );

  $data['event_data_store_agg'] = array(
    'table' => array(
      'base' => array(
        'field' => 'edsa_id',
        'title' => t('Event Data Store Aggregated records'),
        'help' => t('Query the aggregated stored data relating to events.'),
      ),
      'group' => t('Event Data Store'),
    ),
    'edsa_id' => array(
      'title' => t('Record ID'),
      'help' => t('The unique ID of this specific record.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'event_name' => array(
      'title' => t('Event name'),
      'help' => t('The name of the data store event.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'event_data_store_views_handler_filter_event_name',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'period' => array(
      'title' => t('Record interval'),
      'help' => t('The interval for the aggregated record: hourly, daily, weekly, monthly.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'event_data_store_views_handler_filter_agg_period',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'timestamp' => array(
      'title' => t('Event date'),
      'help' => t('The date the event occurred.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
        'is date' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'date_views_filter_handler',
        'is date' => TRUE,
      ),
      'argument' => array(
        'handler' => 'date_views_argument_handler_simple',
        'empty field name' => t('Undated'),
        'is date' => TRUE,
      ),
    ),
    'value' => array(
      'title' => t('Record data/value'),
      'help' => t('The aggregated value for this store.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
        'allow empty' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
  );

  return $data;
}
