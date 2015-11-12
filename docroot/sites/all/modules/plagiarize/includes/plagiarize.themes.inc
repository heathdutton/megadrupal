<?php
/**
 * @file
 * Theme functions for the plagiarize module.
 */
/**
 * Theme the summary list that can be placed into a node body.
 */
function theme_plagiarize_summary_list($vars) {
  return '<div class="embedded-summary-list">' . theme('item_list', $vars) . '</div>';
}
