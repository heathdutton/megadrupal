<?php

/**
 * @file
 * Provide views data and handlers for Variable log.
 */

/**
 * Implements hook_views_data().
 */
function variable_log_views_data() {
  $data = array();

  $data['variable_log']['table']['group']  = t('Variable log');

  $data['variable_log']['table']['base'] = array(
    'title' => t('Variable log'),
    'access query tag' => 'spinup_order_access',
  );

  // Id field.
  $data['variable_log']['id'] = array(
    'title' => t('Record ID'),
    'help' => t('The unique internal identifier of the record.'),
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

  // Uid field.
  $data['variable_log']['uid'] = array(
    'title' => t('User'),
    'help' => t('user who updated the variable'),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'relationship' => array(
      'title' => t('User'),
      'help' => t('Relate record to the user who updated it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('user'),
    ),
  );

  // Name field.
  $data['variable_log']['name'] = array(
    'title' => t('Variable'),
    'help' => t('The variable name.'),
    'field' => array(
      'handler' => 'variable_log_handler_field_name',
      'click sortable' => TRUE,
      'link_to_log' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Old value field.
  $data['variable_log']['old_value'] = array(
    'title' => t('Old value'),
    'help' => t('The value of variable before update.'),
    'field' => array(
      'handler' => 'views_handler_field_serialized',
    ),
  );

  // Value field.
  $data['variable_log']['value'] = array(
    'title' => t('Value'),
    'help' => t('The value of variable after update.'),
    'field' => array(
      'handler' => 'views_handler_field_serialized',
    ),
  );

  // Timestamp field.
  $data['variable_log']['timestamp'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the record was updated.'),
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

  if (module_exists('variable_store')) {
    $data['variable_log']['options'] = array(
      'title' => t('Options'),
      'help' => t('The options of this variable.'),
      'field' => array(
        'handler' => 'variable_log_handler_field_options',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );
  }

  $data['variable_log']['clear_form'] = array(
    'title' => t('Clear form'),
    'help' => t('Displays the form for clear log'),
    'area' => array(
      'handler' => 'variable_log_handler_area_clear_form',
    ),
  );

  return $data;
}
