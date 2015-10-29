<?php

/**
 * @file
 * Views integration for the bible module
 */

/**
 * Implements hook_views_data()
 *
 */
function bible_views_data(){
  $data = array();

  // Bible Context Table

  $data['bible_context']['table']['group'] = t('Bible');

  // Advertise this table as a possible base table
  $data['bible_context']['table']['base'] = array(
    'title' => t('Bible'),
  );

  $data['bible_context']['vsid'] = array(
    'title' => t('Verse ID'),
    'help' => t('The unique ID number of the verse.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_context']['bid'] = array(
    'title' => t('Bid'),
    'help' => t('The Book ID of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_context']['book'] = array(
    'title' => t('Book Code'),
    'help' => t('The Book code of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => FALSE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_context']['chapter'] = array(
    'title' => t('Chapter number'),
    'help' => t('The Chapter number of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_context']['verse'] = array(
    'title' => t('Verse number'),
    'help' => t('The Verse number of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_context']['context'] = array(
    'title' => t('Text'),
    'help' => t('The verse text of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => FALSE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // Bible List Table

  $data['bible_list']['table']['group'] = t('Bible');

  $data['bible_list']['bid'] = array(
    'title' => t('List Bid'),
    'help' => t('The Book ID of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_list']['bname'] = array(
    'title' => t('Bible Name'),
    'help' => t('The Bible Name of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_list']['lang'] = array(
    'title' => t('Bible Language'),
    'help' => t('The language of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_list']['serialversion'] = array(
    'title' => t('Bible Serial Version'),
    'help' => t('The Serial Version of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Bible Book Name Table
  $data['bible_book_name']['table']['group'] = t('Bible');

  $data['bible_book_name']['table']['join']['bible_list'] = array(
    'left_field' => 'bid',
    'field' => 'bid',
  );

  $data['bible_book_name']['bkid'] = array(
    'title' => t('Bkid'),
    'help' => t('The Book ID of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_book_name']['book'] = array(
    'title' => t('Book name Book Code'),
    'help' => t('The Book code of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => FALSE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_book_name']['fname'] = array(
    'title' => t('Book Long Name'),
    'help' => t('The Book name of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => FALSE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_book_name']['sname'] = array(
    'title' => t('Sname'),
    'help' => t('The Sname of the Bible loaded.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => FALSE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_book_name']['chap'] = array(
    'title' => t('Number of Chapters'),
    'help' => t('The number of chapters in the book.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for accepting a bid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
    ),
    // Information for accepting a bid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a bid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bible_list']['table']['join']['bible_context'] = array(
    'left_table' => 'bible_book_name',
    'left_field' => 'bid',
    'field' => 'bid',
  );

  $data['bible_book_name']['table']['join']['bible_context'] = array(
    'left_field' => 'book',
    'field' => 'book',
  );

  return $data;
}
