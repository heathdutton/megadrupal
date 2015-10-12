<?php

/**
 * @file
 * Wrapper around Mailin API
 */

use Mailin\API;
use Mailin\Log;

/**
 * The Drupal wrapper for the Mailin library.
 */
abstract class DrupalMailin {

  /**
   * The mailin API instance.
   *
   * @var API
   */
  static public $mailinInstance;

  /**
   * Disallow instanciation.
   */
  protected function __construct() {}

  /**
   * Get the Mailin API.
   *
   * @return API
   *   The Mailin API instance.
   */
  public static function get() {
    if (!isset(self::$mailinInstance)) {
      self::$mailinInstance = new API(variable_get('mailin_api_key', ''), new MailinRequestDrupal());
    }

    return self::$mailinInstance;
  }

  /**
   * Get the last Mailin call status.
   *
   * @return boolean
   *   TRUE if the call to the Web services was succesfull and accepted.
   */
  public static function getLastOperationStatus() {
    return Log::getLastOperationStatus();
  }

}
