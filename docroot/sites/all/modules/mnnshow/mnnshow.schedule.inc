<?php

ctools_include('mnnshow.common', 'mnnshow', '');

/**
 * Page callback.
 *
 * This callback return schedule
 * in JSON format.
 *
 * Important!: Cron job is needed to be set to run
 * every hour to clean cached data!
 */
function mnnshow_schedule_get(){
  try {
    $requested_date = new DateTime($_REQUEST['date'], new DateTimeZone('America/New_York'));
  } catch (Exception $e) {
    $requested_date = new DateTime(NULL, new DateTimeZone('America/New_York'));
  }

  if ($cached = cache_get('mnnshow:schedule:' . $requested_date->format('Y-m-d'), 'cache')) {
    $schedule_data = $cached->data;
  }
  else {
    // primetime is 7pm to midnight
    $primetime_start = new DateTime($requested_date->format('m/d/Y 19:0:0'), new DateTimeZone('America/New_York'));
    $primetime_end = new DateTime($requested_date->format('m/d/Y 23:59:59'), new DateTimeZone('America/New_York'));

    $query = 'SELECT s.project_id, s.title, s.topic, a.start_time, a.duration, a.channel, s.narrative AS description ';
    $query .= 'FROM {ac_report_project_cache} s ';
    $query .= 'LEFT JOIN {ac_report_airing_cache} a ON s.project_id = a.project_id ';
    $query .= 'WHERE (a.start_time >= "'.$requested_date->format('Y-m-d').' 0:00:00" AND a.start_time <= "'.$requested_date->format('Y-m-d').' 23:59:59") ';
    $query .= 'OR (a.end_time >= "'.$requested_date->format('Y-m-d').' 0:00:00" AND a.end_time <= "'.$requested_date->format('Y-m-d').' 23:59:59") ';
    $query .= 'ORDER BY a.channel ASC, a.start_time ASC';

    $result = db_query(db_rewrite_sql($query));
    $ch1 = array();
    $ch2 = array();
    $ch3 = array();
    $ch4 = array();
    while ($data = db_fetch_object($result)) {
      $duration = $data->duration / 60;
      $duration = round($duration / 30) * 30; // round to nearest 30
      $start = new DateTime($data->start_time, new DateTimeZone('America/New_York'));
      $end = new DateTime($data->start_time, new DateTimeZone('America/New_York'));
      $end->add(new DateInterval('PT'.$duration.'M'));
      $now = new DateTime(NULL, new DateTimeZone('America/New_York'));
      $midnight = new DateTime($requested_date->format('m/d/Y 0:0:0'), new DateTimeZone('America/New_York'));
      if ($start < $midnight){
        $interval = $start->diff($midnight);
        $duration -= intval($interval->format('%i'));
      }
      if ($now >= $start && $now <= $end)
        $current = 'true';
      else
        $current = 'false';
      if (mnnshow_is_date_in_range($primetime_start, $primetime_end, $start) || mnnshow_is_date_in_range($primetime_start, $primetime_end, $end->sub(new DateInterval('PT1M'))))
        $primetime = 'true';
      else
        $primetime = 'false';
      $row = array(
        'link' => '/show-info/'.$data->project_id.'/'.mnnshow_url_feiendly_title($data->title),
        'title' => $data->title,
        'category' => isset($data->topic) ? $data->topic : '',
        'start' => $start->format("H-i"),
        'isCurrent' => $current,
        'isPrimetime' => $primetime,
        'duration' => $duration,
        'description' => $data->description,
      );
      switch ($data->channel){
        case 'Channel 1':
          $ch1[] = $row;
          break;
        case 'Channel 2':
          $ch2[] = $row;
          break;
        case 'Channel 3':
          $ch3[] = $row;
          break;
        case 'Channel 4':
          $ch4[] = $row;
          break;
      }
      //print_r($row);
    }

    // time column
    $time_col = array();
    $now = new DateTime(NULL, new DateTimeZone('America/New_York'));
    $hour = $now->format('H');
    $minute = $now->format('i') >= 30 ? '30' : '00';
    $now->setTime($hour, $minute, 0); // round down to nearest half hour

    $time_increment = clone $requested_date;
    $time_increment->setTime(0, 0, 0);
    $duration = 30;
    for ($i = 0; $i < 48; $i++){
      $time_increment == $now ? $current = 'true' : $current = 'false';
      if (mnnshow_is_date_in_range($primetime_start, $primetime_end, $time_increment) || mnnshow_is_date_in_range($primetime_start, $primetime_end, $time_increment))
        $primetime = 'true';
      else
        $primetime = 'false';
      $time_col[] = array(
        'display' => $time_increment->format('g:i A'),
        'start' => $time_increment->format("H-i"),
        'isCurrent' => $current,
        'isPrimetime' => $primetime,
        'duration' => $duration
      );
      $time_increment->add(new DateInterval('PT30M'));
    }

    // dates
    $prevDate = clone $requested_date;
    $nextDate = clone $requested_date;
    $prevDate->sub(new DateInterval('P1D'));
    $nextDate->add(new DateInterval('P1D'));

    $schedule_data = array(
      'schedDate' => $requested_date->format('m/d/Y'),
      'prevDate' => $prevDate->format('m/d/Y'),
      'nextDate' => $nextDate->format('m/d/Y'),
      'displayDate' => $requested_date->format('l, F j'),
      'utc_timezone_offset' => mnnshow_get_utc_timezone_offset(),
      'time' => $time_col,
      'ch1' => $ch1,
      'ch2' => $ch2,
      'ch3' => $ch3,
      'ch4' => $ch4
    );

    // Cache the results and set the cache lifetime for one hour (60 * 60).
    cache_set('mnnshow:schedule:' . $requested_date->format('Y-m-d'), $schedule_data, 'cache', time() + (60 * 60));
  }

  // Print the data in json format.
  mnnshow_cors_headers();
  drupal_json($schedule_data);
}

/**
 * Page callback.
 */
function mnnshow_schedule_table_header(){
  $channels = mnnshow_get_channels();
  //print_r($channels);

  mnnshow_cors_headers();
  drupal_json(array(
                'channel1' => $channels['1'],
                'channel2' => $channels['2'],
                'channel3' => $channels['3'],
                'channel4' => $channels['4']
              ));
}