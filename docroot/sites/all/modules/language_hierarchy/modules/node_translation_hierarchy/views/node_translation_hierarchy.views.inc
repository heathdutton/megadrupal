<?php

/**
 * Implements hook_views_data_alter().
 */
function node_translation_hierarchy_views_data_alter(&$data) {
  // Content: Language
  $data['node']['language']['filter']['handler'] = 'language_hierarchy_views_handler_filter_node_language';
}
