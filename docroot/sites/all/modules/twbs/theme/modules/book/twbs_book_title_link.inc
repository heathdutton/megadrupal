<?php

/**
 * Returns HTML for a link to a book title when used as a block title.
 *
 * @param $variables
 *   An associative array containing:
 *   - link: An array containing title, href and options for the link.
 *
 * @ingroup themeable
 */
function twbs_book_title_link($variables) {
  $link = $variables['link'];

  $link['options']['attributes']['class'] = array('book-title');

  return l($link['title'], $link['href'], $link['options']);
}
