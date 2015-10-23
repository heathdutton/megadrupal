<?php
/**
 * additional themingfunctions for agreservations and booking calendar
 */


/**
 * Format the time row headings in the week and day view.
 */
//function theme_agreservations_time_row_heading($start_time, $next_start_time, $curday_date) {
////  static $format_hour, $format_ampm;
////  if (empty($format_hour)) {
//  ////    $format = //variable_get('date_format_short', 'm/d/Y - H:i');
////    $format =  'm/d/Y - H:i';
//  ////    $format_hour = str_replace(array('a', 'A'), '', date_limit_format($format, array('hour', 'minute')));
////    $format_hour = str_replace(array('a', 'A'), '', date_limit_format($format, array('hour', 'minute')));
////    $format_ampm = strstr($format, 'a') ? 'a' : (strstr($format, 'A') ? 'A' : '');
////  }
////  if ($start_time == '00:00:00' && $next_start_time == '23:59:59') {
////    $hour = t('All');
////  }
////  elseif ($start_time == '00:00:00') {
////    $date = new DateTime($curday_date .' '. $next_start_time);
////    $hour = t('< @time', array('@time' => $date->format($format_hour)));
////  }
////  else {
////    $date = new DateTime($curday_date .' '. $start_time);
////    $hour = $date->format($format_hour);
////  }
////  if (!empty($date)) {
////    $ampm = $date->format($format_ampm);
////  }
////  else {
////    $ampm = '';
////  }
//  return array('hour' => $hour, 'ampm' => $ampm);
//}

