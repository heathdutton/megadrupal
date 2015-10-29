<?php


/**
 * @file
 * Base class plugin for Project issue metrics.
 *
 * All Project issue metrics should extend from this plugin.
 */

$plugin = array(
  'title' => t('Project issue metrics'),
  'description' => t("Base plugin for all Project issue metrics."),
  'non_metric' => TRUE,
  'handler' => array(
    'class' => 'ProjectIssueMetric',
  ),
);
