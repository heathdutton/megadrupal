<?php

/**
 * @file
 * Drupal table storage plugin for single metric per table..
 */

$plugin = array(
  'handler' => array(
    'class' => 'SamplerStorageHandlerDrupalDatabaseTablePerMetric',
    'parent' => 'drupal_database_table',
  ),
);

