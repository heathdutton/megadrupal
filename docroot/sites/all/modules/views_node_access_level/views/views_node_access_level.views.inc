<?php

/**
 * Implements hook_views_data_alter().
 */
function views_node_access_level_views_data_alter(&$views_data) {
  $views_data['node_access']['level'] = array(
    'title' => t('Access Level'),
    'help' => t('Filter by access.'),
    'real field' => 'nid',
    'filter' => array(
      'handler' => 'views_handler_filter_node_access_level',
      'help' => t('Filter for content by node access level.'),
    ),
  );
}
