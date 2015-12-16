<?php

/**
 * @file
 * Defines the MaPS Suite Log Logger class.
 */

namespace Drupal\maps_suite\Log;

/**
* MaPS Suite Log Logger class.
*/
abstract class Logger {

  /**
   * Store the active log objects.
   *
   * @var \SplObjectStorage
   */
  private static $logs = NULL;

  /**
   * Get the logs property and instanciate it if necessary.
   *
   * @return \SplObjectStorage
   */
  protected static function getlogs() {
    if (is_null(self::$logs)) {
      self::$logs = new \SplObjectStorage();
    }

    return self::$logs;
  }

  /**
   * Attach a log object.
   *
   * @param string $type
   *   The type of log.
   *
   * @param int id
   *   The log id.
   *
   * @param boolean test
   *   Defined if it is used in test for modify the log file path.
   *
   * @return Drupal\maps_suite\Log\LogInterface
   */
  public static function attachLog($type, $id = NULL, $test = FALSE) {
    if (is_null($id)) {
      $id = microtime();
    }

    self::getlogs()->attach(new Log($type, $id, $test));
    return self::getLog($type);
  }

  /**
   * Retrieve the given log from the logger.
   *
   * @return LogInterface
   */
  public static function getLog($type = NULL) {
    if (!variable_get('maps_suite_logs_enabled', TRUE)) {
      return new Broken();
    }

    if (isset($type)) {
      foreach (self::getlogs() as $log) {
        if ($log->getType() === $type) {
          return $log;
        }
      }
    }

    return new Broken();
  }

  /**
   * Remove definitely an existing log.
   *
   * Caution: you have to use the log system properly, because any remaining reference
   * to the log instance to clear in any other memory location will cause the process
   * to fail.
   *
   * @param $type
   *   The log type to remove.
   */
  public static function clearLog($type) {
    foreach (self::$logs as $log) {
      if ($log->getType() === $type) {
        self::$logs->detach($log);
        unset($log);
      }
    }
  }

}
