<?php
/**
 * @file
 * FPP Bundles Exception.
 */

/**
 * Class FppBundlesException.
 */
class FppBundlesException extends \Exception {
  /**
   * Throw Drupal error and store it to "Recent log messages".
   *
   * @param string $message
   *   Exception message.
   *
   * @return self
   *   Exception, which can be thrown.
   */
  public static function set($message) {
    $exception = new self($message);

    watchdog_exception('FPP Bundles', $exception, $message);

    return $exception;
  }
}
