<?php
/**
 * @file
 * lessons_lesson.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function lessons_lesson_field_default_fields() {
  $fields = array();

  // Exported field: 'node-lesson-field_lesson_description'
  $fields['node-lesson-field_lesson_description'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_lesson_description',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'lesson',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Briefly explain the objective of this lesson. E.g. "This lesson will teach you how to create a custom content type." Or, "Learn to implement hook_menu and create a custom page callback."',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '2',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_lesson_description',
      'label' => 'Description',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '1',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Briefly explain the objective of this lesson. E.g. "This lesson will teach you how to create a custom content type." Or, "Learn to implement hook_menu and create a custom page callback."');
  t('Description');
  t('Lessons should be written as an ordered list of steps walking the user from start to completion.');
  t('Overview');
  t('Provide a narrative overview of the lesson here including (1) what this lesson will teach and (2) some context about why this lesson is important. (Suggested length: 2 - 5 sentences.)');
  t('Steps');

  return $fields;
}
