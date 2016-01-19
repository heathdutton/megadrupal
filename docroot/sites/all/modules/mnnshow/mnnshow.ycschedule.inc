<?php

ctools_include('mnnshow.common', 'mnnshow', '');

/**
 * The channels/days/times that YC is on the air.
 */
function mnnshow_yc_schedules() {
  // schedule 1
  $schedules[0][] = array(
    'channel' => 1,
    'day' => 0, // monday
    'start_time' => '16:00:00', // 4pm
    'stop_time' => '16:59:59', // 5pm
  );
  $schedules[0][] = array(
    'channel' => 1,
    'day' => 1, // tuesday
    'start_time' => '16:00:00', // 4pm
    'stop_time' => '16:59:59', // 5pm
  );
  $schedules[0][] = array(
    'channel' => 1,
    'day' => 2, // wednesday
    'start_time' => '16:00:00', // 4pm
    'stop_time' => '16:59:59', // 5pm
  );
  $schedules[0][] = array(
    'channel' => 1,
    'day' => 3, // thursday
    'start_time' => '16:00:00', // 4pm
    'stop_time' => '16:59:59', // 5pm
  );
  $schedules[0][] = array(
    'channel' => 1,
    'day' => 4, // friday
    'start_time' => '16:00:00', // 4pm
    'stop_time' => '16:59:59', // 5pm
  );

  // schedule 2
  $schedules[1][] = array(
    'channel' => 4,
    'day' => 0, // monday
    'start_time' => '17:00:00', // 5pm
    'stop_time' => '17:59:59', // 6pm
  );
  $schedules[1][] = array(
    'channel' => 4,
    'day' => 1, // tuesday
    'start_time' => '17:00:00', // 5pm
    'stop_time' => '17:59:59', // 6pm
  );
  $schedules[1][] = array(
    'channel' => 4,
    'day' => 2, // wednesday
    'start_time' => '17:00:00', // 5pm
    'stop_time' => '17:59:59', // 6pm
  );
  $schedules[1][] = array(
    'channel' => 4,
    'day' => 3, // thursday
    'start_time' => '17:00:00', // 5pm
    'stop_time' => '17:59:59', // 6pm
  );
  $schedules[1][] = array(
    'channel' => 4,
    'day' => 4, // friday
    'start_time' => '17:00:00', // 5pm
    'stop_time' => '17:59:59', // 6pm
  );

  return $schedules;
}

/**
 * An array to map numeric DOWs to text DOWs.
 */
function mnnshow_yc_dow() {
  return array('Mon', 'Tue', 'Wed', 'Thurs', 'Fri', 'Sat', 'Sun');
}

/**
 * Page callback.
 *
 * This callback return youth-channel schedule
 * in JSON format.
 *
 * Important!: Cron job is needed to be set to run
 * every hour to clean cached data!
 */
function mnnshow_yc_schedule_get(){
  $schedules = mnnshow_yc_schedules();

  try {
    $requested_date = new DateTime($_REQUEST['date'], new DateTimeZone('America/New_York'));
  } catch (Exception $e) {
    $requested_date = new DateTime(NULL, new DateTimeZone('America/New_York'));
  }

  if ($cached = cache_get('mnnshow:yc_schedule:' . $requested_date->format('Y-m-d'), 'cache')) {
    $schedule_data = $cached->data;
  }
  else {
    //--- date of the first & end of the week (monday & sunday)
    $week = mnnshow_get_week_range($requested_date->format('d'), $requested_date->format('m'), $requested_date->format('Y'));
    $week_start = new DateTime($week['first'], new DateTimeZone('America/New_York'));
    $week_end = new DateTime($week['last'], new DateTimeZone('America/New_York'));

    //--- create where clause
    $wherea = array();
    foreach ($schedules as $schedule_num => $schedule) {
      foreach ($schedule as $item) {
        // cycle thru each on_air item, building the where clause
        // reset the current_day on each loop
        // then add the on_air 'day' to current_day
        $current_day = clone $week_start;
        $current_day->add(new DateInterval('P' . $item['day'] . 'D'));
        $w = 'a.channel = "Channel ' . $item['channel'] . '" AND (';
        $w .= '(a.start_time >= "'.$current_day->format('Y-m-d').' ' . $item['start_time'] . '" AND a.start_time <= "'.$current_day->format('Y-m-d').' ' . $item['stop_time'] . '") ';
        $w .= 'OR (a.end_time > "'.$current_day->format('Y-m-d').' ' . $item['start_time'] . '" AND a.end_time <= "'.$current_day->format('Y-m-d').' ' . $item['stop_time'] . '")';
        $w .= ') ';
        $wherea[] = $w;
      }
    }

    $where = implode(' OR ', $wherea);

    //--- run the query
    $query = 'SELECT s.project_id, s.title, s.topic, a.start_time, a.duration, a.channel ';
    $query .= 'FROM {ac_report_project_cache} s ';
    $query .= 'LEFT JOIN {ac_report_airing_cache} a ON s.project_id = a.project_id ';
    $query .= 'WHERE ('. $where .') ';
    $query .= 'ORDER BY a.start_time ASC';
    $result = db_query(db_rewrite_sql($query));

    //--- individual shows
    $shows = array();
    foreach ($schedules as $schedule_num => $schedule) {
      $shows['schedule-' . $schedule_num] = array();
    }
    while ($data = db_fetch_object($result)) {
      $show = mnnshow_yc_set_show_properties($data, $schedules);
      $schedule_num = $show['schedule_num'];
      $shows['schedule-' . $schedule_num][] = $show;
    }

    $dow_map = mnnshow_yc_dow();
    //--- day of week columns & time rows
    foreach ($schedules as $schedule_num => $schedule) {
      $all_times = array();
      foreach ($schedule as $item) {
        $day_num = $item['day'] + 1;
        $day_str = $dow_map[$item['day']];
        $day_col['schedule-' . $schedule_num][$day_num] = $day_str;

        $all_times[] = $item['start_time'];
      }
      $unique_times = array_unique($all_times);
      foreach ($unique_times as $unique_time) {
        $unique_datetime = new DateTime('1/1/2012 ' . $unique_time);
        $time_row['schedule-' . $schedule_num][] = $unique_datetime->format('g:i a');
      }
    }

    // dates
    $prevDate = clone $requested_date;
    $nextDate = clone $requested_date;
    $prevDate->sub(new DateInterval('P7D'));
    $nextDate->add(new DateInterval('P7D'));

    $schedule_data = array(
      'schedDate' => $requested_date->format('m/d/Y'),
      'prevDate' => $prevDate->format('m/d/Y'),
      'nextDate' => $nextDate->format('m/d/Y'),
      'displayDate' => mnnshow_format_date_range($week_start, $week_end),
      'utc_timezone_offset' => mnnshow_get_utc_timezone_offset(),
      'shows' => $shows,
      'dayCol' => $day_col,
      'timeRow' => $time_row
    );

    // Cache the results and set the cache lifetime for one hour (60 * 60).
    cache_set('mnnshow:yc_schedule:' . $requested_date->format('Y-m-d'), $schedule_data, 'cache', time() + (60 * 60));
  }

  drupal_set_header('Access-Control-Allow-Origin: *');
  drupal_set_header('Access-Control-Allow-Headers: X-Requested-With');
  drupal_set_header('Access-Control-Max-Age: 3600');
  drupal_json($schedule_data);
}

/**
 * Set all show properties from the returned database data.
 * @return array : the show with data.
 */
function mnnshow_yc_set_show_properties($db_data, $schedules) {
  $show = array();

  // parse database data
  $start_datetime = new DateTime($db_data->start_time, new DateTimeZone('America/New_York'));
  $show['duration_actual'] = $db_data->duration / 60; // convert seconds to minutes
  $show['duration'] = round($show['duration_actual'] / 30) * 30; // round to nearest 15 minutes
  $show['link'] = '/show-info/'.$db_data->project_id.'/'.mnnshow_url_feiendly_title($db_data->title);
  $show['title'] = $db_data->title;
  $show['category'] = (isset($db_data->topic)) ? $db_data->topic : '';
  $show['channel'] = $db_data->channel;

  $end_datetime = clone $start_datetime;
  $end_datetime->add(new DateInterval('PT' . $show['duration'] . 'M'));

  $show['start_date'] = $start_datetime->format('Y/m/d');

  // day_of_week (monday = 1, sunday = 7)
  $show['day_of_week'] = intval($start_datetime->format('w'));
  if ($show['day_of_week'] == 0) {
    $show['day_of_week'] = 7;
  }

  // is_today
  $today = new DateTime(NULL, new DateTimeZone('America/New_York'));
  if ($today->format('Y-m-d') == $start_datetime->format('Y-m-d')) {
    $show['is_today'] = 'true';
  }
  else {
    $show['is_today'] = 'false';
  }

  // is_current
  $now = new DateTime(NULL, new DateTimeZone('America/New_York'));
  if ($now >= $start_datetime && $now <= $end_datetime) {
    $show['is_current'] = 'true';
  }
  else {
    $show['is_current'] = 'false';
  }

  // is_primetime (primetime is 7pm to midnight)
  $primetime_start = new DateTime($start_datetime->format('m/d/Y 19:00:00'), new DateTimeZone('America/New_York'));
  $primetime_end = new DateTime($start_datetime->format('m/d/Y 23:59:59'), new DateTimeZone('America/New_York'));
  if (mnnshow_is_date_in_range($primetime_start, $primetime_end, $start_datetime)
      || mnnshow_is_date_in_range($primetime_start, $primetime_end, $end_datetime)) {
    $show['is_primetime'] = 'true';
  }
  else {
    $show['is_primetime'] = 'false';
  }

  // schedule number
  foreach ($schedules as $schedule_num => $schedule) {
    foreach ($schedule as $item) {

      $format_string = $start_datetime->format('m/d/Y ') . $item['start_time'];
      $item_start = new DateTime($format_string, new DateTimeZone('America/New_York'));

      $format_string = $end_datetime->format('m/d/Y ') . $item['stop_time'];
      $item_end = new DateTime($format_string, new DateTimeZone('America/New_York'));

      // is channels/days equal
      if ($item['channel'] == (int) preg_replace('/\D/', '', $show['channel'])
          && ($item['day'] + 1) == $show['day_of_week']) {

        // is show's start or end in this item's start/end range
        if (mnnshow_is_date_in_range($item_start, $item_end, $start_datetime)
            || mnnshow_is_date_in_range($item_start, $item_end, $end_datetime)) {
          $show['schedule_num'] = $schedule_num;

          // is show's start before this item's start
          if ($start_datetime < $item_start){
            $diff = $item_start->diff($start_datetime);
            $show['duration'] -= intval($diff->format('%i'));
          }
          // is show's end after this item's end
          if ($end_datetime > $item_end){
            $diff = $end_datetime->diff($item_end);
            $show['duration'] -= intval($diff->format('%i'));
          }

          break 2; // exit both loops
        }
      }
    }
  }

  // ordinal (used for horizontal placement in grid)
  static $last_date = '';
  static $last_channel = '';
  static $last_ordinal = 0;
  static $last_duration = 0;
  if (($show['start_date'] == $last_date) && ($show['channel'] == $last_channel)) {
    $show['ordinal'] = $last_ordinal + $last_duration;
    $last_ordinal = $show['ordinal'];
    $last_duration = $show['duration'];
  }
  else {
    $last_date = $show['start_date'];
    $last_channel = $show['channel'];
    $last_ordinal = 0;
    $last_duration = $show['duration'];
    $show['ordinal'] = 0;
  }
  unset($show['start_date']);

  return $show;
}


