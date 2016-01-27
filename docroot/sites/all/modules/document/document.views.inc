<?php

/**
 * This file is used to tell the views module about the new Document table.
 */
function document_views_data() {
  // Basic table information.

  // ----------------------------------------------------------------
  // document table
  //  New group within Views called 'Document'
  //  The group will appear in the UI in the dropdown tha allows you
  //  to narrow down which fields and filters are available.


  $data = array();
  $data['document']['table']['group'] = t('Document');

  // Let Views know that our table joins to the 'node'
  // base table. This means it will be available when listing
  // nodes and automatically make its fields appear.
  // We also show up for node revisions.
  $data['document']['table']['join'] = array(
      'node_revisions' => array(
          'type' => 'INNER',
          'left_field' => 'vid',
          'field' => 'vid'),
      'node' => array(
          'type' => 'INNER',
          'left_field' => 'vid',
          'field' => 'vid'));

  // Document Type
  $data['document']['type'] = array(
      'title' => t('Document Type'),
      'help' => t('Document Type.'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE),
      'filter' => array(
          'handler' => 'views_handler_filter_string'),
      'argument' => array(
          'handler' => 'views_handler_argument_string'),
      'sort' => array('handler' => 'views_handler_sort'));

  // Author
  $data['document']['author'] = array(
      'title' => t('Document Author'),
      'help' => t('Document Author.'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE),
      'filter' => array(
          'handler' => 'views_handler_filter_string'),
      'argument' => array(
          'handler' => 'views_handler_argument_string'),
      'sort' => array('handler' => 'views_handler_sort'));

  // Publish Year
  $data['document']['publish_year'] = array(
      'title' => t('Year of Publication'),
      'help' => t('Year of Publication.'),
      'field' => array(
          'handler' => 'views_handler_field_numeric',
          'click sortable' => TRUE),
      'filter' => array(
          'handler' => 'views_handler_filter_numeric'),
      'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE),
      'sort' => array('handler' => 'views_handler_sort'));

  // Keywords
  $data['document']['keywords'] = array(
      'title' => t('Keywords'),
      'help' => t('Document Keywords.'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE),
      'filter' => array(
          'handler' => 'views_handler_filter_string'),
      'argument' => array(
          'handler' => 'views_handler_argument_string'),
      'sort' => array('handler' => 'views_handler_sort'));

  // Url
  $data['document']['url'] = array(
      'title' => t('Document Url'),
      'help' => t('Document Url.'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => FALSE));

  // External
  $data['document']['external'] = array(
      'title' => t('External'),
      'help' => t('Is External Document.'),
      'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE),
      'filter' => array(
          'handler' => 'views_handler_filter_numeric'),
      'argument' => array(
          'handler' => 'views_handler_argument_numeric'),
      'sort' => array('handler' => 'views_handler_sort'));

  return $data;
}
