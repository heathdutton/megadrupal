<?php

/**
 * @file Code to define the default project_issue node type and its fields.
 */

/**
 * Create a default node type for project_issue nodes.
 */
function _project_issue_create_issue_node_type() {
  $t = get_t();

  $type = array(
    'type' => 'project_issue',
    'name' => $t('Issue'),
    'base' => 'node_content',
    'description' => $t('An issue that can be tracked, such as a bug report, feature request, or task.'),
    'has_title' => '1',
    'title_label' => $t('Title'),
    'custom' => 1,
    'modified' => 1,
    'locked' => 0,
    'help' => '',
  );

  $type = node_type_set_defaults($type);
  node_type_save($type);

  // Add a default body field, but give it a heavy weight so our other
  // metadata can live above it in the edit UI.
  node_add_body_field($type);
  $instance = field_info_instance('node', 'body', 'project_issue');
  $instance['widget']['weight'] = 40;
  foreach (array('default', 'teaser', 'issuemetadata') as $view_mode) {
    $instance['display'][$view_mode]['weight'] = 0;
  }
  // We want it to appear in the nodechanges output in the same order as the
  // edit tab, hence the weight of the widget, not the other view modes.
  $instance['display']['nodechanges']['weight'] = 40;
  $instance['display']['nodechanges']['module'] = 'text';
  $instance['display']['nodechanges']['type'] = 'text_default';
  field_update_instance($instance);

  // Define node workflow and comment settings.
  variable_set('node_options_project_issue', array('status', 'revision'));
  variable_set('comment_project_issue', COMMENT_NODE_OPEN);
  variable_set('comment_default_per_page_project_issue', '300');
  variable_set('comment_form_location_project_issue', COMMENT_FORM_BELOW);

  // Tell project_issue that this node type is an issue node type.
  variable_set('project_behavior_project_issue', 'project_issue');

  // Configure comments to be effectively threaded, but remove reply links and
  // don't indent threaded comments.
  variable_set('comment_default_mode_project_issue', COMMENT_MODE_THREADED);
  variable_set('project_issue_show_comment_reply_link_project_issue', 0);
  variable_set('project_issue_indent_threaded_comments_project_issue', 0);

  if (module_exists('flag_tracker')) {
    // Setup the the flag tracker flag to use our flag.
    variable_set('flag_tracker_track_flag_project_issue', 'project_issue_follow');
    variable_set('flag_tracker_show_followers_project_issue', 1);
  }

  // Define default fields and field instances.

  // Status
  $field = array(
    'cardinality' => '1',
    'entity_types' => array(),
    'field_name' => 'field_issue_status',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        1 => $t('Active'),
        13 => $t('Needs work'),
        8 => $t('Needs review'),
        2 => $t('Fixed'),
        4 => $t('Postponed'),
        3 => $t('Closed (duplicate)'),
        5 => $t("Closed (won't fix)"),
        6 => $t("Closed (works as designed)"),
        18 => $t("Closed (cannot reproduce)"),
        7 => $t("Closed (fixed)"),
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => '0',
    'type' => 'list_integer',
  );
  if (!field_info_field('field_issue_status')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => array(
      0 => array(
        'value' => '1',
      ),
    ),
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '1',
      ),
      'nodechanges' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '6',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_issue_status',
    'label' => $t('Status'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 6,
    ),
  );
  if (!field_info_instance('node', 'field_issue_status', 'project_issue')) {
    field_create_instance($instance);
  }

  // Priority
  $field = array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array(),
    'field_name' => 'field_issue_priority',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        400 => $t('Critical'),
        300 => $t('Major'),
        200 => $t('Normal'),
        100 => $t('Minor'),
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => '0',
    'type' => 'list_integer',
  );
  if (!field_info_field('field_issue_priority')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => array(
      0 => array(
        'value' => '200',
      ),
    ),
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '2',
      ),
      'nodechanges' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '5',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_issue_priority',
    'label' => $t('Priority'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );
  if (!field_info_instance('node', 'field_issue_priority', 'project_issue')) {
    field_create_instance($instance);
  }

  // Category
  $field = array(
    'cardinality' => '1',
    'entity_types' => array(),
    'field_name' => 'field_issue_category',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        1 => $t('Bug report'),
        2 => $t('Task'),
        3 => $t('Feature request'),
        4 => $t('Support request'),
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => '0',
    'type' => 'list_integer',
  );
  if (!field_info_field('field_issue_category')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => NULL,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '3',
      ),
      'nodechanges' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '4',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_issue_category',
    'label' => $t('Category'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 4,
    ),
  );
  if (!field_info_instance('node', 'field_issue_category', 'project_issue')) {
    field_create_instance($instance);
  }

  // Component
  $field = array(
    'cardinality' => '1',
    'entity_types' => array(),
    'field_name' => 'field_issue_component',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'dereference_list_allowed_list_values',
      'entityreference_field' => 'field_project',
      'dereferenced_field' => 'field_project_components',
    ),
    'translatable' => '0',
    'type' => 'list_text',
  );
  if (!field_info_field('field_issue_component')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => NULL,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '4',
      ),
      'nodechanges' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '2',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_issue_component',
    'label' => $t('Component'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );
  if (!field_info_instance('node', 'field_issue_component', 'project_issue')) {
    field_create_instance($instance);
  }

  // Assigned
  $field = array(
    'cardinality' => '1',
    'entity_types' => array(),
    'field_name' => 'field_issue_assigned',
    'module' => 'entityreference',
    'settings' => array(
      'target_type' => 'user',
      'handler' => 'assigned',
      'handler_submit' => 'Change handler',
      'handler_settings' => array(
        'project_field' => 'field_project',
        'sort' => array(
          'type' => 'property',
          'property' => 'name',
          'field' => 'none',
          'direction' => 'ASC',
        ),
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
      ),
    ),
    'translatable' => '0',
    'type' => 'entityreference',
  );
  if (!field_info_field('field_issue_assigned')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => array(
      array(
        'target_id' => '0', // Default to Unassigned on new issues.
      ),
    ),
    'description' => '',
    'entity_type' => 'node',
    'field_name' => 'field_issue_assigned',
    'label' => $t('Assigned'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );
  $assigned_display = array(
    'module' => 'token_formatters',
    'type' => 'token_formatters_entity_reference',
    'label' => 'inline',
    'weight' => '5',
    'settings' => array(
      'text' => '[user:project-issue-assignment-link]',
      'link' => '',
      'check_access' => 1,
    ),
  );
  $instance['display'] = array(
    'default' => $assigned_display,
    'issuemetadata' => $assigned_display,
    'nodechanges' => $assigned_display,
    'teaser' => array(
      'label' => 'above',
      'settings' => array(),
      'type' => 'hidden',
      'weight' => 5,
    ),
  );
  if (!field_info_instance('node', 'field_issue_assigned', 'project_issue')) {
    field_create_instance($instance);
  }

  // Project
  $field = array(
    'entity_types' => array(),
    'settings' => array(
      'target_type' => 'node',
      'handler' => 'project_behavior',
      'handler_settings' => array(
        'behavior' => 'project',
        'sort' => array(
          'type' => 'property',
          'property' => 'title',
          'direction' => 'ASC',
        ),
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
      ),
      'handler_submit' => 'Change handler',
    ),
    'field_name' => 'field_project',
    'type' => 'entityreference',
    'module' => 'entityreference',
    'cardinality' => '1',
    'columns' => array(
      'target_id' => array(
        'description' => 'The id of the target entity',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
    ),
  );
  if (!field_info_field('field_project')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'entity_types' => array(),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'nodechanges' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
    ),
    'settings' => array(
      'target_type' => 'node',
      'handler' => 'project_behavior',
      'handler_settings' => array(
        'behavior' => 'project',
        'sort' => array(
          'type' => 'none',
        ),
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
      ),
      'handler_submit' => 'Change handler',
    ),
    'entity_type' => 'node',
    'required' => 1,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_project',
    'label' => $t('Project'),
    'type' => 'entityreference',
    'module' => 'entityreference',
    'widget' => array(
      'weight' => 0,
      'type' => 'entityreference_autocomplete',
      'active' => 1,
      'settings' => array(
        'match_operator' => 'STARTS_WITH',
        'size' => '60',
        'path' => '',
      ),
    ),
  );
  if (!field_info_instance('node', 'field_project', 'project_issue')) {
    field_create_instance($instance);
  }

  // File field
  $field = array(
    'field_name' => 'field_issue_files',
    'cardinality' => FIELD_CARDINALITY_UNLIMITED,
    'type' => 'file',
    'settings' => array(
      'display_field' => 1,
      'display_default' => 1,
      'uri_scheme' => 'public',
    ),
    'module' => 'file',
    'active' => '1',
    'locked' => '0',
  );
  if (!field_info_field('field_issue_files')) {
    field_create_field($field);
  }

  $instance = array(
    'field_name' => 'field_issue_files',
    'entity_type' => 'node',
    'bundle' => 'project_issue',
    'label' => $t('Files'),
    'widget' => array(
      'weight' => 50,
      'type' => 'file_generic',
      'module' => 'file',
      'active' => 1,
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
    ),
    'settings' => array(
      'file_directory' => 'issues',
      'file_extensions' => 'jpg jpeg gif png txt xls pdf ppt pps odt ods odp gz tgz patch diff zip test info po pot psd',
      'max_filesize' => '',
      'description_field' => 0,
      'user_register_form' => false,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'type' => 'file_default',
        'settings' => array(
        ),
        'weight' => 50,
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(
        ),
        'weight' => 50,
      ),
      'issuemetadata' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(
        ),
        'weight' => 50,
      ),
      'nodechanges' => array(
        'module' => 'file',
        'type' => 'file_default',
        'label' => 'inline',
        'settings' => array(
        ),
        'weight' => 50,
      ),
    ),
    'required' => 0,
    'description' => '',
  );
  if (!field_info_instance('node', 'field_issue_files', 'project_issue')) {
    field_create_instance($instance);
  }

  // Issue changes.
  $field = array(
    'translatable' => '0',
    'settings' => array(
      'properties' => array(
        'title' => 'title',
      ),
      'revision_comment_field' => 'comment_body',
      'hide_core_revision_log' => TRUE,
    ),
    'field_name' => 'field_issue_changes',
    'type' => 'nodechanges_revision_diff',
    'cardinality' => FIELD_CARDINALITY_UNLIMITED,
  );
  if (!field_info_field('field_issue_changes')) {
    field_create_field($field);
  }

  $instance = array(
    'label' => $t('Issue changes'),
    'widget' => array(
      'weight' => 1,
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'nodechanges_diff_default',
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'nodechanges_diff_default',
        'settings' => array(),
        'weight' => -1,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'settings' => array(),
        'weight' => -1,
      ),
      'issuemetadata' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'settings' => array(),
        'weight' => -1,
      ),
    ),
    'required' => 1,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_issue_changes',
    'entity_type' => 'comment',
    'bundle' => 'comment_node_project_issue',
  );
  if (!field_info_instance('comment', 'field_issue_changes', 'comment_node_project_issue')) {
    field_create_instance($instance);
  }

  // Parent.
  $field = array(
    'translatable' => '0',
    'settings' => array(
      'target_type' => 'node',
      'handler' => 'issues',
      'handler_settings' => array(
        'target_bundles' => array(),
        'sort' => array(
          'type' => 'none',
        ),
      ),
    ),
    'field_name' => 'field_issue_parent',
    'type' => 'entityreference',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  );
  if (!field_info_field('field_issue_parent')) {
    field_create_field($field);
  }

  $instance = array(
    'label' => 'Parent issue',
    'widget' => array(
      'weight' => 31,
      'type' => 'entityreference_autocomplete',
      'active' => 1,
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'size' => '60',
        'path' => '',
      ),
    ),
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'issue_id',
        'weight' => 6,
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
      'issuemetadata' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
      'nodechanges' => array(
        'label' => 'inline',
        'type' => 'issue_id',
        'weight' => 31,
        'settings' => array(),
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_issue_parent',
    'entity_type' => 'node',
    'bundle' => 'project_issue',
  );
  if (!field_info_instance('node', 'field_issue_parent', 'project_issue')) {
    field_create_instance($instance);
  }

  // Related.
  $field = array(
    'translatable' => '0',
    'settings' => array(
      'target_type' => 'node',
      'handler' => 'issues',
      'handler_settings' => array(
        'target_bundles' => array(),
        'sort' => array(
          'type' => 'none',
        ),
      ),
    ),
    'field_name' => 'field_issue_related',
    'type' => 'entityreference',
    'active' => '1',
    'locked' => '0',
    'cardinality' => FIELD_CARDINALITY_UNLIMITED,
  );
  if (!field_info_field('field_issue_related')) {
    field_create_field($field);
  }

  $instance = array(
    'label' => 'Related issues',
    'widget' => array(
      'weight' => 32,
      'type' => 'entityreference_autocomplete',
      'active' => 1,
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'size' => '60',
        'path' => '',
      ),
    ),
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'issue_id',
        'weight' => 7,
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
      'issuemetadata' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
      'nodechanges' => array(
        'label' => 'inline',
        'type' => 'issue_id',
        'weight' => 32,
        'settings' => array(),
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_issue_related',
    'entity_type' => 'node',
    'bundle' => 'project_issue',
  );
  if (!field_info_instance('node', 'field_issue_related', 'project_issue')) {
    field_create_instance($instance);
  }

  // Set default bundle settings for our pseudo fields.
  $bundle_settings = field_bundle_settings('node', 'project_issue');
  $bundle_settings['extra_fields']['display'] = array(
    'update_link' => array(
      'issuemetadata' => array(
        'weight' => '100',
        'visible' => TRUE,
      ),
      'default' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
      'teaser' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
      'nodechanges' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
      'diff_standard' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
    ),
    'follow_link' => array(
      'issuemetadata' => array(
        'weight' => '110',
        'visible' => TRUE,
      ),
      'default' => array(
        'weight' => '110',
        'visible' => FALSE,
      ),
      'teaser' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
      'nodechanges' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
      'diff_standard' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
    ),
    'jump_links' => array(
      'issuemetadata' => array(
        'weight' => '120',
        'visible' => TRUE,
      ),
      'default' => array(
        'weight' => 120,
        'visible' => FALSE,
      ),
      'teaser' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
      'nodechanges' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
      'diff_standard' => array(
        'weight' => '100',
        'visible' => FALSE,
      ),
    ),
  );
  field_bundle_settings('node', 'project_issue', $bundle_settings);
}

/**
 * Uninstall the default project issue node type.
 */
function _project_issue_delete_issue_node_type() {
  $fields = array(
    'field_issue_status',
    'field_issue_priority',
    'field_issue_category',
    'field_issue_component',
    'field_issue_assigned',
    'field_issue_parent',
    'field_issue_related',
    'field_project',
    'field_issue_changes',
  );
  foreach ($fields as $field) {
    field_delete_field($field);
    watchdog('project', 'Deleted the :field_name field from all content type instances.', array(':field_name' => $field));
  }
}
