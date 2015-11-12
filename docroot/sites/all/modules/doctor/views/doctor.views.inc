<?php
/**
 * @file
 * Views integration for the Doctor module.
 */

/**
 * Implements hook_views_data()
 */
function doctor_views_data() {
  $data['doctor']['table']['group']  = t('Doctor');

  $data['doctor']['table']['base'] = array(
    'title' => t('Doctor'),
    'help' => t('A Doctor listen items.'),
  );

  $data['doctor']['id'] = array(
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

  $data['doctor']['type'] = array(
    'title' => t('Bundle'),
    'help' => t('The bundle of the doctor listen.'),
    'field' => array(
      'handler' => 'doctor_handler_bundle_formatter',
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

  $data['doctor']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the doctor listen item was created.'),
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

  $data['doctor']['data'] = array(
    'title' => t('Data'),
    'help' => t('The doctor listen item data.'),
    'field' => array(
      'handler' => 'doctor_handler_data_formatter',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  return $data;
}
