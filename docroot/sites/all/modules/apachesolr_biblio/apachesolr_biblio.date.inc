<?php

/**
 * @file
 *
 * Format biblio date format to apachesolr dateform.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function apachesolr_biblio_format_date_value($value) {
  // Dates have lots of special cases.
  // First, there are four identified formats that have to be taken care of.
  // 1999           represents 12 months in the year
  // 1999 Mar
  // 1999 Mar 6
  // 1999 Mar-Jun   represents a range of months
  // 2005 Nov 21-27  A range of days.
  $values = array();
  $parts = array_filter(explode(' ', $value));
  $months_lookup = array(
    'jan',
    'feb',
    'mar',
    'apr',
    'may',
    'jun',
    'jul',
    'aug',
    'sep',
    'oct',
    'nov',
    'dec',
    'jan',
  );
  $months = isset($parts[1]) ? array_filter(explode('-', $parts[1])) : NULL;
  $days = isset($parts[2]) ? array_filter(explode('-', $parts[2])) : NULL;
  // This is the case that just a year is given, in which case we add 12
  // values, one for each month of the year.
  if (count($parts) == 1) {
    foreach (array_slice($months_lookup, 0, 12) as $m) {
      $values[] = apachesolr_date_iso(strtotime("$m $value"));
    }
  }
  // In this case, there is a range of months.
  else {
    if (count($months) > 1) {
      $startpos = array_search(strtolower($months[0]), $months_lookup);
      $endpos = array_search(strtolower($months[1]), $months_lookup);
      $month_range = range($startpos, $endpos);
      $m = 0;
      foreach ($month_range as $m) {
        $v = $months_lookup[$m] . " {$parts[0]}";
        $values[] = apachesolr_date_iso(strtotime($v));
      }
      if ($m) {
        // Cap off the last month. Advance the month by one...
        $v = $months_lookup[$m + 1];
        // 11 is December!
        if ($m == 11) {
          // We have to advance the year, too.
          $y = $parts[0] + 1;
        }
        else {
          $y = $parts[0];
        }
        $v .= " $y";
        // ... and then shave one second off so that it is still in
        // the last month.
        $values[] = apachesolr_date_iso(strtotime($v) - 1);
      }
    }
    // In this case there is a range of days. Add the first day.
    else {
      if (count($days) > 1) {
        $values[] = apachesolr_date_iso(strtotime("{$parts[1]} {$days[0]}, {$parts[0]}"));
      }
      else {
        $parts = array_filter($parts);
        if (count($parts) == 2) {
          $v = "{$parts[1]} {$parts[0]}";
          $values[] = apachesolr_date_iso(strtotime($v));
        }
        elseif (count($parts) == 3) {
          $v = "{$parts[1]} {$parts[2]}, {$parts[0]}";
          $values[] = apachesolr_date_iso(strtotime($v));
        }
      }
    }
  }

  return $values;
}
