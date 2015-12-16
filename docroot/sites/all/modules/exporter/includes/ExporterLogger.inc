<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 18.7.13
 * Time: 17:11
 */

class ExporterLogger {
  public $log_level;

  function __construct($log_level = WATCHDOG_INFO) {
    $this->log_level = $log_level;
  }

  function log($message, $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL) {
    if ($severity <= $this->log_level) {
      watchdog('exporter', $message, $variables, $severity, $link);
    }
  }

  function logException(ExporterException $e, $link = NULL) {
    $this->log($e->getMessage(), $e->getVariables(), $e->getSeverity(), $link);
  }

  function error($message, $variables = array(), $link = NULL) {
    $this->log($message, $variables, WATCHDOG_ERROR, $link);
  }

  function warning($message, $variables = array(), $link = NULL) {
    $this->log($message, $variables, WATCHDOG_WARNING, $link);
  }

  function notice($message, $variables = array(), $link = NULL) {
    $this->log($message, $variables, WATCHDOG_NOTICE, $link);
  }

  function info($message, $variables = array(), $link = NULL) {
    $this->log($message, $variables, WATCHDOG_INFO, $link);
  }

  function debug($message, $variables = array(), $link = NULL) {
    $this->log($message, $variables, WATCHDOG_DEBUG, $link);
  }
}
