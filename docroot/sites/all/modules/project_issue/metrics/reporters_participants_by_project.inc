<?php

/**
 * @file
 * Metric plugin for counting the number of unique issue reporters/participants.
 *
 * This metric is intended to by used with the periodic method plugin.
 *
 * Values are calculated per project.
 *
 * This metric accepts the following options:
 * - object_ids:
 *     An array of nids. If passed it will restrict the results to nodes in
 *     this list.
 */

$plugin = array(
  'title' => t('Issue reporters/participants per project'),
  'description' => t("Counts the number users reporting issues and participating in issues in a period of time."),
  'data_type' => array(
    'reporters' => 'int',
    'participants' => 'int',
  ),
  'handler' => array(
    'class' => 'ProjectIssueReportersParticipantsByProject',
    'parent' => 'project_issue_metric',
  ),
);
