<?php
/**
 * Preview API Views API.
 */

/**
 * Implements hook_views_data().
 */
function preview_api_views_data() {
  $data['node']['headless_preview_node']['moved to'] = array('views_entity_node', 'preview_api_node');
  $data['views_entity_node']['preview_api_node'] = array(
    'field' => array(
      'title' => t('Preview API'),
      'help' => t('Provide a simple link to preview.'),
      'handler' => 'views_handler_field_node_link_preview_api',
    ),
  );
  return $data;
}
