<?php
/**
 * @file
 * Defines all frequencies provided by this module.
 */

/**
 * Checks if the current time is within one day of the last run time.
 *
 * @param int $last_run
 *   The last time this content type was updated.
 *
 * @return bool
 *   Returns true if within a day otherwise false.
 */
function _random_weight_daily($last_run) {

  // Calculate different since last run.
  $period = REQUEST_TIME - $last_run;

  // Change period to days.
  $days_passed = (($period / 60) / 24);

  // If its been a day.
  return $days_passed >= 1;
}

/**
 * Checks if the current time is within one week of the last run time.
 *
 * @param int $last_run
 *   The last time this content type was updated.
 *
 * @return bool
 *   Returns true if within one week otherwise false.
 */
function _random_weight_weekly($last_run) {
  $days_in_week = 7;

  // Calculate different since last run.
  $period = REQUEST_TIME - $last_run;

  // Change period to days.
  $days_passed = (($period / 60) / 24);

  // If its been a week or not.
  return $days_passed >= $days_in_week;
}

/**
 * Checks if the current time is within 30 days of the last run time.
 *
 * @param int $last_run
 *   The last time this content type was updated.
 *
 * @return bool
 *   Returns true is within 30 days otherwise false.
 */
function _random_weight_monthly($last_run) {

  $days_in_month = 30;

  // Calculate different since last run.
  $period = REQUEST_TIME - $last_run;

  // Change period to days.
  $days_passed = (($period / 60) / 24);

  // If its been a month or not.
  return $days_passed >= $days_in_month;
}
