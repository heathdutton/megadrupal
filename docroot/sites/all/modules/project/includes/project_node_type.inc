<?php

/**
 * @file
 *  Project node types.
 */

/**
 * Install the default project node type.
 */
function project_install_default_project_node_type() {
  $t = get_t();

  /**
   * Set up project-specific field for short name.
   */
  if (!field_info_field('field_project_machine_name')) {
    $machine_name = array(
      'type' => 'machine_name',
      'field_name' => 'field_project_machine_name',
      'label' => $t('Project short name'),
    );
    field_create_field($machine_name);
  }

  /**
   * Set up a default project node type, which includes title, body, and short
   * name.
   */
  $node_type = array(
    'type' => 'project',
    'name' => $t('Project'),
    'base' => 'node_content',
    'description' => $t('Content type for project management.'),
    'title_label' => $t('Name'),
    'custom' => 1,
    'modified' => 1,
    'locked' => 0,
  );
  $rename = FALSE;
  if (node_type_get_type('project_project') && !node_type_get_type('project')) {
    // project_project was renamed to project.
    $node_type['old_type'] = 'project_project';
    $rename = TRUE;
  }
  $node_type = node_type_set_defaults($node_type);
  node_type_save($node_type);
  if ($rename) {
    db_update('node')
      ->condition('type', 'project_project')
      ->fields(array(
        'type' => 'project',
      ))
      ->execute();
  }
  $description = $t('Description');
  node_add_body_field($node_type, $description);

  // Define node workflow and comment settings.
  variable_set('node_options_project', array('status', 'revision'));
  if (module_exists('comment')) {
    variable_set('comment_project', COMMENT_NODE_CLOSED);
  }

  // Set the project behavior setting for this node type say it's a project.
  variable_set('project_behavior_project', 'project');

  $project_type_instance = array(
    'bundle' => 'project',
    'description' => $t(''),
    'entity_type' => 'node',
    'field_name' => 'field_project_type',
    'label' => $t('Project type'),
    'required' => 1,
    'widget' => array(
      'weight' => '-3',
    ),
  );
  if (!field_info_instance('node', 'field_project_type', 'project')) {
    field_create_instance($project_type_instance);
  }
  $machine_name_instance = array(
    'bundle' => 'project',
    'description' => $t(''),
    'entity_type' => 'node',
    'field_name' => 'field_project_machine_name',
    'label' => $t('Short name'),
    'required' => 1,
    'widget' => array(
      'weight' => '-2',
    ),
  );
  if (!field_info_instance('node', 'field_project_machine_name', 'project')) {
    field_create_instance($machine_name_instance);
  }

  // If pathato is enabled, set default patterns.
  if (module_exists('pathauto')) {
    _project_set_pathauto_defaults();
  }
}

/**
 * Uninstall the default project node type.
 */
function project_uninstall_default_project_node_type() {
  field_delete_field('field_project_machine_name');
  watchdog('project', 'Deleted the :field_name field from all content type instances.', array(':field_name' => 'field_project_machine_name'));
  field_delete_field('field_project_components');
  watchdog('project', 'Deleted the :field_name field from all content type instances.', array(':field_name' => 'field_project_components'));
}

