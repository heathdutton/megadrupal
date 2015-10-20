<?php
/**
 * @file
 * Calendar view functionality.
 */

module_load_include('inc', 'room_reservations', '/view/room_reservations.view');

/**
 * Theme the room reservation calendar page.
 *
 * @global object $user
 *   Drupal user object.
 * @global type $base_url
 *   Application base url.
 *
 * @param array $dates
 *   An array containing information about all the possible days for which a
 *   reservtion can be made.
 * @param array $categories
 *   An array of all the room categories.
 * @param array $reservations
 *   An array representing all the reservations that have been made for the
 *   day that is being displayed on the calendar.
 * @param array $hours
 *   An array representing every half hour time slot in a single day.
 * @param array $building_hours
 *   A two dimensional array, with the first dimension representing a single
 *   day for which a reservation can be made, and the second dimension
 *   representing information about the facility's open hours for that day, such
 *   as whether the facility is open that day, the number of open shifts,
 *   open and close hours, and a string that can be used to display the hours
 *   in a user friendly way.
 * @param string $building_hours_display
 *   A user friendly string of the open hours for the day being displayed
 *   on the calendar.
 * @param array $rooms
 *   An array representing all of the rooms that can be reserved.
 * @param string $selected_category
 *   The current category the user has selected, if any.
 * @param array $user_reservations
 *   An array representing the current user's reservations for the allowable
 *   reservation period.
 *
 * @return string
 *   The html for displaying the page.
 */
function theme_room_reservations($variables) {
  global $user;
  global $base_url;
    
  // to support minimum adv booking per category; let's get available dates per cat
  $datespercat = _room_reservations_dates(null, null, true);
  
  // pass date format to JS
  $format = strtolower(variable_get('room_reservations_picker_format', 'y/m/d'));
  drupal_add_js(array('room_reservations' => array('dateformat' => $format)), 'setting');
  
  extract($variables);
  // User information.
  $full_access = FALSE;
  $user_login_name = NULL;
  if ($user->uid) {
    $user_login_name = $user->name;
    $full_access = user_access('administer room reservations system') || user_access('edit any room reservation');
  }
  
  // Determine which date has been selected by the user.
  foreach ($dates as $day) {
    if ($day['selected'] == TRUE) {
      $day_of_week = $day['day-of-week'];
      $month_number = $day['month-number'];
      $month = $day['month'];
      $xday = $day['day'];
      $year = $day['year'];
      $yyyymmdd = $day['yyyymmdd'];
    }
  }
  
  $calendar_text = check_markup(_room_reservations_get_variable('calendar_text'));
  $reserve_room_instructions_text = check_markup(_room_reservations_get_variable('reserve_instructions'));
  if (!$reserve_room_instructions_text) {
    $reserve_room_instructions_text = t('To reserve a room, click on the desired time/day in the calendar below. You will be asked to login.');
  }
  // Room reservations container.
  $output = "<div id='rooms'>";
  $output .= "<div id='tabbedPanels'>";
  
  // Info box - user reservations, maps, link to policies.
  $output .= "<div id='info'><div>$calendar_text</div>
    <div><img id='arrow' src='" . base_path() . drupal_get_path('module', 'room_reservations') . "/images/007354-blue-jelly-icon-arrows-arrow-sparkle.png' />
    <span id='start'>" . $reserve_room_instructions_text . '</span></div></div>';
  
  // Calendar date.
  $date = format_date(strtotime($month . ' ' . $xday . ', ' . $year), 'custom', 'l, F d, Y'); 
  $output .= '<h3 class="date">' . t('Reservation Calendar') . '</h3>' .
    '<div class="hours">'  . $date . '&nbsp;&nbsp;' . $building_hours_display . '</div>';
  
  // add new Day Selector as popup calendar since we now allow going out up to 6 months rather than 15 days.
  $form = drupal_get_form('room_reservations_admin_date_picker', $yyyymmdd);
  $output .= '<div id="date-change">' . drupal_render($form) . '</div>';
  
  // Reservation calendar grid:
  //
  // Each block on the grid is assigned one (or more) of the following classes:
  //
  // closed - the building is closed;
  // booked - the building is open and the time slot has been reserved;
  // open - the building is open, the time slot has not been reserved, but the user must login to reserve the time slot;
  // reservable - the building is open, the time slot has not been reserved and the user is able to reserve the time slot.
  // setup - buffer zones added before/after bookings to allow for setup/takedown (per category)
  // 
  
  // Tabs.
  $output .= "<ul class='room-tabs'>";
  $i = 0;
  foreach ($categories as $category) {
    // Set the first tab as active
    // @todo: use JS to set active based on anchor
    $active = ($i == 0) ? " class='active'" : "";
    $i++;
    $id = strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $category['title']));
    $output .= '<li><a' . $active . " href='#" . $id . "'>" . $category['title'] . '</a></li>';
  }
  
  // Panel container.
  $output .= "</ul><div class='panelContainer'>";
  // If the user is logged in, the class for each unbooked time slot is 'reservable'.  If the user is not logged in, the class is 'open'.
  // Only logged in users can click a block of time to open the reservation form.
  $unbooked_class = ($user->uid) ? 'reservable' : 'open';
  
  // Create a tab for each room category.  Each tab contains a grid of time slots and rooms.
  $i = 0;
  foreach ($categories as $category) {
    $table = array();
    // Show the first tab and hide all others.
    if (!$selected_category) {
      $show = ($i == 0) ? 'show' : 'hide';
      $i++;
    }
    else {
      $show = ($category['title'] == $selected_category) ? 'show' : 'hide';
    }
    $id = strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $category['title']));
    $r = 0; $c = 0;
    
    // Date and building hours.
    $flipped = variable_get('room_reservations_calendar_flipped', 0);
    $orientclass = $flipped ? 'orient-horiz' : 'orient-vert';
    
    // this is the main DIV wrapper for the "table" within each Category
    $output .= '<div id="' . $id . '" class="panel ' . $show . '"><div class="gcolumns' . ' ' . $orientclass . '">';
    
    $table[$r][$c] = "<li class='room-info'>" . t('Room') . '</li>';
    $r++;
    $table[$r][$c] = "<li class='room-info room-info-heading'>" . t('Capacity') . '</li>';
    $r++;
   
    // Available hours column.
    foreach ($hours as $time_slot) {
      $time_display = ($time_slot['class'] == 'odd') ? t($time_slot['display']) : "";
      $table[$r][$c] = "<li class='" . $time_slot['class'] . " timeslot' time='" . $time_slot['time'] . "' " . '>' . $time_display . '</li>';
      $r++;
    }
    $table[$r][$c] = "<li class='room-info room-info-footer'>" . t('Room') . '</li>';
    $r++;
    $table[$r][$c] = "<li class='room-info'>" . t('Capacity') . '</li>';
    $r++;
    // Count the number of rooms in the selected category.
    $rooms_per_category = 0;
    foreach ($rooms as $room) {
      $rid = $room['nid'];
      if ($room['reservations_room_category']['und'][0]['target_id'] == $category['nid']) {
        $rooms_per_category++;
      }
    }
    
    // Column for each room in the category.
    foreach ($rooms as $room) {
      $rid = $room['nid'];
      $room_name = $room['title'];
      $room_link = l($room_name, 'node/' . $rid);
      $room_desc = $room['body']['und'][0]['safe_value'];
      
      // use qtip if we have it
      if (module_exists('qtip')) {
        $room_info = '<span class="qtip-link"><span class="qtip-tooltip">' . $room_desc .  '</span>' . $room_link . '</span>';
      }
      else {
        $room_info = $room_link;
      }
      
      if ($room['reservations_room_category']['und'][0]['target_id'] == $category['nid']) {
        $c++; $r = 0;
        // Room name, capacity.
        //$output .= '<div class="grid-column room-column"><ul>';
        //$output .= '<li class="room-info">' . $room_info . '</li>';
        $table[$r][$c] = '<li class="room-info">' . $room_info . '</li>';
        $r++;
        $table[$r][$c] = "<li class='room-info room-info-heading'>" . $room['reservations_room_capacity']['und'][0]['value'] . '</li>';
        // Populate each time slot.
        foreach ($hours as $time_slot) {
          $r++;
          $time = $time_slot['time'];
          $open = $time_slot['open'];
          
          // lets use slot class from reservation if it is set
          $slotclass = isset($reservations[$rid][$time_slot['time']]['class']) ? $reservations[$rid][$time_slot['time']]['class'] : $time_slot['class'];
          
          // to support min adv booking per cat; let's simply mark all slots as closed for dates not availble to this user for this cat
          if (!isset($datespercat[$category['nid']][$yyyymmdd])) {
            $open = false; 
          }
          
          // Determine if the building is open or closed for this time slot.
          if ($open) {
            //$booked_class = ($reservations[$rid][$time]['booked']) ? 'booked' : $unbooked_class;
            $booked_class = ($reservations[$rid][$time]['booked']) ? '' : $unbooked_class;
          }
          else {
            $booked_class = 'closed';
          }
          // The time slot can be reserved by the current user.
          $viewable_class = '';
          $widthclass = '';
          if ($booked_class == 'reservable' && 
            (user_access('create room reservations standard') || user_access('create room reservations extended') || user_access('administer room reservations system'))) {
            $link = l(
              '<img src="' . base_path() . drupal_get_path('module', 'room_reservations') . '/images/clear.png" />',
              'node/add/room-reservations-reservation/' . $month_number . '/' . $xday . '/' . $time_slot['time'] . '/' . $rid,
              array('html' => true)
            );
            $viewable = '';
          }
          // The time slot can be reserved by the current user, but the user must login first.
          elseif ($booked_class == 'open') {
            $link = "<a href='" . $base_url . "/user/login?destination=node/add/room-reservations-reservation/" . $month_number . "/" . $xday . "/" .
              $time_slot['time'] . "/" . $rid . "'><img src='" . base_path() . drupal_get_path('module', 'room_reservations') .
              "/images/clear.png' /></a>";
            $viewable = '';
          }
          elseif ($booked_class == 'closed') {
            $link = '';
          }
          else {
            // The time slot has a reservation that can be edited by the current user.
            $reservation = node_load($reservations[$rid][$time]['id']);
            $viewable_class = node_access('update', $reservation) ? 'viewable' : '';
            //$viewable_class = (($full_access) || ($reservations[$rid][$time]['user'] == $user->uid)) ? 'viewable' : '';
            if ($viewable_class == 'viewable') {
              $id = $reservations[$rid][$time]['id'];
              $link = $id ? l($reservations[$rid][$time]['name'], 'node/' . $id . '/edit', 
                array('attributes' => array(
                  'title' => $reservations[$rid][$time]['name'],
                  'class' => 'booking-span')
                )
              ) : '';
            }
            // The time slot has a reservation that cannot be viewed by the current user. and we are NOT allowed to see the Title
            else if (isset($reservations[$rid][$time]['blocked']) && $reservations[$rid][$time]['blocked']) {
              $link = t('Booked');
            }    
            // The time slot has a reservation that cannot be edited by the current user. but we are allowed to see the Title
            else {
              $link = $reservations[$rid][$time]['name'];
            }
            $slots = isset($reservations[$rid][$time]['slots']) ? $reservations[$rid][$time]['slots'] : '';
            $widthclass = $slots ? 'colspan' . $reservations[$rid][$time]['slots'] : '';
          }
          
          // allow other modules to modify the $link
          drupal_alter('room_reservations_link', $link, $reservations[$rid][$time]); 
          
          // allow other modules adding a custom class to slots
          $custom_class = '';
          drupal_alter('room_reservations_custom_class', $custom_class, $reservations[$rid][$time]);
                   
          // add div wrapper to display better
          $link = $link ? '<div class="booking-span">' . $link . '</div>' : '';
          
          // we used book class to determine if linked or not; which we needed for pre/post slots as well as actual reservation slots
          // but we don't want to show booked class now for the slots which are just buffer slots
          if (stristr($slotclass, 'setup')) {
            $booked_class = '';
          }
          $table[$r][$c] = "<li class='" . $slotclass . ' ' . $booked_class . ' ' . $custom_class . ' ' . $viewable_class .  ' ' . $widthclass. "'>" . $link . "</li>";
        }
        // Room name and capacity.
        $r++;
        $table[$r][$c] = '<li class="room-info">' . $room_info . '</li>';
        $r++;
        $table[$r][$c] = '<li class="room-info">' . $room['reservations_room_capacity']['und'][0]['value'] . '</li>';
      }
    }
    
    // remove extra table labels based on admin setting
    $compressed = false;
    if (variable_get('room_reservations_compressed_labels', 0)) {
      $compressed = true;
    }
    
    // dump our table contents in std or flipped orientation
    if ($flipped) {
      $table = _room_reservations_transpose($table);
      $m = $r;
      $n = $c;
    }
    else {
      $m = $c;
      $n = $r;
    }
    for ($x = 0; $x <= $m; $x++){
      if ($flipped && $compressed && ($x == 1 || $x == $m || $x == $m - 1)) continue;
      $output .= "<div class='grid-column hours-column'><ul>";
      for ($y = 0; $y <= $n; $y++) {
        if (!$flipped && $compressed && ($y == 1 || $y == $n || $y == $n - 1)) continue;
        $output .= $table[$y][$x];
      }
      $output .= '</ul></div>';
    }  
    
    // end of main DIV wrapper for "table"
    $output .= '</div></div>';
  }
  
  // end of panelContainer that holds all tables for all Categories
  $output .= '</div>';
  
  $output .= '<div class="clear"></div></div>';
  $output .= '</div>';
  
  return $output;
}

function room_reservations_admin_date_picker($form, &$form_state) {
  $yyyymmdd = $form_state['build_info']['args'][0];
  $parts = explode('-', $yyyymmdd);
  if (user_access('create room reservations extended')) {
    $advancedays = variable_get('room_reservations_advance_extended', 180);
  }
  else {
    $advancedays = variable_get('room_reservations_advance_standard', 14);
  }
  $yearnow = date('Y');
  $absdaynow = date('z');
  $absdaydefault = date('z', mktime(0, 0, 0, $parts[1], $parts[2], $yearnow));
  if ($absdaynow > $absdaydefault) {
    $yeardefault = $yearnow + 1;
  }
  else {
    $yeardefault = $yearnow;
  }  
  
  $format = str_replace('y', 'Y', strtolower(variable_get('room_reservations_picker_format', 'y/m/d')));
  $form['date'] = array(
    '#type' => 'date_popup',
    '#default_value' => $yeardefault . '-' . $parts[1] . '-' . $parts[2] . ' 00:00:00',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => date_default_timezone(),
    '#date_format' => $format,
    '#date_increment' => 1,
    '#date_year_range' => '-0:+1',
    '#datepicker_options' => array(
      'minDate' => '+0d',
      'maxDate' => $advancedays -1 . 'D',
    )
  );
  
  $form['#after_build'][] = '_room_reservations_admin_date_picker_afterbuild';  
  return $form;
}

function _room_reservations_admin_date_picker_afterbuild($form, &$form_state) {
  $form['date']['date']['#title'] = '';
  $form['date']['date']['#description'] = t('click in box to select date');
  return $form;  
}

function _room_reservations_transpose($array) {
  array_unshift($array, null);
  return call_user_func_array('array_map', $array);
}

