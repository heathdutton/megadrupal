<?php

/**
 * @file
 * Mirrors entity plugin declaration.
 */

/**
 * Entity Declaration: node.
 */
$plugin = array(
  'title' => t('Node'),
  'category' => 'Entity',
  'views' => array(
    'base_table' => 'node',
    'filters' => array(
      'type' => 'node',
    ),
    'sorts' => array(
      'nid' => 'node',
    ),
  ),
  'feeds' => array(
    'processor' => 'FeedsNodeProcessor',
  ),
  'properties' => array(
    'nid' => array(
      'type' => 'integer',
      'feeds' => array(
        'unique' => TRUE,
      ),
    ),
    'title' => array(
      'type' => 'text',
      'views' => array(
        'link_to_node' => FALSE,
      ),
    ),
    'uid' => array(
      'type' => 'text',
      'views' => array(
        'link_to_user' => FALSE,
      ),
    ),
    'status' => array(
      'type' => 'boolean',
    ),
    'created' => array(
      'type' => 'date',
    ),
    'promote' => array(
      'type' => 'boolean',
    ),
    'sticky' => array(
      'type' => 'boolean',
    ),
  ),
);
