<?php

/**
 * @file
 * API documentation for Single Page Site.
 */

/**
 * Example of how to use hook_single_page_site_output().
 */
function hook_single_page_site_output(&$output, $current_item_count) {
  // Alter the output of the current page.
  $output .= 'Attach string';
}
