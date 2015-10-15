<?php
define('TIMETRACKER_INCREMENT', 15);
define('TIMETRACKER_LIVETRACK_CYCLE', 30);

function timetracker_table_form($form, &$form_state, $case, $week = '') {
  global $user;
  $account = $user;

  drupal_set_title('Track time for Case #'.$case->nid);

  $increment = TIMETRACKER_INCREMENT;
  for ($h = 0; $h <= 23; $h++) {
    for ($m = 0; $m <= 60-$increment; $m += $increment) {
      $label = $m != 0 ? ".$m" : (string)$h;
      $key = sprintf('%02d%02d', $h, $m);
      $headers[$key] = $label;
    }
  }

  if (empty($week)) {
    $week = date('o-\WW');
  }

  // Start date is first second of the first ISO day (Monday) from the week.
  $start_date = new DateTime($week);
  $today = new DateTime();

  // Prevents from showing hours in the future!
  if ($start_date > $today) {
    $start_date = new DateTime(date('o-\WW'));
  }

  // Max date is the last date from current week (i.e. Sunday), this the same of
  // saying the day before the next week, that is exactly what we calculate below.
  $max_date = clone $start_date;
  $max_date->modify('+1 week')->modify('-1 day');

  if ($max_date > $today) {
    $max_date = clone $today;
  }

  $bypass_locked_check = FALSE;
  $form['days']['#tree'] = TRUE;

  $range = time_entry_create(array(
    'time' => $start_date->format('U') + 1,
    'end' => $max_date->format('U') + 3600 * 24,
  ));
  $time_entries = timetracker_find_by_user($account, $range);

  for ($day = clone $max_date; $day->format('Ymd') >= $start_date->format('Ymd'); $day->modify('-1 day')) {
    $date = $day->format('Ymd');

    if (!timetracker_is_period_locked($day->format('c'), $day->format('Y-m-d\T23:59:59P'), $account, $case)) {
      $bypass_locked_check = TRUE;
    }

    $form['days'][$date] =  array(
      '#type' => 'fieldset',
      '#title' => sprintf('<span class="human-day">%s</span> <span class="formatted-day">%s</span>', _timetracker_format_human_date($day), $day->format('d/m/Y')),
      '#attributes' => array('#class' => array('day')),
      'times' => array(),
    );

    if (date('Ymd') == $date) {
      $form['days'][$date]['#attributes']['#class'][] = 'today';
    }


    $weekday = $day->format('w');
    if ($weekday == 0 || $weekday == 6) { // Add classes for weekends to prevent misclicks
      $form['days'][$date]['#attributes']['#class'][] = 'weekend';
    }
    $form['days'][$date]['#attributes']['#class'][] = 'weekday-'. $weekday; // We're currently not using this class but it may come handy in the future

    foreach ($headers as $time => $value) {
      $form['days'][$date]['times'][$date . $time] = array(
        '#type' => 'checkbox',
        '#title' => substr($time, -2) == '00' ? $value : '&bull;',
        '#attributes' => array(
          'class' => array(substr($time, -2) == '00' ? 'full-hour' : 'fragment'),
        ),
        '#default_value' => FALSE,
      );

      $start = new DateTime($date . $time);
      $end = clone $start;
      $end->modify('+ '. ($increment-1).' minutes 59 seconds');

      $range = time_entry_create(array(
        'time' => $start->format('U') + 1,
        'end' => $end->format('U')
      ));
      $timetracker = timetracker_find_by_user_with_preloaded_time_entries($range, $time_entries);

      if (count($timetracker)) {
        $disabled = FALSE;
        foreach ($timetracker as $track) {
          if ($track->timetrack_case != $case->nid) {
            $disabled = TRUE;
            // We don't use Drupal's #disabled property because we need to
            // check the submitted value later, and Drupal ignores it when the
            // form element is disabled.
            $form['days'][$date]['times'][$date . $time]['#timetracker_disabled'] = TRUE;
            $form['days'][$date]['times'][$date . $time]['#attributes']['class'][] = 'disabled';
          }
          else {
            switch ($track->timetrack_status) {
              case 'approved':
                $disabled = TRUE;
                $form['days'][$date]['times'][$date . $time]['#timetracker_disabled'] = TRUE;
                $form['days'][$date]['times'][$date . $time]['#attributes']['class'][] = 'disabled';
                $form['days'][$date]['times'][$date . $time]['#attributes']['class'][] = 'approved';
                break;
                case 'rejected':
                $form['days'][$date]['times'][$date . $time]['#attributes']['class'][] = 'rejected';
                break;
            }
          }
        }

        if (!$disabled) {
          $form['days'][$date]['times'][$date . $time]['#default_value'] = TRUE;
          $form['days'][$date]['times'][$date . $time]['#attributes']['class'][] = 'active';
        }
      }
    }

    $bypass_locked_check = FALSE;
  }

  $form['time_headers'] = array(
    '#value' => $headers
  );

  $form['increment'] = array(
    '#type' => 'value',
    '#value' => $increment,
  );

  $form['case'] = array(
    '#type' => 'value',
    '#value' => $case->nid,
  );

  $form['user'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $prev_date = clone $start_date;
  $prev_date->modify('-1 week');
  $prev_link = array(
    'title' => t('← Previous week'),
    'href'  => 'timetracker/track/'. arg(2) .'/'. $prev_date->format('o-\WW'),
  );

  if ($max_date < $today) {
    $next_date = clone $max_date;
    $next_date->modify('+1 week');
    $next_link = array(
      'title' => t('Next week →'),
      'href'  => 'timetracker/track/'. arg(2) .'/'. $next_date->format('o-\WW'),
    );
  }

  $pager_links = array();

  $pager_links[] = $prev_link;
  if (isset($next_link)) {
    $pager_links[] = $next_link;
  }

  $form['pager'] = array(
    '#theme' => 'links',
    '#links' => $pager_links,
    '#attributes' => array(
      'class' => array('inline', 'timetracker-pager')
    )
  );

  $path = drupal_get_path('module', 'timetracker');
  $form['#attached']['css'][] = array(
    'type' => 'file',
    'data' => $path . '/css/timetracker.table.form.css',
  );
  $form['#attached']['js'][] = array(
    'type' => 'file',
    'data' => $path . '/js/timetracker.table.form.js',
  );
  $form['#attached']['css'][] = array(
    'type' => 'file',
    'data' => $path . '/css/timetracker.table.form.livetrack.css',
  );
  $form['#attached']['js'][] = array(
    'type' => 'file',
    'data' => $path . '/js/timetracker.table.form.livetrack.js',
  );
  $form['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => array(
      'timetracker' => array(
        'path' => $path,
        'livetrack' => array(
          'cycle' => TIMETRACKER_LIVETRACK_CYCLE,
        ),
      ),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function timetracker_table_form_submit($form, &$form_state) {
  module_load_include('inc', 'timetracker');
  $user = $form_state['values']['user'];
  $case = $form_state['values']['case'];
  $days = $form_state['values']['days'];
  $increment = $form_state['values']['increment'];
  $error = FALSE;
  $duration = 0;

  foreach ($days as $day_str => $day) {
    foreach ($day['times'] as $element => $value) {
      $form_element = $form['days'][$day_str]['times'][$element];

      // Only call track functions for allowed, changed elements
      if ((empty($form_element['#timetracker_disabled']) || !$form_element['#timetracker_disabled']) && $form_element['#default_value'] != $value) {
        if ($value) {
          if (!isset($start)) {
            $start = $element;
          }

          $duration += $increment * 60;
        }
        if (!$value) {
          $time = time_entry_create(array(
            'type' => 'timetrack',
            'time' => strtotime($element),
            'duration' => $increment * 60,
          ));

          timetracker_track_delete($time, $user, $case, array('operator' => '!=', 'value' => 'approved'));
        }
      }
      elseif (!empty($form_element['#timetracker_disabled']) && $form_element['#timetracker_disabled'] && $form_element['#default_value'] != $value) {
        $error = t('Error trying to track a disallowed time. Please refresh the Time tracker form and try again.');
      }

      if (isset($start) && (!$value || $element == end(array_keys($day['times'])))) {
        $time = time_entry_create(array(
          'type' => 'timetrack',
          'time' => strtotime($start),
          'duration' => $duration,
        ));

        $saved = timetracker_track_save($time, $user, $case, 'new', $error);
        if ($saved === FALSE) {
          if (empty($error)) {
            $error = t('Error while trying to save your tracked time.');
          }
        }

        unset($start);
        $duration = 0;
      }
    }
  }

  if ($error) {
    $return['status'] = 'error';
    $return['message'] = $error;
  }
  else {
    $return['status'] = 'status';
    $return['message'] = t('Thank you for tracking! Time tracker updated!');
  }

  return _timetracker_table_form_return($return);
}

function theme_timetracker_table_form($variables) {
  $form = $variables['form'];
  $output = '';
  $rows = array();

  foreach ($form['days'] as $datestamp => $day) {
    if(is_numeric($datestamp)){
      $rows[$datestamp] = array(
        'class' => $day['#attributes']['#class']
      );
      $rows[$datestamp]['data'][] = array(
        'data' => $day['#title'],
        'header' => TRUE
      );
      foreach ($day['times'] as $timestamp => $time) {
        if(isset($time['#type']) && $time['#type'] == 'checkbox') {
          $timestamp = substr($timestamp, 8);
          $timestamp = str_replace(':', '-', $timestamp);

          $time['#attributes']['class'][] = 'time-' .$timestamp;
          $rows[$datestamp]['data'][] = array(
            'data' => drupal_render($time),
            'class' => $time['#attributes']['class'],
          );
        }
      }
    }
  }

  $headers = array();
  $headers[] = '';
  foreach ($form['time_headers']['#value'] as $key => $header) {
    $headers[] = array(
      'data' => $header,
      'id' => 'timetracker-time-' . str_replace(':', '-', $key)
    );
  }

  $table = array(
    'header' => $headers,
    'rows' => $rows,
    'attributes' => array('class' => array('timetracker-table-form')),
    'sticky' => FALSE,
  );

  $output .= theme('table', $table);
  drupal_render($form['days']);
  drupal_render($form['time_headers']);

  $output .= drupal_render_children($form);
  return $output;
}

function _timetracker_table_form_return($return) {
  if (isset($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
    drupal_json_output($return);
    exit;
  }
  else {
    drupal_set_message($return['message'], $return['status']);
  }
}

function _timetracker_format_human_date($date) {
  $days = array(
    'today' => t('Today'),
    '0'     => t('Sunday'),
    '1'     => t('Monday'),
    '2'     => t('Tuesday'),
    '3'     => t('Wednesday'),
    '4'     => t('Thursday'),
    '5'     => t('Friday'),
    '6'     => t('Saturday')
  );
  if ($date->format('d-m-Y') == date('d-m-Y')) {
    return $days['today'];
  }
  else {
    return $days[$date->format('w')];
  }
}
