<?php
/**
 * @file
 * Contains \InspectTrace.
 */


/**
 * Don't use this class any more. It won't exist in next version.
 *
 * @deprecated
 */
class InspectTrace extends Inspect {

  /**
   * Use Inspect::trace() instead.
   *
   * @deprecated
   *
   * @see Inspect::trace()
   *
   * @param \Exception|falsy $exception
   *   Default: NULL (~ create new backtrace).
   *   Exception: trace that.
   * @param mixed $options
   *   Default: NULL.
   *   array|object: list of options.
   *   integer: interprets to severity.
   *   string: interprets to message.
   *
   * @return boolean|NULL
   *   NULL: user isnt permitted to log inspections.
   *   FALSE: on error.
   */
  public static function log($exception = NULL, $options = NULL) {
    // @todo: Remove this function/method from version > 7.x-6.0.
    watchdog(
      'inspect trace',
      'Use Inspect::trace() instead - class InspectTrace is obsolete since Inspect v. 7.x-6.0, and won\'t exist in next version.',
      NULL,
      WATCHDOG_ERROR
    );

    return static::trace($exception, $options);
  }

  /**
   * Use Inspect::traceFile() instead.
   *
   * @deprecated
   *
   * @see Inspect::traceFile()
   *
   * {@inheritdoc}
   */
  public static function file($exception = NULL, $options = NULL) {
    // @todo: Remove this function/method from version > 7.x-6.0.
    watchdog(
      'inspect trace',
      'Use Inspect::traceFile() instead - class InspectTrace is obsolete since Inspect v. 7.x-6.0, and won\'t exist in next version.',
      NULL,
      WATCHDOG_ERROR
    );

    return static::traceFile($exception, $options);
  }

  /**
   * Use Inspect::traceGet() instead.
   *
   * @deprecated
   *
   * @see Inspect::traceGet()
   *
   * {@inheritdoc}
   */
  public static function get($exception = NULL, $options = NULL) {
    // @todo: Remove this function/method from version > 7.x-6.0.
    watchdog(
      'inspect trace',
      'Use Inspect::traceGet() instead - class InspectTrace is obsolete since Inspect v. 7.x-6.0, and won\'t exist in next version.',
      NULL,
      WATCHDOG_ERROR
    );

    return static::traceGet($exception, $options);
  }

}
