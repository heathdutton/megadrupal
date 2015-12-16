<?php

/**
 * @file
 * Gregorian for datex. Nothinf special, always calls DateTime methods to 
 * handle stuff.
 */


/**
 * Default Gregorian for datex.
 */
class DatexGregorian extends DatexCalendarIterface {
  public static function convert($timestamp) {
    $date = new DateTime('@' . $timestamp);

    return array(
      $date->format('Y'),
      $date->format('m'),
      $date->format('n'),
      $date->format('j'),
      $date->format('H'),
      $date->format('i'),
      $date->format('s'),
      $date->format('z'),
      $date->format('w'),
    );
  }

  /**
   * Opposite of DatexCalendarIterface::convert(). 
   */
  public static function toGregorian($year = 0, $month = 0, $day = 0) {
    $date = new DateTime();
    if (!$year) {
      $year = $date->format('Y');
    }
    if (!$month) {
      $month = $date->format('n');
    }
    if (!$day) {
      $day = $date->format('j');
    }
    return array($year, $month, $day);
  }

  public static function isLeap($year) {
    $date = new DateTime;
    $date->setDate($year, 1, 1);
    return $date->format('L');
  }

  public function getInfo($name) {
  }

  public function calendarFormat() {
    return array();
  }

  public static function dayOfWeek($year, $dayOfYear = 0) {
    return 0;
  }

  /**
   * Provides an array containing default arguments for INTL formatter.
   */
  public function getIntlArgs() {
  }

  public function fixGranularities(array $granuls) {
  }

  public function formatPHP($format) {
    return $this->xformat($format);
  }

  public function setDate($y = NULL, $m = NULL, $d = NULL) {
    if ($y === NULL) {
      $y = date('Y');
    }

    if ($m === NULL) {
      $m = date('n');
    }

    if ($d === NULL) {
      $d = date('j');
    }

    $this->xsetDate($y, $m, $d);
  }

  public function objectFromDate($date, $tz = NULL) {
    return $this->xobjectFromDate($date, $tz);
  }
}
