<?php

/**
 * @file
 * Metric plugin for counting the number of new releases per project.
 */
$plugin = array(
  'title' => t('New releases per project'),
  'description' => t("Counts how many new releases were created for each project during each sample period."),
  'data_type' => array(
    'releases' => 'int',
  ),
  'handler' => array(
    'class' => 'ProjectReleaseMetricNewReleases',
    'parent' => 'project_release_metric',
  ),
);

