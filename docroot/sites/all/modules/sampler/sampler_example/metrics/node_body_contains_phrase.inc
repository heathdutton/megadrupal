<?php

/**
 * @file
 * Metric plugin for measuring nodes containing a specified phrase.
 *
 * This metric is meant to be used with the periodic sampling method.  In this
 * example, we're calculating how many nodes created during the specified time
 * period use a declared phrase.
 *
 * This metric accepts the following options:
 *   node_body_search_phrase:
 *     What phrase to search the node body for.  Defaults to 'module'.
 */

$plugin = array(
  'title' => t('Node body contains phrase (default: "module")'),
  'description' => t("Shows how many posts the phrase 'module' was mentioned in."),
  // The object doesn't really have a base table, so just set the metric
  // table itself as the base table.
  'object_base_table' => FALSE,
  'default_options' => array(
    'time_unit' => 'day',
    'time_unit_interval' => 15,
  ),
  'data_type' => array(
    'nodes' => 'int',
  ),
  'handler' => array(
    'class' => 'SamplerExampleMetricNodeBodyContainsPhrase',
  ),
);

