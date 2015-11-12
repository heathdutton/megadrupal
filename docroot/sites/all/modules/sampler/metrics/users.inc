<?php


/**
 * @file
 * Metric plugin for measuring total users created and users created
 * during a sample period.
 *
 * This metric is meant to be used with the periodic sampling method.
 */

$plugin = array(
  'title' => t('Users'),
  'description' => t("Shows how many users were created."),
  'data_type' => array(
    'period_users' => 'int',
    'total_users' => 'int',
  ),
  // The object doesn't really have a base table, so just set the metric
  // table itself as the base table.
  'object_base_table' => FALSE,
  'handler' => array(
    'class' => 'SamplerMetricUsers',
    'parent' => 'single_object',
  ),
);
