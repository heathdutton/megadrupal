<?php

/**
 * @file
 * Admin page callbacks for the booknavigationblocks module.
 */


/**
 * Builds and returns the bookblock settings form.
 *
 * @ingroup forms
 */
function bookblock_admin_settings($form, &$form_state) {
  $books = book_get_books();
  $bookblock_books = variable_get('bookblock_books', array());
  if ($books) {
    foreach ($books as $book) {
      // Sanitize the title, as unsanitized data is returned bu book_get_books.
      $book['title'] = check_plain($book['title']);
      if (!$book['has_children']) {
        $book['title'] = $book['title'] . ' <em>(' . t('no child pages') . ')</em>';
      }
      if (in_array($book['nid'], $bookblock_books)) {
        $book['title'] = $book['title'] . ' ' . l('configure', 'admin/structure/block/manage/bookblock/'. $book['nid'] .'/configure', array('query' => array('destination' => 'admin/content/book/blocks')));
      }
      $books[$book['nid']] = $book['title'];
    }
    $form['bookblock_books'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Generate a navigation block for each of the following books'),
      '#default_value' => $bookblock_books,
      '#options' => $books,
      '#description' => t('For each book that you select, a separate navigation block will be created. You can enable these blocks on the blocks administration page or use the context module.'),
    );
    $form['array_filter'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
    return system_settings_form($form);
  }
  else {
    drupal_set_message(t('No books have been created yet.'));
  }
}


// Need to add a submit handler to remove from the database any blocks which have been unset
// on this admin page.

// Collect together the $bids that have been unset
// and send them off to db_delete.

// if ($bids) {
//   db_delete('block')
//     ->condition('bid', $bids, 'NOT IN')
//     ->condition('theme', $theme)
//     ->execute();
// }