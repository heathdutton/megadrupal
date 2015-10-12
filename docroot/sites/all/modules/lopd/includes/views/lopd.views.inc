<?php

/**
 * @file
 * Views interface for Spain LOPD.
 */

function lopd_views_data() {
  $data = array();
  $data['lopd']['table'] = array(
    'group' => t("LOPD"),
    'title' => t("LOPD"),
    'help' => t("Shows LOPD operations registered."),
  );
  $data['lopd']['table']['base'] = array(
    'field' => 'lopdid',
    'title' => 'LOPD registers',
    'weight' => 10,
  );

  // This table references the {user} table.
  // This creates an 'implicit' relationship to the node table, so that when 'User'
  // is the base table, the fields are automatically available.
  $data['lopd']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Next, describe each of the individual fields in this table to Views. For
  // each field, you may define what field, sort, argument, and/or filter
  // handlers it supports. This will determine where in the Views interface you
  // may use the field.
  $data['lopd']['lopdid'] = array(
    'title' => t('LOPD ID'),
    'help' => t('The LOPD ID of the register.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['lopd']['uid'] = array(
    'title' => t('User'),
    'help' => t('User that did the operation.'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User'),
    ),
  );
  $data['lopd']['authname'] = array(
    'title' => t('Name'),
    'help' => t('Authentication name that did this operation.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
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
  $data['lopd']['ip'] = array(
    'title' => t('IP'),
    'help' => t('IP address that did this operation.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
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
  $data['lopd']['operation'] = array(
    'title' => t('Operation'),
    'help' => t('Operation that has been registered.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
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
  $data['lopd']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('The timestamp where the operation was registered.'),
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
  return $data;
}
