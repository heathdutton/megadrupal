<?php
/**
 * @param $time
 * @param null $tz
 * @return array
 */
function _calendar_systems_get_granularity_from_time($time, $tz = NULL) {
  $granularity = array();
  $allgranularity = array('year', 'month', 'day', 'hour', 'minute', 'second', 'timezone');
  $temp = date_parse($time);
  // Special case for 'now'.
  if ($time == 'now') {
    $granularity = array('year', 'month', 'day', 'hour', 'minute', 'second');
  }
  else {
    // This PHP date_parse() method currently doesn't have resolution down to
    // seconds, so if there is some time, all will be set.
    foreach ($allgranularity as $g) {
      if ((isset($temp[$g]) && is_numeric($temp[$g])) || ($g == 'timezone' && (isset($temp['zone_type']) && $temp['zone_type'] > 0))) {
        $granularity[] = $g;
      }
    }
  }
  if ($tz) {
    $granularity[]='timezone';
  }

  return $granularity;
}




/**
 * @param $format
 * @return array
 */
function _calendar_systems_get_granularity_from_format($format) {
  return date_format_order($format);
}


/**
 * @param $format
 * @return array
 */
function _calendar_systems_get_granularity_part($part, $granularity) {
  $result = array();
  foreach ($granularity as $value) {
    switch ($part) {
      case 'date':
        if (in_array($value, array('year', 'month', 'day'))) {
          $result[] = $value;
        }
        break;
      case 'time':
        if (in_array($value, array('hour', 'minute', 'second', 'timezone'))) {
          $result[] = $value;
        }
        break;
      default:
    }
  }
  return $result;
}


/**
 * @param $format
 * @return array
 */
function _calendar_systems_get_format_part($part, $format) {
  $granularity = _calendar_systems_get_granularity_from_format($format);
  $granularity = _calendar_systems_get_granularity_part($part, $granularity);
  return date_limit_format($format, $granularity);
}

/**
 * Internal helper to check whether the required patch is applied or not.
 *
 * @return
 *   Boolean value.
 */
function _calendar_systems_is_patch_applied($cache = TRUE) {
  static $result;
  if ($cache) {
    $cache_object = cache_get('calendar_systems_is_patch_applied');
    if (is_object($cache_object)) {
      $result = $cache_object->data;
    }
  }
  if (is_null($result)) {
    $content = file_get_contents(DRUPAL_ROOT . '/includes/common.inc');
    // Check against patch fingerprint:
    $patch_applied = (strpos($content, 'foreach (module_implements(\'format_date\') AS $module) {') !== FALSE) ?
      TRUE : FALSE;
    $new_patch_applied = (strpos($content, 'foreach (module_implements(\'format_date_calendar_systems\') AS $module) {') !== FALSE) ?
      TRUE : FALSE;
    if (!$new_patch_applied && $patch_applied) {
      $result = 'outdated';
    } elseif ($new_patch_applied) {
      $result = TRUE;
    } else {
      $result = $patch_applied;
    }
    cache_set('calendar_systems_is_patch_applied', $result, 'cache',  REQUEST_TIME  + (60*60));
  }
  return $result;
}


/**
 * @param $date
 * @param $format
 * @return bool|mixed
 */
function _calendar_systems_fix_display($date, $format, $calendar_system_name) {
  $calendar = calendar_systems_get_calendar_instance($calendar_system_name);
  $date = new DateObject($date);
  $timestamp = $date->format("U");
  if (!$timestamp) {
    return FALSE;
  }
  else {
    $content = $calendar->smartGet($format, $timestamp);
    return $content;
  }
}

/**
 * Helper function
 *
 * @return
 *   A List of valid date format characters
 */
function _calendar_systems_get_date_format() {
  $chars=array(
    'L' => 'year',
    'o' => 'year',
    'Y' => 'year',
    'y' => 'year',
    'F' => 'month',
    'm' => 'month',
    'M' => 'month',
    'n' => 'month',
    't' => 'month',
    'd' => 'day',
    'D' => 'day',
    'j' => 'day',
    'l' => 'day',
    'N' => 'day',
    'S' => 'day',
    'w' => 'day',
    'z' => 'day',
    'a' => 'hour',
    'A' => 'hour',
    'B' => 'hour',
    'g' => 'hour',
    'G' => 'hour',
    'h' => 'hour',
    'H' => 'hour',
    'i' => 'minute',
    's' => 'second',
    'u' => 'second',
    'e' => 'hour',
    'O' => 'hour',
    'P' => 'hour',
    'T' => 'hour',
    'Z' => 'second',
    'I' => 'day',
    'c' => '',
    'r' => '',
    'U' => '',
  );

  return $chars;
}


/**
 * Helper function
 * @param $format
 * @param $granularity
 * @return string
 */
function _calendar_systems_fix_whitespace($format, $granularity) {
  $new_format = '';
  $list = _calendar_systems_get_date_format();

  //remove whitespaces from end
  $i = strlen($format) - 1;
  for (; $i >= 0; $i--) {
    $c = $format[$i];
    if (isset($list[$c])) {
      break;
    }
  }
  $format = substr($format, 0, $i + 1);

  //remove whitespaces from begining
  $i = 0;
  for (; $i < strlen($format); $i++) {
    $c = $format[$i];
    if (isset($list[$c])) {
      break;
    }
  }

  $new_format = substr($format, $i, strlen($format) - $i);

  return $new_format;
}


/**
 * Helper function
 * @param $format
 * @param $granularity
 * @return string
 */
function _calendar_systems_get_format($format, $granularity) {
  $new_format = '';

  switch ($format) {
    case 'small':
    case 'short':
      $new_format = variable_get('date_format_short', 'm/d/Y - H:i');
      break;
    case 'large':
      $new_format = variable_get('date_format_long', 'l, F j, Y - H:i');
      break;
    case 'medium':
      $new_format = variable_get('date_format_medium', 'D, m/d/Y - H:i');
      break;
    default:
      $new_format = $format;
      break;
  }
  if ($granularity) {
    $new_format = _calendar_systems_fix_by_granularity($new_format, $granularity);
    $new_format = _calendar_systems_fix_whitespace($new_format, $granularity);
  }
  return $new_format;
}

/**
 * Helper function
 * @param $format
 * @param $granularity
 * @return string
 */
function _calendar_systems_fix_by_granularity($format, $granularity) {
  $new_format = '';
  $list = _calendar_systems_get_date_format();
  for ($i=0; $i < strlen($format); $i++) {
    $c = $format[$i];
    if (isset($list[$c])) {
      if (isset($granularity[$list[$c]])) {
        $new_format .= $c;
      }
    }
    else{
      $new_format .= $c;
    }
  }

  return $new_format;
}

/**
 * @param $date
 * @param null $format
 * @return array|bool|int|mixed|null|string
 *
 * Internal helper method to convert and validate date
 */
function _calendar_systems_date_convert($date, $format = NULL, $calendar_system = NULL, $options = array()) {
  if (is_null($calendar_system)){
    $calendar_system = calendar_systems_get_calendar_system_name();
  }
  if ($calendar_system == 'default') {
    return $date;
  }
  // Get proper calendar instance:
  $calendar = calendar_systems_get_calendar_instance($calendar_system);
  $result = FALSE;
  if (is_array($date)) {
    $r = $calendar->isDateValid(isset($date['month'])?$date['month']:NULL, isset($date['day'])?$date['day']:NULL, isset($date['year'])?$date['year']:NULL);
    if ($r !== 0 && $r !== FALSE) {
      list($date['year'], $date['month'], $date['day']) = $calendar->toGregorian($date['year'], $date['month'], $date['day']);
      $result = $date;
    }
  } else {
    $date_parts = array();
    if (isset($options['iso_fixed_size_position']) && $options['iso_fixed_size_position']) {
      $format_parser = '(?<year>[0-9]{4})(?<month>[0-9]{2})?(?<day>[0-9]{2})?(?<hour>[0-9]{2})?(?<minute>[0-9]{2})?(?<second>[0-9]{2})?';
      if (preg_match_all("%$format_parser%", $date, $matches)) {
        $date_parts['year'] = @$matches['year'][0];
        $date_parts['month'] = @$matches['month'][0];
        $date_parts['day'] = @$matches['day'][0];
        $date_parts['hour'] = @$matches['hour'][0];
        $date_parts['minute'] = @$matches['minute'][0];
        $date_parts['second'] = @$matches['second'][0];
      }
    } elseif (!is_null($format)) {
      $format = preg_quote($format);
      $format_parser = preg_replace('%([a-zA-Z])%', '(?<$1>[0-9]+)', $format); //dDjFmMnYy
      if (preg_match_all("%$format_parser%", $date, $matches)) {
        $date_parts['year'] = @$matches['Y'][0];
        $date_parts['month'] = @$matches['m'][0];
        $date_parts['day'] = @$matches['d'][0];
        $date_parts['hour'] = @$matches['H'][0];
        $date_parts['minute'] = @$matches['i'][0];
        $date_parts['second'] = @$matches['s'][0];
      }
    } else {
      // Extract date and time:
      $_date = preg_replace('/([0-9]+\-[0-9]+\-[0-9]+) (.*)/si', '$1', $date);
      $time = preg_replace('/([0-9]+\-[0-9]+\-[0-9]+) (.*)/si', '$2', $date);

      if (preg_match('/^([0-9]{2,4})[-\/\\\]([0-9]{1,2})[-\/\\\]([0-9]{1,2})( +([0-9]{1,2})[:]([0-9]{1,2})[:]([0-9]{1,2}))?/', $_date, $regs)) {
        $date_parts['year'] = $regs[1];
        $date_parts['month'] = $regs[2];
        $date_parts['day'] = $regs[3];
        $format = "Y-m-d $time";
      }
    }

    // Convert to timestamp:
    if ($date_parts) {
      if (@empty($date_parts['month'])) {
        $date_parts['month'] = 1;
      }
      if (@empty($date_parts['day'])) {
        $date_parts['day'] = 1;
      }
      $r = $calendar->isDateValid($date_parts['month'], $date_parts['day'], $date_parts['year']);
      if ($r !== 0 && $r !== FALSE) {
        $result = $calendar->strToTimestamp("{$date_parts['year']}-{$date_parts['month']}-{$date_parts['day']} 10:10:10");
        $result = date($format, $result);
      }
    }
  }

  return $result;
}