<?php

/**
 * @file
 * Views integration for the jquery_booklet module.
 */

/**
 * Implements hook_views_data().
 */
function jquery_booklet_views_data() {

  $data['booklets'] = array(
    'table' => array(
      'base' => array(
        'field' => 'bid',
        'title' => t('Booklets'),
        'help' => t('Booklets created by users'),
      ),
      'group' => 'Booklets',
    ),
    'bid' => array(
      'title' => t('Bid'),
      'help' => t('The unique id of the booklet'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
    'booklet_title' => array(
      'title' => t('Booklet Title'),
      'help' => t('The title of the booklet.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
    ),
    'front_cover_fid' => array(
      'title' => t('Front Cover Fid'),
      'help' => t('The file id of the front cover image.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
    'back_cover_fid' => array(
      'title' => t('Back Cover Fid'),
      'help' => t('The file id of the back cover image.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
    'width' => array(
      'title' => t('Width'),
      'help' => t('The width of the booklet.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
    'height' => array(
      'title' => t('Height'),
      'help' => t('The height of the booklet.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
    'settings' => array(
      'title' => 'Settings',
      'help' => 'Various settings related to booklet.',
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => FALSE,
      ),
    ),
    'created' => array(
      'title' => t('Created'),
      'help' => t('The Unix timestamp when the booklet was created.'),
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
    ),
  );
  $data['booklet_pages'] = array(
    'table' => array(
      'base' => array(
        'field' => 'bpid',
        'title' => t('Booklet Pages'),
        'help' => t('The pages in the booklet'),
      ),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
        ),
        'booklets' => array(
          'left_field' => 'bid',
          'field' => 'bid',
        ),
      ),
      'group' => 'Booklet Pages',
    ),
    'bpid' => array(
      'title' => t('Bpid'),
      'help' => t('The unique id of the booklet page.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
    'bid' => array(
      'title' => t('Bid'),
      'help' => t('The booket this page belongs to.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
      'relationship' => array(
        'base' => 'booklets',
        'base field' => 'bid',
        'handler' => 'views_handler_relationship',
        'label' => t('Booklet'),
        'title' => t('Booklet'),
        'help' => t('Relates booklet with booklet pages.'),
      ),
    ),
    'nid' => array(
      'title' => t('Nid'),
      'help' => t('The node.nid this record tracks.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
      'relationship' => array(
        'base' => 'node',
        'base field' => 'nid',
        'handler' => 'views_handler_relationship',
        'label' => t('Booklet Page Node'),
        'title' => t('Node of the booklet page'),
        'help' => t('Relates node with the booklet page.'),
      ),
    ),
    'weight' => array(
      'title' => t('Weight'),
      'help' => t('The weight of the page.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => FALSE,
      ),
    ),
  );

  return $data;
}
