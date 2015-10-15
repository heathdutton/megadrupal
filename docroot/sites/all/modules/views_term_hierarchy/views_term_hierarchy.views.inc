<?php

/**
 * Implements hook_views_data_alter()
 */
function views_term_hierarchy_views_data_alter(&$data) {
  $data['taxonomy_term_data']['hierarchy_sort'] = array(
    'title' => t('Term hierarchy'),
    'help' => t('Sort by term hierarchy.'),
    'sort' => array(
      'handler' => 'views_term_hierarchy_sort',
    ),
  );
}
