<?php
/**
 * @file
 * Views related functionality.
 */

/**
 * Implements hook_views_data_alter().
 */
function vhfnioo_views_data_alter(array &$data) {
  array_walk_recursive($data, function (&$value) {
    // @codingStandardsIgnoreStart
    if ('views_handler_filter_numeric' === $value) {
      $value .='_ioo';
    }
    // @codingStandardsIgnoreEnd
  });
}
