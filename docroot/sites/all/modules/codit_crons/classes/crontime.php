<?php
/**
 * @file
 * CronTime class definition.
 */

class CronTime {
  // Property Declarations.

  // This is the time when cron last ran this function.
  protected $CronLastRan;
  // The name of this cron function.
  protected $CronJobName;

  protected $CronShouldRunAfter;

  // Patterns related to the timing string of when cron should run the function.
  protected $PatternAfterTime;
  protected $PatternDay;
  protected $PatternMonth;
  protected $PatternSkipQty;
  protected $PatternSkipUnit;
  protected $PatternSubmitted = NULL;

  // Flag for making sure this runs now if it has never run before.
  protected $FirstRun = NULL;

  // The amount of time since cron last ran this function.
  protected $TimeSinceLastRan;
  // Array of allowed months with variation on spellings.
  protected $allowedMonths = array(
    'jan' => 'january',
    'january' => 'january',
    'feb' => 'february',
    'february' => 'february',
    'mar' => 'march',
    'march' => 'march',
    'apr' => 'april',
    'april' => 'april',
    'may' => 'may',
    'jun' => 'june',
    'june' => 'june',
    'jul' => 'july',
    'july' => 'july',
    'aug' => 'august',
    'august' => 'august',
    'sep' => 'september',
    'sept' => 'september',
    'september' => 'september',
    'oct' => 'october',
    'october' => 'october',
    'nov' => 'november',
    'november' => 'november',
    'dec' => 'december',
    'december' => 'december',
  );


  /**
   * Builds CronTime upon instantiation.
   *
   * @param string $cron_job_name
   *   The machine name of the cron function.
   * @param array $parameters
   *   Allows for addition of other parameters.  Currently supported params:
   *    - 'PatternSubmitted' => 'timing string to be parsed, see README.md'
   */
  public function __construct($cron_job_name = '', $parameters = array()) {
    try {
      // A CronJobName is mandatory.
      $this->setCronJobName($cron_job_name);
      // Unpack any parameters.  Parameters will override default properties.
      foreach ($parameters as $property => $value) {
        $this->$property = $value;
      }

      $this->setPatternSubmitted($this->PatternSubmitted);

    }
    catch (Exception $e) {
      watchdog('Codit: Cron', 'Exception caught !messsage', array('!message' => $e->getMessage()), WATCHDOG_NOTICE, $link = NULL);
    }
  }


  /**
   * Setter for CronJobName.
   *
   * @param string $cron_job_name
   *   The unique machine name for the cron function.
   *
   * @return \CronTime
   *   For chaining.
   *
   * @throws Exception
   */
  protected function setCronJobName($cron_job_name = '') {
    if (!empty($cron_job_name) && (strlen($cron_job_name) <= 32)) {
      $this->CronJobName = $cron_job_name;
    }
    else {
      $msg_vars = array('!CronJobName' => $cron_job_name);
      throw new Exception(t('Can not instantiate a CronTime object without passing a valid cron name that is 32 characters or less.', $msg_vars));
    }
    return $this;
  }

  /**
   * Getter for the CronJobName.
   *
   * @return string
   *   The cron name of this cron function.
   */
  public function getCronJobName() {
    return (property_exists($this, 'CronJobName')) ? $this->CronJobName : NULL;
  }


  /**
   * Setter for the timing string pattern that also parses the string.
   *
   * @param string $pattern
   *   A valid timing string. See Readme.md for supported timing strings.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function setPatternSubmitted($pattern = '') {
    if (!empty($pattern)) {
      $pattern = strtolower(trim($pattern));
      // Remove multiple spaces.
      $pattern = preg_replace('!\s+!', ' ', $pattern);
      $this->PatternSubmitted = $pattern;

      // Now that the pattern is set, go ahead and parse it.
      // Extracting for skip_qty, skip_units, time, day, month.
      $time_pieces = explode(' ', $this->getPatternSubmitted());
      $this->parsePatternMonth($time_pieces);
      $this->parsePatternSkipQty($time_pieces);
      $this->parsePatternSkipUnit($time_pieces);
      $this->parsePatternAfterTime($time_pieces);
      $this->parsePatternDay($time_pieces);

      // Use the parsed info to build the time.
      $this->getNextRun();

    }
    else {
      // There is no timing sentence so log that it's going to run every cron.
      watchdog('Codit: Cron', 'There is no timing pattern registered for cron: %CronJobName so it will run each time cron runs.', array('%CronJobName' => $this->getCronJobName()), WATCHDOG_INFO, NULL);
    }
    return $this;
  }


  /**
   * Getter for submitted timing string pattern.
   *
   * @return string|NULL
   *   - string The timing string pattern submitted.
   *   - NULL if unavailable.
   */
  public function getPatternSubmitted() {
    return (property_exists($this, 'PatternSubmitted')) ? $this->PatternSubmitted : NULL;
  }


  /**
   * Getter for the SkipUnit from the timing string.
   *
   * @return string|NULL
   *   - string Timing unit like seconds, minutes, hours, days, months, years.
   *   - NULL if unavailable.
   */
  public function getPatternSkipUnit() {
    return (property_exists($this, 'PatternSkipUnit')) ? $this->PatternSkipUnit : NULL;
  }

  /**
   * Evaluates a time pattern and extracts and sets the unit of time.
   *
   * @param string $time_pieces
   *   Text containing a unit.
   */
  protected function parsePatternSkipUnit($time_pieces = array()) {
    // Units should appear 2 index point after the word 'every'.
    $parsed_unit = $this->parseRetriever('every', $time_pieces, 2);
    $this->setPatternSkipUnit($parsed_unit);
  }

  /**
   * Setter for valid PatternSkipUnit. As in 'every 6 {PatternSkipUnit}'.
   *
   * @param string $possible_unit
   *   A unit defining the amount of time (seconds, hours, days, weeks...).
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function setPatternSkipUnit($possible_unit = '') {
    $possible_unit = strtolower(trim($possible_unit));
    if (!empty($possible_unit)) {
      $allowed_units = array(
        'second' => 'seconds',
        'seconds' => 'seconds',
        'sec' => 'seconds',
        'minute' => 'minutes',
        'minutes' => 'minutes',
        'min' => 'minutes',
        'days' => 'days',
        'day' => 'days',
        'hour' => 'hours',
        'hours' => 'hours',
        'hr' => 'hours',
        'hrs' => 'hours',
        'weeks' => 'weeks',
        'week' => 'weeks',
        'wk' => 'weeks',
        'wks' => 'weeks',
        'months' => 'months',
        'month' => 'months',
        'years' => 'years',
        'year' => 'years',
        'yr' => 'years',
        'yrs' => 'years',
      );
      // Check to see if $possible_unit is an allowed unit.
      if (!empty($allowed_units[$possible_unit])) {
        $unit = $allowed_units[$possible_unit];
      }
    }
    $this->PatternSkipUnit = (!empty($unit)) ? $unit : NULL;
    return $this;
  }


  /**
   * Setter for Month to set in timing string.
   *
   * @param string $possible_month
   *   The month to set in the timing of the cron function.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function setPatternMonth($possible_month = '') {
    // Check to see that month name is present.
    if (!empty($possible_month) && !empty($this->allowedMonths[$possible_month])) {
      $this->PatternMonth = $this->allowedMonths[$possible_month];
    }
    return $this;
  }


  /**
   * Getter for PatternMonth from the timing string.  As in 'after 4th of July'.
   *
   * @return string|NULL
   *   - string The name of the month set.
   *   - NULL If no month is set.
   */
  public function getPatternMonth() {
    return (property_exists($this, 'PatternMonth')) ? $this->PatternMonth : NULL;
  }


  /**
   * Finds a month and sets the integer representation of that month if found.
   *
   * @param array $time_pieces
   *   An array of parts that make up a time phrase.
   */
  private function parsePatternMonth($time_pieces = array()) {
    foreach (is_array($time_pieces) ? $time_pieces : array() as $key => $value) {
      if (!empty($this->allowedMonths[$value])) {
        $this->setPatternMonth($value);
      }
    }
  }


  /**
   * Setter for the SkipQty.  As in 'every {3} days...'.
   *
   * @param int $possible_qty
   *   The number of units to skip each time.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function setPatternSkipQty($possible_qty = NULL) {
    $qty = NULL;
    // Make sure its an integer.
    if (!empty($possible_qty)) {
      // Make sure it is an integer.
      settype($possible_qty, "integer");
      $qty = ((!empty($possible_qty)) && ($possible_qty > 0)) ? $possible_qty : NULL;
    }
    $this->PatternSkipQty = $qty;
    return $this;
  }


  /**
   * Getter for the SkipQty.
   *
   * @return int|NULL
   *   - int The number of units to skip each time.
   *   - NULL If there are no units to skip.
   */
  public function getPatternSkipQty() {
    return (property_exists($this, 'PatternSkipQty')) ? $this->PatternSkipQty : NULL;
  }

  /**
   * Finds a quantity reference and returns it.
   *
   * @param array $time_pieces
   *   An array of parts that make up a time phrase.
   *
   * @return int|NULL
   *   - Integer reprenting the qty found in the time pieces.
   *   - NULL if there is no positive integer to be found.
   */
  protected function parsePatternSkipQty($time_pieces = array()) {
    $qty = NULL;
    // The skip qty will always appear after 'every'.
    // Find 'every'.
    $parsed_qty = $this->parseRetriever('every', $time_pieces, 1);
    $this->setPatternSkipQty($parsed_qty);
  }

  /**
   * Setter for the AfterTime.  As in '... after {02:15}'.
   *
   * @param string $possible_time
   *   A string of the 24 hour format time after which the cron function runs.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function setPatternAfterTime($possible_time = '') {
    if (!empty($possible_time)) {
      // Need to sanitize for HH:MM:SS
      $time_components = date_parse($possible_time);
      if ($time_components['error_count'] !== 0) {
        $possible_time = NULL;
        // Log message.
        foreach ($time_components['errors'] as $error) {
          watchdog('Codit: Cron', 'The setPatternAfterTime() did not like the time of !crontime due to !cronerror.', array('!crontime' => $possible_time, '!cronerror' => $error), WATCHDOG_INFO, NULL);
        }
      }
    }
    $this->PatternAfterTime = $possible_time;
    return $this;
  }


  /**
   * Getter for AfterTime. As in '... after {02:15}'.
   *
   * @return string|NULL
   *   - string The 24 hour format time after which the cron function runs.
   *   - NULL if not available
   */
  public function getPatternAfterTime() {
    return (property_exists($this, 'PatternAfterTime')) ? $this->PatternAfterTime : NULL;
  }


  /**
   * Parser for the PatternAfterTime as in '... after 13:00'.
   *
   * @param array $time_pieces
   *   The exploded time string to parse.
   */
  protected function parsePatternAfterTime($time_pieces = array()) {
    // Time comes after the word 'after'.
    $time = $this->parseRetriever('after', $time_pieces, 1);
    $this->setPatternAfterTime($time);
  }


  /**
   * Setter for PatternDay.
   *
   * @param int $possible_day
   *   The interger day to advance to.
   *
   * @return \CronTime
   *   For chaining.
   */
  private function setPatternDay($possible_day = '') {
    // Make sure it is an integer.
    settype($possible_day, "integer");
    // Need to sanitize for valid $possible_day being 1-31.
    // @TODO Make it check the number of days in the target month (29-31 risky).
    if (($possible_day < 1) || ($possible_day > 31)) {
      $possible_day = NULL;
    }
    $this->PatternDay = $possible_day;
    return $this;
  }


  /**
   * Getter for PatternDay.
   *
   * @return string|NULL
   *   Returns the day if there is one, NULL if there is not.
   */
  public function getPatternDay() {
    return (property_exists($this, 'PatternDay')) ? $this->PatternDay : NULL;
  }


  /**
   * Find the day that comes after the word 'the'.
   *
   * @param array $time_pieces
   *   The exploded time string to parse.
   */
  protected function parsePatternDay($time_pieces = array()) {
    // Day comes after the word 'the'.
    $day = $this->parseRetriever('the', $time_pieces, 1);
    $this->setPatternDay($day);
  }


  /**
   * Parser to find the needle in an array, and return the item offset from it.
   *
   * @param string $needle
   *   The thing in the array we are looking for.
   * @param array $haystack
   *   The array to search through for the needle within haystack.
   * @param int $offset
   *   How many items away from the needle to jump to get the returned item.
   *
   * @return string
   *   The item that resides at needle + offset.
   */
  public function parseRetriever($needle = '', $haystack = array(), $offset = 0) {
    // Locate the $needle.
    $needle_index = array_search($needle, $haystack);
    $target_index = $needle_index + $offset;
    if ((!is_null($needle_index)) && ($needle_index !== FALSE) && (!empty($haystack[$target_index]))) {
      return $haystack[$target_index];
    }
    return NULL;
  }


  /**
   * Gets the time since the last time the specified cron function ran from db.
   *
   * @return \CronTime
   *   For chaining.
   */
  private function queryGetCronTime() {
    $cron_function_name = $this->getCronJobName();
    if (!empty($cron_function_name)) {
      // Get the time when $cron_function_name last ran.
      $n_time_last_ran = db_query('SELECT ran_date FROM {codit_crons} WHERE cron_name = :name', array(':name' => $cron_function_name))->fetchField();
      // Check to see if we have a time.  If not, it hasn't ever run.
      if (($n_time_last_ran == 0) ||
          (empty($n_time_last_ran)) ||
          (!is_numeric($n_time_last_ran)) ||
          ((int) $n_time_last_ran != $n_time_last_ran)) {
        // Means this has never run or seems to be not a valid time, set Flag.
        $this->FirstRun = TRUE;
        // Initialize the time as now.
        $n_time_last_ran = time();
      }
      $this->setCronLastRan($n_time_last_ran);
    }
    return $this;
  }


  /**
   * Sets the current time in the table as the last time the function executed.
   *
   * @return \CronTime
   *   For chaining.
   */
  public function querySetCronTime() {
    $cron_function_name = $this->getCronJobName();
    $txn = db_transaction();
    try {

      $db_write_status = db_merge('codit_crons')
        ->key(array('cron_name' => $cron_function_name))
        ->insertFields(array(
          'cron_name' => $cron_function_name,
          'ran_date' => time(),
        ))
        ->updateFields(array(
          'ran_date' => time(),
        ))
        ->execute();
      // Provide feedback on what happened.
      switch ($db_write_status) {
        case MergeQuery::STATUS_INSERT:
          watchdog('Codit: Cron', 'Cron function !cron_name executed for the first time.', array('!cron_name' => $cron_function_name), WATCHDOG_INFO, NULL);
          $this->setCronLastRan(time());
          break;

        case MergeQuery::STATUS_UPDATE:
          watchdog('Codit: Cron', 'Cron function !cron_name executed.', array('!cron_name' => $cron_function_name), WATCHDOG_INFO, NULL);
          $this->setCronLastRan(time());
          break;

        default:
          throw new Exception("The run time recording for cron function $cron_function_name failed to record the time in the cron_time table.  This cron function will execute on the next cron.");
      }
    }
    catch (Exception $e) {
      // Something went wrong somewhere in the transaction, so roll back.
      $txn->rollback();
      // Log the exception to watchdog.
      watchdog_exception('type', $e);
    }
    return $this;
  }


  /**
   * Getter for the CronLastRan, reading it from the DB if not available.
   *
   * @return int
   *   Unix / epoch time stamp for of when the cron last ran.
   */
  public function getCronLastRan() {
    // Check to see if we already have the time.
    if (empty($this->CronLastRan)) {
      // We don't know when it ran yet, so query the DB.
      $this->queryGetCronTime();
    }
    return (property_exists($this, 'CronLastRan')) ? $this->CronLastRan : NULL;
  }


  /**
   * Setter for CronLastRan. Uses current time if no time is provided.
   *
   * @param string $when
   *   The last time the cron ran.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function setCronLastRan($when = '') {
    $when = (empty($when)) ? time() : $when;
    $this->CronLastRan = $when;
    return $this;
  }


  /**
   * Calculates when the next full execution should be allowed.
   *
   * @return int
   *   Unix epoch time of the next threshold for full execution of cron func.
   */
  public function getNextRun() {
    // Case Race, first to evaluate TRUE wins.  Start with most complex and
    // move toward simpler to avoid false positives.
    switch (TRUE) {
      // Day, month, afterTime [on the 4th of July after 14:00].
      case ($this->getPatternDay()) && ($this->getPatternMonth()) && ($this->getPatternAfterTime()):
        $this->rollforwardToMonth();
        $this->rollforwardToDay();
        $this->addAfterTime();
        break;

      // day, month [on the 4th of July].
      case ($this->getPatternDay()) && ($this->getPatternMonth()):
        $this->rollforwardToMonth();
        $this->rollforwardToDay();
        break;

      // Skip, units, day,afterTime [on the 15th of every 4 months after 16:00].
      case ($this->getPatternSkipQty()) && ($this->getPatternSkipUnit()) && ($this->getPatternDay()) && ($this->getPatternAfterTime()):
        $this->addTimeFromUnits();
        $this->rollbackMonthToFirst();
        $this->rollforwardToDay();
        $this->addAfterTime();
        break;

      // Skip, units, day  ['on the 6th of every 1 month']].
      case ($this->getPatternSkipQty()) && ($this->getPatternSkipUnit()) && ($this->getPatternDay()):
        $this->addTimeFromUnits();
        $this->rollbackMonthToFirst();
        $this->rollforwardToDay();
        break;

      // Skip, units, PatternAfterTime  ['every 2 months after 14:00']].
      case ($this->getPatternSkipQty()) && ($this->getPatternSkipUnit()) && ($this->getPatternAfterTime()):
        $this->addTimeFromUnits();
        $this->rollbackMonthToFirst();
        $this->addAfterTime();
        break;

      // Skip, units  ['every 2 months']].
      case ($this->getPatternSkipQty()) && ($this->getPatternSkipUnit()):
        $this->addTimeFromUnits();
        break;

      // Only has skip  and no units.
      // Only had units and no skip.
      default:
        return NULL;
    }
    // Check to make sure the next run is more than last run.  If not,
    // something went badly... fire a warning.

    return $this->getCronLastRan();
  }


  /**
   * Rollforward getCronShouldRunAfter to the given PatternMonth.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function rollforwardToMonth() {
    // As in 'on the 4th of July'.
    // Make sure we have a $PatternMonth.
    if (!empty($this->getPatternMonth())) {
      $time_to_build_upon = $this->getCronShouldRunAfter();
      $time_string = $this->getPatternMonth();
      // Roll To that month but uncertain as to whether it went back or forward.
      $new_time = (strtotime($time_string, $time_to_build_upon));
      // The month pick always lands us in the the given year, so that may be
      // past or future.  Only the future is valid.
      if ($new_time - time() < 0) {
        // We are in the past, so add a year.
        $time_string = '+1 year';
        $new_time = (strtotime($time_string, $new_time));
      }
      $this->setCronShouldRunAfter($new_time);
    }
    return $this;
  }


  /**
   * Adds {SkipQty}{SkipUnits} to the base time.  A rollforward.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function addTimeFromUnits() {
    // Check to see if I have SkipQty and SkipUnits.
    if (!empty($this->getPatternSkipQty()) && !empty($this->getPatternSkipUnit())) {
      // Get the starting time to add upon.
      $time_to_build_upon = $this->getCronShouldRunAfter();
      $time_string = $this->getPatternSkipQty() . ' ' . $this->getPatternSkipUnit();
      $new_time = (strtotime($time_string, $time_to_build_upon));
      $this->setCronShouldRunAfter($new_time);
    }
    return $this;
  }

  /**
   * Round down the month to day 1 @ 00:00 for the month CronShouldRunAfter.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function rollbackMonthToFirst() {
    // Get the starting time to build upon.
    $time_to_build_upon = $this->getCronShouldRunAfter();
    $time_string = 'first day of';
    // Roll back to the first day of the month that $time_to_build_upon is in.
    $new_time = (strtotime($time_string, $time_to_build_upon));
    $this->setCronShouldRunAfter($new_time);
    $this->rollbackToMidnight();
    return $this;
  }

  /**
   * For the day that CronShouldRunAfter is on, set the time to midnight 00:00.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function rollbackToMidnight() {
    // Get the starting time to build upon.
    $time_to_build_upon = $this->getCronShouldRunAfter();
    // Now roll back to midnight of that day.
    $time_string = 'midnight';
    $new_time = (strtotime($time_string, $time_to_build_upon));
    $this->setCronShouldRunAfter($new_time);
    return $this;
  }


  /**
   * Adds the PatternAfterTime to the current CronShouldRunAfter.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function addAfterTime() {
    // Check to see if an PatternAfterTime exists.
    if (!empty($this->getPatternAfterTime())) {
      // Will roll back time to midnight first.
      $this->rollbackToMidnight();
      $time_to_build_upon = $this->getCronShouldRunAfter();
      $time_string = $this->getPatternAfterTime();
      $new_time = (strtotime($time_string, $time_to_build_upon));
      $this->setCronShouldRunAfter($new_time);
    }
    return $this;
  }


  /**
   * Takes the month that CronShouldRunAfter is in and goes to that PatternDay.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function rollforwardToDay() {
    // As in 'on the 15th ...'.
    // Check if I have the Pattern.
    if (!empty($this->getPatternDay())) {
      // Rollback the month we are in, to the first of the month.
      $this->rollbackMonthToFirst();
      $time_to_build_upon = $this->getCronShouldRunAfter();
      // Rollforward to the Day in the month that the CronShouldRunAfter is in.
      // Need to subtract 1 day since months don't start at 0.
      $time_string = $this->getPatternDay() - 1 . ' days';
      $new_time = (strtotime($time_string, $time_to_build_upon));
      $this->setCronShouldRunAfter($new_time);
    }
    return $this;
  }


  /**
   * Sets the value of when the cron should be run next.
   *
   * @param int $cron_future_time
   *   An unix epoch  time integer.
   *
   * @return \CronTime
   *   For chaining.
   */
  protected function setCronShouldRunAfter($cron_future_time = 0) {
    // This needs to be safe in that accidentally passing it nothing, should
    // not wipe out any times previously set.  There is no reason for 0 time.
    if (!empty($cron_future_time)) {
      $this->CronShouldRunAfter = $cron_future_time;
    }
    return $this;
  }


  /**
   * Gets the unix time for when this cron funciton is clear to run.
   *
   * @return int
   *   Unix time epoch for the threshold when cron function should fully run.
   */
  public function getCronShouldRunAfter() {
    if (empty($this->CronShouldRunAfter)) {
      // Can't have  null starting place, so try to get when it last ran.
      $this->setCronShouldRunAfter($this->getCronLastRan());
      // There is still a chance this could be empty, so back it up with the
      // the current time if it is.
      if (empty($this->CronShouldRunAfter)) {
        $this->setCronShouldRunAfter(time());
      }
    }
    return $this->CronShouldRunAfter;
  }

  /**
   * Returns a human readable version of CronShouldRunAfter.
   *
   * This is just for debugging and messages.
   *
   * @return string
   *   A human readable threshold for when the cron function should fully run.
   */
  public function getCronShouldRunAfterHumanReadable() {
    return date('m/d/Y - H:i', $this->getCronShouldRunAfter());
  }


  /**
   * Evaluates whether this cron function should fully execute.
   *
   * @return bool
   *   Indicating whether the current time meets the criteria to fully execute
   *   this cron function.
   */
  public function evalShouldItRun() {
    $next_run = $this->getCronShouldRunAfter();
    $now = time();
    if ($this->FirstRun) {
      return TRUE;
    }
    else {
      // Are we past the threshold right now?
      return ($next_run - $now > 0) ? FALSE : TRUE;
    }
  }

  /**
   * Getter for the Elysia cron style rule.
   * 
   * @return string
   *   Representing the Elysia Cron rule.
   */
  public function getElysiaCronAPIRule() {
    $rule_array = array(
      'minute' => '*',
      'hour' => '*',
      'day_of_month' => '*',
      'month' => '*',
      'day_of_week' => '*',
    );

    // If skip and units are minutes.
    if (($this->getPatternSkipUnit() == 'minutes') && (!empty($this->getPatternSkipQty()))) {
      // Means there are minutes to be set.
      if ($this->getPatternSkipQty() >= 60) {
        $rule_array['hour'] = '*/' . round($this->getPatternSkipQty() / 60);
      }
      else {
        $rule_array['minute'] = '*/' . $this->getPatternSkipQty();
      }
    }

    // If skip and units are hours.
    if (($this->getPatternSkipUnit() == 'hours') && (!empty($this->getPatternSkipQty()))) {
      // Means there are hours to be set.
      if ($this->getPatternSkipQty() >= 24) {
        $rule_array['day_of_month'] = '*/' . round($this->getPatternSkipQty() / 24);
      }
      else {
        $rule_array['hour'] = '*/' . $this->getPatternSkipQty();
      }
    }

    // If skip and units are days.
    if (($this->getPatternSkipUnit() == 'days') && (!empty($this->getPatternSkipQty()))) {
      // Means there are days to be set.
      $rule_array['day_of_month'] = '*/' . $this->getPatternSkipQty();
    }

    // If skip and units are weeks.
    if (($this->getPatternSkipUnit() == 'weeks') && (!empty($this->getPatternSkipQty()))) {
      // Means there are weeks to be set as days or months.
      if ($this->getPatternSkipQty() >= 4) {
        $rule_array['month'] = '*/' . round($this->getPatternSkipQty() / 4);
      }
      else {
        $rule_array['day_of_month'] = '*/' . $this->getPatternSkipQty() * 7;
      }
    }

    // If skip and units are months.
    if (($this->getPatternSkipUnit() == 'months') && (!empty($this->getPatternSkipQty()))) {
      // Means there are months to be set.
      $rule_array['month'] = '*/' . $this->getPatternSkipQty();
    }

    // Check for an after time.
    if (!empty($this->getPatternAfterTime())) {
      // There is an after time.
      $time = explode(':', $this->getPatternAfterTime());
      $rule_array['hour'] = $time['0'];
      if (!empty($time['1']) && ($time['1'] !== '00')) {
        $rule_array['minute'] = $time['1'];
      }
    }

    // Check for a specific month.
    if ((!empty($this->getPatternMonth()))) {
      $months_to_numeric = array(
        'january' => 1,
        'february' => 2,
        'march' => 3,
        'april' => 4,
        'may' => 5,
        'june' => 6,
        'july' => 7,
        'august' => 8,
        'september' => 9,
        'october' => 10,
        'november' => 11,
        'december' => 12,
      );
      if (!empty($months_to_numeric[$this->getPatternMonth()])) {
        $rule_array['month'] = $months_to_numeric[$this->getPatternMonth()];
      }
    }

    // If a specific day of the month is set.
    if ((!empty($this->getPatternDay()))) {
      // Means there are day of the month to be set.
      $rule_array['day_of_month'] = $this->getPatternDay();
    }

    return implode(' ', $rule_array);
  }
}
