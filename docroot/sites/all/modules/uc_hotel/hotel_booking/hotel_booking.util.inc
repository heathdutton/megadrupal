<?php
/*
 * @file hotel_booking.util.inc
 * Provides util functions for hotel_booking module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */

/**
 * Fetch array of base rates
 * @param $name_only boolean TRUE to fetch entire row, else false
 * @return array
*/
function hotel_booking_get_rates($name_only = FALSE) {
  $rates = db_query("SELECT * FROM {hotel_booking_rates}", array(), array('fetch' => PDO::FETCH_OBJ));
  $rate_list = array();
  foreach ($rates as $row) {
    if ($name_only) {
      $rate_list[$row->hbrid] = check_plain($row->name);
    }
    else {
      $rate_list[$row->hbrid] = $row;
    }
  }
  return $rate_list;
}

/**
 * Fetch array of rate modifiers
 * @param $names_only boolean TRUE to fetch name of modifier only
 * @return array of modifiers
*/
function hotel_booking_get_rate_modifiers($names_only = FALSE) {
  $rates = db_query("SELECT * FROM {hotel_booking_rate_modifiers}
                     ORDER BY method DESC, rate ASC", array(), array('fetch' => PDO::FETCH_OBJ));
  $modifiers = array();
  if ($names_only) {
    //we add a 'base rate' option - ie no modifier
    $modifiers[0] = t('Base rate');
  }
  foreach ($rates as $row) {
    if ($names_only) {
      switch ($row->method) {
        case 'P':
          $modifiers[$row->hbrmid] = t('Base rate plus !percent%', array('!percent' => number_format($row->rate, 2, '.', ',')));
          break;
        default:
          $modifiers[$row->hbrmid] = t('Base rate plus !rate', array(
            '!rate' => uc_currency_format($row->rate)));
      }
    }
    else {
      $modifiers[$row->hbrmid] = $row;
    }
  }
  return $modifiers;
}

/**
 * Fetch array of occupancy modifiers
 * @param $names_only boolean TRUE to fetch names only
 * @param $filter string A for adults, C for children, B for both
*/
function hotel_booking_get_occupancy_modifiers($names_only = FALSE, $filter = FALSE) {
  $query = "SELECT * FROM {hotel_booking_occupancy_modifiers}";
  if ($filter && !in_array($filter, array('A', 'B', 'C'))) {
    watchdog('hotel booking', 'Invalid filter passed for function hotel_booking_get_occupancy_modifiers, valid filters are A, B or C');
    return array();
  }
  $params = array();
  if ($filter) {
    $query .= "
    WHERE type = :type";
    $params[':type'] = $filter;
  }
  $query .= "
            ORDER BY type ASC, threshold ASC, rate ASC";
  $results = db_query($query, $params, array('fetch' => PDO::FETCH_OBJ));
  $modifiers = array();
  foreach ($results as $row) {
    if ($names_only) {
      $modifiers[$row->hbomid] = t(
        '!rate each additional over !threshold',
        array(
          '!rate' => uc_currency_format($row->rate),
          '!threshold' => $row->threshold
        )
      );
    }
    else {
      $modifiers[$row->hbomid] = $row;
    }
  }
  return $modifiers;
}

/**
 * Check given date againt the current expiration date (according to settings). Returns true in case the given date is
 * yet to be expired, false otherwise
 * @param $date object date object
 */
function hotel_booking_non_zeroed_date($date) {
  $expiration_dt = hotel_booking_expiration_date();
  return (int)$date->format('U') > (int)$expiration_dt->format('U');
}

/**
 * Helper function to get the proper expiration date.
 * Using cron exripation process and cancellation process
 */
function hotel_booking_expiration_date() {
  $expiration = variable_get('hotel_booking_expiration', 'yesterday');
  $today = new DateObject('now', date_default_timezone(), DATE_FORMAT_ISO);
  $adjustments = array(
    'yesterday' => '-1 days',
    'tomorrow' => '+1 days'
  );
  if (isset($adjustments[$expiration])) {
    $today->modify($adjustments[$expiration]);
  }
  return $today;
}

/**
 * Helper function to determine if there is a booking item in the cart.
 */
function hotel_booking_item_in_cart() {
  $items = uc_cart_get_contents();
  foreach ($items as $item) {
    if ($item->data['module'] == 'hotel_booking') {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Util to test if hotel_booking item in order
 * @param $order object std uc_order object
*/
function hotel_booking_item_in_order($order) {
  foreach ($order->products as $product) {
    if ($product->data['module'] == 'hotel_booking') {
      //this is a hotel booking product
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Utility function to fetch check in night and check out night
 * @param $nights array array of nights as per product->data['nights'] in cart and order products
 * @return array($check_in, $check_out)
 * @author larowlan leerowlands at rowlands-bcs.com
*/
function _hotel_booking_check_in_out($nights) {
  $check_in = new DateObject($nights[0], date_default_timezone(), DATE_FORMAT_ISO);
  $last_night = new DateObject($nights[count($nights) - 1], date_default_timezone(), DATE_FORMAT_ISO);
  $last_night->modify('+ 1 day'); //check out is day after last night
  return array($check_in, $last_night);
}

/**
 * Helper function to remove addon items from the cart.
 */
function hotel_booking_remove_upgrades($data = array()) {
  if ($data) {
    foreach ($data as $nid => $cart_item_id) {
      db_delete('uc_cart_products')->condition('cart_item_id', $cart_item_id)->execute();
    }
  }
}

/**
 * fetch default values for booking form from either
 * a)submitted values
 * b)user's last search (auth user)
 * c)user's last search (anon user - from session vars)
 * @param $form_state form state as submitted
 * @return array of form default values
*/
function hotel_booking_defaults($form_state) {
  global $user;
  //retrieve defaults
  if (isset($form_state['values'])) { //form was submitted
    $defaults = array(
      'check_in' => $form_state['values']['check_in'],
      'nights'   => $form_state['values']['nights'],
      'adults'   => $form_state['values']['adults'],
      'children' => $form_state['values']['children'],
      'smoking'  => $form_state['values']['smoking']
    );
  }
  else { //has user submitted form before
    if ($user->uid && isset($user->hotel_booking_details)) { //user is logged in and has defaults
      //get defaults
      $defaults = $user->hotel_booking_details;
      $date = new DateObject($defaults['check_in'], date_default_timezone(), DATE_FORMAT_ISO);
      if (!$date || (int)$date->format('U') < REQUEST_TIME) {
        $today = date_now();
        $date = new DateObject('now', date_default_timezone(), DATE_FORMAT_ISO); //todays date
        $date->modify('+1 day');
        $defaults['check_in'] = date_format_date($today, variable_get('uc_date_format_default', 'd M Y')); //today's date
      }
    }
    elseif (isset($_SESSION['hotel_booking_defaults'])) {
      $defaults = $_SESSION['hotel_booking_defaults']; //anonymous user but has submitted before
      $date = new DateObject($defaults['check_in'], date_default_timezone(), DATE_FORMAT_ISO);
      if (!$date || (int)$date->format('U') < time()) {
        $today = date_now();
        $date = new DateObject('now', date_default_timezone(), DATE_FORMAT_ISO); //todays date
        $date->modify('+1 day');
      }
    }
    else { //just use defaults
      $date = new DateObject('now', date_default_timezone(), DATE_FORMAT_ISO); //todays date
      $date->modify('+1 day');
      $defaults = array(
        'nights'   => 3, //optimistic!
        'adults'   => 2, //most stays would be two adults
        'children' => 0,
        'smoking'  => 3
      );
    }
    //now we turn the check in date into a DATE_ISO Format for date field
    $defaults['check_in'] = $date->format(DATE_FORMAT_DATETIME);
  }
  return $defaults;
}

/**
 * function hotel_booking_save_defaults
 * saves search values submitted for user convenience
 *
 * @param $form_state the submitted form_state var
 * @return none
 */

function hotel_booking_save_defaults($form_state) {
  //get submitted vars into a nice array
  $defaults = array(
    'nights' => $form_state['values']['nights'],
    'adults' => $form_state['values']['adults'],
    'children' => $form_state['values']['children']
  );
  if (variable_get('hotel_booking_display_smoking', TRUE)) {
    $defaults['smoking'] = $form_state['values']['smoking'];
  }
  else {
    $defaults['smoking'] = 0;
  }
  $date = new DateObject($form_state['values']['check_in'], date_default_timezone(), DATE_FORMAT_ISO);
  $defaults['check_in'] = $form_state['values']['check_in'];

  global $user;
  //save search details for convenience
  if ($user->uid) { //user is logged in
    //set defaults
    $hotel_booking_details = array('hotel_booking_details' => $defaults);
    user_save($user, $hotel_booking_details);
  }
  else {
    $_SESSION['hotel_booking_defaults'] = $defaults;
  }
}

/**
 * Function returns query to return provide list of nodes available
 * for given date and parameters
 * @param $alias string alias for the node table to join against
 * @return string
 * @see hotel_booking_tnid_sql
*/
function hotel_booking_available_rooms_sql($alias = 'node', $field = 'tnid') {
  $sql = hotel_booking_tnid_sql($alias, $field) ." IN (SELECT nid FROM (
            SELECT nid,
            COUNT(*) AS nights
            FROM {hotel_booking_availability_calendars}
            WHERE calendar_dt >= :check_in1
            AND calendar_dt < :check_out2 /*this has to be less than as they dont stay this night*/
            AND minimum_stay <= :nights1
            AND minimum_occupancy <= :occupants1
            AND available > 0
            GROUP BY 1) AS nights
         WHERE nights.nights = :nights2
      )
      AND ". hotel_booking_tnid_sql($alias, $field) ." NOT IN (
        SELECT nid
        FROM {hotel_booking_availability_calendars}
        WHERE calendar_dt = :check_in2
        AND no_check_in = 1
      )
      AND ". hotel_booking_tnid_sql($alias, $field) ." NOT IN (
        SELECT nid
        FROM {hotel_booking_availability_calendars}
        WHERE calendar_dt = :check_out2
        AND no_check_out = 1
      )
      AND :occupants2 BETWEEN minimum_occupancy AND capacity";
  return $sql;
}

/**
 * Function returns array
 * @param $check_in string date/time
 * @param $check_out string date/time
 * @param $nights int number of nights
 * @param $adults int number of adults
 * @param $children int number of children
 * @return formatted array suitable for sql args
*/
function hotel_booking_available_rooms_sql_args($check_in, $check_out, $nights, $adults, $children) {
  return array(
    ':check_in1' => $check_in,
    ':check_out1' => $check_out,
    ':nights1' => $nights,
    ':occupants1' => ($adults + $children),
    ':nights2' => $nights,
    ':check_in2' => $check_in,
    ':check_out2' => $check_out,
    ':occupants2' => ($adults + $children));
}

/**
 * This function provides a case statement
 * to query against tnid or nid of the node for multi-lingual sites
 * where availability is set against translation source but used by translations
 *
 * @param $alias string alias of the node table
 * @param $field string alias of the field table
*/
function hotel_booking_tnid_sql($alias, $field) {
  if ($field == 'nid') {
    return sprintf('%s.nid', $alias);
  }
  return sprintf("CASE WHEN %s.%s = 0 THEN %s.nid ELSE %s.%s END", $alias, $field,
                 $alias, $alias, $field);
}

/**
 * Returns array of node ids available for booking
 * @param $check_in string date/time
 * @param $check_out string date/time
 * @param $nights int number of nights
 * @param $adults int number of adults
 * @param $children int number of children
 * @param $smoking int value for smoking (3 = no preference)
 * @param $nid array array of node id - pass this to check specific rooms only
 * @return array of nids
 *
*/
function hotel_booking_get_available_rooms($check_in, $check_out, $nights, $adults, $children, $smoking = 3, $nids = array()) {
  $rooms = array();

  $sql = "SELECT hbrt.nid
          FROM {hotel_booking_room_types} hbrt
          INNER JOIN {node} n
          ON hbrt.vid = n.vid
          WHERE ". hotel_booking_available_rooms_sql('n'); //joining against node to get correct vid
  $params = hotel_booking_available_rooms_sql_args(
    $check_in,
    $check_out,
    $nights,
    $adults,
  $children);
  if ($smoking < 3) {
    $sql .= " AND smoking = :smoking";
    $params[':smoking'] = $smoking;
  }
  if (count($nids)) {
    //check specific rooms
    $sql .= " AND ". hotel_booking_tnid_sql('n', 'tnid') ." IN (:nids) ";
    $params[':nids'] = $nids;
  }
  if (function_exists('i18n_get_lang')) {
    //language testing
    $sql .= " AND n.language IN (':null', ':language')";
    $params[':null'] = ''; //no language
    $params[':language'] = i18n_get_lang(); //current language
  }
  $results = db_query($sql, $params)->fetchAll();
  foreach ($results as $room) {
    $rooms[$room->nid] = $room->nid;
  }
  return $rooms;
}


/**
 * Gets array of room rate results
 * @param $check_in string date/time of check in
 * @param $check_out string date/time of check out
 * @param $rooms array of nids of available rooms
 * @param $adults int number of adults
 * @param $children int number of children
 * @param $smoking int smoking/non smoking flag
 * @param $values array of form_state['values']
 * @return array of form array(results => array (nid1 => array('prices' => array(date1 => rate, date2 => rate)),
                                            nid2 => array('prices' => array(date1 => rate, date2 => rate))),
                                totals => array(nid1 => price, nid2 => price))
*/
function hotel_booking_calculate_prices($check_in, $check_out, $rooms,
                                         $adults, $children, $smoking, $values) {
  if (!$rooms || count($rooms) == 0) {
    return array(
      'totals' => array(),
      'results' => array()
    );
  }
  $dates = $results = array();
  //cache caldates
  $start_date = new DateObject($check_in, date_default_timezone(), DATE_FORMAT_ISO);
  $end_date = new DateObject($check_out, date_default_timezone(), DATE_FORMAT_ISO);
  $query = "SELECT r.calendar_dt, r.rate, hbrt.nid
            FROM {hotel_booking_room_types} hbrt
            INNER JOIN {node} n
              ON hbrt.vid = n.vid
            INNER JOIN {hotel_booking_rate_calendars} r
              ON hbrt.hbrid = r.hbrid
            WHERE hbrt.nid IN (:rooms)";
  $date_clauses = array();
  while ((int)$start_date->format('U') < (int)$end_date->format('U')) {
    /*for some reason some versions of MySQL don't like date fields
     and in clauses, have to do it like this*/
    $dates[] = $start_date->format(DATE_FORMAT_DATE);
    $start_date->modify('+ 1 day');
  }

  //cache baserates - get them in one query not heaps
  if (count($dates) > 0) {
    $query .= " AND (r.calendar_dt IN (:dates))";
  }
  $baserates = db_query($query, array(':rooms' => $rooms, ':dates' => $dates));
  foreach ($baserates as $baserate) {
    if (!isset($results[$baserate->nid])) {
      //initialise
      $results[$baserate->nid] = array('prices' => array());
    }
    $results[$baserate->nid]['prices'][$baserate->calendar_dt] = $baserate->rate;
  }
  /*results is now a 2D array of form array (nid1 => array('prices' => array(date1 => rate, date2 => rate)),
                                            nid2 => array('prices' => array(date1 => rate, date2 => rate)))*/
  //rate modifiers
  foreach (module_implements('modify_room_rate') as $module) {
    $function = $module .'_modify_room_rate';
    if (function_exists($function)) {
      $nids = array_keys($results);
      $nodes = node_load_multiple($nids);
      foreach ($results as $nid => $details) {
        $total = 0;
        $reset = TRUE;
        foreach ($details['prices'] as $date => $price) {
          $price = $function($price, $nodes[$nid], $date, $adults,
                              $children, $smoking, $check_in, $check_out, $reset, $values);
          $results[$nid]['prices'][$date] = $price;
          $total += $price;
          $reset = FALSE; //can used static vars for this node, need new ones when node changes
        }
        $results[$nid]['total'] = $total;
        $sorted_rates[$nid] = $total;
      }
    }
  }
  return array(
    'results' => $results,
    'totals' => $sorted_rates
  );
}

/**
 * Provides utility to calculate end date
 * @param $check_in string date/time of check in
 * @param $nights int number of nights of stay
 * @param $last_night boolean, TRUE to return last night instead of check out date
 * @return string date/time of check out/last night
*/
function hotel_booking_calculate_checkout($check_in, $nights, $last_night = FALSE) {

  $last_dt = new DateObject($check_in, date_default_timezone(), DATE_FORMAT_ISO);
  $last_dt->modify('+'. ($nights - 1) .' days');
  if ($last_night) {
    return $lastnight->format(DATE_FORMAT_DATETIME);
  }
  $last_dt->modify('+ 1 day');
  return $last_dt->format(DATE_FORMAT_DATETIME);
}

/**
 * Get from price for a room
 * looks at lowest base rate + modifiers
 * for future dates
 * @param $nid node id of room
 * @return mixed the price
*/
function hotel_booking_room_from_rate($nid) {
  $query = "SELECT MIN(hbrc.rate) AS rate, hbrm.method,
            CASE WHEN hbrm.rate IS NULL
              THEN 0
              ELSE hbrm.rate
            END AS modifier
            FROM {hotel_booking_room_types} hbrt
            INNER JOIN {hotel_booking_rate_calendars} hbrc
            ON hbrc.hbrid = hbrt.hbrid
            INNER JOIN {node} n
            ON n.vid = hbrt.vid
            LEFT JOIN {hotel_booking_rate_modifiers} hbrm
            ON hbrt.hbrmid = hbrm.hbrmid
            WHERE calendar_dt > :date
            AND hbrc.rate <> 0
            AND n.nid = :nid
            GROUP BY 2, 3";
  $params = array(':date' => date('Y-m-d'), ':nid' => $nid);
  $details = db_query($query, $params)->fetch();
  $rate = $details->rate;
  if (!$rate) {
    return 0;
  }
  if (!$details->modifier) {
    return $rate;
  }
  if ($details->method == 'V') {
    $rate += $details->modifier;
  }
  else {
    $rate += ($rate * ($details->modifier/100));
  }
  return $rate ? $rate : 0;
}


/**
 * Sets lowest room rate for a hotel_room_type node
 * @param $nid int the room nid
*/
function hotel_booking_set_room_from_rate($nid) {
  $from_rate = hotel_booking_room_from_rate($nid);
  db_update("hotel_booking_room_types")
    ->condition('nid', $nid)
    ->fields(array(
      'from_rate' => $from_rate
    ))
    ->execute();
}

/**
 * Sets the lowest room rate for all rooms
 * using a rate id after a rate update
 * @param $id int the rate id
 * @param $type string the rate type hbrid|hbrmid
*/
function hotel_booking_after_price_change($id, $type) {
  $query = "SELECT nid FROM {hotel_booking_room_types}
              WHERE";
  if ($type == 'hbrid') {
    $query .= " hbrid = :id";
  }
  else {
    $query .= " hbrmid = :id";
  }
  $rooms = db_query($query, array(':id' => $id))->fetch();
  if ($rooms) {
    foreach ($rooms as $room) {
      hotel_booking_set_room_from_rate($room['nid']);
    }
  }
}

/**
 * Make rooms auto available if set
*/
function hotel_booking_auto_availability($node = NULL) {
  $months = variable_get('hotel_booking_auto_available', 0);
  if (!$months) {
    return;
  }
  if ($node) {
    $nodes = array($node);
  }
  else {
    $nodes = db_query("SELECT nid FROM {node}
                    WHERE type = 'hotel_room_type'", array())->fetchAll();
  }
  $date = new DateObject('now', date_default_timezone(), DATE_FORMAT_ISO);
  $date->modify("+$months months");
  $available_sql = "SELECT default_available FROM {hotel_booking_room_types}
                            WHERE nid = :nid";
  $date_sql = "SELECT MAX(calendar_dt) AS cal_dt
          FROM {hotel_booking_availability_calendars}
          WHERE nid = :nid";
  foreach ($nodes as $nid) {
    $node_date = db_query($date_sql, array(':nid' => $nid->nid))->fetch();
    $available = db_query($available_sql, array(':nid' => $nid->nid))->fetch();
    if ($node_date) {
      $node_date = new DateObject($node_date->cal_dt, date_default_timezone(), DATE_FORMAT_DATETIME);
    }
    else {
      $node_date = new DateObject('now', date_default_timezone(), DATE_FORMAT_ISO);
    }
    //update the availability
    while ((int)$node_date->format('U') < (int)$date->format('U')) {
      $record = array(
        'nid' => $nid->nid,
        'calendar_dt' => $node_date->format(DATE_FORMAT_DATE),
        'available' => $available->default_available
      );
      drupal_write_record('hotel_booking_availability_calendars', $record);
      $node_date->modify('+1 day');
    }
  }

}