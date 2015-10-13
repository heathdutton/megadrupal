<?php

/**
 * @file Code to define the default project_release node type and its fields.
 */

/**
 * Create a default node type for project_release nodes.
 */
function _project_release_create_release_node_type() {
  $t = get_t();

  $type = array(
    'type' => 'project_release',
    'name' => $t('Release'),
    'base' => 'node_content',
    'description' => $t('A release of a project with a specific version number.'),
    'has_title' => '1',
    'title_label' => $t('Title'),
    'custom' => 1,
    'modified' => 1,
    'locked' => 0,
    'help' => '',
  );

  $type = node_type_set_defaults($type);
  node_type_save($type);
  $instance = node_add_body_field($type, 'Release notes');
  $instance['description'] = 'Enter a description of this release, such as a list of the major changes or updates.';
  field_update_instance($instance);

  // Define node workflow and comment settings.
  variable_set('node_options_project_release', array('status', 'revision'));
  variable_set('comment_project_release', COMMENT_NODE_CLOSED);

  // Set the project behavior setting for this node type say it's a release.
  variable_set('project_behavior_project_release', 'project_release');

  // Access to releases is handled project-by-project. See project_node_access().
  variable_set('node_permissions_project_release', FALSE);

  // Define default fields and field instances.

  // Project
  field_create_field(array(
    'entity_types' => array(),
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
    'field_name' => 'field_release_project',
    'type' => 'entityreference',
    'module' => 'entityreference',
    'cardinality' => '1',
    'columns' => array(
      'target_id' => array(
        'description' => $t('The id of the target entity'),
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
    ),
  ));
  field_create_instance(array(
    'entity_types' => array(),
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
      'user_register_form' => FALSE,
    ),
    'required' => 1,
    'description' => '',
    'default_value' => NULL,
    'label' => $t('Project'),
    'type' => 'entityreference',
    'module' => 'entityreference',
    'widget' => array(
      'weight' => '-1',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'size' => '60',
        'path' => '',
      ),
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '7',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'field_name' => 'field_release_project',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // Full version number.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(
      'max_length' => '255',
    ),
    'field_name' => 'field_release_version',
    'type' => 'text',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Version'),
    'widget' => array(
      'weight' => '0',
      'type' => 'text_textfield',
      'active' => 1,
      'settings' => array(
        'size' => '60',
      ),
    ),
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '4',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 1,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_version',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // Major component of version number.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(),
    'field_name' => 'field_release_version_major',
    'type' => 'number_integer',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Version major'),
    'widget' => array(
      'weight' => '4',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'number_integer',
        'formatter_settings' => array(
          'thousand_separator' => '',
          'prefix_suffix' => 0,
        ),
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '8',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_version_major',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // The minor version number of a release.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(),
    'field_name' => 'field_release_version_minor',
    'type' => 'number_integer',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Version minor'),
    'widget' => array(
      'weight' => '4',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'number_integer',
        'formatter_settings' => array(
          'thousand_separator' => '',
          'prefix_suffix' => 0,
        ),
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '9',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_version_minor',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // The patch level version number of a release.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(),
    'field_name' => 'field_release_version_patch',
    'type' => 'number_integer',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Version patch'),
    'widget' => array(
      'weight' => '4',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'number_integer',
        'formatter_settings' => array(
          'thousand_separator' => '',
          'prefix_suffix' => 0,
        ),
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '14',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_version_patch',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // A text string that can be used to provide additional information about a
  // release. Ex: BETA
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(
      'max_length' => '255',
    ),
    'field_name' => 'field_release_version_extra',
    'type' => 'text',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Version extra'),
    'widget' => array(
      'weight' => '4',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(),
    ),
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '12',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_version_extra',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // Numeric code for ordering releases that define "version_extra".
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(),
    'field_name' => 'field_release_version_ext_weight',
    'type' => 'number_integer',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Version extra weight'),
    'widget' => array(
      'weight' => '4',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'number_integer',
        'formatter_settings' => array(
          'thousand_separator' => '',
          'prefix_suffix' => 0,
        ),
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '13',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_version_ext_weight',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // The first span of digits found in version_extra. This is needed because we
  // cannot natural sort natively without a stored procedure.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(),
    'field_name' => 'field_release_version_ext_delta',
    'type' => 'number_integer',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Version extra delta'),
    'widget' => array(
      'weight' => '4',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'number_integer',
        'formatter_settings' => array(
          'thousand_separator' => '',
          'prefix_suffix' => 0,
        ),
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '11',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_version_ext_delta',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // Version control system label, a tag or branch name.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(
      'max_length' => '255',
    ),
    'field_name' => 'field_release_vcs_label',
    'type' => 'text',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('VCS Label'),
    'widget' => array(
      'weight' => '1',
      'type' => 'text_textfield',
      'active' => 1,
      'settings' => array(
        'size' => '60',
      ),
    ),
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'type' => 'text_plain',
        'weight' => '1',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => $t('The label (branch or tag) in a version control system that represents this release.'),
    'default_value' => NULL,
    'field_name' => 'field_release_vcs_label',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // Build type: static, built once from a tag; dynamic, built multiple times
  // from a branch.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(
      'allowed_values' => array(
        'static' => $t('Static'),
        'dynamic' => $t('Dynamic'),
      ),
      'allowed_values_function' => '',
    ),
    'field_name' => 'field_release_build_type',
    'type' => 'list_text',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Release type'),
    'widget' => array(
      'weight' => '2',
      'type' => 'options_select',
      'active' => 1,
      'settings' => array(),
    ),
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '5',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 1,
    'description' => $t('How is this release built? Can be either \'Static\' if the files associated with it are built once and remain unchanged (e.g. an official release from a tag), or \'Dynamic\' if the files are regularly rebuilt and updated (e.g. a development snapshot rebuilt from the end of a branch).'),
    'default_value' => array(
      0 => array(
        'value' => 'static',
      ),
    ),
    'field_name' => 'field_release_build_type',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // Flag to record the update status for this release. Allowed values:
  // PROJECT_RELEASE_UPDATE_STATUS_CURRENT (0),
  // PROJECT_RELEASE_UPDATE_STATUS_NOT_CURRENT (1),
  // PROJECT_RELEASE_UPDATE_STATUS_NOT_SECURE (2)
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(),
    'field_name' => 'field_release_update_status',
    'type' => 'number_integer',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Update status'),
    'widget' => array(
      'weight' => '4',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'number_integer',
        'formatter_settings' => array(
          'thousand_separator' => '',
          'prefix_suffix' => 0,
        ),
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'min' => '',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'hidden',
        'weight' => '10',
        'settings' => array(),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_update_status',
    'entity_type' => 'node',
    'bundle' => 'project_release',
  ));

  // Field collection for release files.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(
      'path' => '',
    ),
    'field_name' => 'field_release_files',
    'type' => 'field_collection',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '-1',
  ));
  field_create_instance(array(
    'label' => $t('Release files'),
    'widget' => array(
      'weight' => '5',
      'type' => 'field_collection_embed',
      'active' => 0,
      'settings' => array(),
    ),
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'field_collection_fields',
        'weight' => '0',
        'settings' => array(
          'view_mode' => 'full',
        ),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'field_name' => 'field_release_files',
    'entity_type' => 'node',
    'bundle' => 'project_release',
    'default_value' => NULL,
  ));

  // Stores information about files attached to release nodes.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(
      'display_field' => 0,
      'display_default' => 1,
      'uri_scheme' => 'public',
    ),
    'field_name' => 'field_release_file',
    'type' => 'file',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Release file'),
    'widget' => array(
      'weight' => '1',
      'type' => 'file_generic',
      'active' => 1,
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
    ),
    'settings' => array(
      'file_directory' => 'project',
      'file_extensions' => 'zip gz tar bz2 rar tgz tar.gz dmg rpm deb',
      'max_filesize' => '',
      'description_field' => 0,
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'file_default',
        'weight' => '0',
        'settings' => array(),
      ),
    ),
    'required' => 0,
    'description' => '',
    'field_name' => 'field_release_file',
    'entity_type' => 'field_collection_item',
    'bundle' => 'field_release_files',
  ));

  // An MD5 hash of the file.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(
      'max_length' => '32',
    ),
    'field_name' => 'field_release_file_hash',
    'type' => 'text',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Release file hash'),
    'widget' => array(
      'weight' => '2',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'text_default',
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'text_default',
        'weight' => '1',
        'settings' => array(),
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_file_hash',
    'entity_type' => 'field_collection_item',
    'bundle' => 'field_release_files',
  ));

  // Number of times this file has been downloaded. This value is updated by
  // site specific means.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(),
    'field_name' => 'field_release_file_downloads',
    'type' => 'number_integer',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'label' => $t('Release file downloads'),
    'widget' => array(
      'weight' => '3',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'number_integer',
        'formatter_settings' => array(
          'thousand_separator' => ',',
          'prefix_suffix' => 0,
        ),
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'min' => '0',
      'max' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'number_integer',
        'weight' => '2',
        'settings' => array(
          'thousand_separator' => ',',
          'prefix_suffix' => 1,
          'decimal_separator' => '.',
          'scale' => 0,
        ),
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_release_file_downloads',
    'entity_type' => 'field_collection_item',
    'bundle' => 'field_release_files',
  ));

  // Field to mark if a project has releases.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(
      'allowed_values' => array(
        '0' => '',
        '1' => '',
      ),
      'allowed_values_function' => '',
    ),
    'field_name' => 'field_project_has_releases',
    'type' => 'list_boolean',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'bundle' => 'project',
    'description' => $t(''),
    'entity_type' => 'node',
    'field_name' => 'field_project_has_releases',
    'label' => $t('Has project releases'),
    'widget' => array(
      'weight' => '-1',
      'type' => 'options_onoff',
      'module' => 'options',
      'settings' => array(
        'display_label' => TRUE,
      ),
    ),
    'default_value' => array(
      0 => array(
        'value' => '1',
      ),
    ),
  ));

  // Per-project custom version format string.
  field_create_field(array(
    'translatable' => '0',
    'settings' => array(
      'max_length' => '255',
    ),
    'field_name' => 'field_release_version_format',
    'type' => 'text',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '1',
  ));
  field_create_instance(array(
    'bundle' => 'project',
    'entity_type' => 'node',
    'field_name' => 'field_release_version_format',
    'label' => $t('Custom version format string'),
    'widget' => array(
      'weight' => '0',
      'type' => 'text_textfield',
      'active' => 1,
      'settings' => array(
        'size' => '50',
      ),
      'display' => array(
        'default' => array(
          'type' => 'hidden',
        ),
      ),
    ),
  ));

  // Add a recommended toggle to the API taxonomy, if there is one.
  $api_vocabulary = taxonomy_vocabulary_load(variable_get('project_release_api_vocabulary', 0));
  if ($api_vocabulary !== FALSE) {
    field_create_field(array(
      'translatable' => '0',
      'settings' => array(
        'allowed_values' => array(
          0 => '',
          1 => $t('Recommended for project releases'),
        ),
        'allowed_values_function' => '',
      ),
      'field_name' => 'field_release_recommended',
      'type' => 'list_boolean',
      'active' => '1',
      'locked' => '0',
      'cardinality' => '1',
    ));
    field_create_instance(array(
      'label' => $t('Recommended'),
      'widget' => array(
        'weight' => '1',
        'type' => 'options_onoff',
        'active' => 1,
        'settings' => array(
          'display_label' => 0,
        ),
      ),
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'hidden',
          'weight' => '0',
          'settings' => array(),
        ),
      ),
      'required' => 0,
      'description' => '',
      'default_value' => array(
        0 => array(
          'value' => 1,
        ),
      ),
      'field_name' => 'field_release_recommended',
      'entity_type' => 'taxonomy_term',
      'bundle' => $api_vocabulary->machine_name,
    ));
  }
}

/**
 * Uninstall the default project release node type.
 */
function _project_release_delete_release_node_type() {
  $fields = array(
    'field_release_project',
    'field_release_version',
    'field_release_version_major',
    'field_release_version_minor',
    'field_release_version_patch',
    'field_release_version_extra',
    'field_release_version_ext_weight',
    'field_release_version_ext_delta',
    'field_release_vcs_label',
    'field_release_build_type',
    'field_release_update_status',
    'field_project_has_releases',
    'field_release_version_format',
    'field_release_recommended',
  );
  foreach ($fields as $field) {
    field_delete_field($field);
    watchdog('project_release', 'Deleted the :field_name field from all content type instances.', array(':field_name' => $field));
  }
}
