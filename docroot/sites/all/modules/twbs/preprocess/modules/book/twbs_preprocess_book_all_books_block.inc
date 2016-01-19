<?php

/**
 * Processes variables for book-all-books-block.tpl.php.
 *
 * All non-renderable elements are removed so that the template has full access
 * to the structured data but can also simply iterate over all elements and
 * render them (as in the default template).
 *
 * @param $variables
 *   An associative array containing the following key:
 *   - book_menus
 *
 * @see book-all-books-block.tpl.php
 */
function twbs_preprocess_book_all_books_block(&$variables) {
  // Remove all non-renderable elements.
  $elements = $variables['book_menus'];
  $variables['book_menus'] = array();
  foreach (element_children($elements) as $index) {
    $variables['book_menus'][$index] = $elements[$index];
  }
}
