<?php

/**
 * @file
 * StudyroomAvailabily class file.
 */


/**
 * Class to contain functions to determine availability.
 */
class SpaceAvailability {
  /**
   * Default constructor.
   */
  public function __construct() {
    $this->limit_start = new DateObject(studyroom_limit_start());
    $this->limit_end = new DateObject(studyroom_limit_end());
    $this->increment = variable_get('studyroom_time_increment', '15');
  }

  /**
   * Build Availability for all spaces based on date.
   */
  public function checkAvailabilityByDate($spaces, $date, $is_today = FALSE) {
    $studyroom = new Studyroom();
    $this->user_types = studyroom_reservation_type_user_access();

    foreach ($spaces as $space_id => $space) {
      $spaces[$space_id]->hour_limits = $space->hoursPerDate($date);
      $spaces[$space_id]->reservations = studyroom_reservation_per_space_per_day($space_id, $date->format('Y-m-d'));
      $spaces[$space_id]->max_days_advance = $studyroom->getMaxDaysAdvance($space);

      $this->spaces[$space_id] = $space->label();
      $url = studyroom_space_uri($space);
      $this->spaces_header[$space_id] = l(t($space->shortLabel()), $url['path'] . '/' . $date->format('Y/W'));
    }

    $rows = array();
    $time = new DateObject($date->format('Y-m-d') . ' ' . studyroom_limit_start());
    $this->current_time = time();
    $this->today = date_now();
    $this->day_diff = $this->today->difference($date, 'days', FALSE);

    // Loop through each time_increment of the day.
    for ($x = 0; $x <= $this->rowCount(); $x++) {
      $rows[$x]['hour'] = date_format_date($time, 'custom', 'g:i');
      $rows[$x]['ampm'] = date_format_date($time, 'custom', 'a');

      // Loop through each space.
      foreach ($spaces as $id => $space) {
        $this->entity = $space;
        $resv_count = 0;
        $type = '';
        $rows[$x]['values'][$id]['class'][] = 'space';

        if ($space->hour_limits !== FALSE && ($time >= $space->hour_limits['start_time'] && $time < $space->hour_limits['end_time'])) {
          // Its a good time of day.
          if (!empty($space->reservations)) {
            $loop_time = strtotime(date_format_date($date, 'custom', 'd-m-Y') . ' ' . date_format_date($time, 'custom', 'H:i:s'));
            foreach ($space->reservations as $reservation) {
              if ($loop_time >= $reservation->start_time && $loop_time < $reservation->end_time) {
                $resv_count += 1;
                $type = $reservation->type;
                $label = $reservation->label;
              }
            }
          }

          // If any reservation found, create available cell content.
          if ($resv_count) {
            $rows[$x]['values'][$id]['entry'] = $label;
            $rows[$x]['values'][$id]['class'][] = 'reserved';
            $rows[$x]['values'][$id]['class'][] = $type;
          }
          else {
            if ($is_today && $this->current_time >= date_format_date($time, 'custom', 'U')) {
              $rows[$x]['values'][$id]['entry'] = '&nbsp;';
            }
            else {
              $datetime = strtotime(date_format_date($date, 'custom', 'Y-m-d') . $time->format('H:i'));
              $rows[$x]['values'][$id]['entry'] = $this->cellAvailable($datetime, $this->day_diff);
            }
            $rows[$x]['values'][$id]['class'][] = 'empty';
          }
        }
        else {
          $rows[$x]['values'][$id]['entry'] = 'closed';
          $rows[$x]['values'][$id]['class'][] = 'closed';
        }

        $rows[$x]['values'][$id]['classes'] = $this->classes($rows[$x]['values'][$id]['class']);
        // End of weekend loop.
      }
      // Increment time for next loop.
      $time->add(new DateInterval("PT" . $this->increment . "M"));
    }

    return $rows;
  }

  /**
   * Determine number of rows.
   */
  public function rowCount() {
    // @todo Determine if space_hours field has more restricted time limits.
    $start_end_diff = $this->limit_start->diff($this->limit_end);
    $minutes = $start_end_diff->h * 60;
    $minutes += $start_end_diff->i;
    return floor($minutes / $this->increment);
  }

  /**
   * Build Availability for a space based on week range.
   */
  public function spaceAvailabilityByWeek($entity, $year, $week) {
    $this->user_types = studyroom_reservation_type_user_access();
    $this->reservations = studyroom_reservation_per_space_per_range($entity, $year, $week);
    $this->entity = $entity;

    // Get studyroom values.
    $studyroom = new Studyroom();
    $this->entity->max_days_advance = $studyroom->getMaxDaysAdvance($entity);

    $this->current_time = time();
    $this->today = date_now();

    // Fix first day of week.
    $this->week = date_week_days_ordered($entity->field_space_hours[$entity->language]);
    $this->week_days = $this->weekDays($week, $year);

    $this->today = $this->today->format('Y-m-d');

    $rows = array();
    $time = clone $this->limit_start;

    // Loop through each time_increment of the day.
    for ($x = 0; $x <= $this->rowCount(); $x++) {
      $rows[$x]['hour'] = date_format_date($time, 'custom', 'g:i');
      $rows[$x]['ampm'] = date_format_date($time, 'custom', 'a');

      // Loop through each day of the week.
      foreach ($this->week_days as $id => $column) {
        $resv_count = 0;
        $type = '';
        $rows[$x]['values'][$id]['class'][] = 'day';

        if ($time >= $this->week[$id]['start_time'] && $time < $this->week[$id]['end_time']) {
          // Its a good time of day.
          if (isset($this->reservations[$column['fulldate']])) {
            $loop_time = strtotime($column['fulldate'] . ' ' . date_format_date($time, 'custom', 'H:i:s'));
            foreach ($this->reservations[$column['fulldate']] as $resv) {
              if ($loop_time >= $resv->start_time && $loop_time < $resv->end_time) {
                $resv_count += 1;
                $type = $resv->type;
                $label = $resv->label;
                if (user_access('access reservations overview') && !$resv->locked) {
                  $uri = $resv->uri();
                  $label = l($label, $uri['path']);
                }
              }
            }
          }

          // If any reservation found, create available cell content.
          if ($resv_count) {
            $rows[$x]['values'][$id]['entry'] = $label;
            $rows[$x]['values'][$id]['class'][] = 'reserved';
            $rows[$x]['values'][$id]['class'][] = $type;
          }
          else {
            // Only process today and forward dates.
            if ($column['fulldate'] >= $this->today) {
              if ($column['today'] && $this->current_time >= date_format_date($time, 'custom', 'U')) {
                $rows[$x]['values'][$id]['entry'] = '&nbsp;';
              }
              else {
                $datetime = strtotime($column['fulldate'] . $time->format('H:i'));
                $rows[$x]['values'][$id]['entry'] = $this->cellAvailable($datetime, $column['days_advance']);
              }
            }
            else {
              $rows[$x]['values'][$id]['entry'] = '&nbsp;';
            }
            $rows[$x]['values'][$id]['class'][] = 'empty';
          }

        }
        else {
          /* $rows[$x]['values'][$id]['entry'] = 'unavailable'; */
          $rows[$x]['values'][$id]['entry'] = 'closed';
          $rows[$x]['values'][$id]['class'][] = 'closed';
        }

        // Add today class if weekday is today.
        if ($column['today']) {
          $rows[$x]['values'][$id]['class'][] = 'today';
        }

        $rows[$x]['values'][$id]['classes'] = $this->classes($rows[$x]['values'][$id]['class']);
        // END OF WEEKDAYS LOOP.
      }

      // Increment time for next loop.
      $time->add(new DateInterval("PT" . $this->increment . "M"));
    }

    return $rows;
  }

  /**
   * Function to check if user access to create reservation.
   *
   * @todo If more that one type allowed, determine better filtering/weighting.
   */
  public function cellAvailable($datetime, $days_diff) {
    $link = FALSE;
    $types = '';
    $url = $this->entity->space_id . '/' . $datetime;
    if (is_array($this->user_types) && is_array($this->entity->reservation_types)) {
      $types = array_intersect_key($this->user_types, $this->entity->reservation_types);
    }

    if (!empty($types)) {
      if (count($types) == 1) {
        $type = array_shift($types);
        $url = $type->type . '/' . $url;
      }

      // Determine if ok to display link.
      if ($days_diff <= $this->entity->max_days_advance) {
        $link = TRUE;
      }
      else {
        // If admin resv, allow override of max_days_advance.
        foreach ($types as $type) {
          if (!$type->public && $type->ui) {
            $admin_type = $type->type;
          }
        }
        if (isset($admin_type) && entity_access('create', 'studyroom_reservation_type', $admin_type)) {
          $link = TRUE;
        }
      }
    }

    /* l('available', "reservation/add/$url", array('attributes' => array('class' => 'ctools-use-modal'))) */
    return ($link) ? l(t('available'), "reservation/add/$url") : 'available';
  }

  /**
   * Create week days array with name, dates, etc.
   */
  public function weekDays($week, $year) {
    $week_range = date_week_range($week, $year);
    $week_start = $week_range[0];

    $weekdays = array();
    foreach (date_week_days_ordered(date_week_days_abbr('TRUE')) as $key => $value) {
      $weekdays[$key]['header_id'] = strtolower($value) . '-' . $key;
      $weekdays[$key]['weekday'] = $value;
      $weekdays[$key]['date'] = $week_start->format('m/d');
      $weekdays[$key]['fulldate'] = $week_start->format('Y-m-d');

      // Determine number of days differrence from today.
      $day_diff = $this->today->difference($week_start, 'days', FALSE);
      $weekdays[$key]['today'] = ($day_diff == 0) ? TRUE : FALSE;
      $weekdays[$key]['days_advance'] = $day_diff;

      // Increment date.
      $week_start->add(new DateInterval('P1D'));
    }
    return $weekdays;
  }

  /**
   * Convert classes_array to class.
   */
  public function classes($class_array = array()) {
    foreach ($class_array as &$class) {
      $class = drupal_html_class($class);
    }
    return implode(' ', $class_array);
  }
}
