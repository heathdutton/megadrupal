<?
/**
 * Created on Jan 27, 2012
 * Created by Mark Libkuman
 * libkuman@openflows.com
 * www.openflows.com
 * 
 * Created for Manhattan Neighborhood Network
 * 
 */
 
 
function cm_airing_grid_get_grid_time_constraints($day,  $start_hour, 
    &$mysql_day_of_week, &$start_time, &$end_time) {
 
  if (!$day) {
    $day = time();
  }  
  
  $day_of_week = date('w', $day);
  if ($day_of_week == 7) {
    $day_of_week = 0;
  }
  
  $mysql_day_of_week = $day_of_week + 1;
  if ($mysql_day_of_week == 8) {
    $mysql_day_of_week = 1;
  }
  
  $start_time = mktime($start_hour, 0, 0, 
      date('m', $day), date('d', $day), date('Y', $day));

  //add 6 hours to start time for the end_time grid constraint
  $end_time = $start_time + (60*60*6) - 1;
  
  if (date('I', $start_time) != date('I', $end_time)) {
    $row_time_range_contains_dst = TRUE;
  } else {
	$row_time_range_contains_dst = FALSE;   
  }
   
  if ($row_time_range_contains_dst) {
    if (date('I', $start_time) == 1) {
      $end_time = $end_time + 60*60;
    }
    else {
      $end_time = $end_time - 60*60;
    }  
  }
}

function cm_airing_grid_validate_start_hour($start_hour){
  if ($start_hour && is_numeric($start_hour) && 
      $start_hour>=0 && $start_hour <= 18) {
    return $start_hour;
  }
  else if ($start_hour && is_numeric($start_hour) && 
      $start_hour>18) {
    $start_hour = 18;    
  }  
  else if ($start_hour != NULL && $start_hour == 0) {
    //do nothing, because start time is actually 0
  }
  else {
    $start_hour = date('H');
    if ($start_hour > 18) {
      $start_hour = 18;
    }
  }
  return $start_hour;
}

 
/*
 * This is a util funtion that allows you to add x amount of units to
 * a date. I.e. i want to add 4 weeks or 3 days to a date.
 * 
 * The important thing in this function is it takes into affect
 * DST so that if i add 1 week to Monday at 4pm, i get the next week at 
 * 4pm regardless of if there was DST change within the week.
 * 
 * @param $time the time to which to add 
 * @param $type will be either hour, day or week
 * @param $multiplier the amount of hours, days, or weeks
 */
function cm_airing_grid_add_to_date($time, $type, $multiplier) {
   if (!$multiplier) {
     $multiplier = 1;
   }

   if ($type == "week") {
     $new_time = $time + $multiplier*7*24*60*60;
   }
   else if ($type == "hour") {
     $new_time = $time + $multiplier*60*60;
   }
   else if ($type == "day") {
     $new_time = $time + $multiplier*24*60*60;
   }
   if (date('I', $time) != date('I', $new_time)) {
      if (date('I', $new_time)) {
        $new_time = $new_time - 60*60;
      }
      else if (date('I', $time)) {
        $new_time = $new_time + 60*60;
      }
   }
   return $new_time;
 }
 

function cm_airing_grid_get_date_options(&$years, &$months, &$days,
                                          &$hours, &$minutes, &$ampm){
  $ty = date('Y');
  for ($year = 2005; $year <= $ty + 1; $year++) {
    $years[$year] = $year;
  }
  $months = array( '01' => '01', '02' => '02', '03' => '03', '04' => '04', '05' => '05', '06' => '06', '07' => '07', '08' => '08', '09' => '09', '10' => '10', '11' => '11', '12' => '12',);
  $days = array( '01' => '01', '02' => '02', '03' => '03', '04' => '04', '05' => '05', '06' => '06', '07' => '07', '08' => '08', '09' => '09', '10' => '10', '11' => '11', '12' => '12', '13' => '13', '14' => '14', '15' => '15', '16' => '16', '17' => '17', '18' => '18', '19' => '19', '20' => '20', '21' => '21', '22' => '22', '23' => '23', '24' => '24', '25' => '25', '26' => '26', '27' => '27', '28' => '28', '29' => '29', '30' => '30', '31' => '31',);
  $hours = array( '01' => '01', '02' => '02', '03' => '03', '04' => '04', '05' => '05', '06' => '06', '07' => '07', '08' => '08', '09' => '09', '10' => '10', '11' => '11', '12' => '12',);
  $minutes = array('00' => '00', '30' => '30');
  $ampm = array('am' => 'A.M.', 'pm' => 'P.M.');
}


function _class_scheduler_fix_hour($hour, $ampm){
    if($ampm == 'am' && $hour == 12){
      $hour = 0;
    }
    else if($ampm == 'pm' && $hour != 12){
      $hour += 12;
    }
  return $hour;
}


/*
    Determine the start and end times for a given quarter, relative to the
    current date.
    
    $start_time and $end_time are passed in by reference and are set by this
    function to the correct times

    if $offset is zero, the start and end times for the current quarter is
    returned. For positive values, a quarter that many quarters in the future
    is returned, and for negative values, that many quarters in the past
 */

function _cm_airing_grid_get_quarter_dates(&$start_time, &$end_time, $offset) {

  if($offset > 0) {
    $start_time = _cm_airing_grid_get_next_quarter();
    $offset--;
    while ( $offset ) {
      $offset--;
      $start_time = _cm_airing_grid_get_next_quarter($start_time);
    }
    $end_time = _cm_airing_grid_get_next_quarter($start_time) - 1;
  }
  else if ($offset < 0) { 
    $offset = abs($offset);
    $start_time--;
    $start_time = _cm_airing_grid_get_this_quarter();
    while ( $offset ) {
      $start_time--;
      $offset--;
      $start_time = _cm_airing_grid_get_this_quarter($start_time);
    }
    $end_time = _cm_airing_grid_get_next_quarter($start_time) - 1;
  }
  else {
    $start_time = _cm_airing_grid_get_this_quarter();
    $end_time = _cm_airing_grid_get_next_quarter($start_time) - 1;
  }
}

/** 
 * WARNING: does not actually return the third sunday, but actually returns
 * the second from the last sunday, which is usually the third sunday, but not
 * always.
 *
 * TODO: change the name of this function
 *
 * Params: $month: month as int 1 through 12
 *         $year:  4 digit year
 *         $day_of_week: 0 for sunday, through 6 for saturday
 *         $offset:  0 for 1st, positive #s for 2nd, 3rd, etc.
 *                   negative for last, 2nd from last, etc.
 *
 * Returns:  the day of month of the desired day and week offset (1 through 31)
 *
 * FIXME: change the default offset to 1
 *
 * The offset is used to determine
 */
function _cm_airing_grid_get_third_sunday($month, $year, $day_of_week = 0, $offset = -2) {

  // get the first of the month as seconds from the epoch
  $first = mktime(0, 0, 0,$month, 1, $year);

  // get an empty list of this month's sundays (or whatever day quarter begins)
  $l_suns = array();

  // How many days are in this month?
  $nod = date('t', $first);

  $i = NULL;
  for ( $i = 0; $i < $nod; $i++ ) {
    if ( date('w', $first) == $day_of_week ) {
      //store this date in our list
      $l_suns[] = $first;
    }
    $first += 24*60*60;
  }
  $o = count($l_suns) + $offset;
  if ( $offset < 0 ) {
    $o = count($l_suns) + $offset;
  }
  else {
    $o = $offset;
  }
  // return day of month (1 through 31)
  return date('j', $l_suns[$o]); 
}

//returns timestamp of 3rd sunday of correct month
function _cm_airing_grid_get_next_quarter($targ = NULL){
  //TODO:  we should add these as params to this function:
         //  $periods:  quarters? semesters?  set this to n where 1/n is the length in years of your period
         //  $modulo:  how many months into the year does the new period begin? for periods beginning in December, choose 0
  if($targ){
    $now_month = date("n", $targ);
    $now_day = date("j", $targ);
    $now_year = date("Y", $targ);
  }
  else {
    $now_month = date("n");
    $now_day = date("j");
    $now_year = date("Y");
  }
  //TODO change this to use the period and modulo from the TODO above
  if( $now_month % 3 == 0 ){
    $this_month_quarter = _cm_airing_grid_get_third_sunday($now_month, $now_year);
    if($this_month_quarter > $now_day){
      return mktime(0,0,0, $now_month, _cm_airing_grid_get_third_sunday($now_month, $now_year), $now_year);
    }
    else {
      $now_month += 3;
    }
  }
  else {
    $now_month += (3 - $now_month % 3);
  }
  if($now_month > 12){
    $now_year++;
    $now_month -= 12;
  }
  return mktime(0,0,0, $now_month, _cm_airing_grid_get_third_sunday($now_month, $now_year), $now_year);
}

function _cm_airing_grid_get_this_quarter($targ = NULL){
  if($targ){
    $now_month = date("n", $targ);
    $now_day = date("j", $targ);
    $now_year = date("Y", $targ);
  }
  else {
    $now_month = date("n");
    $now_day = date("j");
    $now_year = date("Y");
  }
  if( $now_month % 3 == 0 ){
    $this_month_quarter = _cm_airing_grid_get_third_sunday($now_month, $now_year);
    if($this_month_quarter <= $now_day){
      return mktime(0,0,0, $now_month, _cm_airing_grid_get_third_sunday($now_month, $now_year), $now_year);
    }
    else {
      $now_month -= 3;
    }
  }
  else {
    $now_month -= ($now_month % 3);
  }
  if($now_month < 1){
    $now_year--;
    $now_month += 12;
  }
  return mktime(0,0,0, $now_month, _cm_airing_grid_get_third_sunday($now_month, $now_year), $now_year);
}

// for a given timestamp $end, return the stamp of the next half hour mark (e.g. 4:30, 11:00)
// warning: returns same time if this IS the half hour
function _cm_airing_grid_next_half_hour($end) {
  if( $end % 1800 == 0 ) {
    return $end;
  }
  $minutes = date('i', $end);
  $r = $end % 3600; // number of seconds since the hour
  $hour = $end - $r; // last hour timestamp
  if ( $r < 1800 ) { // it's before :30
    return $hour + 1800;
  }
  else { // it's between :30 and :59
    return $hour + 3600;
  }
}




