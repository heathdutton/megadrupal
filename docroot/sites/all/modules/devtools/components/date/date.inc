<?php

/**
 * Returns the date for the first of the month of the given date.
 * Date should be in the 'standard' YYYY-MM-DD format.
 * @param  [type] $date   [description]
 * @param  [type] $format [description]
 * @return [type]         [description]
 */
function floor_date($date, $format = NULL) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    // date should be in the 'standard' YYYY-MM-DD format
    $date{8} = 0;
    $date{9} = 1;

    return $date;
  }
}

/**
 * Returns the timestamp for 00:00:00am of the specified date
 * @param  [type] $timestamp [description]
 * @return [type]            [description]
 */
function floor_timestamp($timestamp) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    return strtotime(date('m/d/y 0:0:0', $timestamp));
  }
}

/**
 * Returns the date for the last of the month of the given date.
 * Date should be in the 'standard' YYYY-MM-DD format.
 * @param  [type] $date   [description]
 * @param  [type] $format [description]
 * @return [type]         [description]
 */
function ceil_date($date, $format = NULL) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $days = date('t', strtotime($date));

    $date{8} = floor($days / 10);
    $date{9} = ($days % 10);

    return $date;
  }
}

/**
 * Returns the timestamp for 11:59:59pm (23:59:59) of the specified date
 * @param  [type] $timestamp [description]
 * @return [type]            [description]
 */
function ceil_timestamp($timestamp) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    return mktime(23, 59, 59, date('m', $timestamp), date('d', $timestamp), date('Y', $timestamp));
  }
}