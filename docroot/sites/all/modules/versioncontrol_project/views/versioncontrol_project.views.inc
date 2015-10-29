<?php

/**
 * @file
 * Provide views data and handlers for versioncontrol_project.module
 */


/**
 * Implementation of hook_views_data()
 */
function versioncontrol_project_views_data() {
  $data['versioncontrol_project_projects']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'versioncontrol_repositories' => array(
      'left_field' => 'repo_id',
      'field' => 'repo_id',
      'type' => 'INNER',
    ),
    'versioncontrol_operations' => array(
      'left_field' => 'repo_id',
      'field' => 'repo_id',
    ),
    'versioncontrol_item_revisions' => array(
      'left_field' => 'repo_id',
      'field' => 'repo_id',
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function versioncontrol_project_views_data_alter(&$data) {
  $data['versioncontrol_repositories']['table']['join']['node'] = array(
    'left_field' => 'repo_id',
    'left_table' => 'versioncontrol_project_projects',
    'field' => 'repo_id',
  );
  $data['node']['table']['join']['versioncontrol_operations'] = array(
    'left_field' => 'nid',
    'left_table' => 'versioncontrol_project_projects',
    'field' => 'nid',
  );
  $data['node']['table']['join']['versioncontrol_item_revisions'] = array(
    'left_field' => 'nid',
    'left_table' => 'versioncontrol_project_projects',
    'field' => 'nid',
  );
  $data['node']['table']['join']['versioncontrol_repositories'] = array(
    'left_field' => 'nid',
    'left_table' => 'versioncontrol_project_projects',
    'field' => 'nid',
    'type' => 'INNER',
  );
}
