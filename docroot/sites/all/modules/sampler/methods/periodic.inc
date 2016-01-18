<?php

/**
 * @file
 * Method plugin to generate sample sets using a periodic sample strategy.
 *
 * Periodic sampling is some regular time interval (eg. 'once a week').  For
 * daily sampling, the daily timestamp is always set to midnight UTC of the day
 * being sampled, and for weekly sampling, to midnight UTC Sunday of the week
 * being sampled.
 *
 * The daily/weekly timestamp code was partially lifted from project_usage
 * module's date_api.inc.
 *
 * This plugin accepts the following options:
 *   startstamp:
 *     A Unix timestamp specifying the start of the sample period.  Defaults
 *     to the last sample time for the metric if provided, or to what would
 *     have been the last sample time if none exists.  Can also be passed as
 *     a string in any form that http://php.net/strtotime accepts.
 *   endstamp:
 *     A Unix timestamp specifying the end of the sample period.  Defaults
 *     to the current time.  Can also be passed as a string in any form that
 *     http://php.net/strtotime accepts.
 *   time_unit:
 *     Unit of time used to sample for metrics.  The strings 'day' and 'week'
 *     are supported, or for a custom time unit, pass an integer with the
 *     number of seconds in the unit.  Defaults to week.
 *   time_unit_interval:
 *     An integer representing the interval to count the time unit by, eg. if
 *     time_unit is week, and time_unit_interval is 2, sampling period would be
 *     every two weeks.  Defaults to 1.
 *   single_sample:
 *     Boolean.  If TRUE, all time_unit and time_unit_interval settings are
 *     ignored, and startstamp and endstamp are treated as the beginning
 *     and end of a single sample period.
 *   abort_timestamp:
 *     Unix timestamp.  If the last sample timestamp is greater than this value,
 *     then abort the entire sample set.  Defaults to the current time.
 *
 * This plugin passes the following additional properties to sample objects:
 *   sample_startstamp:
 *     Unix timestamp representing the beginning of the sample time range.
 *   sample_endstamp:
 *     Unix timestamp representing the end of the sample time range.
 */

$plugin = array(
  'handler' => array(
    'class' => 'SamplerMethodHandlerPeriodic',
  ),
);
