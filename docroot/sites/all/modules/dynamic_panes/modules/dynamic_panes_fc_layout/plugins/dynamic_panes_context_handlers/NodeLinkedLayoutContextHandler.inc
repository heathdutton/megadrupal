<?php

/**
 * @file
 * The Node linked layout context plugin.
 */

$plugin = array(
  'title' => t('Node Linked Layout Context Handler'),
  'description' => t('Node Linked Layout Context Handler'),
  'handler' => array(
    'class' => 'Drupal\dynamic_panes_fc_layout\NodeLinkedLayoutContextHandler',
    'file' => '../../src/NodeLinkedLayoutContextHandler.php',
  ),
  'single' => TRUE,
  'dp_layout_handler' => 'Drupal\dynamic_panes_fc_layout\FieldCollectionLayoutHandler',
  'dp_context_type' => 'entity:node',
);
