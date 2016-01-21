<?php

/**
 * @file pieces.views.inc
 * Provides support for the Mosaik Pieces module.
 */

/**
 * Implements hook_views_data
 */
function pieces_views_data() {
  $data = array();

  $data['pieces']['table']['group'] = t('Pieces');
  $data['pieces']['table']['base'] = array(
    'field' => 'name',
    'title' => t('Piece'),
    'description' => t('This tables contains info about your Mosaik Pieces.'),
    'weight' => -10,
  );

  $data['pieces']['table']['default_relationship'] = array(
    'mosaik' => array(
      'table' => 'mosaik',
      'field' => 'name',
    ),
  );

  // Mosaik Pieces name
  $data['pieces']['name'] = array(
    'title' => t('Piece Name'),
    'help' => t('The mosaik piece Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Mosaik Piece Title
  $data['pieces']['title'] = array(
    'title' => t('Piece Title'),
    'help' => t('The Mosaik Piece title'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['pieces']['edit_piece'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to the piece edit page.'),
      'handler' => 'views_handler_field_piece_link_edit',
    )
  );

  $data['pieces']['delete_piece'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to the piece delete page.'),
      'handler' => 'views_handler_field_piece_link_delete',
    )
  );

  //and last but not least...a little lie ^^" forgive me
  $data['pieces']['table']['entity type'] = 'node';

  return $data;
}