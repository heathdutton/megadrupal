<?php

/**
 * @file
 * Integrate custom table into views
 */

/**
 * Implements hook_views_data().
 */
function bounce_convert_views_data() {
  $data = array();
  $data['bounce_convert_impression']['table']['group'] = t('Bounce Convert Campaign');

  $data['bounce_convert_impression']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'campaign_id',
    ),
  );

  $data['bounce_convert_impression']['campaign_id'] = array(
    'title' => t('Campaign node ID'),
    'help' => t('Campaign node ID related impression/conversion'),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'handler' => 'views_handler_relationship',
      'skip base' => 'node',
    ),
  );
  $data['bounce_convert_impression']['webform_id'] = array(
    'title' => t('Webform node ID'),
    'help' => t('Webform node ID related impression/conversion'),
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
  );
  $data['bounce_convert_impression']['sid'] = array(
    'title' => t('Sumbmission ID'),
    'help' => t('Submission ID related impression/conversion'),
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
  );

  $data['bounce_convert_impression']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('User ID {uid} of a user for impression/conversion'),
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
  );
  $data['bounce_convert_impression']['impression'] = array(
    'title' => t('Impression Status'),
    'help' => t('Impression status for tracking record'),
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
  );
  $data['bounce_convert_impression']['conversion'] = array(
    'title' => t('Conversion Status'),
    'help' => t('Conversion status whether convert or not'),
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
  );
  $data['bounce_convert_impression']['ip_address'] = array(
    'title' => t('IP address of a user who visit the page.'),
    'help' => t('Conversion'),
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
  $data['bounce_convert_impression']['path'] = array(
    'title' => t('Internal path'),
    'help' => t('Internal path to page visited (relative to Drupal root.)'),
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

  $data['bounce_convert_impression']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('Timestamp of when the page was visited.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['bounce_convert_impression']['id'] = array(
    'title' => t('Unique identifier for impression'),
    'help' => t('Primary key for table'),
    'field' => array('handler' => 'views_handler_field'),
    'argument' => array('handler' => 'views_handler_argument_numeric'),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'sort' => array('handler' => 'views_handler_sort_numeric'),
  );

  return $data;
}
