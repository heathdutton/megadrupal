<?php
/**
 * @file
 * Template for Latest searches block
 *
 * Variables:
 * - $links: an array of links to saved searches
 * - $display: 'list' or 'implode'
 * - $separator: the separator to use in 'implode' display mode
 *
 * Variables unlikely be used:
 * - $count: the number of items in the search data list
 * - $items: the unformatted search data list
 * - $nofollow: the rel=nofollow status
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

if ($display == 'list') {
  echo theme('item_list', array('items' => $links));
}
else {
  echo implode($separator, $links);
}
