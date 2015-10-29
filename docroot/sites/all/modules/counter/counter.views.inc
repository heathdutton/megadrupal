<?php

/**
 * @file
 * Integrated to Views.
 */

/**
 * Implements hook_views_data().
 */
function counter_views_data() {
  // Basic table information.
  $data['counter']['table']['group']  = t('Counter');

  // Advertise this table as a possible base table.
  $data['counter']['table']['base'] = array(
    'field' => 'counter_id',
    'title' => t('Counter'),
    'help' => t('Counter list.'),
    'weight' => 10,
  );

  $data['users']['table']['join'] = array(
    'counter' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      'type' => 'LEFT',
    ),
  );

  $data['counter']['counter_id'] = array(
    'title' => t('Counter ID'),
    'help' => t('The Counter ID.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  $data['counter']['ip'] = array(
    'title' => t('IP Address'),
    'help' => t('The Counter IP Address.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['counter']['url'] = array(
    'title' => t('URL'),
    'help' => t('The URL when Counter counted.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['counter']['created'] = array(
    'title' => t('Created'),
    'help' => t('The created time of Counter item.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['counter']['uid'] = array(
    'title' => t('UID'),
    'help' => t('The UID who visit Counter URL.'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  
  // nid, type, browser_name, browser_version, platform
  $data['counter']['nid'] = array(
    'title' => t('NID'),
    'help' => t('The NID of URL if node was visited.'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  
  $data['counter']['type'] = array(
    'title' => t('NODE TYPE'),
    'help' => t('The NODE TYPE of node if node was visited.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['counter']['browser_name'] = array(
    'title' => t('BROWSER NAME'),
    'help' => t('The Browser Name of Client Browser.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['counter']['browser_version'] = array(
    'title' => t('BROWSER VERSION'),
    'help' => t('The Browser Version of Client Browser.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  $data['counter']['platform'] = array(
    'title' => t('PLATFORM'),
    'help' => t('The Platform Name of Client System/OS.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function counter_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'counter'),
    ),
    'handlers' => array(
      'counter_views_handler_filter_string_service' => array(
        'parent' => 'views_handler_filter_many_to_one',
      ),
      'counter_views_handler_field_hostname' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
