<?php

/**
 * @file
 * Views hooks for Node Compare.
 */

/**
 * Implements hook_views_data_alter().
 */
function node_compare_views_data_alter(&$data) {
  $data['node']['node_compare_link'] = array(
    'title' => t('“Add to Compare” link'),
    'help' => t('Link to add node to compare.'),
    'group' => t('Content'),
    'field' => array(
      'additional fields' => array(
        'nid' => array(
          'table' => 'node',
          'field' => 'nid',
        ),
        'type' => array(
          'table' => 'node',
          'field' => 'type',
        ),
      ),
      'handler' => 'node_compare_handler_field_compare_link',
    ),
  );
}
