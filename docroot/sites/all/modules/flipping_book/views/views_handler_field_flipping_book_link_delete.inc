<?php

/**
 * @file
 * Definition of views_handler_field_flipping_book_link_delete.
 */

/**
 * Field handler to present a link to delete a flipping_book.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_flipping_book_link_delete extends views_handler_field_flipping_book_link {

  /**
   * Renders the link.
   */
  function render_link($flipping_book, $values) {
    // Ensure user has access to delete this flipping_book.
    if (!user_access('administer flipping_book')) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = FLIPPING_BOOK_MANAGEMENT_PATH . "/$flipping_book->fbid/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    return $text;
  }
}
