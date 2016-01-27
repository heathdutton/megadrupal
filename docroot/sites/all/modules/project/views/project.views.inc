<?php


/**
 * @file
 * Provide views data and handlers for project.module.
 */

/**
 * Implements hook_views_data().
 *
 * Provides views with access to the data model provided by the project module.
 */
function project_views_data() {
  $data['project_maintainer']['table']['group'] = t('Project maintainers');
  $data['project_maintainer']['table']['join'] = array(
    // Directly links to node table.
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['project_maintainer']['uid'] = array(
    'title' => t('Maintainer uid'),
    'help' => t('The user that maintains or co-maintains a project. If you need more fields than the uid add the content: author relationship'),
    'relationship' => array(
      'title' => t('Author'),
      'help' => t('Relate content to the user who created it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('author'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  $data['project_maintainer']['update_project'] = array(
    'title' => t('Update Projects'),
    'help' => t('Can this user update(edit) the given project and modify its settings.'), // The help that appears on the UI.
    // Information for displaying the node ID.
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    // Information for accepting a node ID as an argument.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    // Information for accepting a node ID as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a node ID.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['project_maintainer']['administer_maintainers'] = array(
    'title' => t('Update Projects'),
    'help' => t('Can this user manipulate the maintainers for the given project.'), // The help that appears on the UI.
    // Information for displaying the node ID.
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    // Information for accepting a node ID as an argument.
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    // Information for accepting a node ID as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a node ID.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function project_views_data_alter(&$data) {
  // Restrict content type by the behavior defined on the content types..
  $data['node']['project_type'] = array(
    'group' => t('Project'),
    'title' => t('Project system behavior'),
    'help' => t('This filter restricts content to the set of types that have a specific purpose in the Project system.'),
    'real field' => 'type',
    'filter' => array(
      'handler' => 'project_handler_filter_project_behavior',
    ),
  );

  // Project sorting method filter.  This is a bit of a hack until Views
  // supports exposed sorting.
  $data['views']['project_sort_method'] = array(
    'group' => t('Project'),
    'title' => t('Sort method'),
    'help' => t('This is a filter that provides a sort method select box that can be exposed to the user.'),
    'filter' => array(
      'handler' => 'project_handler_filter_project_sort_method',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function project_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'project') .'/views/handlers',
    ),
    'handlers' => array(
      'project_handler_filter_project_behavior' => array(
        'parent' => 'views_handler_filter',
      ),
      'project_handler_filter_project_sort_method' => array(
        'parent' => 'views_handler_filter',
      ),
      'project_handler_filter_project_status' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function project_views_plugins() {
  $project_path = drupal_get_path('module', 'project');
  $path = drupal_get_path('module', 'project') .'/views/plugins';
  return array(
    'module' => 'project',
    'style' => array(
      'project_list' => array(
        'title' => t('Project list'),
        'help' => t('Displays a list of project rows with additional project related information.'),
        'handler' => 'project_plugin_style_project_list',
        'path' => "$project_path/views/plugins",
        'theme' => 'project_views_view_project_list',
        'theme file' => 'theme.inc',
        'theme path' => "$project_path/views/theme",
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
      ),
      'collapsible_summary' => array(
        'title' => t('Collapsible'),
        'help' => t('Displays the summary in a collapsible fieldset.'),
        'handler' => 'views_plugin_style_summary',
        'path' => $path,
        'theme' => 'project_views_view_collapsible_summary',
        'theme file' => 'theme.inc',
        'theme path' => "$project_path/views/theme",
        'type' => 'summary', // only shows up as a summary style
        'uses options' => TRUE,
      ),
    ),
    'row' => array(
      'project_node' => array(
        'title' => t('Project node'),
        'help' => t('Display the project node with download table.'),
        'handler' => 'project_plugin_row_project_node_view',
        'path' => $path,
        'theme' => 'project_views_view_row_project_node',
        'theme file' => 'theme.inc',
        'theme path' => "$project_path/views/theme",
        'base' => array('node'), // only works with 'node' as base.
        'uses options' => TRUE,
        'type' => 'normal',
      ),
      'project_fields' => array(   // Project row style for fields.
        'title' => t('Project fields'),
        'help' => t('Displays project fields.'),
        'handler' => 'project_plugin_row_project_fields_view',
        'path' => $path,
        'theme' => 'project_views_view_row_project_fields',
        'theme file' => 'theme.inc',
        'theme path' => "$project_path/views/theme",
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
    'argument validator' => array(
      'project_nid' => array(
        'title' => t('Project node'),
        'handler' => 'project_plugin_argument_validate_project_nid',
        'path' => $path,
      ),
      'project_node_type' => array(
        'title' => t('Project node type'),
        'handler' => 'project_plugin_argument_validate_project_node_type',
        'path' => $path,
      ),
    ),
    'argument default' => array(
      'project_plugin_argument_default_project_type' => array(
        'title' => t('Project-enabled node type from URL'),
        'handler' => 'project_plugin_argument_default_project_type',
        'path' => $path,
        'parent' => 'fixed', // so that the parent class is included
      ),
    ),
  );
}
