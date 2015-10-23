<?php

/**
 * Timeslot selection handler for use in referencing from a session.
 *
 * This should be used for the entityreference field on session entities which
 * points to the timeslot when the session is on. It displays the form widget
 * as a select list of timeslots showing the containing schedule and the start
 * and end time for each timeslot.
 */
class SessionTimeslotSelectionHandler extends EntityReference_SelectionHandler_Generic {

  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    $class = __CLASS__;
    return new $class($field, $instance, $entity_type, $entity);
  }

  /**
   * Implements EntityReferenceHandler::settingsForm().
   */
  public static function settingsForm($field, $instance) {
    $entity_info = entity_get_info($field['settings']['target_type']);

    // Merge-in default values.
    $field['settings']['handler_settings'] += array(
      'target_bundles' => array(),
      'sort' => array(
        'type' => 'none',
      )
    );

    if (!empty($entity_info['entity keys']['bundle'])) {
      $bundles = array();
      foreach ($entity_info['bundles'] as $bundle_name => $bundle_info) {
        $bundles[$bundle_name] = $bundle_info['label'];
      }

      $form['target_bundles'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Target bundles'),
        '#options' => $bundles,
        '#default_value' => $field['settings']['handler_settings']['target_bundles'],
        '#size' => 6,
        '#description' => t('The bundle of the entity type that can be referenced. Only one timeslot bundle should be selected.'),
        // TODO: add validation for single bundle.
        '#element_validate' => array('_entityreference_element_validate_filter'),
      );
    }
    else {
      $form['target_bundles'] = array(
        '#type' => 'value',
        '#value' => array(),
      );
    }

    return $form;
  }

  /**
   * Implements EntityReferenceHandler::getReferencableEntities().
   *
   * Completely bypass buildEntityFieldQuery(), as we want a regular SQL query
   * in order to group and order the timeslots by their containing schedules.
   */
  public function getReferencableEntities($match = NULL, $match_operator = 'CONTAINS', $limit = 25) {
    // We also come here when getting entities for the default value widget.
    // There's no way to check this is what we're doing so just return nothing
    // if the settings are missing.
    if (empty($this->field['settings']['handler_settings']['behaviors']['session_timeslot']['status'])) {
      return array();
    }

    // Figure out which fields are on schedules and point to timeslots.
    $timetable_set = timetable_get_timetable_set($this->field);
    $schedule_timeslot_field_name = $timetable_set['schedule']['field_name'];
    $schedule_entity_type = $timetable_set['schedule']['entity_type'];

    // Get details about the schedule entity type for the query.
    $schedule_entity_info = entity_get_info($schedule_entity_type);
    $schedule_entity_base_table = $schedule_entity_info['base table'];
    $schedule_entity_id_key = $schedule_entity_info['entity keys']['id'];
    $schedule_entity_label_key = $schedule_entity_info['entity keys']['label'];

    $results = db_query("SELECT
      ts.id,
      time.field_timeslot_time_value AS timeslot_value,
      time.field_timeslot_time_value2 AS timeslot_value2,
      schedule.$schedule_entity_label_key AS schedule_label
      FROM {eck_timeslot} ts
      JOIN {field_data_field_timeslot_time} time
        ON ts.id = time.entity_id
      JOIN {field_data_{$schedule_timeslot_field_name}} sts
        ON ts.id = sts.{$schedule_timeslot_field_name}_target_id
      JOIN {{$schedule_entity_base_table}} schedule
        ON sts.entity_id = schedule.$schedule_entity_id_key
      ")->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    // TODO: get this from the field instance so we match whatever format the
    // site builder wants.
    $timefield_display_format = array(
      'hour' => 'G',
      'minute' => 'i',
      'separator' => ':',
      'period' => 'none',
      'period_separator' => '',
    );

    $timeslot_options = array();
    foreach ($results as $timeslot_id => $timeslot_data) {
      $timeslot_options[$timeslot_id] = check_plain($timeslot_data['schedule_label']) . ': '
        . timefield_integer_to_time($timefield_display_format, $timeslot_data['timeslot_value']) . '-'
        . timefield_integer_to_time($timefield_display_format, $timeslot_data['timeslot_value2']);
    }

    $options = array(
      'timeslot' => $timeslot_options,
    );

    return $options;
  }

}
