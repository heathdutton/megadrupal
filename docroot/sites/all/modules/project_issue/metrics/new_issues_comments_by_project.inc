<?php

/**
 * @file
 * Metric plugin for counting the number of new issues and comments.
 *
 * This metric is intended to by used with the periodic method plugin.
 *
 * Values are calculated per project.  The totals for new issues, new comments,
 * and a sum total of both are calculated.
 *
 * This metric accepts the following options:
 * - object_ids:
 *     An array of nids. If passed it will restrict the results to nodes in
 *     this list.
 */

$plugin = array(
  'title' => t('New issues and comments by project'),
  'description' => t("Counts the number of new issues and new comments for each project in a period of time."),
  'data_type' => array(
    'new_issues' => 'int',
    'new_comments' => 'int',
    'new_total' => 'int',
  ),
  'handler' => array(
    'class' => 'ProjectIssueNewIssuesCommentsByProject',
    'parent' => 'project_issue_metric',
  ),
);
