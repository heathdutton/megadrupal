<?php
/**
 * @file
 * Provides generic utility functions for the Table Size module.
 */

/**
 * A collection of generic utility functions for the Table Size module.
 */
class TableSizeUtil {

  /**
   * Returns and array of cron run options.
   *
   * @return array
   */
  public static function getRunOptions() {
    return array(
      TableSizeCron::RUN_NEVER => t('never run.'),
      TableSizeCron::RUN_EVERY_TIME => t('run on every cron.'),
      TableSizeCron::RUN_BETWEEN_INTERVALS => t('run only once per day, between a specified time interval.'),
    );
  }

  /**
   * Converts a time string like 14:15 or 05:30 to a number of minutes.
   *
   * @param string $time
   *
   * @return int
   */
  public static function timeToMinutes($time) {
    if (!preg_match('/^\d{1,2}:\d{1,2}$/', $time)) {
      throw new TableSizeException(t('Invalid time string given.'));
    }
    $time = trim($time, ' 0');
    $time = explode(':', $time);
    return $time[0] * 60 + $time[1];
  }

}