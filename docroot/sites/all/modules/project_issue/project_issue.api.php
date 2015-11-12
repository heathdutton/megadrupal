<?php

/**
 * @file
 * API documentation for hooks invoked (provided) by the Project issue module.
 */

/**
 * Alter hook for the internal links at the top of issue node pages.
 *
 * @param array $links
 *   Reference to an array of all the "Jump to" links to render at the top of
 *   issue node pages. By default, these are the links for "Most recent
 *   comment", "Add new comment", etc.
 * @param stdClass $node
 *   The fully-loaded project_issue node the links belong to.
 *
 * @see project_issue_internal_links()
 * @see drupal_alter()
 */
function hook_project_issue_internal_links_alter(&$links, $node) {
  $links[] = l(t('Most excellent comment'), "node/$node->nid", array('fragment' => 'excellent')); // If only it were so easy. ;)
}

/**
 * Return an array of user IDs of users who may be assigned to an issue.
 *
 * @param $issue
 *   An issue node, may not have been saved yet.
 * @param $project
 *   A fully loaded project node.
 * @param $current
 *   The current value of the field.
 *
 * @return
 *   An array of user IDs keyed by user ID that may be assigned to an issue.
 */
function hook_project_issue_assignees($issue, $project, $current) {
  // Add additional users.
  $assigned = array();
  $result = db_query("SELECT uid FROM {mymodule_table_grants} WHERE project_id = :id", array(':id' => $project->nid));
  foreach ($result as $row) {
    $assigned[$row->uid] = $row->uid;
  }
  return $assigned;
}

/**
 * Allow the list of allowed assignees for an issue to be altered.
 *
 * @param $assignees
 *   The current list of user IDs allowed to be assigned to the issue, keyed
 *   by user ID.
 * @param $issue
 *   An issue node, may not have been saved yet.
 * @param $project
 *   A fully loaded project node.
 */
function hook_project_issue_assignees_alter(&$assignees, $issue, $project) {
  // Remove the users that we are temporarily restricting from the list.
  $restricted_users = variable_get('restricted_users', array());
  foreach ($restricted_users as $user_id) {
    unset($assignees[$user_id]);
  }
}

/**
 * Alter hook for the 'Create a new issue' links.
 *
 * @param array $link
 *   The link definition array, suitable for use with theme_links().
 * @param $project
 *   A fully-loaded node object representing a specific project to build the
 *   create link for, or NULL if we should go to the pick-a-project landing
 *   page first.
 *
 * @see project_issue_get_create_link()
 * @see theme_links()
 */
function hook_project_issue_create_link_alter(&$link, $project) {
  $link['title'] = t('Report a new bug');
}
