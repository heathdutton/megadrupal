<?php


/**
 * @file
 * Metric plugin for measuring total nodes created and nodes created
 * during a sample period.
 *
 * This metric is meant to be used with the periodic sampling method.
 */

$plugin = array(
  'title' => t('Nodes'),
  'description' => t("Shows how many nodes were created."),
  'data_type' => array(
    'period_nodes' => 'int',
    'total_nodes' => 'int',
  ),
  // The object doesn't really have a base table, so just set the metric
  // table itself as the base table.
  'object_base_table' => FALSE,
  'handler' => array(
    'class' => 'SamplerMetricNodes',
    'parent' => 'single_object',
  ),
);
