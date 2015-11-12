<?php

/**
 * @file
 * Views integration file for node accessibility.
 */

/**
 * Implementation of hook_views_handlers().
 */
function node_accessibility_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'node_accessibility') . '/includes',
    ),
    'handlers' => array(),
  );
}

/**
 * Implements hook_views_data()
 */
function node_accessibility_views_data() {
  $data['node_accessibility_problems']['table']['group'] = t("Accessibility Validation Problem");

  $data['node_accessibility_problems']['table']['join'] = array(
    'node'  => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'quail_api_tests' => array('left_field' => 'id', 'field' => 'test_id'),
  );

  $data['node_accessibility_problems']['id'] = array(
    'title' => t("ID"),
    'help' => t("The unique accessibility validation problem ID."),
    'field'  => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort'   => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'argument' => array('handler' => 'views_handler_argument_numeric'),
  );

  $data['node_accessibility_problems']['test_id'] = array(
    'title'    => t("Test ID"),
    'help'     => t("The unique ID of an accessibility validation test."),
    'relationship' => array(
      'base'       => 'quail_api_tests',
      'base field' => 'id',
      'field'      => 'test_id',
      'handler'    => 'views_handler_relationship',
      'label'      => t("Test ID"),
    ),
  );

  $data['node_accessibility_problems']['line'] = array(
    'title' => t("Line"),
    'help' => t("The line number associated with the accessibility validation problem."),
    'field'  => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort'   => array('handler' => 'views_handler_sort'),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'argument' => array('handler' => 'views_handler_argument_numeric'),
  );

  $data['node_accessibility_problems']['element'] = array(
    'title' => t("Snippet"),
    'help' => t("The snippet of markup associated with the accessibility validation problem."),
    'field'  => array(
      'handler' => 'quail_api_handler_field_markup',
    ),
    'filter' => array('handler' => 'views_handler_filter_string'),
  );

  $data['node_accessibility_problems']['validate_link'] = array(
    'field' => array(
      'title' => t("Validate Link"),
      'help' => t("Provide a simple link to the node accessibility validate page."),
      'handler' => 'node_accessibility_handler_link_validate_node',
      'path' => drupal_get_path('module', 'node_accessibility') . '/includes',
    ),
  );

  return $data;
}
