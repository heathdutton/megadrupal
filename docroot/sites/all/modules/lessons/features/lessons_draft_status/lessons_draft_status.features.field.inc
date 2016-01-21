<?php
/**
 * @file
 * lessons_draft_status.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function lessons_draft_status_field_default_fields() {
  $fields = array();

  // Exported field: 'comment-comment_node_lesson-field_update_lesson_status'
  $fields['comment-comment_node_lesson-field_update_lesson_status'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_update_lesson_status',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'ready-for-review' => 'Ready for review',
          'needs-revision' => 'Needs revision',
          'up-to-date' => 'Final / Up to date',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'comment_node_lesson',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'comment',
      'field_name' => 'field_update_lesson_status',
      'label' => 'Update status',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'node-lesson-field_lesson_draft_status'
  $fields['node-lesson-field_lesson_draft_status'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_lesson_draft_status',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'draft' => 'Draft (work in progress)',
          'ready-for-review' => 'Ready for review',
          'needs-revision' => 'Needs revision',
          'up-to-date' => 'Final / Up to date',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'lesson',
      'default_value' => array(
        0 => array(
          'value' => 'draft',
        ),
      ),
      'deleted' => '0',
      'description' => '<ul>
  <li><strong>Draft (work in progress)</strong> - Lesson authors set a lesson\'s status to "Draft" to let users know it is not finished. Users follow draft lessons at their own risk.</li>
  <li><strong>Ready for review</strong> - Lesson authors should mark a lesson "Ready for review" when they think it\'s done and ready for someone to try out.</li>
  <li><strong>Needs revision</strong> - Normally this status is set by a peer reviewer if instructions need to be revised for any reason.</li>
  <li><strong>Final / Up to date</strong> - This status should be set by a peer reviewer or co-maintainer to indicate the lesson has been reviewed, followed, and is ready to share with the world.</li>
</ul>',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '0',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_lesson_draft_status',
      'label' => 'Draft Status',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '10',
      ),
    ),
  );

  // Exported field: 'node-lesson-field_lesson_last_peer_review'
  $fields['node-lesson-field_lesson_last_peer_review'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_lesson_last_peer_review',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'lesson',
      'default_value' => array(
        0 => array(
          'value' => '0',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '7',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_lesson_last_peer_review',
      'label' => 'Last peer review',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '11',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('<ul>
  <li><strong>Draft (work in progress)</strong> - Lesson authors set a lesson\'s status to "Draft" to let users know it is not finished. Users follow draft lessons at their own risk.</li>
  <li><strong>Ready for review</strong> - Lesson authors should mark a lesson "Ready for review" when they think it\'s done and ready for someone to try out.</li>
  <li><strong>Needs revision</strong> - Normally this status is set by a peer reviewer if instructions need to be revised for any reason.</li>
  <li><strong>Final / Up to date</strong> - This status should be set by a peer reviewer or co-maintainer to indicate the lesson has been reviewed, followed, and is ready to share with the world.</li>
</ul>');
  t('Draft Status');
  t('Last peer review');
  t('Update status');

  return $fields;
}
