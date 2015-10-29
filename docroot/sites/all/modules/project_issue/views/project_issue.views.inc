<?php

/**
 * @file
 * Provides support for Views integration.
 */

/**
 * Implementation of hook_views_data().
 */
/*
function project_issue_views_data() {
  $data = array();

  $data['project_issues']['table']['group']  = t('Project issue');

  $data['project_issues']['table']['join'] = array(
    'node' => array(
      'type' => 'INNER',
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'project_projects' => array(
      'type' => 'INNER',
      'left_table' => 'project_projects',
      'left_field' => 'nid',
      'field' => 'pid',
    ),
    'project_release_nodes' => array(
      'type' => 'INNER',
      'left_table' => 'project_release_nodes',
      'left_field' => 'nid',
      'field' => 'rid',
    ),
  );

  $data['project_issues']['pid'] = array(
    'title' => t('Project'),
    'help' => t('The project an issue is tied to.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    // Information for accepting a pid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    // Information for accepting a pid as a filter
    'filter' => array(
      'handler' => 'project_issue_handler_filter_issue_project',
    ),
    'relationship' => array(
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Project node'),
    ),
  );

  $data['node']['project_issue_queue'] = array(
    'title' => t('Project issue queue'),
    'help' => t("Displays a link to the issue queue for the project this issue is assigned to."),
    'field' => array(
      'field' => 'title',
      'group' => t('Project issue'),
      'handler' => 'project_issue_handler_field_issue_queue_link',
      'click sortable' => TRUE,
    ),
  );

  $data['project_issues']['category'] = array(
    'title' => t('Category'),
    'help' => t("The issue's category (bug, task, feature, etc)."),
    'field' => array(
      'handler' => 'project_issue_handler_field_issue_category',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'project_issue_handler_filter_issue_category',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['project_issues']['priority'] = array(
    'title' => t('Priority'),
    'help' => t("The issue's priority (critical, normal, minor)."),
    'field' => array(
      'handler' => 'project_issue_handler_field_issue_priority',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'project_issue_handler_sort_issue_priority_weight',
      'help' => t("Sort by the issue's priority."),
    ),
    'filter' => array(
      'handler' => 'project_issue_handler_filter_issue_priority',
      'help' => t("Filter on each issue's status."),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['project_issues']['rid'] = array(
    'title' => t('Version'),
    'help' => t('The version associated with the issue (depends on project_release.module).'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'project_issue_handler_filter_issue_version',
    ),
    'relationship' => array(
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Project release node'),
    ),
  );

  $data['project_issues']['assigned'] = array(
    'title' => t('Assigned'),
    'help' => t('The user the issue is assigned to.'),
    'relationship' => array(
      'base' => 'users',
      'handler' => 'views_handler_relationship',
      'label' => t('Assigned user'),
    ),
  );

  $data['project_issues']['sid'] = array(
    'title' => t('Status'),
    'help' => t('The status of each issue'),
    'field' => array(
      'handler' => 'project_issue_handler_field_issue_status',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t("Sort by the issue's status."),
    ),
    'filter' => array(
      'handler' => 'project_issue_handler_filter_issue_status',
      'help' => t("Filter on each issue's status."),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  return $data;
}
*/

/**
 * Implements hook_views_data_alter().
 */
function project_issue_views_data_alter(&$data) {
  // Add multiple autocomplete ability and ability to limit to "projects with issue queue enabled".
  $data['field_data_field_project']['field_project_target_id']['filter']['handler'] = 'project_issue_handler_filter_issue_project';
  if (isset($data['search_api_index_project_issues'])) {
    $data['search_api_index_project_issues']['field_project']['filter']['handler'] = 'ProjectIssueHandlerFilterSearchApiIssueProject';
  }

  // Use our custom project-argument-aware handler for component.
  $data['field_data_field_issue_component']['field_issue_component_value']['filter']['handler'] = 'project_issue_handler_filter_dereference_list';
  $data['field_revision_field_issue_component']['field_issue_component_value']['filter']['handler'] = 'project_issue_handler_filter_dereference_list';
  if (isset($data['search_api_index_project_issues'])) {
    $data['search_api_index_project_issues']['field_issue_component']['filter']['handler'] = 'ProjectIssueHandlerFilterSearchApiDereferenceList';

    // … and version.
    $data['search_api_index_project_issues']['field_issue_version']['filter']['handler'] = 'ProjectIssueHandlerFilterSearchApiVersion';
  }

  // Use our meta statuses handler for status.
  $data['field_data_field_issue_status']['field_issue_status_value']['filter']['handler'] = 'project_issue_handler_filter_issue_status';
  $data['field_revision_field_issue_status']['field_issue_status_value']['filter']['handler'] = 'project_issue_handler_filter_issue_status';
  if (isset($data['search_api_index_project_issues'])) {
    $data['search_api_index_project_issues']['field_issue_status']['filter']['handler'] = 'SearchApiViewsHandlerFilterIssueStatus';
  }

  if (module_exists('tracker')) {
    // Swap out the handlers for the uid_touch filter and argument.
    $data['node']['uid_touch']['argument']['handler'] = 'views_handler_argument_tracker_comment_user_uid';
    $data['node']['uid_touch']['filter']['handler'] = 'views_handler_filter_tracker_comment_user_uid';
  }

}

/**
 * Implements hook_field_views_data_alter().
 */
function project_issue_field_views_data_alter(&$result, $field, $module) {
  if ($field['field_name'] == 'field_issue_status') {
    // Look for SQL storage, as we're going to add a SQL specific field.
    if ($field['storage']['type'] == 'field_sql_storage') {
      // Create a copy of the field.
      $copy_field = $result['field_data_field_issue_status']['field_issue_status'];

      // Tweak some details of our copied field.
      $copy_field['title'] = t('Open issue count');
      $copy_field['field']['handler'] = 'project_issue_handler_field_issue_status_open_count';

      $result['field_data_field_issue_status']['open_field_field_issue_status'] = $copy_field;
    }
  }
}

/**
 * Implements hook_views_handlers().
 */
function project_issue_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'project_issue') .'/views/handlers',
    ),
    'handlers' => array(
      'project_issue_handler_field_issue_priority' => array(
        'parent' => 'views_handler_field',
      ),
      'project_issue_handler_field_issue_queue_link' => array(
        'parent' => 'views_handler_field',
      ),
      'project_issue_handler_filter_dereference_list' => array(
        'parent' => 'views_handler_filter_field_list',
      ),
      'project_issue_handler_filter_issue_project' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'project_issue_handler_filter_issue_status' => array(
        'parent' => 'views_handler_filter_field_list',
      ),
      'project_issue_handler_filter_issue_version' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'project_issue_handler_sort_issue_priority_weight' => array(
        'parent' => 'views_handler_sort',
      ),
    ),
  );
}

function project_issue_views_plugins() {
  $path = drupal_get_path('module', 'project_issue') . '/views/plugins';
  $views_path = drupal_get_path('module', 'views');
  return array(
    'style' => array(
      'project_issue_table' => array(
        'title' => t('Project issue table'),
        'help' => t('Table with colored rows depending on issue status.'),
        'handler' => 'project_issue_table_plugin_style',
        'parent' => 'table',
        'path' => $path,
        'theme' => 'views_view_table',
        'theme file' => 'theme.inc',
        'theme path' => "$views_path/theme",
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
    'row' => array(
      'project_issue_node' => array(
        'title' => t('Project issue node'),
        'help' => t('Display a project issue node with special styling.'),
        'handler' => 'project_issue_plugin_row_project_issue_node_view',
        'path' => $path,
        'theme' => 'project_issue_issue_link',
        'register theme' => FALSE,
        'base' => array('node'), // only works with 'node' as base.
        'uses fields' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
    'access' => array(
      'project_issue_access_per_user_queue' => array(
        'title' => t('View per-user issue queues'),
        'help' => t('Access will be granted if the user is requested in the URL or for users viewing their own per-user issue queues.'),
        'handler' => 'project_issue_plugin_access_per_user_queue',
        'uses options' => TRUE,
        'path' => $path,
      ),
    ),
    'argument validator' => array(
      'project_issue_nid' => array(
        'title' => t('Project issue node'),
        'handler' => 'project_issue_plugin_argument_validate_project_issue_nid',
        'path' => $path,
      ),
      'project_nid_having_issue_queue' => array(
        'title' => t('Project with issue queue'),
        'handler' => 'project_issue_plugin_argument_validate_project_nid',
        'path' => $path,
        'parent' => 'project_nid',
      ),
    ),
  );
}

