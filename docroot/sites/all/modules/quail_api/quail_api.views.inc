<?php

/**
 * @file
 * Views integration file for the quail api.
 */

/**
 * Implements hook_views_handlers().
 */
function quail_api_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'quail_api') . '/includes',
    ),
    'handlers' => array(),
  );
}

/**
 * Implements hook_views_data().
 */
function quail_api_views_data() {
  $data['quail_api_tests']['table']['group'] = t("Accessibility Validation Test");

  $data['quail_api_tests']['id'] = array(
    'title' => t("ID"),
    'help' => t("The unique accessibility validation test ID."),
    'field'  => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort'   => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'argument' => array('handler' => 'views_handler_argument_numeric'),
  );

  $data['quail_api_tests']['severity'] = array(
    'title' => t("Severity"),
    'help' => t("A number that represents the severity of the accessibility validation test. This is called a display level by the quail library."),
    'field'  => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort'   => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'argument' => array('handler' => 'views_handler_argument_numeric'),
  );

  $data['quail_api_tests']['machine_name'] = array(
    'title' => t("Machine Name"),
    'help' => t("A machine friendly name for the accessibility validation test."),
    'field'  => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort'   => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'argument' => array('handler' => 'views_handler_argument_string'),
  );

  $data['quail_api_tests']['human_name'] = array(
    'title' => t("Human Name"),
    'help' => t("A human friendly name for the accessibility validation test. Consider this the title of the test."),
    'field'  => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort'   => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_string'),
    'argument' => array('handler' => 'views_handler_argument_string'),
  );

  $data['quail_api_tests']['description'] = array(
    'title' => t("Description"),
    'help' => t("The description of the accessibility validation test."),
    'field'  => array(
      'handler' => 'quail_api_handler_field_markup',
    ),
    'filter' => array('handler' => 'views_handler_filter_string'),
  );

  return $data;
}
