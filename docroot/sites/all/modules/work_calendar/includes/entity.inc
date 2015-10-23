<?php

/**
 * Define a WorkCalendar object with its operations.
 */
class WorkCalendar extends Entity {

  /**
   * @defgroup wc_week_days_constants Constants to represent week days
   * @{
   * Constants that represent bit numbers to compound the week byte.
   *
   * The week byte stores which week days are working days in the calendar.
   */
  const SUNDAY    = 1;   // 00000001
  const MONDAY    = 2;   // 00000010
  const TUESDAY   = 4;   // 00000100
  const WEDNESDAY = 8;   // 00001000
  const THURSDAY  = 16;  // 00010000
  const FRIDAY    = 32;  // 00100000
  const SATURDAY  = 64;  // 01000000
  /**
   * @} End of "defgroup wc_week_days_constants".
   */

  public $name;
  public $label;
  public $description;
  public $week = 0;
  public $week_days;

  /**
   * Initialize object attributes and decode `week`.
   */
  function __construct(array $values = array(), $entityType = 'work_calendar') {
    parent::__construct($values, $entityType);
    $this->week_days = array();
    foreach ($this->dayCodes() as $name => $code) {
      if ($this->week & $code) {
        $this->week_days[$name] = $name;
      }
    }
  }

  /**
   * Returns whether the entity is locked, thus may not be deleted or renamed.
   *
   * Entities provided in code are automatically treated as locked, as well
   * as any fixed profile type.
   */
  function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }

  /**
   * Encodes given week days into week byte.
   */
  function updateWeek($week_days) {
    $codes = $this->dayCodes();
    $this->week = 0;
    foreach ($week_days as $name) {
      if (!empty($name)) {
        $this->week = $this->week | $codes[$name];
      }
    }
    $this->week_days = $week_days;
  }

  /**
   * Map week day names to our internal bits.
   */
  private static function dayCodes() {
    static $codes;
    if (empty($codes)) {
      foreach (date_week_days_untranslated() as $name) {
        $codes[$name] = constant('self::' . strtoupper($name));
      }
    }
    return $codes;
  }

  /**
   * Returns the untranslated week day name for the requested date.
   *
   * @param $date DateObject or date string.
   */
  private static function weekDayName($date) {
    $week_day = date_day_of_week($date);
    $names = date_week_days_untranslated();
    return $names[$week_day];
  }

  /**
   * Returns start and end date for a year or month period.
   */
  private static function periodDates($year, $month = NULL) {
    if (is_null($month)) {
      $start = "$year-01-01 00:00:00";
      $end = "$year-12-31 00:00:00";
    }
    else {
      $start = "$year-$month-01 00:00:00";
      $days = date_days_in_month($year, $month);
      $end = "$year-$month-$days 00:00:00";
    }
    return array($start, $end);
  }

  /**
   * Returns dates in a period corresponding to requested week days.
   *
   * Internally it builds a rrule to do the job.
   */
  private static function weekDaysDates($start, $end, $week_days) {
    // Transform $week_days to a rrule "byday" string. Example: SU,MO,TU,WE.
    // date_repeat api wants the byday array to start by the week day of $start.
    $byday = array();
    // Convert $week_days to a key based array and find the index of the week
    // day of start in order to assign bydays starting from this index.
    $week_days = array_values($week_days);
    $index = array_search(self::weekDayName($start), $week_days);
    $count = count($week_days);
    for ($offset = 0; $offset < $count; $offset++) {
      $wday = $week_days[($index + $offset) % $count];
      $byday[] = strtoupper(substr($wday, 0, 2));
    }

    // Request date_repeat to run the rrule.
    $rrule = 'RRULE:FREQ=WEEKLY;INTERVAL=1;BYDAY=' . implode(',', $byday) . ';';
    // Don't require date_repeat module to be enabled. Just use its api.
    module_load_include('module', 'date_repeat', 'date_repeat');
    module_load_include('inc', 'date_repeat', 'date_repeat_calc');
    $dates = date_repeat_calc($rrule, (string)$start, (string)$end);

    // Strip hours from dates.
    foreach ($dates as $key => $date) {
      $dates[$key] = substr($date, 0, 10);
    }

    // Special case. First day is always included in despite of the rrule.
    // We need to exclude it by hand if neccesary.
    $week_day = self::weekDayName($dates[0]);
    if (!in_array($week_day, $week_days)) {
      array_shift($dates);
    }

    return $dates;
  }

  /**
   * Returns a list of opening days for the requested period.
   */
  function getOpenDaysInPeriod($start, $end) {
    $week_days = $this->week_days;
    $dates = self::weekDaysDates($start, $end, $week_days);
    return $dates;
  }

  /**
   * Returns a list of opening days for the requested year or month.
   */
  function getOpenDays($year, $month = NULL) {
    list($start, $end) = self::periodDates($year, $month);
    return $this->getOpenDaysInPeriod($start, $end);
  }

  /**
   * Returns a list of closed days for the requested period.
   */
  function getClosedDaysInPeriod($start, $end) {
    $week_days = array_diff(date_week_days_untranslated(), $this->week_days);
    $dates = self::weekDaysDates($start, $end, $week_days);
    return $dates;
  }

  /**
   * Returns a list of closed days for the requested year or month.
   */
  function getClosedDays($year, $month = NULL) {
    list($start, $end) = self::periodDates($year, $month);
    return $this->getClosedDaysInPeriod($start, $end);
  }

  /**
   * Returns boolean indicating wether the requested day the business is open.
   */
  function isOpenDay($year, $month, $day) {
    $date = new DateObject("$year-$month-$day 00:00:00");
    $week_day = self::weekDayName($date);
    return in_array($week_day, $this->week_days);
  }

  /**
   * Returns boolean indicating wether the requested day the business is closed.
   */
  function isClosedDay($year, $month, $day) {
    $date = new DateObject("$year-$month-$day 00:00:00");
    $week_day = self::weekDayName($date);
    return !in_array($week_day, $this->week_days);
  }
}
