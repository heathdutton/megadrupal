<?php
/**
 * @file
 * Template for Top searches block
 *
 * Variables
 * - $ar_scores: an array of links to saved searches
 * - $display: 'list' or 'implode'
 * - $separator: the separator to use in 'implode' display mode
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

if ($display == 'list') {
  echo theme('item_list', array('items' => $scores));
}
else {
  echo implode($separator, $scores);
}
