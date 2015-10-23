<?php

/**
 * @file
 * MongoDB storage plugin for multiple metrics per table.
 */

$plugin = array(
  'handler' => array(
    'class' => 'SamplerStorageHandlerMongoDBCollection',
  ),
);

