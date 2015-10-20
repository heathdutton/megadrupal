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
