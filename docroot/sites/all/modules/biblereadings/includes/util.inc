<?php

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function biblereadings_populate_books() {
  $path = drupal_get_path('module', 'biblereadings');
  $file = fopen("$path/resources/bible_books_pipe.csv", 'r');

  $books = array();

  $count = 0;
  while ($line = trim(fgets($file))) {
    $split = explode('|', $line);
    $o = new stdClass();
    $o->position = $split[0];
    $o->abbreviation = $split[1];
    $o->name = $split[2];
    $o->chapters = $split[3];

    drupal_write_record('biblereadings_books', $o);
    $count++;
  }

  fclose($file);

  drupal_set_message("Successfully populated Bible Readings database with $count books.");
}

function biblereadings_populate_books_access() {
  return (biblereadings_books_table_is_empty() && user_access('administer bible readings'));
}

function biblereadings_books_table_is_empty() {
  $sql = "select count(*) from {biblereadings_books}";
  $result = db_query($sql)->fetchField();
  return ($result == 0);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function biblereadings_get_books() {
  $sql = "select * from {biblereadings_books} order by position";
  $result = db_query($sql);
  $books = array();
  foreach ($result as $o) {
    $books[strtolower($o->name)] = $o;
  }
  return $books;
}

function biblereadings_get_subscribers($active_only = TRUE) {
  $query = "SELECT * FROM {biblereadings_subscriptions}";
  if ($active_only) {
    $query .= ' WHERE unsubscribed = 0';
  }
  $result = db_query($query);

  $items = array();

  foreach ($result as $item) {
    $items[] = array(
        'name' => $item->name,
        'email' => $item->email,
        'joined' => $item->joined,
    );
  }

  return $items;
}

function biblereadings_format_regular_reading($reading, $abbreviated = FALSE, $link = FALSE, $version = 'NKJV') {
  if ($abbreviated) {
    $regular = $reading->abbreviation;
  }
  else {
    $regular = $reading->bookname;
  }
  if ($reading->startchapter) {
    $regular .= sprintf(' %s-%s', $reading->startchapter, $reading->endchapter);
  }

  if ($link) {
    $url = sprintf('http://www.biblegateway.com/passage/?search=%s&version=%s', urlencode($regular), $version);
    $regular = l($regular, $url, array('attributes' => array('target' => '_blank')));
  }

  return $regular;
}

function biblereadings_format_proverb($reading, $abbreviated = FALSE, $link = FALSE, $version = 'NKJV') {
  if ($abbreviated) {
    $pr = t('Pr');
  }
  else {
    $pr = t('Proverbs');
  }
  $proverb = sprintf('%s %d', $pr, $reading->proverb);

  if ($link) {
    $url = sprintf('http://www.biblegateway.com/passage/?search=%s&version=%s', urlencode($proverb), $version);
    $proverb = l($proverb, $url, array('attributes' => array('target' => '_blank')));
  }

  return $proverb;
}

function biblereadings_format_psalms($reading, $abbreviated = FALSE, $link = FALSE, $version = 'NKJV') {
  if ($reading->psalms[1]) {
    if ($abbreviated) {
      $ps = t('Ps');
    }
    else {
      $ps = t('Psalms');
    }

    $psalms = sprintf("%s %d-%d", $ps, $reading->psalms[0], $reading->psalms[1]);
  }
  else {
    if ($abbreviated) {
      $ps = t('Ps');
    }
    else {
      $ps = t('Psalm');
    }

    $psalms = sprintf("%s %d", $ps, $reading->psalms[0]);
  }

  if ($link) {
    $url = sprintf('http://www.biblegateway.com/passage/?search=%s&version=%s', urlencode($psalms), $version);
    $psalms = l($psalms, $url, array('attributes' => array('target' => '_blank')));
  }

  return $psalms;
}

function biblereading_load($id) {
  return db_query("SELECT * from {biblereadings_readings} WHERE id = :id", array(':id' => $id))->fetchObject();
}

function biblereadings_readings_by_date($date) {

  $start_date = strtotime(date('m/d/Y', $date));
  $end_date = strtotime(date('m/d/Y', $date)) + ONE_DAY - 1;

  $items = biblereadings_readings_by_date_range($start_date, $end_date);

  return $items[0];
}

function biblereadings_readings_by_date_range($startdate, $enddate) {
  $items = array();

  $q = db_query("SELECT r.*, b.name as bookname, b.abbreviation
      FROM {biblereadings_readings} r
      JOIN {biblereadings_books} b
        ON b.id = r.startbook
      WHERE date >= :startdate and date <= :enddate
      ORDER BY date ASC", array(':startdate' => $startdate, ':enddate' => $enddate));

  foreach ($q as $i) {
    $i->psalms = biblereadings_get_today_psalms($i->date);
    $i->proverb = biblereadings_get_today_proverb($i->date);
    $items[] = $i;
  }
  return $items;
}

function biblereadings_get_date_range($range_type) {
  $fullday = 24 * 60 * 60;
  $day_of_week = date('w');
  $day_of_month = date('d');
  $current_year = date('Y');
  $next_year = $current_year + 1;

  $today = strtotime(date('m/d/Y'));
  $this_week_sunday = $today - ($day_of_week * $fullday);
  $first_of_month = $today - (($day_of_month - 1) * $fullday);
  $first_of_next_month = strtotime(date('m/d/Y', strtotime('first day of next month')));
  $last_of_next_month = strtotime(date('m/d/Y', strtotime('first day of next month', $first_of_next_month))) - $fullday;

  $first_of_year = strtotime('1/1/' . $current_year);
  $first_of_next_year = strtotime('1/1/' . $next_year);

  $start = 0;
  $end = 0;

  switch ($range_type) {
    case 'this_week':
      $start = $this_week_sunday;
      $end = $start + (6 * $fullday);
      break;
    case 'next_week':
      $start = $this_week_sunday + (7 * $fullday);
      $end = $start + (6 * $fullday);
      break;
    case 'this_month':
      $start = $first_of_month;
      $end = $first_of_next_month - $fullday;
      break;
    case 'next_month':
      $start = $first_of_next_month;
      $end = $last_of_next_month;
      break;
    case 'next_30_days':
      $start = $today;
      $end = $today + (30 * $fullday);
      break;
    case 'next_60_days':
      $start = $today;
      $end = $today + (60 * $fullday);
      break;
    case 'next_90_days':
      $start = $today;
      $end = $today + (90 * $fullday);
      break;
    case 'this_year':
      $start = $first_of_year;
      $end = $first_of_next_year - $fullday;
      break;
  }

  return array(
      'start' => $start,
      'end' => $end,
  );
}

function biblereadings_readings_this_week() {
  $fullday = 24 * 60 * 60;
  $day = date('w');
  $today = strtotime(date('m/d/Y'));
  $sunday = $today - ($day * $fullday);
  $saturday = $today + ((6 - $day) * $fullday);

  $readings = biblereadings_readings_by_date_range($sunday, $saturday);

  return $readings;
}

function biblereadings_get_today_psalms($date) {
  $d = date('d', $date);
  if ($d != 31) {
    $endpsalm = $d * 5;
    $startpsalm = $endpsalm - 4;
    return array($startpsalm, $endpsalm);
  }
  else {
    return array(119);
  }
}

function biblereadings_get_today_proverb($date) {
  $d = date('j', $date);
  return $d;
}

function biblereadings_subscription_exists($email) {
  $query = "SELECT COUNT(*) FROM {biblereadings_subscriptions} WHERE email = :email";
  $result = db_query($query, array(':email' => $email));

  return ($result->fetchField() > 0);
}

function biblereadings_unsubscribe($email) {
  $query = "UPDATE {biblereadings_subscriptions} SET unsubscribed = :unsubscribed WHERE email = :email";
  db_query($query, array(':email' => $email, ':unsubscribed' => time()));
}

function biblereadings_email_daily_readings_to_user($date, $user) {
  static $readings;
  static $default_params;

  if (!isset($readings)) {
    $readings = biblereadings_readings_by_date($date);
  }

  if (!isset($default_params)) {
    $default_params = array(
        'subject' => t('Your Bible Readings for !date', array('!date' => date('F j'))),
        'readings' => $readings,
    );
  }

  $params = array();
  $params['name'] = $user['name'];
  $params += $default_params;

  // D7 converts html mail to plain text.  In the interest of time, we are going to flow
  // with this instead of building an alternate implementation of the mail system.
  // See here for details when ready:
  // http://drupal.org/node/900794
//  if ($user['preference'] == 'html') {
//    $mail_key = 'biblereadings_html_mail';
//  }
//  else {
  $mail_key = 'biblereadings_normal_mail';
//  }
  drupal_mail('biblereadings', $mail_key, $user['email'], language_default(), $params);
}