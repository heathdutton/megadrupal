<?php

/**
 * @file
 * Hooks provided by the studyroom module.
 */

/**
 * Provides a method to update date fields that use a time increment value.
 *
 * @param int $time_increment
 *   The new time increment value.
 */
function hook_studyroom_time_increment_update($time_increment) {
  foreach (studyroom_reservation_types() as $bundle) {
    $instance = field_info_instance('studyroom_reservation', 'field_reservation_datetime', $bundle->type);
    $instance['widget']['settings']['increment'] = $time_increment;
    field_update_instance($instance);
  }
}

/**
 * Provides a method to retrieve the start and end time of a studyroom space.
 *
 * @param StudyroomSpace $space
 *  StudyroomSpace object for which the hours are needed.
 *
 * @param DateObject $date
 *  The date for which the hours are needed.
 *
 * @return array
 *  start_time: DateObject representing the opening time of the space
 *  end_time: DateObject representing the closing time of the space
 *
 *  Return FALSE (boolean) if a start and end time cannot be retrieved for the given study space and date
 */
function hook_studyroom_space_hours($space, $date) {
  return array(
    'start_time' => new DateObject('2015-07-14 08:00:00'),
    'end_time' => new DateObject('2015-07-14 22:00:00'),
  );
}
