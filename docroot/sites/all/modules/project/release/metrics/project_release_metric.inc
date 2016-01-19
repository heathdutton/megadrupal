<?php


/**
 * @file
 * Base class plugin for Project release metrics.
 *
 * All Project release metrics should extend from this plugin.
 */

$plugin = array(
  'title' => t('Project release metrics'),
  'description' => t("Base plugin for all Project release metrics."),
  'non_metric' => TRUE,
  'handler' => array(
    'class' => 'ProjectReleaseMetric',
  ),
);
