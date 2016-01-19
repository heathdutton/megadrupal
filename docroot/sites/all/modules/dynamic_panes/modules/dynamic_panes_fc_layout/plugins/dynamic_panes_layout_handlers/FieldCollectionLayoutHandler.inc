<?php

/**
 * @file
 * The Field Collection layout handler plugin.
 */

$plugin = array(
  'title' => t('Field Collection layout handler'),
  'description' => t('Field Collection layout handler.'),
  'handler' => array(
    'class' => 'Drupal\dynamic_panes_fc_layout\FieldCollectionLayoutHandler',
    'file' => '../../src/FieldCollectionLayoutHandler.php',
  ),
  'single' => TRUE,
  'dp_provider_class' => 'Drupal\dynamic_panes_fc_layout\FieldCollectionLayoutProvider',
  'dp_layout_class' => 'Drupal\dynamic_panes_fc_layout\FieldCollectionLayout',
  'dp_region_class' => 'Drupal\dynamic_panes_fc_layout\FieldCollectionRegion',
  'dp_block_class' => 'Drupal\dynamic_panes_fc_layout\FieldCollectionBlock',
);
