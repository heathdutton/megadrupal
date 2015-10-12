<?php

/**
 * @file
 * Hooks invoked by the Project Source API.
 */


/**
 * Implements hook_project_src_info_alter().
 */
function project_src_test_alter_project_src_info_alter(&$projects) {
  if (isset($projects['7.x']['drewpaul_inc_theme']['title'])) {
    $projects['7.x']['drewpaul_inc_theme']['title'] .= ' Altered';
  }
}


/**
 * Implements hook_project_src_releases_alter().
 */
function project_src_test_alter_project_src_releases_alter(&$releases, $info) {
  if (isset($releases['7.x-1.1']['name'])) {
    $releases['7.x-1.1']['name'] .= ' Altered';
  }
}


/**
 * Implements hook_project_src_terms_alter().
 */
function project_src_test_alter_project_src_terms_alter(&$terms, $info, $type) {
  if (is_array($terms)) {
    $terms[] = array('Added term name', 'Added term value');
  }
}
