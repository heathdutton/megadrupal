<?php

/**
 * @file
 * Resource definitions for gitlab_webhooks
 *
 * @author Mathew Winstone <mwinstone@coldfrontlabs.ca>
 */

/**
 * Callback for webhook push notice
 */
function _gitlab_webhooks_push_notice($before, $after, $ref, $user_id, $user_name, $project_id, $repository, $commits, $total_commits_count, $parameters) {
 watchdog('gitlab', 'Commit notification trigger from %user_name for repository %repo', array('%user_name' => $user_name, '%repo' => $repository['name']), WATCHDOG_INFO);

 module_invoke_all('gitlab_push_notice', array(
   'before' => $before,
   'after' => $after,
   'ref' => $ref,
   'user_id' => $user_id,
   'user_name' => $user_name,
   'project_id' => $project_id,
   'repository' => $repository,
   'commits' => $commits,
   'total_commits_count' => $total_commits_count
    ),
    $parameters
  );
}

/**
 * Callback for webhook tag push notice
 */
function _gitlab_webhooks_tag_push_notice($payload, $parameters) {
 watchdog('gitlab', 'Tag push notification trigger from %user_name for repository %repo', array('%user_name' => $payload['user_name'], '%repo' => $payload['repository']['name']), WATCHDOG_INFO);

 module_invoke_all('gitlab_push_notice', $payload, $parameters);
}

/**
* Access callback for webhook commit notice
*/
function _gitlab_webhooks_push_notice_access($action) {
 // @todo
 return TRUE;
}

/**
 * Callback for webhook issues notice
 */
function _gitlab_webhooks_issues_notice($payload, $parameters) {
  watchdog('gitlab', 'Issues notification trigger', array(), WATCHDOG_INFO);
  //$payload = drupal_json_decode($payload);
  module_invoke_all('gitlab_issues_notice', $payload, $parameters);
}

/**
* Access callback for webhook commit notice
*/
function _gitlab_webhooks_issues_notice_access($action) {
 // @todo
 return TRUE;
}
