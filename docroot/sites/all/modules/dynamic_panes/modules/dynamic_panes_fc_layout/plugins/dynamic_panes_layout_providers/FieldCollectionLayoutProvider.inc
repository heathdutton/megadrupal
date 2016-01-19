<?php

/**
 * @file
 * The Field Collection layout provider plugin.
 */

$plugin = array(
  'title' => t('Field Collection layout provider'),
  'description' => t('Field Collection layout provider.'),
  'handler' => array(
    'class' => 'Drupal\dynamic_panes_fc_layout\FieldCollectionLayoutProvider',
    'file' => '../../src/FieldCollectionLayoutProvider.php',
  ),
  'single' => TRUE,
);
