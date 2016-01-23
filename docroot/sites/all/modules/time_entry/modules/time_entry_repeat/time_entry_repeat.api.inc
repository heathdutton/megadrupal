<?php
/**
 * @file
 *
 * Provides the Time Entry Repeat API functions.
 */

/**
 * Check if a repeated time entry is in a given range.
 *
 * @param $time
 *   A time entry object with repetition of the time you want to search in the
 *   range.
 * @param $range
 *   A time entry object of the range (note: you don't need to have this
 *   object stored in the database, you can create it on-the-fly).
 *
 * @return
 *   Array of time entry objects for each of the repeated dates, each one of
 *   them populated with start and end times.
 *
 *   Note that if you pass a $time without repeat property, you still can get
 *   one return if it's in the range.
 *
 * @see time_entry_in_range().
 */
function time_entry_repeat_in_range($time, $range) {
  $repeat = 0;
  $return = array();

  switch ($time->repeat) {
    case 'yearly':
      // @todo: Yealy repetitions needs a special attention due to bissext years.
      $return = _time_entry_repeat_in_range_regular($time, $range, 365 * 30 * 24 * 60 * 60);
      break;
    case 'monthly':
      // @todo: Monthly repetitions needs a special attention due to different days in a month.
      $return = _time_entry_repeat_in_range_regular($time, $range, 30 * 24 * 60 * 60);
      break;
    case 'weekly':
      $return = _time_entry_repeat_in_range_regular($time, $range, 7 * 24 * 60 * 60);
      break;
    case 'daily':
      $return = _time_entry_repeat_in_range_regular($time, $range, 24 * 60 * 60);
      break;
    default:
      if (is_int($time->repeat)) {
        $return = _time_entry_repeat_in_range_regular($time, $range, $time->repeat);
      }
      break;
  }

  return $return;
}

/**
 * Auxiliar function to get a regular repeated time in a given range.
 *
 * @param $time
 *   A time entry object with repetition of the time you want to search in the
 *   range.
 * @param $range
 *   A time entry object of the range (note: you don't need to have this
 *   object stored in the database, you can create it on-the-fly).
 * @param $repeat
 *   Number of seconds of regular repetition.
 *
 * @see time_entry_repeat_in_range().
 */
function _time_entry_repeat_in_range_regular($time, $range, $repeat) {
  $range_start = $range->time;
  $range_end = $range->end;
  $return = array();

  $start = $time->time;
  $duration = $time->duration;
  $end = $time->end;

  $in_range = time_entry_in_range($time, $range);
  if (!empty($in_range)) {
    $return[] = $in_range;
  }

  while ($end < $range_end) {
    $time = time_entry_create(array('time' => $end + $repeat, 'duration' => $duration));
    $start = $time->time;
    $end = $time->end;

    $in_range = time_entry_in_range($time, $range);
    if (!empty($in_range)) {
      $return[] = $in_range;
    }
  }

  return $return;
}

/**
 * Load a repeat property from a given time entry.
 *
 * @param $time
 *   A time entry object.
 *
 * @return
 *   The repeat value or FALSE if it doesn't exists.
 */
function time_entry_repeat_load($time) {
  list($id) = entity_extract_ids('time_entry', $time);
  $repeat = db_query('SELECT "repeat" FROM {time_entry_repeat} WHERE "id" = :id', array(':id' => $id))->fetchObject();

  if ($repeat) {
    return $repeat->repeat;
  }

  return FALSE;
}

/**
 * Save a repeat property for a given time entry.
 *
 * @param $time
 *   A time entry object with the repeat property.
 */
function time_entry_repeat_save($time) {
  list($id) = entity_extract_ids('time_entry', $time);

  return db_merge('time_entry_repeat')
    ->key(array('id' => $id))
    ->fields(array(
      '"repeat"' => $time->repeat,
    ))
    ->execute();
}

/**
 * Delete a repeat property from a given time entry.
 *
 * @param $time
 *   A time entry object.
 */
function time_entry_repeat_delete($time) {
  list($id) = entity_extract_ids('time_entry', $time);
  $repeat = time_entry_repeat_load($time);

  if ($repeat) {
    return db_delete('time_entry_repeat')
             ->condition('id', $id)
             ->execute();
  }
  return FALSE;
}

/**
 * Get a list of repeat options.
 */
function time_entry_repeat_get_options() {
  return array(
    'yearly' => t('Yearly'),
    'monthly' => t('Monthly'),
    'weekly' => t('Weekly'),
    'daily' => t('Daily'),
  );
}