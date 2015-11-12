<?php
/**
 * @file
 * current-search-group-title.theme.inc
 */


/**
 * Theme the 'current search group title' for search blocks.
 *
 * @param array $variables
 * @return string
 */
function wetkit_omega_current_search_group_title(array $variables) {
  return '<h3 class="background-light current-search-group-title">' . $variables['title'] . '</h3>';
}
