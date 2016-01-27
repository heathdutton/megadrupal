<?php

/**
 * Implements hook_views_data().
 */
function node_field_views_data() {
  $data = array();
  $data['node']['node_fields'] = array(
    'title' => t('Node fields'),
    'real field' => 'nid',
    'help' => t('All node fields of the node.'),
    'field' => array(
      'handler' => 'node_field_handler_field_node_field',
    ),
  );
  return $data;
}