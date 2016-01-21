<?php
/**
 * @file
 * tutor_statistics.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function tutor_statistics_field_default_fields() {
  $fields = array();

  // Exported field: 'node-tutor_exercise-field_question'.
  $fields['node-tutor_exercise-field_question'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_question',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'tutorfield',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'tutorfield',
    ),
    'field_instance' => array(
      'bundle' => 'tutor_exercise',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Select a question to display, if any.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'tutorfield',
          'settings' => array(),
          'type' => 'tutorfield_formatter',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_question',
      'label' => 'Question',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'tutorfield',
        'settings' => array(),
        'type' => 'tutorfield_widget',
        'weight' => '1',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Question');
  t('Select a question to display, if any.');

  return $fields;
}
