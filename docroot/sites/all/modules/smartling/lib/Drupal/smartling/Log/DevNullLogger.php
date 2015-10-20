<?php

namespace Drupal\smartling\Log;

class DevNullLogger implements LoggerInterface {

  public function emergency($message, array $context = array(), $ignore_settings = FALSE) {
    return $this->log(WATCHDOG_EMERGENCY, $message, $context);
  }

  public function alert($message, array $context = array(), $ignore_settings = FALSE) {
    return $this->log(WATCHDOG_ALERT, $message, $context);
  }

  public function critical($message, array $context = array(), $ignore_settings = FALSE) {
    return $this->log(WATCHDOG_CRITICAL, $message, $context);
  }

  public function error($message, array $context = array(), $ignore_settings = FALSE) {
    return $this->log(WATCHDOG_ERROR, $message, $context);
  }

  public function warning($message, array $context = array(), $ignore_settings = FALSE) {
    return $this->log(WATCHDOG_WARNING, $message, $context);
  }

  public function notice($message, array $context = array(), $ignore_settings = FALSE) {
    return $this->log(WATCHDOG_NOTICE, $message, $context);
  }

  public function info($message, array $context = array(), $ignore_settings = FALSE) {
    return $this->log(WATCHDOG_INFO, $message, $context);
  }

  public function debug($message, array $context = array(), $ignore_settings = FALSE) {
    return $this->log(WATCHDOG_DEBUG, $message, $context);
  }

  public function log($level, $message, array $context = array(), $ignore_settings = FALSE) {
    return TRUE;
  }
}