<?php
/**
 * @file
 *
 * Contains Time tracker API and internal functions.
 */

/**
 * Load Time tracks from a given time.
 *
 * @param $time
 *   A time entry object with the time period.
 * @param $user
 *   Optional integer with the user id to filter tracks. Or a array with value
 *   and operator keys.
 * @param $case
 *   Optional integer with the case id to filter tracks. Or a array with value
 *   and operator keys.
 * @param $status
 *   Optional string with the track status to filter tracks. Or a array with
 *   value and operator keys.
 *
 * @return
 *   Array with the loaded Time tracks.
 *
 * @see timetracker_track_list()
 */
function timetracker_track_load($time, $user = NULL, $case = NULL, $status = NULL) {
  $tracks = timetracker_track_list($time, $user, $case, $status);
  $time_entries = array();

  if (!empty($tracks['time_entry'])) {
    $time_entries = entity_load('time_entry', array_keys($tracks['time_entry']));
  }

  foreach ($time_entries as $time_entry) {
    // We need to set $end manually due to a bug in Time Entry
    // See http://drupal.org/node/1854630
    $time_entry->end = $time_entry->time + $time_entry->duration;
  }

  return $time_entries;
}

/**
 * List Time tracks from a given time.
 *
 * This function doesn't load the Time entry entities. If you want them, use
 * timetracker_track_load() instead.
 *
 * @param $time
 *   A time entry object with the time period.
 * @param $user
 *   Optional integer with the user id to filter tracks. Or a array with value
 *   and operator keys.
 * @param $case
 *   Optional integer with the case id to filter tracks. Or a array with value
 *   and operator keys.
 * @param $status
 *   Optional string with the track status to filter tracks. Or a array with
 *   value and operator keys.
 *
 * @return
 *   A TimeEntryEntityFieldQuery object with the executed query.
 *
 * @see timetracker_track_laod()
 * @see time_entry_get_time_entry_ids_by_period()
 */
function timetracker_track_list($time, $user = NULL, $case = NULL, $status = NULL) {
  $fields = array();

  if (isset($user)) {
    $fields['field_timetrack_user'] = array(
      'column' => 'target_id',
      'operator' => '=',
      'value' => 0
    );

    if (is_array($user)) {
      $fields['field_timetrack_user'] = array_merge($fields['field_timetrack_user'], $user);
    }
    else {
      $fields['field_timetrack_user']['value'] = $user;
    }
  }

  if (isset($case)) {
    $fields['field_timetrack_case'] = array(
      'column' => 'target_id',
      'operator' => '=',
      'value' => 0,
    );

    if (is_array($case)) {
      $fields['field_timetrack_case'] = array_merge($fields['field_timetrack_case'], $case);
    }
    else {
      $fields['field_timetrack_case']['value'] = $case;
    }
  }

  if (isset($status)) {
    $fields['field_timetrack_status'] = array(
      'column' => 'value',
      'operator' => '=',
      'value' => 0,
    );

    if (is_array($status)) {
      $fields['field_timetrack_status'] = array_merge($fields['field_timetrack_status'], $status);
    }
    else {
      $fields['field_timetrack_status']['value'] = $status;
    }
  }

  return time_entry_get_time_entry_ids_by_period((int) $time->time, (int) $time->end, 'timetrack', $fields);
}

/**
 * Save Time track in a given time frame and return the resulted track.
 *
 * This function takes care of existing tracks of the same user, case and
 * status intersected in the given time frame, merging all of them in a new
 * unique track. The old tracks are deleted.
 *
 * Existing tracks of the same user and different case intersected in the given
 * time frame are no allowed, and produces a error, returning FALSE.
 *
 * @param $time
 *   A time entry object with the time period.
 * @param $user
 *   Integer with the user id.
 * @param $case
 *   Integer with the case id.
 * @param $status
 *   Optional string with the track status. The default value is 'new'.
 *
 * @return
 *   The saved Time track object. Please note that the returned track can have
 *   a start or end times different from the given ones, if a merge is made.
 *   The returned value will be FALSE if the track can't be saved (see function
 *   description for why).
 */
function timetracker_track_save($time, $user, $case, $status = 'new', &$error = NULL) {
  $existing_tracks = timetracker_track_load($time, $user);
  $common_tracks = timetracker_track_load($time, $user, $case, $status);
  $time->type = 'timetrack';

  $transaction = db_transaction();

  if (!empty($existing_tracks)) {
    // We don't know what to do when there are tracks intersecting the time
    // frame, so we do nothing and returns FALSE.
    if (count($existing_tracks) > count($common_tracks)) {
      foreach ($existing_tracks as $track) {
        // Except for existing tracks touching the edges, those can be ignored.
        if ($track->time + $track->duration == $time->time || $track->time == $time->end) {
          continue;
        }
        // Those are really an intersection, so abort.
        else {
          return FALSE;
        }
      }
    }

    foreach ($common_tracks as $track) {
      $time = $time->merge($track);
      timetracker_track_delete($track, $user, $case, $status);
    }
  }

  $time->timetrack_user = $user;
  $time->timetrack_case = $case;
  $time->timetrack_status = $status;

  //invoke access hook
  $access = module_invoke_all('timetracker_track_save', $time);

  if (in_array(FALSE, $access, TRUE)) {
    $transaction->rollback();
    $errors = drupal_get_messages('error');
    $error = $errors['error'][0];
    return FALSE;
  }

  return time_entry_save($time);
}

/**
 * Delete Time track in a given time frame for the given filter conditions.
 *
 * This function takes care of existing tracks intersected with the given time
 * frame, creating new tracks for the non intersected areas. The old tracks are
 * deleted.
 *
 * Let's suppose you have the following time track saved at database (where +
 * is a tracked time and - is a untracked time) from 2am to 7am:
 *
 * 01      02      03      04      05      06      07      08      09      10
 * --------++++++++++++++++++++++++++++++++++++++++--------------------------
 *
 * Note that you have a single Time entry entity.
 *
 * Then you call timetracker_track_delete() with a $time parameter from 4am to
 * 6am. Now this is what you get:
 *
 * 01      02      03      04      05      06      07      08      09      10
 * --------++++++++++++++++----------------++++++++--------------------------
 *
 * Now you have two Time entries, one from 2am to 4am and another from 6am to
 * 7am. Those are new Time entry entities (with new ids), the old one was
 * completely deleted.
 *
 * @param $time
 *   A time entry object with the time period to be deleteed.
 * @param $user
 *   Optional integer with user id to filter tracks to be deleted. Or a array
 *   with value and operator keys.
 * @param $case
 *   Optional integer with case id to filter tracks to be deleted. Or a array
 *   with value and operator keys.
 * @param $status
 *   Optional string with track status to filter tracks to be deleted. Or a
 *   array with value and operator keys.
 *
 * @see timetracker_track_laod()
 */
function timetracker_track_delete($time, $user = NULL, $case = NULL, $status = NULL) {
  $tracks = timetracker_track_load($time, $user, $case, $status);
  
  foreach ($tracks as $track) {
    // Intersected tracks needs special treatment
    if ($track->time < $time->time || $track->end > $time->end) {
      if ($track->time < $time->time) {
        $track_before = clone $track;
        $track_before->id = NULL;
        $track_before->end = $time->time;
        $track_before->duration = $track_before->end - $track_before->time;
        time_entry_save($track_before);
      }

      if ($track->end > $time->end) {
        $track_after = clone $track;
        $track_after->id = NULL;
        $track_after->time = $time->end;
        $track_after->duration = $track_after->end - $track_after->time;
        time_entry_save($track_after);
      }
    }

    time_entry_delete($track);
  }
}
