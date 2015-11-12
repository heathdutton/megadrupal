<?php

/**
 * @file
 * Metric plugin for counting the issue response rate and average response
 * time, for issues that got responses.
 *
 * This metric is intended to by used with the periodic method plugin.
 *
 * This metric accepts the following options:
 * - object_ids:
 *     An array of Project node IDs. If passed it will restrict the results to
 *     nodes in this list.
 */

$plugin = array(
  'title' => t('Response rate and time by project'),
  'description' => t('Counts the response rate and time for each project’s issues in a period of time.'),
  'data_type' => array(
    'response_rate' => 'int',
    'response_time' => 'int',
  ),
  'handler' => array(
    'class' => 'ProjectIssueResponsesByProject',
    'parent' => 'project_issue_metric',
  ),
);
