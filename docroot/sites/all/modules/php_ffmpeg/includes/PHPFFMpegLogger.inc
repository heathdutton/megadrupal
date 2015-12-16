<?php
/**
 * @file
 * Contains the PHPFFMpegLogger class.
 */

/**
 * Drupal watchdog to PSR logger adapter
 */
class PHPFFMpegLogger implements \Psr\Log\LoggerInterface {

  /**
   * Create a new instance of PHPFFMpegLogger.
   *
   * @param $type
   *   The type to use when logging messages to Drupal's watchdog.
   */
  public function __construct($type) {
    $this->type = (string) $type;
  }

  /*
   * {@inheritdoc}
   */
  public function emergency($message, array $context = array()) {
    $this->log(__FUNCTION__, $message, $context);
  }

  /*
   * {@inheritdoc}
   */
  public function alert($message, array $context = array()) {
    $this->log(__FUNCTION__, $message, $context);
  }

  /*
   * {@inheritdoc}
   */
  public function critical($message, array $context = array()) {
    $this->log(__FUNCTION__, $message, $context);
  }

  /*
   * {@inheritdoc}
   */
  public function error($message, array $context = array()) {
    $this->log(__FUNCTION__, $message, $context);
  }

  /*
   * {@inheritdoc}
   */
  public function warning($message, array $context = array()) {
    $this->log(__FUNCTION__, $message, $context);
  }

  /*
   * {@inheritdoc}
   */
  public function notice($message, array $context = array()) {
    $this->log(__FUNCTION__, $message, $context);
  }

  /*
   * {@inheritdoc}
   */
  public function info($message, array $context = array()) {
    $this->log(__FUNCTION__, $message, $context);
  }

  /*
   * {@inheritdoc}
   */
  public function debug($message, array $context = array()) {
    debug(__FUNCTION__);
    $this->log(__FUNCTION__, $message, $context);
  }

  /*
   * {@inheritdoc}
   */
  public function log($level, $message, array $context = array()) {
    if (is_string($level) && defined('WATCHDOG_' . strtoupper($level))) {
      $severity = constant('WATCHDOG_' . strtoupper($level));
    }
    else {
      $severity = WATCHDOG_NOTICE;
    }
    $message = preg_replace('/{([A-Za-z0-9_\.]+)}/', '@$1', $message);
    $variables = array();
    foreach ($context as $k => $v) {
      $variables['@' . $k] = $v;
    }
    watchdog($this->type, $message, $variables, $severity);
  }
}