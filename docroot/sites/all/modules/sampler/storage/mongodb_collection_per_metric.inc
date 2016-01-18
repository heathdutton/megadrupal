<?php

/**
 * @file
 * MongoDB storage plugin for single metric per table.
 */

$plugin = array(
  'handler' => array(
    'class' => 'SamplerStorageHandlerMongoDBCollectionPerMetric',
    'parent' => 'mongodb_collection',
  ),
);

