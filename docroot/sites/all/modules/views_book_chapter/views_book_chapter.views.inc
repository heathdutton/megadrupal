<?php
/**
 * @file
 * Default views file.
 */

/**
 * Implements hook_views_data().
 */
function views_book_chapter_views_data() {

  $data = array();

  $data['book_chapter']['table']['group'] = t('Book');
  $data['book_chapter']['table']['join'] = array(
    'node' => array(
      'table'      => 'menu_links',
      'left_table' => 'book',
      'left_field' => 'mlid',
      'field'      => 'mlid',
    ),
  );

  $data['book_chapter']['mlid'] = array(
    'title'  => t('Chapter'),
    'help'   => t('The chapter of the book.'),
    'filter' => array(
      'handler' => 'views_book_chapter_handler_filter_book_chapters',
    ),
  );

  return $data;
}
