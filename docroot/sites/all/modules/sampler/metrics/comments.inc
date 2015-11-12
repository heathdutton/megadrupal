<?php


/**
 * @file
 * Metric plugin for measuring total comments created and comments created
 * during a sample period.
 *
 * This metric is meant to be used with the periodic sampling method.
 */

$plugin = array(
  'title' => t('Comments'),
  'description' => t("Shows how many comments were created."),
  'data_type' => array(
    'period_comments' => 'int',
    'total_comments' => 'int',
  ),
  // The object doesn't really have a base table, so just set the metric
  // table itself as the base table.
  'object_base_table' => FALSE,
  'handler' => array(
    'class' => 'SamplerMetricComments',
    'parent' => 'single_object',
  ),
);
