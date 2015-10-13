<?php
/**
 * @file
 * Contains default views on behalf of the transactions  module.
 */

/**
 * Implements hook_views_data_alter().
 */
function community_tasks_views_data_alter(&$tables) {
  $tables['node']['community_task_state'] = array(
    'title' => t('Community Tasks state'),
    'help' => t("Node type '@type' only. Possible states are: Open, Committed, Completed", array('@type' => 'community-task')),
    'field' => array(
      'handler' => 'views_handler_field_community_tasks_state',
      'format' => 1
    ),
  );
}