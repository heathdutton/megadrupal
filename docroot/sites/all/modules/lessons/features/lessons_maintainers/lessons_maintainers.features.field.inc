<?php
/**
 * @file
 * lessons_maintainers.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function lessons_maintainers_field_default_fields() {
  $fields = array();

  // Exported field: 'node-lesson-field_lesson_maintainers'
  $fields['node-lesson-field_lesson_maintainers'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_lesson_maintainers',
      'foreign keys' => array(
        'users' => array(
          'columns' => array(
            'target_id' => 'uid',
          ),
          'table' => 'users',
        ),
      ),
      'indexes' => array(
        'target_id' => array(
          0 => 'target_id',
        ),
      ),
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'behaviors' => array(
            'views-select-list' => array(
              'status' => 0,
            ),
          ),
          'sort' => array(
            'direction' => 'ASC',
            'field' => 'none',
            'property' => 'uid',
            'type' => 'none',
          ),
          'target_bundles' => array(),
        ),
        'handler_submit' => 'Change handler',
        'target_type' => 'user',
      ),
      'translatable' => '0',
      'type' => 'entityreference',
    ),
    'field_instance' => array(
      'bundle' => 'lesson',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'link' => FALSE,
          ),
          'type' => 'entityreference_label',
          'weight' => '1',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_lesson_maintainers',
      'label' => 'Lesson Maintainers',
      'required' => 0,
      'settings' => array(
        'nodeaccess_userreference' => array(
          'all' => array(
            'view' => 'view',
          ),
          'all_published' => '0',
          'author' => array(
            'delete' => 'delete',
            'update' => 'update',
            'view' => 'view',
          ),
          'author_published' => '1',
          'create' => array(
            'ladder' => 0,
            'lesson' => 0,
            'page' => 0,
          ),
          'priority' => '0',
          'referenced' => array(
            'delete' => 0,
            'deny_delete' => 0,
            'deny_update' => 0,
            'deny_view' => 0,
            'update' => 'update',
            'view' => 'view',
          ),
          'referenced_published' => '1',
          'unused' => '0',
          'views' => array(
            'view' => '',
            'view_args' => '',
          ),
        ),
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'entityreference',
        'settings' => array(
          'match_operator' => 'CONTAINS',
          'path' => '',
          'size' => '60',
        ),
        'type' => 'entityreference_autocomplete',
        'weight' => '13',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Lesson Maintainers');

  return $fields;
}
