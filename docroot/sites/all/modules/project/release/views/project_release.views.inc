<?php

/**
 * @file
 * Provides support for Views integration.
 */

/**
 * Implements hook_views_handlers().
 */
function project_release_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'project_release') .'/views/handlers',
    ),
    'handlers' => array(
      'project_release_handler_filter_version' => array(
        'parent' => 'views_handler_filter_field_list',
      ),
      'project_release_handler_relationship_supported_versions' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 *
 * Exposes all fields to the views system.
 */
function project_release_views_data() {
  $data = array();

  $data['project_release_supported_versions']['table']['group'] = t('Project release');

  $data['project_release_supported_versions']['nid'] = array(
    'title' => t('Project ID'),
    'help' => t('Project node ID of the supported version information.'),
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
    'relationship' => array(
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Project node'),
    ),
  );
  $data['project_release_supported_versions']['tid'] = array(
    'title' => t('Supported API Compatibility term'),
    'help' => t('The term ID of a supported API compatibility version.'),
    'field' => array(
      'handler' => 'views_handler_field_taxonomy',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'project_release_handler_filter_version_api_tid',
    ),
  );
  $data['project_release_supported_versions']['supported'] = array(
    'title' => t('Supported'),
    'help' => t('Is the specific branch supported by a given project.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'type' => 'yes-no',
    ),
  );
  $data['project_release_supported_versions']['recommended'] = array(
    'title' => t('Recommended branch'),
    'help' => t('Is the specific branch the recommended branch for a given project.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'type' => 'yes-no',
    ),
  );
  $data['project_release_supported_versions']['snapshot'] = array(
    'title' => t('Show snapshot'),
    'help' => t('Show development snapshot releases from a branch.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'type' => 'yes-no',
    ),
  );
  $data['project_release_supported_versions']['recommended_release'] = array(
    'title' => t('Recommended release'),
    'help' => t('The recommended release from a specific branch of a project.'),
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
    'relationship' => array(
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Recommended release node'),
    ),
  );
  $data['project_release_supported_versions']['latest_release'] = array(
    'title' => t('Latest release'),
    'help' => t('The latest release from a specific branch of a project.'),
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
    'relationship' => array(
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Latest release node'),
    ),
  );
  $data['project_release_supported_versions']['latest_security_release'] = array(
    'title' => t('Latest security release'),
    'help' => t('The latest security release from a specific branch of a project (if any).'),
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
    'relationship' => array(
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Latest security release node'),
    ),
  );
  $data['project_release_supported_versions']['release_type'] = array(
    'title' => t('Latest or recommended release'),
    'help' => t('Filter based on the latest or recommended release from a specific branch of a project.'),
    'filter' => array(
      'handler' => 'project_release_handler_filter_release_node_nid',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function project_release_views_data_alter(&$data) {
  // Use our custom project-argument-aware handler for version.
  $data['field_data_field_issue_version']['field_issue_version_value']['filter']['handler'] = 'project_release_handler_filter_version';
  $data['field_revision_field_issue_version']['field_issue_version_value']['filter']['handler'] = 'project_release_handler_filter_version';

  // Add a relationship for information and filters about supported releases.
  $data['field_data_field_release_project']['supported_releases_rel'] = array(
    'group' => t('Project release'),
    'title' => t('Supported releases'),
    'help' => t('Create a relationship to more information about supported releases.'),
    'relationship' => array(
      'base' => 'project_release_supported_versions',
      'base field' => 'nid',
      'relationship field' => 'field_release_project_target_id',
      'handler' => 'project_release_handler_relationship_supported_versions',
      'label' => t('supported versions'),
    ),
  );
}
