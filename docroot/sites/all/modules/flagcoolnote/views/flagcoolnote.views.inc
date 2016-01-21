<?php

/**
 * @file
 *   Provides views integration
 */

/**
 * Implements hook_views_data().
 */
function flagcoolnote_views_data() {
  // Describe table to use in views
  $data['flagcoolnote_submissions']['table']['group'] = t('FlagCoolNote');

  // Join with webform_submissions table to retrive results
  $data['flagcoolnote_submissions']['table']['join']['webform_submissions'] = array(
    'left_table' => 'webform_submissions',
    'left_field' => 'sid',
    'field' => 'sid',
  );

  // NID field
  $data['flagcoolnote_submissions']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'flagcoolnote_views_handler_field_node',
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
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

  // Add a view link of flagged node.
  $data['flagcoolnote_submissions']['view_node'] = array(
    'field' => array(
      'title' => t('View link'),
      'help' => t('Provide a simple link to view the flagged node.'),
      'handler' => 'flagcoolnote_views_handler_field_node_link',
      'link_type' => 'view',
    ),
  );

  // No need to add uid field because webform module already provide that

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function flagcoolnote_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'flagcoolnote') . '/views',
    ),
    'handlers' => array(
      'flagcoolnote_views_handler_field_node' => array(
        'parent' => 'views_handler_field',
        'file' => 'flagcoolnote_views_handler_field_node.inc',
      ),
      'flagcoolnote_views_handler_field_node_link' => array(
        'parent' => 'views_handler_field',
        'file' => 'flagcoolnote_views_handler_field_node_link.inc',
      ),
    ),
  );
}