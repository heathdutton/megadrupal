<?php
/**
 * @file
 *
 * Watchdog logger.
 */

namespace Drupal\PSR3\Logger;

use Psr\Log\AbstractLogger;
use Psr\Log\LogLevel;

/**
 * Class Watchdog
 * @package Drupal\PSR3\Logger
 *
 * PSR3 logger using Drupal's watchdog.
 */
final class Watchdog extends AbstractLogger {

  /**
   * Singleton.
   * @var self
   */
  private static $instance;

  /**
   * @var string
   */
  private $watchdogType = __CLASS__;

  /**
   * @return \Drupal\PSR3\Logger\Watchdog
   */
  public static function getMainLogger() {
    if (empty(self::$instance)) {
      self::$instance = new self();
    }
    return self::$instance;
  }

  /**
   * @param string $watchdogType
   */
  public function setWatchdogType($watchdogType) {
    $this->watchdogType = $watchdogType;
  }

  /**
   * Logs with an arbitrary level.
   *
   * @param mixed $level
   * @param string $message
   * @param array $context
   * @return null
   */
  public function log($level, $message, array $context = array()) {
    $message = preg_replace('/\{([a-zA-Z0-9.-]{1,})\}/', '@$1', $message);
    watchdog($this->watchdogType, $message, $context, $this->translateLevel($level));
  }

  /**
   * Translate PSR3 level to Drupal watchdog level.
   *
   * @param string $psr3Level
   * @return int
   */
  private function translateLevel($psr3Level) {
    $map = array(
      LogLevel::CRITICAL => WATCHDOG_CRITICAL,
      LogLevel::ALERT => WATCHDOG_ALERT,
      LogLevel::DEBUG => WATCHDOG_DEBUG,
      LogLevel::EMERGENCY => WATCHDOG_EMERGENCY,
      LogLevel::ERROR => WATCHDOG_ERROR,
      LogLevel::INFO => WATCHDOG_INFO,
      LogLevel::NOTICE => WATCHDOG_NOTICE,
      LogLevel::WARNING => WATCHDOG_WARNING,
    );
    return array_key_exists($psr3Level, $map) ? $map[$psr3Level] : WATCHDOG_INFO;
  }

}
