<?php

/**
 * @file
 * API functions for GitLab Webhooks
 */

/**
 * Triggered when a push is made to a repository
 *
 * @param string $before
 *  Git commit id prior
 * @param string $after
 *  Git commit id after
 * @param $ref
 *  Git path reference
 * @param string $user_id
 *  User id who performed the commit (Gitlab ID)
 * @param string $user_name
 *  User name (GitLab user name)
 * @param array $repository
 *  Array of information about the repository
 * @param $commits
 *  Array of information about each commit being made
 * @param integer $total_commits_count
 *  Number of commits being made
 */
function hook_gitlab_push_notice($before, $after, $ref, $user_id, $user_name, $repository, $commits, $total_commits_count)) {
  // @todo see if this is the best signature for this hooks
  // perhaps an array would be preferable
}

/**
 * Triggered when an issue is created or modified for repository
 *
 * @param array $payload
 *  Array of information about the issue notice
 * @param array $parameters
 *  Array of URL parameters from the issue notice hook request
 */
function hook_gitlab_issues_notice($payload, $parameters) {

}