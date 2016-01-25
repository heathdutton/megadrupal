<?php

class DcycleEnvironment {
  static function Exec($command, &$lines = array(), &$result = NULL) {
    if (self::ExecAvailable()) {
      return exec($command, $lines, $result);
    }
    else {
      throw new Exception(t('exec() function is not available.'));
    }
  }

  /**
   * Checks if a command is available on the system.
   *
   * @param $command
   *   A command to check for, for example phantomjs
   * @param &$error = NULL
   *   If provided, return some text explaining the error if there is one.
   * @param $check_command = NULL.
   *   If typing the command gives you an interactive shell and another command
   *   should be used instead, enter it here. For example typing phantomjs into
   *   exec() enters an interactive shell, so "phantomjs -v" should be used instead
   *   for checking. For example, enter " -v" here
   */
  static function CommandAvailable($command, &$error = NULL, $check_command = '') {
    $path = self::CommandPath($command);
    if (in_array($code = self::ExecResult($path . $check_command), array(127))) {
      $error = t('The command @command was not found (exit code @code) at path @path; if the command exists on your system, consider running drush vset dcycle_command_path_@command /path/to/@command', array('@command' => $command, '@code' => $code, '@path' => $path));
      return FALSE;
    }
    return TRUE;
  }

  static function ExecResult($command) {
    $lines = array();
    $result = NULL;
    self::Exec($command, $lines, $result);
    return $result;
  }

  static function DrushExists() {
    return (self::ExecResult(self::CommandPath('drush')) == 0);
  }

  static function RunCommand($command, $params, &$lines = array(), &$result = NULL) {
    $executed_command = self::CommandPath($command) . ' ' . $params;
    $return = self::Exec($executed_command, $lines, $result);
    return $return;
  }

  static function DrushCommand($command, &$lines = array(), &$result = NULL) {
    return self::RunCommand('drush', $command, $lines, $result);
  }

  static function DrushCommandExists($command) {
    return self::DrushCommand('help ' . $command);
  }

  static function CommandPath($command) {
    return variable_get('dcycle_command_path_' . $command, $command);
  }

  static function ExecAvailable() {
    return function_exists('exec');
  }
}
