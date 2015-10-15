<?php

/**
 * Implement hook_views_data().
 */
function tracekit_views_data() {
  $data['tracekit_report']['table']['group'] = t('TraceKit reports');

  $data['tracekit_report']['table']['base'] = array(
    'field' => 'tracekit_report_id',
    'title' => t('TraceKit reports'),
    'help' => t('Collected TraceKit client reports.'),
  );

  $data['tracekit_report']['tracekit_report_id'] = array(
    'title' => t('TraceKit report id'),
    'help' => t('The primary identifier for a report.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['tracekit_report']['mode'] = array(
    'title' => t('Mode'),
    'help' => t('Mode of the error.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['tracekit_report']['message'] = array(
    'title' => t('Message'),
    'help' => t('Message of the error.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['tracekit_report']['useragent'] = array(
    'title' => t('Useragent'),
    'help' => t('Useragent of the reporting user.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['tracekit_report']['created'] = array(
    'title' => t('Date'),
    'help' => t('The date of the report creation time.'),
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

  $data['tracekit_report']['uid'] = array(
    'title' => t('Reporting user'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Reporting user'),
      'title' => t('User who has reported the error.'),
    ),
  );

  return $data;
}
