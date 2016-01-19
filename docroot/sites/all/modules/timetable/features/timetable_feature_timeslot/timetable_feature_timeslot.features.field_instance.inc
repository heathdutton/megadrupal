<?php
/**
 * @file
 * timetable_feature_timeslot.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function timetable_feature_timeslot_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'timeslot-timeslot-field_timeslot_time'
  $field_instances['timeslot-timeslot-field_timeslot_time'] = array(
    'bundle' => 'timeslot',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'timefield',
        'settings' => array(
          'display_format' => array(
            'hour' => 'G',
            'minute' => 'i',
            'period' => 'none',
            'period_separator' => '',
            'separator' => ':',
          ),
        ),
        'type' => 'timefield_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'timeslot',
    'field_name' => 'field_timeslot_time',
    'label' => 'Time',
    'required' => 1,
    'settings' => array(
      'disable_plugin' => 0,
      'input_format' => array(
        'am_text' => 'AM',
        'atPosition' => 'left bottom',
        'closeButtonText' => 'Done',
        'deselectButtonText' => 'Deselect',
        'myPosition' => 'left top',
        'nowButtonText' => 'Now',
        'periodSeparator' => '',
        'pm_text' => 'PM',
        'separator' => ':',
        'showCloseButton' => 0,
        'showDeselectButton' => 0,
        'showLeadingZero' => 1,
        'showNowButton' => 0,
        'showPeriod' => 0,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'timefield',
      'settings' => array(
        'label_position' => 'above',
      ),
      'type' => 'timefield_standard',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Time');

  return $field_instances;
}
