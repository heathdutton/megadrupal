<?php
/**
 * @file
 * timetable_feature_schedule.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function timetable_feature_schedule_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-schedule-field_schedule_timeslots'
  $field_instances['node-schedule-field_schedule_timeslots'] = array(
    'bundle' => 'schedule',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'timetable',
        'settings' => array(),
        'type' => 'timetable_form_link',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_schedule_timeslots',
    'label' => 'Schedule timeslots',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'schedule_timeslot' => array(
          'room_entity_type_bundle' => 'room:room',
          'session_entity_type_bundles' => array(
            'comment:comment_node_article' => 0,
            'comment:comment_node_page' => 0,
            'comment:comment_node_schedule' => 0,
            'comment:comment_node_scheduled_item' => 0,
            'comment:comment_node_session' => 0,
            'comment:comment_node_sponsor' => 0,
            'node:article' => 0,
            'node:page' => 0,
            'node:schedule' => 0,
            'node:scheduled_item' => 'node:scheduled_item',
            'node:session' => 'node:session',
            'node:sponsor' => 0,
            'room:room' => 0,
            'taxonomy_term:tags' => 0,
            'user:user' => 0,
          ),
          'status' => 1,
        ),
        'session_timeslot' => array(
          'status' => 0,
        ),
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'inline_entity_form',
      'settings' => array(
        'fields' => array(),
        'type_settings' => array(
          'allow_existing' => 0,
          'delete_references' => 1,
          'label_plural' => 'entities',
          'label_singular' => 'entity',
          'match_operator' => 'CONTAINS',
          'override_labels' => 0,
        ),
      ),
      'type' => 'inline_entity_form',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-session-field_session_timeslot'
  $field_instances['node-session-field_session_timeslot'] = array(
    'bundle' => 'session',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'timetable',
        'settings' => array(),
        'type' => 'schedule_link',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_session_timeslot',
    'label' => 'Timeslot',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'schedule_timeslot' => array(
          'status' => 0,
        ),
        'session_timeslot' => array(
          'schedule_entity_type_bundle' => 'node:schedule',
          'status' => 1,
        ),
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Schedule timeslots');
  t('Timeslot');

  return $field_instances;
}
