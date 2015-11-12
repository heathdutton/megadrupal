<?php

/**
 * @file
 * Definition of WoW\HttpStatus.
 */

/**
 * Defines the WoW\HttpStatus class.
 */
class WoWHttpStatus extends WoWHttpResponse {

  /**
   * The status message from the response, if a response was received.
   *
   * @var string
   */
  protected $reason;

  /**
   * Constructor.
   */
  public function __construct($response, $date) {
    parent::__construct($response, $date);

    if (!empty($this->data)) {
      $this->reason = $this->getArray('reason');
    }
  }

  /**
   * Return reason.
   *
   * @var string
   */
  public function getReason() {
    return $this->reason;
  }
}
