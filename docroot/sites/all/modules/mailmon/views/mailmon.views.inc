<?php

/**
 * @file
 * Views integration and data for the mailmon module.
 */

/**
 * Implements hook_views_data().
 */
function mailmon_views_data() {
  // Basic table information.
  $data['mailmon']['table']['group'] = t('Mailmon');
  
  // Advertise this table as a possible base table
  $data['mailmon']['table']['base'] = array(
    'field' => 'maildate',
    'title' => t('Mail Monitor'),
    'help' => t('Lists dates and quantities of emails sent by the system.'),
    'weight' => -15,
  );
  
  // {mailmon}.maildate
  $data['mailmon']['maildate'] = array(
    'title' => t('Date'),
    'help' => t('Date mails were sent in YYYY-MM-DD.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  // {mailmon}.mailcount
  $data['mailmon']['mailcount'] = array(
    'title' => t('Count'),
    'help' => t('Count of mails sent that day.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  
  return $data;
}