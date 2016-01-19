<?php
/**
 * @file
 * cm_project_budget.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function cm_project_budget_field_default_fields() {
  $fields = array();

  // Exported field: 'node-cm_project-field_project_budget'
  $fields['node-cm_project-field_project_budget'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_project_budget',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(
        'profile2_private' => FALSE,
      ),
      'translatable' => '0',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'cm_project',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '<b>Defaults:</b> Daily Series (500 points), Weekly Series (500 points), Bi Weekly (300), Monthly Series (150 points), Prototype Project (25 points)',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 13,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_project_budget',
      'label' => 'Project Budget',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => 'points',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '6',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('<b>Defaults:</b> Daily Series (500 points), Weekly Series (500 points), Bi Weekly (300), Monthly Series (150 points), Prototype Project (25 points)');
  t('Project Budget');

  return $fields;
}
