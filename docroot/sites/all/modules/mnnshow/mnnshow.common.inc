<?php

ctools_include('cleanstring', 'ctools', 'includes');

/**
 * Returns actual timezone offset for server's
 * timezone. Takes in account daylight saving shifts.
 *
 * @return float
 */
function mnnshow_get_utc_timezone_offset() {
  $dtz = new DateTimeZone(date_default_timezone_get());
  $time_in_server_zone = new DateTime('now', $dtz);
  return ($dtz->getOffset( $time_in_server_zone ) / 3600);
}

/**
 * Get the channels (tv stations).
 */
function mnnshow_get_channels(){
  return array(
    1 => array(
      'name' => 'Community Channel',
      'description' => 'Stay up-to-date on local elections, New York politics, and community issues with our Community channel.',
      'watchLiveUrl' => 'live/1-community-channel',
      'tvChannels' => array(

        0 => array(
          'tvStationName' => 'TWC',
          'tvStationChannel' => '34',
        ),
        1 => array(
          'tvStationName' => 'RCN',
          'tvStationChannel' => '82',
        ),
        2 => array(
          'tvStationName' => 'FiOS',
          'tvStationChannel' => '33',
        )
      )
    ),
    2 => array(
      'name' => 'Lifestyle Channel',
      'description' => 'Exercise your mind, body, and palate with entertainment and lifestyle programming for New Yorkers with interests as diverse as our city.',
      'watchLiveUrl' => 'live/2-lifestyle-channel',
      'tvChannels' => array(
        0 => array(
          'tvStationName' => 'TWC',
          'tvStationChannel' => '56'
        ),
        1 => array(
          'tvStationName' => 'RCN',
          'tvStationChannel' => '83',
        ),
        2 => array(
          'tvStationName' => 'FiOS',
          'tvStationChannel' => '34',
        )
      )
    ),
    3 => array(
      'name' => 'Spirit Channel',
      'description' => 'Our Spirit channel broadcasts varied religious and philosophical services, lectures, and discussions for all faiths and worldviews.',
      'watchLiveUrl' => 'live/3-spirit-channel',
      'tvChannels' => array(
        0 => array(
          'tvStationName' => 'TWC',
          'tvStationChannel' => '57'
        ),
        1 => array(
          'tvStationName' => 'RCN',
          'tvStationChannel' => '84',
        ),
        2 => array(
          'tvStationName' => 'FiOS',
          'tvStationChannel' => '35',
        )
      )
    ),
    4 => array(
      'name' => 'Culture Channel',
      'description' => 'Multi-lingual programming that covers the arts, politics, and global news for all of Manhattan’s diverse communities.',
      'watchLiveUrl' => 'live/4-culture-channel',
      'tvChannels' => array(
        0 => array(
          'tvStationName' => 'TWC',
          'tvStationChannel' => '67',
        ),
        1 => array(
          'tvStationName' => 'RCN',
          'tvStationChannel' => '85',
        ),
        2 => array(
          'tvStationName' => 'FiOS',
          'tvStationChannel' => '36',
        )
      )
    )
  );
}

/**
 * Get show href.
 */
function mnnshow_get_show_href($title, $id) {
  $url_title = mnnshow_url_feiendly_title($title);
  $href = 'show-info/' . $id . '/' . $url_title;
  return $href;
}

/**
 * Generate a url friendly title.
 */
function mnnshow_url_feiendly_title($title){
  // remove all non-alphanumeric characters, then remove leading/trailing spaces
  // $url_title = trim(preg_replace("/[^A-Za-z0-9\/ ]/", '', strtolower($title)));
  // replace spaces with dashes
  // $url_title = preg_replace("/[\/ ]/", '-', $url_title);

  $url_title = ctools_cleanstring($title, array('lower case' => true));
  return $url_title;
}

/**
 * Check if a date is within a range.
 */
function mnnshow_is_date_in_range($start_date, $end_date, $date) {
  // assume DateTime object
  return (($date >= $start_date) && ($date <= $end_date));
}

/**
 * get_week_range accepts numeric $month, $day, and $year values.
 * It will find the first monday and the last sunday of the week for the
 * given day, and return them as YYYY-MM-DD HH:MM:SS timestamps
 *
 * @param string $day   : numeric value of the day (01 - 31)
 * @param string $month : numeric value of the month (01 - 12)
 * @param string $year  : four-digit value of the year (2008)
 * @return array|bool   : array('first' => monday of week, 'last' => sunday of week);
 */
function mnnshow_get_week_range($day = '', $month = '', $year = '') {
  // default empties to current values
  if (empty($day)) $day = date('d');
  if (empty($month)) $month = date('m');
  if (empty($year)) $year = date('Y');
  // do some figurin'
  $weekday = date('w', mktime(0, 0, 0, $month, $day, $year));
  $monday = $day - $weekday;
  $start_week = date('Y-m-d H:i:00', mktime(0, 0, 0, $month, $monday + 1, $year));   // monday
  $end_week = date('Y-m-d H:i:00', mktime(0, 0, 0, $month, $monday + 7, $year)); // sunday
  if (!empty($start_week) && !empty($end_week)) {
    return array('first' => $start_week, 'last' => $end_week);
  }
  // otherwise there was an error :'(
  return false;
}

/**
 * format start/end dates of a range for printing to the screen.
 * ex: "April 11 - 18, 2011" or "December 29, 2011 - January 3, 2012"
 *
 * @param DateTime $start : start of the range
 * @param DateTime $end   : end of the range
 * @return string         : the formatted string
 */
function mnnshow_format_date_range($start, $end){
  if ($start->format('Y') == $end->format('Y') && $start->format('m') == $end->format('m'))
    return $start->format('F j - ').$end->format('j, ').$start->format('Y');
  elseif ($start->format('Y') == $end->format('Y'))
    return $start->format('F j - ').$end->format('F j, ').$start->format('Y');
  else
    return $start->format('F j, Y - ').$end->format('F j, Y');
}

/**
 * Set the proper headers for cors responses.
 */
function mnnshow_cors_headers() {
  drupal_set_header('Access-Control-Allow-Origin: *');
  drupal_set_header('Access-Control-Max-Age: 3600');
}
