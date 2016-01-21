<?php

/**
 * @file
 * Drupal external database table storage plugin for single metric per table..
 *
 * Options:
 *   - database:
 *       Required. The name of the external database configuration key in
 *       settings.php
 */

$plugin = array(
  'handler' => array(
    'class' => 'SamplerStorageHandlerDrupalDatabaseTablePerMetricExternal',
    'parent' => 'drupal_database_table_per_metric',
  ),
);

