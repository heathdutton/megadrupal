<?php

/**
 * @file mosaik.views.inc
 * Provides support for the Mosaik module.
 */

/**
 * Implements hook_views_data
 */
function mosaik_views_data() {
  $data = array();

  $data['mosaik']['table']['group'] = t('Mosaik');
  $data['mosaik']['table']['base'] = array(
    'field' => 'name',
    'title' => t('Mosaik'),
    'description' => t('This tables contains info about your Mosaiks.'),
    'weight' => -10,
  );

  $data['mosaik']['table']['default_relationship'] = array(
    'mosaik_pieces' => array(
      'table' => 'mosaik_pieces',
      'field' => 'mosaik_name',
    ),
  );

  // Table Joins
  $data['mosaik']['table']['join'] = array(
    'mosaik_pieces' => array(
      'left_field' => 'mosaik_name',
      'field' => 'name',
    ),
  );

  // Mosaik Name
  $data['mosaik']['name'] = array(
    'title' => t('Mosaik Name'),
    'help' => t('The mosaik Name'),
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
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'label' => t('Mosaik Pieces'),
      'base' => 'mosaik_pieces',
      'base field' => 'mosaik_name',
      'skip base' => array(),
    ),
  );

  // Mosaik title
  $data['mosaik']['title'] = array(
    'title' => t('Title'), // The item it appears as on the UI,
    'help' => t('The Mosaik title'), // The help that appears on the UI,
    // Information for displaying a title as a field
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

  // Mosaik title
  $data['mosaik']['path'] = array(
    'title' => t('Path'), // The item it appears as on the UI,
    'help' => t('The Mosaik path'), // The help that appears on the UI,
    // Information for displaying a title as a field
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

  // Mosaik menu
  $data['mosaik']['menu'] = array(
    'title' => t('Menu'),
    'help' => t('Whether or not the mosaik has a menu entry.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'published-notpublished' => array(t('With Menu'), t('Without Menu')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Menu'),
      'type' => 'yes-no',
      'use equal' => TRUE,
      // Use status = 1 instead of status <> 0 in WHERE statment
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Mosaik sidebars
  $data['mosaik']['sidebars'] = array(
    'title' => t('Hide Sidebars'),
    'help' => t('Whether or not the mosaik has sidebars.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'published-notpublished' => array(
          t('With sidebars'),
          t('Without sidebars')
        ),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Sidebars'),
      'type' => 'yes-no',
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Mosaik title
  $data['mosaik']['layout'] = array(
    'title' => t('Layout'), // The item it appears as on the UI,
    'help' => t('The Mosaik layout'), // The help that appears on the UI,
    // Information for displaying a title as a field
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


  // Define some fields based upon views_handler_field_entity in the entity
  // table so they can be re-used with other query backends.
  // @see views_handler_field_entity

  $data['mosaik']['view_mosaik'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the mosaik.'),
      'handler' => 'views_handler_field_mosaik_link',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['mosaik']['edit_mosaik'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to the mosaik edit page.'),
      'handler' => 'views_handler_field_mosaik_link_edit',
    ),
  );

  $data['mosaik']['delete_mosaik'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to the mosaik delete page.'),
      'handler' => 'views_handler_field_mosaik_link_delete',
    ),
  );

  $data['mosaik']['clone_mosaik'] = array(
    'field' => array(
      'title' => t('Clone link'),
      'help' => t('Provide a simple link to the mosaik cloning page.'),
      'handler' => 'views_handler_field_mosaik_link_clone',
    ),
  );

  $data['mosaik']['mosaik_pieces'] = array(
    'field' => array(
      'title' => t('Pieces'),
      'help' => t('Provide a simple link to mosaik pieces.'),
      'handler' => 'views_handler_field_mosaik_link_pieces',
    ),
  );


  //and last but not least...a little lie ^^" forgive me
  $data['mosaik']['table']['entity type'] = 'node';

  $data['mosaik_pieces']['table']['group'] = t('Mosaik Pieces');
  $data['mosaik_pieces']['table']['base'] = array(
    'field' => 'name',
    'title' => t('Mosaik pieces'),
    'description' => t('This tables contains Mosaik Pieces data.'),
    'weight' => -10,
  );

  // Table Joins
  $data['mosaik_pieces']['table']['join'] = array(
    'mosaik' => array(
      'left_field' => 'name',
      'field' => 'mosaik_name',
    ),
  );

  return $data;
}
