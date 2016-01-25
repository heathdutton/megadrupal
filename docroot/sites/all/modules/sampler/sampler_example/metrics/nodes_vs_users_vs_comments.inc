<?php

/**
 * @file
 * Metric plugin for measuring nodes created versus users created versus
 * comments created.
 *
 * This metric is meant to be used with the periodic sampling method.  In this
 * example, we're calculating how many nodes created during the specified time
 * period versus users and comments created in the same period of time.
 */

$plugin = array(
  'title' => t('Nodes vs. Users vs. Comments'),
  'description' => t("Shows how many nodes, users, and comments were created."),
  // Multiple value storage is supported in some storage plugins -- add the
  // data types in an associative array, key is a single lower-case word,
  // value is data type, in the order they should be stored.
  'data_type' => array(
    'nodes' => 'int',
    'users' => 'int',
    'comments' => 'int',
  ),
  // The object doesn't really have a base table, so just set the metric
  // table itself as the base table.
  'object_base_table' => FALSE,
  'handler' => array(
    'class' => 'SamplerExampleMetricNodesVsUsersVsComments',
  ),
);

