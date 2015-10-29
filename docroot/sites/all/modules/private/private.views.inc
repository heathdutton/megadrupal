<?php

/**
 * @file
 * Views integration functions for the private module.
 */

/**
 * Implements hook_views_data().
 */
function private_views_data() {
  $data = array();

  $data['private']['table']['group'] = t('Content');

  $data['private']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'private' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['private']['private'] = array(
    'title' => t('Private'),
    'help' => t('Whether or not the node is private.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'private_handler_filter_private',
      'label' => t('Private'),
      'type' => 'yes-no',
    ),
  );

  return $data;

}

/**
 * Implements hook_views_handlers().
 */
function private_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'private'),
    ),
    'handlers' => array(
      'private_handler_filter_private' => array(
        'parent' => 'views_handler_filter_boolean_operator',
        'file' => 'private_handler_filter_private.inc',
      ),
    ),
  );
}
