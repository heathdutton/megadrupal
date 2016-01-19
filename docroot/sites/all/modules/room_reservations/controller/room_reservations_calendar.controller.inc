<?php
/**
 * @file
 * Calendar controller functionality.
 */

/**
 * Display the reservation calendar.
 *
 * @param int $selected_month
 *   (optional) The month of the day selected by the user.
 * @param int $selected_day
 *   (optional) The day of the month selected by the user.
 *
 * @return string
 *   The page display formatted as HTML.
 */
function room_reservations($selected_month = NULL, $selected_day = NULL) {
  // likely no need for this as not used anywhere else; but maybe for external modules
  global $yyyymmdd;   
  
  // dates are now keyed by Cat ID; but we only need anyone of these to build calendar but make sure its the one which starts earliest
  $datesbycat = _room_reservations_dates($selected_month, $selected_day);
  $categories = _room_reservations_categories();
  
  // allow other modules to alter the categories
  drupal_alter('room_reservations_categories', $categories);
  
  // if no Categories left; we should not bother with the rest of this
  if (!count($categories)) {
    drupal_set_message(t('There are no configured Room Categories. Please contact the System Administrator'), 'warning');
    drupal_goto();
  }
  
  $earliest = _room_reservations_earliest_category($categories);
  
  $dates = $datesbycat[$earliest];
  
  // Determine which day has been selected by the user. If the user has entered a url that specifies a day outside of the
  // allowable reservation window, then set the current day as the selected day.
  $yyyymmdd = $dates[0]['yyyymmdd'];
  foreach ($dates as $day) {
    if ($day['selected']) {
      $yyyymmdd = $day['yyyymmdd'];
    }
  }
  if ($yyyymmdd == $dates[0]['yyyymmdd']) {
    $dates[0]['selected'] = TRUE;
    $dates[0]['today'] = TRUE;
  }
  
  // a bit hacky; but we need to store what the calendar is using for its date so we can use this in theme functions later
  //$_SESSION['reservations_current_day'] = $yyyymmdd;
  
  // If the day being displayed is today, only display time slots that are later than the current time minus two hours.
  $today_displayed = FALSE;
  foreach ($dates as $day) {
    if (($day['selected']) && $day['today']) {
      $today_displayed = TRUE;
    }
  }
  if ($today_displayed) {
    $hours = _room_reservations_hours('limited');
  }
  else {
    $hours = _room_reservations_hours();
  }
  $_room_reservations_building_hours = _room_reservations_facility_hours();
  $display_hours = _room_reservations_hours();
  
  // Determine the open hours (display version) for the date selected by the user.
  $building_hours_day = $_room_reservations_building_hours[$yyyymmdd];
  $building_hours_display = $building_hours_day['display'];
  
  // For each time slot, determine if the rooms are open or closed.
  $building_hours_day = $_room_reservations_building_hours[$yyyymmdd];
  $int_first_shift_open = intval($building_hours_day['first_shift_open']);
  $int_first_shift_close = intval($building_hours_day['first_shift_close']);
  $int_second_shift_open = intval($building_hours_day['second_shift_open']);
  $int_second_shift_close = intval($building_hours_day['second_shift_close']);
  $open_24 = $building_hours_day['open_24_hours'];
  $x = 0;
  $buffer = variable_get('room_reservations_before_after_hours', 3) * 100;
  foreach ($hours as $time_slot) {
    $int_time_slot_time = intval($time_slot['time']);
    if ($building_hours_day['open_24_hours']) {
      $time_slot['open'] = TRUE;
    }
    elseif ((($int_time_slot_time >= $int_first_shift_open) && ($int_time_slot_time < $int_first_shift_close)) || 
      (($int_time_slot_time >= $int_second_shift_open) && ($int_time_slot_time < $int_second_shift_close))) {
      $time_slot['open'] = TRUE;
    }
    else {
      $time_slot['open'] = FALSE;
    }
    
    // if not open ALL day let's limit display to start just before first open shift (or 2nd if only one used)
    // this assume you must have 1st shift defined and possible 2nd (i.e. can't define only 2nd shift)
    if (!$open_24 && $buffer != 300) {
      if ($int_first_shift_open < 9999 && ($hours[$x]['time'] < $int_first_shift_open - $buffer)) {
        unset($hours[$x]);
      }
      else {
        $hours[$x] = $time_slot;
      }
      // and do the same for closing
      if (isset($hours[$x])) {
        if ($int_second_shift_close < 9999) {
          if ($hours[$x]['time'] >= $int_second_shift_close + $buffer) {
            unset($hours[$x]);
          }
        }
        elseif ($int_first_shift_close < 9999) {
          if ($hours[$x]['time'] >= $int_first_shift_close + $buffer) {
            unset($hours[$x]);
          }
        }
      }
    }
    else {
      $hours[$x] = $time_slot;
    }
    $x++;
  }
  $all_hours = _room_reservations_hours();
  $times = _room_reservations_times();
  $rooms = _room_reservations_rooms();
  
  // Initialize the $reservations array.
  $reservations = array();
  foreach ($rooms as $room) {
    $room_name = $room['name'];
    foreach ($hours as $time_slot) {
      $time = $time_slot['time'];
      $reservations[$room['nid']][$time] = array(
        'time' => $time,
        'display' => $time_slot['display'],
        'class' => $time_slot['class'],
        'id' => '',
        'booked' => FALSE,
        'start' => FALSE,
        'end' => FALSE,
        'user' => '',
        'name' => '',
      );
    }
  }
  
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'room_reservations_reservation')
    ->fieldCondition('reservation_date', 'value', $yyyymmdd . '%', 'like')
    ->fieldOrderBy('reservation_time', 'value', 'DESC')
    // EFQ respects access so if user does not have access to view a reservation (most likely due to some access module then we will not return a 
    // reservation for this slot; this means it will not block this slot from being bookable - which is likely worse than being able to 
    // see the booking title (especially since we have a method to block the title if user really doesn't want anyone to see it)
    // so, let's load the reservations as user 1 to make sure we get them all
    ->addMetaData('account', user_load(1));
  $result = $query->execute();
  if (isset($result['node'])) {
    $result_nids = array_keys($result['node']);
    $results = entity_load('node', $result_nids);
    foreach ($results as $data) {
      $id = $data->nid;
      $time = $data->reservation_time['und'][0]['value'];;
      $rid = $data->reservation_room['und'][0]['target_id'];
      $name = $data->title;
      $user_name = $data->name;
      $reservations[$rid][$time]['booked'] = TRUE;
      $reservations[$rid][$time]['class'] .= ' booked';
      $reservations[$rid][$time]['name'] = $name;
      $reservations[$rid][$time]['user_name'] = $user_name;
      $reservations[$rid][$time]['id'] = $id; 
      $reservations[$rid][$time]['series_id'] = isset($data->reservation_series_id['und'][0]['value']) ? $data->reservation_series_id['und'][0]['value'] : '';
      $reservations[$rid][$time]['user'] = $data->uid;
      $reservations[$rid][$time]['blocked'] = isset($data->reservation_block_title['und'][0]['value']) ? $data->reservation_block_title['und'][0]['value'] : 0;
      
      // add rest of slots as booked for the length of this reservation
      $length = $data->reservation_length['und'][0]['value'];;
      $time_slots = $length / 30;
      $reservations[$rid][$time]['slots'] = $time_slots;
      $remainder = $length % 30;
      if ($remainder > 1) {
        $time_slots++;
      }
      $key = array_search($time, $times);
      $x = 2;
      while ($x <= $time_slots) {
        $key++;
        $next_time = $times[$key];
        $reservations[$rid][$next_time]['booked'] = TRUE;
        $reservations[$rid][$next_time]['class'] .= ' booked';
        $x++;
        // unclear how we handle wrapping a reservation to the next day; but reservation time slots can't go passed midnight; i.e. slot 47 
        if ($key == 47) {
          break;
        }
      }
      
      // add in pre/post buffer for setup/takedown (rev 7.x-1.3+)
      //  - if the slot is part of buffer we add "setup" to class
      //  - if we don't have admin rights; we also mark it as booked so no one can book in these slots
      $category = $categories[$rooms[$rid]['reservations_room_category']['und'][0]['target_id']];
      $preslots = $category['prebuffer'] / 30;
      $postslots = $category['postbuffer'] / 30;
      $startkey = array_search($reservations[$rid][$time]['time'], $times);
      $endkey = $startkey + $time_slots;
      $k = $startkey - $preslots;
      while ($k < $startkey) {
        if (!$reservations[$rid][$times[$k]]['booked']) {
          $reservations[$rid][$times[$k]]['class'] .= ' setup';
        }
        $reservations[$rid][$times[$k]]['booked'] = user_access('book over buffer') ? $reservations[$rid][$times[$k]]['booked'] : true;
        $k++;
      }
      $k = $endkey;
      while ($k < $endkey + $postslots) {
        if (!$reservations[$rid][$times[$k]]['booked']) {
          $reservations[$rid][$times[$k]]['class'] .= ' setup';
        }
        $reservations[$rid][$times[$k]]['booked'] = user_access('book over buffer') ? $reservations[$rid][$times[$k]]['booked'] : true;
        $k++;
      } 
    }
  }
    
  // Get any room_reservation records for the previous day that might carry over into the selected day.
  // Continue to update the $reservation array with that information.
  $previous_day = date('Y-m-d', strtotime("$yyyymmdd -1 days"));
  $max_length = user_access('create room reservations extended length') ? variable_get('room_reservations_max_length_extended', 120) : variable_get('room_reservations_max_length_standard', 120);
  $lhours = _room_reservations_hours();
  $rhours = array_reverse($lhours);
  $slots = ($max_length / 30) - 1;
  $late_times = array();
  $search_times = array(
    '9999',
    '9999',
    '9999',
    '9999',
    '9999',
    '9999',
    '9999',
  );
  for ($j = 0; $j < $slots; $j++) {
    $hour = $rhours[$j];
    $late_times[] = $hour['time'];
    $search_times[$j] = $hour['time'];
  }
  $late_times = array_reverse($late_times);
  
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'room_reservations_reservation')
    ->fieldCondition('reservation_date', 'value', $previous_day . '%', 'like')
    ->fieldCondition('reservation_time', 'value', $search_times,  'in')
    ->fieldOrderBy('reservation_time', 'value', 'DESC')
    // EFQ respects access so let's load the reservations as user 1 to make sure we get them all
    ->addMetaData('account', user_load(1));
  $result = $query->execute();
  if (isset($result['node'])) {
    $result_nids = array_keys($result['node']);
    $results = entity_load('node', $result_nids);
    foreach ($results as $data) {
      $id = $data->nid;
      $time = $data->reservation_time['und'][0]['value'];
      $rid = $data->reservation_room['und'][0]['target_id'];
      $length = $data->reservation_length['und'][0]['value'];
      //$room_name = $data->room;
      $name = $data->title;
      $user_name = $data->name;
      $time_slots = $length / 30;
      $remainder = $length % 30;
      if ($remainder > 1) {
        $time_slots++;
      }
      for ($j = 0; $j < (($max_length / 30) - 1); $j++) {
        if ($late_times[$j] == $time) {
          $carry_over_time_slots = ($time_slots + $j - (($max_length / 30) - 1));
          break;
        }
      }
      if ($carry_over_time_slots > 0) {
        $reservations[$rid]['0000']['booked'] = TRUE;
        $reservations[$rid]['0000']['name'] = $name;
        $reservations[$rid]['0000']['user_name'] = $user_name;
        $reservations[$rid]['0000']['id'] = $id;
        $carry_over_time_slots--;
      }
      while ($carry_over_time_slots > 0) {
        $next_time = $times[$carry_over_time_slots];
        $reservations[$rid][$next_time]['booked'] = TRUE;
        $carry_over_time_slots--;
      }
    }
  }
  $user_reservations = _room_reservations_user_reservations();
  $output = '';
  $output .= theme('room_reservations', array(
    'dates' => $dates,
    'categories' => $categories,
    'reservations' => $reservations, 
    'hours' => $hours,
    'building_hours' => $_room_reservations_building_hours,
    'building_hours_display' => $building_hours_display,
    'rooms' => $rooms, 
    'user_reservations' => $user_reservations));
  return $output;
}

function _room_reservations_earliest_category($categories) {
  $earliest = 1000;
  foreach ($categories as $cat) {
    if ($cat['advmin'] < $earliest) {
      $earliest = $cat['advmin'];
      $result = $cat['nid'];
    }
  }  
  return $result;
}

function _room_reservations_get_dates() {
  // dates are now keyed by Cat ID; but we only need anyone of these to build calendar but make sure its the one which starts earliest
  $datesbycat = _room_reservations_dates($selected_month, $selected_day);
  $categories = _room_reservations_categories();
  $earliest = _room_reservations_earlist_category($categories);
  $dates = $datesbycat[$earliest];
  
  // Determine which day has been selected by the user. If the user has entered a url that specifies a day outside of the
  // allowable reservation window, then set the current day as the selected day.
  $yyyymmdd = $dates[0]['yyyymmdd'];
  foreach ($dates as $day) {
    if ($day['selected']) {
      $yyyymmdd = $day['yyyymmdd'];
    }
  }
  if ($yyyymmdd == $dates[0]['yyyymmdd']) {
    $dates[0]['selected'] = TRUE;
    $dates[0]['today'] = TRUE;
  }
  return $dates;
}