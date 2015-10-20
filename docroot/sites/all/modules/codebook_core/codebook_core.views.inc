<?php

/**
 * @file
 * Views integration for Codebook Core.
 */

/**
 * Implements hook_views_data().
 */
function codebook_core_views_data() {
  $data['views']['codebook_core_node_add'] = array(
    'title' => t('Codebook node add links'),
    'help' => t('Provide links to add Codebook content.'),
    'area' => array(
      'handler' => 'codebook_core_views_handler_area_node_add',
    ),
  );
  return $data;
}
