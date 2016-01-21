<?php

/**
 * @file
 * Base class for all data response classes.
 *
 * These classes function as a container for JSON data used by game clients.
 */

class DrpgDataResponse {
  public $success;
  public $timestamp;

  /**
   * Class constructor.
   */
  public function __construct() {
    $this->timestamp = time();
  }
}
