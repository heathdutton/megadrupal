<?php
/**
 * @file
 * lessons_prerequisites.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function lessons_prerequisites_field_default_fields() {
  $fields = array();

  // Exported field: 'node-lesson-field_lesson_prerequisites'
  $fields['node-lesson-field_lesson_prerequisites'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_lesson_prerequisites',
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
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'lesson',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Please provide a list of prerequisites here. For example:
<ul>
  <li>Install Learn Drupal on your personal computer</li>
  <li>Download and install module X from drupal.org/project/x</li>
  <li>Basic knowledge of PHP and CSS required</li>
  <li>This lesson builds on examples from lessons X, Y and Z, please complete <a href=\'#\'>X</a>, <a href=\'#\'>Y</a> and <a href=\'#\'>Z</a> before doing this lesson</li>
</ul>',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '4',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_lesson_prerequisites',
      'label' => 'Prerequisites',
      'required' => 0,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
        'weight' => '12',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Please provide a list of prerequisites here. For example:
<ul>
  <li>Install Learn Drupal on your personal computer</li>
  <li>Download and install module X from drupal.org/project/x</li>
  <li>Basic knowledge of PHP and CSS required</li>
  <li>This lesson builds on examples from lessons X, Y and Z, please complete <a href=\'#\'>X</a>, <a href=\'#\'>Y</a> and <a href=\'#\'>Z</a> before doing this lesson</li>
</ul>');
  t('Prerequisites');

  return $fields;
}
