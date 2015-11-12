<?php

/**
 * @file
 * The Node ruleset context plugin.
 */

$plugin = array(
  'title' => t('Node Ruleset Context Handler'),
  'description' => t('Node Rulest Context Handler'),
  'handler' => array(
    'class' => 'Drupal\dynamic_panes_fc_layout\NodeRulesetContextHandler',
    'file' => '../../src/NodeRulesetContextHandler.php',
  ),
  'single' => TRUE,
  'dp_layout_handler' => 'Drupal\dynamic_panes_fc_layout\FieldCollectionLayoutHandler',
  'dp_context_type' => 'entity:node',
);
