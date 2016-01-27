<?php
/**
 * @file
 * ideation.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function ideation_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-idea_site-body'
  $field_instances['node-idea_site-body'] = array(
    'bundle' => 'idea_site',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 4,
    ),
    'widget_type' => 'text_textarea_with_summary',
  );

  // Exported field_instance: 'node-idea_site-field_idea_categories'
  $field_instances['node-idea_site-field_idea_categories'] = array(
    'bundle' => 'idea_site',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'ideation',
        'settings' => array(),
        'type' => 'ideation_field',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'ideation',
        'settings' => array(),
        'type' => 'ideation_field',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_idea_categories',
    'label' => 'Categories',
    'required' => 0,
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

  // Exported field_instance: 'node-idea_site-field_idea_status'
  $field_instances['node-idea_site-field_idea_status'] = array(
    'bundle' => 'idea_site',
    'default_value' => array(
      0 => array(
        'value' => 'No Status',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'ideation',
        'settings' => array(),
        'type' => 'ideation_field',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'ideation',
        'settings' => array(),
        'type' => 'ideation_field',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_idea_status',
    'label' => 'Status',
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

  // Exported field_instance: 'node-idea_site-field_idea_watch'
  $field_instances['node-idea_site-field_idea_watch'] = array(
    'bundle' => 'idea_site',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'ideation',
        'settings' => array(),
        'type' => 'ideation_field',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'ideation',
        'settings' => array(),
        'type' => 'ideation_field',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_idea_watch',
    'label' => 'Idea We\'re Watching',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('Categories');
  t('Idea We\'re Watching');
  t('Status');

  return $field_instances;
}
