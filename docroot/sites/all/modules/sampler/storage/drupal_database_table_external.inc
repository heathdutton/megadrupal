<?php

/**
 * @file
 * Drupal external database table storage plugin for multiple metrics per table.
 *
 * Options:
 *   - database:
 *       Required. The name of the external database configuration key in
 *       settings.php
 */

$plugin = array(
  'handler' => array(
    'class' => 'SamplerStorageHandlerDrupalDatabaseTableExternal',
    'parent' => 'drupal_database_table',
  ),
);

