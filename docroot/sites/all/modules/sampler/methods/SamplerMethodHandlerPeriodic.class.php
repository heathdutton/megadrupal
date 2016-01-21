<?php

/**
 * @file
 * Handler class for the periodic method plugin.
 */

class SamplerMethodHandlerPeriodic implements SamplerMethodHandlerInterface {

  private $startstamp;
  private $endstamp;
  private $current_time;

  public function __construct($sampler) {
    $this->sampler = $sampler;

    // Dump in plugin option defaults.
    $this->sampler->options = $this->sampler->options + $this->options();

    $this->current_time = REQUEST_TIME;
  }

  public function options() {
    // Add periodic defaults to the global options.
    return array(
      'startstamp' => NULL,
      'endstamp' => NULL,
      'time_unit' => 'week',
      'time_unit_interval' => 1,
      'single_sample' => NULL,
      'abort_timestamp' => REQUEST_TIME,
    );
  }

  public function buildSampleSet() {

    $timestamps = array();

    // Find the last sample time for the metric.  If there's no data, then
    // calculate what would have been the previous sample period so the first
    // sample can have a valid range.

    // For some reason empty() is showing this object property as empty when
    // it's not.  Sticking it in a variable seems to work around the issue.
    $last_sample_time = $this->sampler->lastSampleTime;
    if (empty($last_sample_time)) {
      switch ($this->sampler->options['time_unit']) {
        case 'day':
          $timestamp_function = 'dailyTimestamp';
          break;
        case 'week':
          $timestamp_function = 'weeklyTimestamp';
          break;
        case 'month':
          $timestamp_function = 'monthlyTimestamp';
          break;
        default:
          $timestamp_function = 'customTimestamp';
          break;
      }
      $startstamp = $this->$timestamp_function($this->current_time, 0 - $this->sampler->options['time_unit_interval'], $this->sampler->options['time_unit']);
    }
    else {
      $startstamp = $last_sample_time;
    }

    $this->startstamp = isset($this->sampler->options['startstamp']) ? $this->sampler->options['startstamp'] : $startstamp;
    $this->endstamp = isset($this->sampler->options['endstamp']) ? $this->sampler->options['endstamp'] : $this->current_time;


    // Convert string stamps into Unix timestamps if necessary.
    foreach (array('startstamp', 'endstamp') as $stamp) {
      if (!is_numeric($this->$stamp)) {
        $this->$stamp = strtotime($this->$stamp);
      }
    }

    // Make these values available to the sampler object.
    $this->sampler->methodPluginData['startstamp'] = $this->startstamp;
    $this->sampler->methodPluginData['endstamp'] = $this->endstamp;

    // Allow single sample range.
    if (isset($this->sampler->options['single_sample'])) {
      $timestamps = array($this->startstamp, $this->endstamp);
    }
    // Build sample set given the calculated settings.
    else {
      // Determine the proper functions for the time unit.
      switch ($this->sampler->options['time_unit']) {
        case 'day':
          $get_samples_function = 'getTimestampsDay';
          break;
        case 'week':
          $get_samples_function = 'getTimestampsWeek';
          break;
        case 'month':
          $get_samples_function = 'getTimestampsMonth';
          break;
        default:
          $get_samples_function = 'getTimestampsCustom';
          break;
      }
      $timestamps = $this->$get_samples_function($this->startstamp, $this->endstamp, $this->sampler->options['time_unit_interval'], $this->sampler->options['time_unit']);
    }

    // Now that we have the list of timestamps, build the sample objects.
    $samples = array();
    // In order to have a time range there needs to be more than one timestamp
    // in the set.
    if (count($timestamps > 1)) {
      $previous_timestamp = current($timestamps);
      while ($next_timestamp = next($timestamps)) {
        $samples[] = $this->makeSample($previous_timestamp, $next_timestamp);
        $previous_timestamp = $next_timestamp;
      }
    }

    // Some necessary sanity checks for periodic sample sets.  If they fail,
    // clear out the sample set.
    if (!$this->validateSampleSet($samples)) {
      $samples = array();
    }

    if (!empty($samples)) {
      $this->buildPluginOutput();
    }

    return $samples;
  }

  /**
   * Creates a sample object.
   *
   * @param $sample_startstamp
   *   Unix timestamp, the begining of the time range for the sample.
   *
   * @param $sample_endstamp
   *   Unix timestamp, the end of the time range for the sample.
   *
   * @return
   *   A sample object.
   */
  public function makeSample($sample_startstamp, $sample_endstamp) {
    $sample = new stdClass();
    $sample->sample_startstamp = $sample_startstamp;
    $sample->sample_endstamp = $sample_endstamp;

    // Single samples are assumed to be ala carte, set the sample timestamp to
    // the time the sample is being taken.
    if ($this->sampler->options['single_sample']) {
      $sample->timestamp = $this->current_time;
    }
    // Otherwise, use the end of the periodic time range for the sample
    // timestamp -- this makes it easy to prevent generating a sample time
    // range that has already been stored.
    else {
      $sample->timestamp = $sample_endstamp;
    }
    return $sample;
  }

  /**
   * Validates the sample set before it is returned to the API.
   *
   * @param $samples
   *   The array of sample objects.
   *
   * @return
   *   TRUE if the sample set passes validation, FALSE otherwise.
   */
  public function validateSampleSet($samples) {

    if (!empty($samples)) {
      $last_sample_point = array_pop($samples);
      // Don't calculate any values if the last sample point's sample_endstamp
      // is past the abort time.
      if ($last_sample_point->sample_endstamp > intval($this->sampler->options['abort_timestamp'])) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * getdate() with timezone adjustment.
   *
   * PHP's getdate() is affected by the server's timezone. We need to cancel it
   * out so everything is UTC.
   *
   * @param $timestamp
   *   An optional, integer UNIX timestamp.
   *
   * @return
   *   An array with results identical to PHP's getdate().
   */
  public function gmgetdate($timestamp = NULL) {
    $timestamp = isset($timestamp) ? $timestamp : REQUEST_TIME;
    $gmt_offset = (int) date('Z', $timestamp);
    return getdate($timestamp - $gmt_offset);
  }

  /**
   * Compute a timestamp for the beginning of a day N days in the future.
   *
   * @param $time
   *   Mixed, either a UTC timestamp or an array returned by
   *   sampler_gmgetdate().
   * @param $days_since
   *   An integer specifying a number of days since. A value of 0 indicates the
   *   current day.  Defaults to 0.
   *
   * @return
   *   UTC UNIX timestamp.
   */
  public function dailyTimestamp($time = NULL, $days_since = 0) {
    $time_parts = is_array($time) ? $time : $this->gmgetdate($time);
    $day = $time_parts['mday'] + $days_since;
    return gmmktime(0, 0, 0, $time_parts['mon'], $day, $time_parts['year']);
  }

  /**
   * Compute a timestamp for the beginning of a week N weeks in the future.
   *
   * The beginning of the week is hard-coded as midnight on Sunday UTC.
   *
   * @param $time
   *   Mixed. Integer timestamp or an array returned by $this->gmgetdate().
   * @param $weeks_since
   *   An integer specifying a number of weeks since. A value of 0 indicates the
   *   current week.  Defaults to 0.
   *
   * @return
   *   GMT UNIX timestamp.
   */
  public function weeklyTimestamp($time = NULL, $weeks_since = 0) {
    $time_parts = is_array($time) ? $time : $this->gmgetdate($time);
    $day = $time_parts['mday'] - $time_parts['wday'] + (7 * $weeks_since);
    return gmmktime(0, 0, 0, $time_parts['mon'], $day, $time_parts['year']);
  }

  /**
   * Compute a timestamp for the beginning of a month N months in the future.
   *
   * The beginning of the month is hard-coded as midnight UTC on the first day
   * of the month.
   *
   * @param $time
   *   Mixed. Integer timestamp or an array returned by $this->gmgetdate().
   * @param $months_since
   *   An integer specifying a number of months since. A value of 0 indicates the
   *   current month.  Defaults to 0.
   *
   * @return
   *   GMT UNIX timestamp.
   */
  public function monthlyTimestamp($time = NULL, $months_since = 0) {
    $time_parts = is_array($time) ? $time : $this->gmgetdate($time);
    $month = $time_parts['mon'] + $months_since;
    return gmmktime(0, 0, 0, $month, 1, $time_parts['year']);
  }


  /**
   * Compute a custom timestamp, given a start, unit, and interval.
   *
   * @param $time
   *   Unix timestamp to use as the base for the calculation.
   * @param $units_since
   *   An integer specifying a number of time units to count.  Defaults to 0.
   * @param $unit
   *   The unit to count by, expressing in seconds.  Defaults to one hour.
   *
   * @return
   *   UTC UNIX timestamp.
   */
  public function customTimestamp($time = NULL, $units_since = 0, $unit = 3600) {
    return $time + ($units_since * $unit);
  }

  /**
   * Build a set of timestamps on a daily time interval.
   *
   * @see dailyTimestamp()
   *
   * @param $startstamp
   *   UNIX timestamp some time during the day of the first day in the sample
   *   set.
   * @param $endstamp
   *   UNIX timestamp some time during the day of the last day in the sample
   *   set.
   * @param $interval
   *   The number of days to count by. Defaults to 1.
   *
   * @return
   *   An array of GMT timestamps sorted in ascending order. The first value is
   *   is the beginning of the day containing $startstamp. Each subsequent
   *   value is the timestamp for the beginning of the next day. The final
   *   value is the beginning of the day containing $endstamp.
   */
  public function getTimestampsDay($startstamp, $endstamp, $interval = 1) {
    $timestamps = array();

    // First, compute the start of the day for the beginning and ending
    // timestamps so we know when to stop.
    $this->sampler->methodPluginData['startstamp'] = $this->dailyTimestamp($startstamp);
    $this->sampler->methodPluginData['endstamp'] = $this->dailyTimestamp($endstamp);

    // Debugging.
    // $startdate = date('Y-m-d H:i:s', $this->sampler->methodPluginData['startstamp']);
    // $enddate = date('Y-m-d H:i:s', $this->sampler->methodPluginData['endstamp']);

    // ...then compute all the days up to that.
    $parts = $this->gmgetdate($startstamp);
    $i = 0;
    while (TRUE) {
      $next_timestamp = $this->dailyTimestamp($parts, $i);
      if ($next_timestamp > $this->sampler->methodPluginData['endstamp']) {
        break;
      }
      else {
        $timestamps[] = $next_timestamp;
      }
      $i = $i + $interval;

      // Debugging.
      // $sample = date('Y-m-d H:i:s', $next_timestamp);
    }

    return $timestamps;
  }

  /**
   * Build a set of timestamps on a weekly time interval.
   *
   * @see weeklyTimestamp()
   *
   * @param $startstamp
   *   UNIX timestamp some time during the week of the first week in the sample
   *   set.
   * @param $endstamp
   *   UNIX timestamp some time during the week of the last week in the sample
   *   set.
   * @param $interval
   *   The number of weeks to count by. Defaults to 1.
   *
   * @return
   *   An array of UTC timestamps sorted in ascending order. The first value is
   *   is the beginning of the week containing $startstamp. Each subsequent
   *   value is the timestamp for the beginning of the next week. The final
   *   value is the beginning of the week in $endstamp.
   */
  public function getTimestampsWeek($startstamp, $endstamp, $interval = 1) {
    $timestamps = array();

    // First, compute the start of the week for the beginning and ending
    // timestamps so we know when to stop.
    $this->sampler->methodPluginData['startstamp'] = $this->weeklyTimestamp($startstamp);
    $this->sampler->methodPluginData['endstamp'] = $this->weeklyTimestamp($endstamp);

    // Debugging.
    // $startdate = date('Y-m-d H:i:s', $this->sampler->methodPluginData['startstamp']);
    // $enddate = date('Y-m-d H:i:s', $this->sampler->methodPluginData['endstamp']);

    // ...then compute all the weeks up to that.
    $parts = $this->gmgetdate($startstamp);
    $i = 0;
    while (TRUE) {
      $next_timestamp = $this->weeklyTimestamp($parts, $i);
      if ($next_timestamp > $this->sampler->methodPluginData['endstamp']) {
        break;
      }
      else {
        $timestamps[] = $next_timestamp;
      }
      $i = $i + $interval;

      // Debugging.
      // $sample = date('Y-m-d H:i:s', $next_timestamp);
    }

    return $timestamps;
  }

  /**
   * Build a set of timestamps on a monthly time interval.
   *
   * @see monthlyTimestamp()
   *
   * @param $startstamp
   *   UNIX timestamp some time during the first month in the sample
   *   set.
   * @param $endstamp
   *   UNIX timestamp some time during the last month in the sample
   *   set.
   * @param $interval
   *   The number of months to count by. Defaults to 1.
   *
   * @return
   *   An array of UTC timestamps sorted in ascending order. The first value is
   *   is the beginning of the month containing $startstamp. Each subsequent
   *   value is the timestamp for the beginning of the next month. The final
   *   value is the beginning of the month in $endstamp.
   */
  public function getTimestampsMonth($startstamp, $endstamp, $interval = 1) {
    $timestamps = array();

    // First, compute the start of the month for the beginning and ending
    // timestamps so we know when to stop.
    $this->sampler->methodPluginData['startstamp'] = $this->monthlyTimestamp($startstamp);
    $this->sampler->methodPluginData['endstamp'] = $this->monthlyTimestamp($endstamp);

    // Debugging.
    // $startdate = date('Y-m-d H:i:s', $this->sampler->methodPluginData['startstamp']);
    // $enddate = date('Y-m-d H:i:s', $this->sampler->methodPluginData['endstamp']);

    // ...then compute all the months up to that.
    $parts = $this->gmgetdate($startstamp);
    $i = 0;
    while (TRUE) {
      $next_timestamp = $this->monthlyTimestamp($parts, $i);
      if ($next_timestamp > $this->sampler->methodPluginData['endstamp']) {
        break;
      }
      else {
        $timestamps[] = $next_timestamp;
      }
      $i = $i + $interval;

      // Debugging.
      // $sample = date('Y-m-d H:i:s', $next_timestamp);
    }

    return $timestamps;
  }

  /**
   * Build a set of timestamps on a custom time interval.
   *
   * @see customTimestamp()
   *
   * @param $startstamp
   *   UNIX timestamp indicating the start of the sample set.
   * @param $endstamp
   *   UNIX timestamp indicating the end of the sample set.
   * @param $interval
   *   The number of units to count by. Defaults to 1.
   * @param $unit
   *   The units of time to count by. Defaults to 1 hour.
   *
   * @return
   *   An array of UTC timestamps sorted in ascending order. The first value is
   *   $startstamp. Each subsequent timestamp is based on the previous
   *   timestamp plus ($interval * $unit). The final value is $endstamp.
   */
  public function getTimestampsCustom($startstamp, $endstamp, $interval = 1, $unit = 3600) {
    $timestamps = array();

    $this->sampler->methodPluginData['startstamp'] = $startstamp;
    $this->sampler->methodPluginData['endstamp'] = $endstamp;

    // Debugging.
    // $startdate = date('Y-m-d H:i:s', $this->sampler->methodPluginData['startstamp']);
    // $enddate = date('Y-m-d H:i:s', $this->sampler->methodPluginData['endstamp']);

    $i = 0;
    while (TRUE) {
      $next_timestamp = $this->customTimestamp($startstamp, $i, $unit);
      if ($next_timestamp > $this->sampler->methodPluginData['endstamp']) {
        break;
      }
      else {
        $timestamps[] = $next_timestamp;
      }
      $i = $i + $interval;

      // Debugging.
      // $sample = date('Y-m-d H:i:s', $next_timestamp);
    }

    return $timestamps;
  }

  /**
   * Given a set of metric options, return a display string that describes the
   * frequency of the sample.
   *
   * Note that it is the caller's responsibility to properly filter the return
   * value of this function.
   *
   * @return
   *   A string representing the frequency of the sample.
   */
  public function buildPluginOutput() {

    $startstamp = isset($this->sampler->methodPluginData['startstamp']) ? $this->sampler->methodPluginData['startstamp'] : NULL;
    $endstamp = isset($this->sampler->methodPluginData['endstamp']) ? $this->sampler->methodPluginData['endstamp'] : NULL;
    $unit = isset($this->sampler->options['time_unit']) ? $this->sampler->options['time_unit'] : NULL;
    $interval = isset($this->sampler->options['time_unit_interval']) ? $this->sampler->options['time_unit_interval'] : NULL;
    $single = isset($this->sampler->options['single_sample']) ? $this->sampler->options['single_sample'] : NULL;

    // Translate timestamps into human-readable dates if necessary.
    if (isset($startstamp)) {
      $start_date = is_numeric($startstamp) ? format_date($startstamp) : $startstamp;
    }
    else {
      $start_date = t('[not specified]');
    }
    if (isset($endstamp)) {
      $end_date = is_numeric($endstamp) ? format_date($endstamp) : $endstamp;
    }
    else {
      $end_date = t('[not specified]');
    }

    // Single samples get a different display.
    if (isset($single) && isset($startstamp) && isset($endstamp)) {
      $sample_frequency_string = t("Single sample from !start_date to !end_date", array('!start_date' => $start_date, '!end_date' => $end_date));
    }
    // Standard sample sets.
    elseif (isset($startstamp) && isset($unit) && isset($interval)) {
      switch ($unit) {
        case 'day':
          $frequency = format_plural($interval, 'day', '@count days');
          break;
        case 'week':
          $frequency = format_plural($interval, 'week', '@count weeks');
          break;
        case 'month':
          $frequency = format_plural($interval, 'month', '@count months');
          break;
        default:
          // Try to figure out if the custom interval is a fairly standard one.
          // These are listed in order highest to lowest so the biggest unit
          //will be found first.
          $custom_units = array(
            3600 => array('single' => 'hour', 'plural' => 'hours'),
            60 => array('single' => 'minute', 'plural' => 'minutes'),
          );
          foreach ($custom_units as $standard_unit => $labels) {
            // No remainder means we can factor out the standard unit into a
            // standard unit plus a multiplier.  We use the multiplier to
            // ensure that we're providing the correct final interval to the
            // unit.
            if ($unit % $standard_unit == 0) {
              $unit_multiplier = $unit / $standard_unit;
              $final_interval = $interval * $unit_multiplier;
              $frequency = format_plural($final_interval, $labels['single'], "@count {$labels['plural']}");
              break;
            }
          }
          // Can't figure out any other sensible time unit, so use seconds.
          if (!isset($frequency)) {
            $frequency = format_plural(($unit * $interval), 'second', '@count seconds');
          }
          break;
      }
      $this->sampler->pluginOutput['method_periodic'] = t("Every !frequency, starting !start_date.", array('!frequency' => $frequency, '!start_date' => $start_date));
    }
  }
}
