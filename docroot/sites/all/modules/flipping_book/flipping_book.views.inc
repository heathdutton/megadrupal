<?php

/**
 * @file
 * Manages integration between FlippingBook schema and Views
 */

/**
 * Implements hook_views_data().
 */
function flipping_book_views_data() {

  $data = array();

  $data['flipping_book']['table']['group'] = t('Flipping Book');
  $data['flipping_book']['table']['base'] = array(
    // This is the identifier field for the view.
    'field' => 'fbid',
    'title' => t('Flipping Book'),
    'help' => t('Flipping Book table contains data regarding Flipping Books imported into the site.'),
    'weight' => -10,
  );

  // Fbid.
  $data['flipping_book']['fbid'] = array(
    'title' => t('Fbid'),
    // The help that appears on the UI.
    'help' => t('The flipping_book ID.'),
    // Information for displaying the nid.
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting a fbid as an argument.
    'argument' => array(
      'handler' => 'views_handler_argument_flipping_book_fbid',
      // The field to display in the summary.
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'fbid',
    ),
    // Information for accepting a nid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Title.
  $data['flipping_book']['title'] = array(
    // The item it appears as on the UI.
    'title' => t('Title'),
    // The help that appears on the UI.
    'help' => t('The flipping_book title.'),
    // Information for displaying a title as a field.
    'field' => array(
      'field' => 'title',
      // The real field. This could be left out since it is the same.
      'group' => t('Flipping Book'),
      // The group it appears in on the UI. Could be left out.
      'handler' => 'views_handler_field_flipping_book',
      'click sortable' => TRUE,
      'link_to_node default' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Directory.
  $data['flipping_book']['dir'] = array(
    'title' => t('Directory'),
    // The item it appears as on the UI.
    'help' => t('The flipping_book directory.'),
    // The help that appears on the UI.
    // Information for displaying a title as a field.
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Created field.
  $data['flipping_book']['created'] = array(
    'title' => t('Post date'),
    // The item it appears as on the UI.
    'help' => t('The date the flipping_book was posted.'),
    // The help that appears on the UI.
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

  // Define some fields based upon views_handler_field_entity in the entity
  // table so they can be re-used with other query backends.
  // @see views_handler_field_entity
  $data['flipping_book']['view_flipping_book'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the flipping_book.'),
      'handler' => 'views_handler_field_flipping_book_link',
    ),
  );

  $data['flipping_book']['delete_flipping_book'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the flipping_book.'),
      'handler' => 'views_handler_field_flipping_book_link_delete',
    ),
  );

  // And last but not least...a little lie ^^" forgive me.
  $data['flipping_book']['table']['entity type'] = 'node';

  return $data;
}
