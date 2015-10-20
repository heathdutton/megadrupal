<?php
/**
 * @file
 * Helper class to manage watchdog and commandline messaging of migrations.
 */

class MigrationMessage {
  /**
   * Logs a system message and outputs it to drush terminal if run from drush.
   *
   * @param string $message
   *   The message to store in the log. Keep $message translatable
   *   by not concatenating dynamic values into it! Variables in the
   *   message should be added by using placeholder strings alongside
   *   the variables argument to declare the value of the placeholders.
   *   See t() for documentation on how $message and $variables interact.
   * @param array $variables
   *   Array of variables to replace in the message on display or
   *   NULL if message is already translated or not possible to
   *   translate.
   * @param int $severity
   *   The severity of the message; one of the following values as defined in
   *   - WATCHDOG_EMERGENCY: Emergency, system is unusable.
   *   - WATCHDOG_ALERT: Alert, action must be taken immediately.
   *   - WATCHDOG_CRITICAL: Critical conditions.
   *   - WATCHDOG_ERROR: Error conditions.
   *   - WATCHDOG_WARNING: Warning conditions.
   *   - WATCHDOG_NOTICE: (default) Normal but significant conditions.
   *   - WATCHDOG_INFO: Informational messages.
   *   - WATCHDOG_DEBUG: Debug-level messages.
   *   - FALSE: Outputs the message to drush without calling Watchdog.
   *
   * @param int $indent
   *   (optional). Sets indentation for drush output. Defaults to 1.
   *
   * @link http://www.faqs.org/rfcs/rfc3164.html RFC 3164: @endlink
   */
  public static function makeMessage($message, $variables = array(), $severity = WATCHDOG_NOTICE, $indent = 1) {
    // Determine what instantiated this message.
    $trace = debug_backtrace();
    if (isset($trace[1])) {
      // $trace[1] is the thing that instantiated this message.
      if (!empty($trace[1]['class'])) {
        $type = $trace[1]['class'];
      }
      elseif (!empty($trace[1]['function'])) {
        $type = $trace[1]['function'];
      }
      else {
        $type = 'unknown';
      }
    }
    if ($severity !== FALSE) {
      watchdog($type, $message, $variables, $severity);
    }
    // Check to see if this is run by drush and output is desired.
    if (drupal_is_cli() && variable_get('migration_tools_drush_debug', FALSE)) {
      $formatted_message = format_string($message, $variables);
      drush_print($type . ': ' . $formatted_message, $indent);
      if ((variable_get('migration_tools_drush_stop_on_error', FALSE)) && ($severity <= WATCHDOG_ERROR) && $severity !== FALSE) {
        throw new MigrateException("$type: Stopped for debug.\n -- Run \"drush mi {migration being run}\" to try again. \n -- Run \"drush vset migration_tools_drush_stop_on_error FALSE\" to disable auto-stop.");
      }
    }
  }

  /**
   * Outputs a visual separator using the message system.
   */
  public static function makeSeparator() {
    self::makeMessage("------------------------------------------------------", array(), FALSE, 0);
  }

  /**
   * Dumps a var to drush terminal if run by drush.
   *
   * @param mixed $var
   *   Any variable value to output.
   * @param string $var_id
   *   An optional string to identify what is being output.
   */
  public static function varDumpToDrush($var, $var_id = 'VAR DUMPs') {
    // Check to see if this is run by drush and output is desired.
    if (drupal_is_cli() && variable_get('migration_tools_drush_debug', FALSE)) {
      drush_print("{$var_id}: \n", 0);
      if (!empty($var)) {
        drush_print_r($var);
      }
      else {
        drush_print("This variable was EMPTY. \n", 1);
      }
    }
  }
}
