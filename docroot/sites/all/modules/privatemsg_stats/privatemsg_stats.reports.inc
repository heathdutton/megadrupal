<?php

function privatemsg_stats_month_page() {
  $messages_stats = db_query("SELECT FROM_UNIXTIME(timestamp, '%Y-%m-%d') as date, COUNT(*) as count
    FROM {pm_message}
    WHERE timestamp > :date
    GROUP BY date
    ORDER BY date DESC", array(':date' => strtotime('1 month ago')))
    ->fetchAllKeyed();

  $authors_stats = db_query("SELECT FROM_UNIXTIME(timestamp, '%Y-%m-%d') as date, COUNT(DISTINCT author) as count
    FROM {pm_message}
    WHERE timestamp > :date
    GROUP BY date
    ORDER BY date DESC", array(':date' => strtotime('1 month ago')))
    ->fetchAllKeyed();

  $times = _privatemsg_stats_date_range('1 month ago', 'now');

  return theme('privatemsg_stats_chart', array(
    'times' => $times,
    'authors' => array_map(function($day) use ($authors_stats) {
      return $authors_stats[$day] ? (int)$authors_stats[$day] : 0;
    }, $times),
    'messages' => array_map(function($day) use ($messages_stats) {
      return $messages_stats[$day] ? (int)$messages_stats[$day] : 0;
    }, $times),
  ));
}

function privatemsg_stats_year_page() {
  $messages_stats = db_query("SELECT FROM_UNIXTIME(timestamp, '%Y %M') as date, COUNT(*) as count
    FROM {pm_message}
    WHERE timestamp > :date
    GROUP BY date
    ORDER BY date DESC", array(':date' => strtotime('1 year ago')))
    ->fetchAllKeyed();

  $authors_stats = db_query("SELECT FROM_UNIXTIME(timestamp, '%Y %M') as date, COUNT(DISTINCT author) as count
    FROM {pm_message}
    WHERE timestamp > :date
    GROUP BY date
    ORDER BY date DESC", array(':date' => strtotime('1 year ago')))
    ->fetchAllKeyed();

  $times = _privatemsg_stats_date_range('1 year ago', 'now', '+1 month', 'Y F');

  return theme('privatemsg_stats_chart', array(
    'times' => $times,
    'authors' => array_map(function($day) use ($authors_stats) {
      return $authors_stats[$day] ? (int)$authors_stats[$day] : 0;
    }, $times),
    'messages' => array_map(function($day) use ($messages_stats) {
      return $messages_stats[$day] ? (int)$messages_stats[$day] : 0;
    }, $times),
  ));
}

function privatemsg_stats_all_page() {
  $messages_stats = db_query("SELECT YEAR(FROM_UNIXTIME(timestamp)) as date, COUNT(*) as count
    FROM {pm_message}
    GROUP BY date
    ORDER BY date DESC")
    ->fetchAllKeyed();

  $authors_stats = db_query("SELECT YEAR(FROM_UNIXTIME(timestamp)) as date, COUNT(DISTINCT author) as count
    FROM {pm_message}
    GROUP BY date
    ORDER BY date DESC")
    ->fetchAllKeyed();

  $times = array_keys($messages_stats + $authors_stats);
  sort($times);

  return theme('privatemsg_stats_chart', array(
    'times' => $times,
    'authors' => array_map(function($day) use ($authors_stats) {
      return $authors_stats[$day] ? (int)$authors_stats[$day] : 0;
    }, $times),
    'messages' => array_map(function($day) use ($messages_stats) {
      return $messages_stats[$day] ? (int)$messages_stats[$day] : 0;
    }, $times),
  ));
}

function _privatemsg_stats_date_range($first, $last, $step = '+1 day', $format = 'Y-m-d') {
  $dates = array();
  $current = strtotime($first);
  $last = strtotime($last);

  while ($current <= $last) {
    $dates[] = date($format, $current);
    $current = strtotime($step, $current);
  }

  return $dates;
}
