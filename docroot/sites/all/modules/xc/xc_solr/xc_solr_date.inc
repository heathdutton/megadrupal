<?php
/**
 * @file
 * Utility functions for Solr date manipulation
 * <code>
 *   $date = xc_solr_str_to_date('1900-01-01T00:00:01Z');
 *   ... // manipulation with date_* functions
 *   $solr_date_str = xc_solr_date_to_solr($date);
 *
 *   $ISO_date_str = xc_solr_solr_to_iso('1900-01-01T00:00:01Z'); // 1900-01-01T00:00:01+0000
 *   $solr_date_str = xc_solr_iso_to_solr('1900-01-01T00:00:01+0000'); // 1900-01-01T00:00:01Z
 *
 *   $last_years_end = xc_solr_date_modify('1900-01-01T00:00:01Z', '-1 second');
 * </code>
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Create query facet parameters instead of date facet parameter.
 *
 * The date facet is not working right in several cases, because some dates belong
 * to not only one, but two categories. This is a hack, so if Solr date facet
 * will work properly, we will not use this function.
 *
 * @param $field (String)
 *   The name of field
 * @param $start_date (String)
 *   Solr type date format for start date (eg. '1900-01-01T00:00:01Z')
 * @param $end_date (String)
 *   Solr type date format for end date (eg. '1900-01-01T00:00:01Z')
 * @param $gap_str
 *   Solr type gap format for intervals (eg. '+1YEAR')
 *
 * @return (array)
 *   The array of Solr facet parameters
 */
function xc_solr_date_facet_factory($field, $start_date, $end_date, $gap_str) {
  $facets = array();

  if ($end_date == 'NOW') {
    $end_date = format_date(time(), 'custom', 'Y-m-d\TH:i:s\Z');
    // $end_date = xc_solr_date_to_solr(new DateTime('now', new DateTimeZone(date_default_timezone_get())));
  }

  if (is_null($gap_str) || empty($gap_str)) {
    $facets[] = $field . ':{' . $start_date . ' TO ' . $end_date . '}';
  }
  else {
    $gap = xc_solr_parse_gap($gap_str);
    if ($gap === FALSE) {
      $facets[] = $field . ':{' . $start_date . ' TO ' . $end_date . '}';
    }
    else {
      $gap_str = $gap->quantity . ' ' . strtolower($gap->measure) . ' -1 second';
      do {
        $do_next = TRUE;
        $internal_date = xc_solr_date_modify($start_date, $gap_str);
        if (strcmp($internal_date, $end_date) >= 0) {
          $facets['facet.query'][] = $field . ':[' . $start_date . ' TO ' . $end_date . ']';
          $do_next = FALSE;
        }
        else {
          $facets['facet.query'][] = $field . ':[' . $start_date . ' TO ' . $internal_date . ']';
          $start_date = xc_solr_date_modify($internal_date, '+1 second');
        }
      } while ($do_next);
    }
  }

  return $facets;
}

function xc_solr_parse_gap($gap_str) {
  if (is_array($gap_str)) {
    do {
      $gap_str = $gap_str[0];
    }
    while (is_array($gap_str));
  }
  elseif (!is_string($gap_str)) {
    xc_util_dsm(gettype($gap_str) . ' ' . $gap_str . ' ' . var_export($gap_str, TRUE), 'gap_str');
    return NULL;
  }
  if (preg_match('/^\+?(\d+)\s?(DAY|WEEK|MONTH|YEAR)S?$/i', $gap_str, $matches)) {
    $gap = new stdClass();
    $gap->text = $gap_str;
    $gap->quantity = $matches[1];
    $gap->measure  = $matches[2];
    $gap->date_part_length = 0;
    switch ($gap->measure) {
      case 'WEEK':
      case 'DAY':
        $gap->date_part_length = 10; break;
      case 'MONTH':
        $gap->date_part_length = 7; break;
      case 'YEAR':
        $gap->date_part_length = 4; break;
    }
    return $gap;
  }
  else {
    return FALSE;
  }
}

/**
 * Modify a Solr date string
 *
 * @param $date_str (String)
 *   A Solr date string, like 1900-01-01T00:00:01Z
 * @param $modify (String)
 *   String in a relative format accepted by strtotime(). Examples: "now",
 *   "+1 day", "+1 week", "+1 week 2 days 4 hours 2 seconds", "next Thursday",
 *   "last Monday"
 *
 * @return (String)
 *   A the new Solr date string, like 1900-01-01T00:00:01Z
 */
function xc_solr_date_modify($date_str, $modify) {
  $date = xc_solr_str_to_date($date_str);
  date_modify($date, $modify);
  return xc_solr_date_to_solr($date);
}

/**
 * Transform a Solr date string to DateTime object
 *
 * @param $date_str (String)
 *   A Solr date string, like 1900-01-01T00:00:01Z
 *
 * @return (DateTime)
 *   A DateTime object
 */
function xc_solr_str_to_date($date_str) {
  static $date_time_zone;
  if (is_null($date_time_zone)) {
    $date_time_zone = new DateTimeZone('GMT');
  }
  return date_create(xc_solr_solr_to_iso($date_str), $date_time_zone);
}

/**
 * Transform DateTime object to Solr date string
 *
 * @param $date (DateTime)
 *   A DateTime object
 *
 * @return (String)
 *   A Solr date string, like 1900-01-01T00:00:01Z
 */
function xc_solr_date_to_solr($date) {
  return xc_solr_iso_to_solr(date_format($date, DATE_ISO8601));
}

/**
 * Transform Solr date format (1900-01-01T00:00:01Z) to ISO 8601 format
 * (1900-01-01T00:00:01+0000)
 *
 * @param $date_str (string)
 *   Date in Solr format (1900-01-01T00:00:01Z)
 *
 * @return (String)
 *   Date in ISO 8601 format (1900-01-01T00:00:01+0000)
 */
function xc_solr_solr_to_iso($date_str) {
  return str_replace('Z', '+0000', $date_str);
}

/**
 * Transform ISO 8601 date format (1900-01-01T00:00:01+0000) to Solr date
 * format (1900-01-01T00:00:01Z)
 *
 * @param $date_str (string)
 *   Date in ISO 8601 format (1900-01-01T00:00:01+0000)
 *
 * @return (String)
 *   Date in Solr format (1900-01-01T00:00:01Z)
 */
function xc_solr_iso_to_solr($date_str) {
  return preg_replace('/[\+\-]\d\d00/', 'Z', $date_str);
}

