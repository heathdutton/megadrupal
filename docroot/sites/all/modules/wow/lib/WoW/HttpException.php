<?php

/**
 * @file
 * Definition of WoW\HttpException.
 */

/**
 * Defines the WoW\HttpException class.
 */
class WoWHttpException extends Exception {

  public function __construct(WoWHttpStatus $status) {
    parent::__construct($status->getReason(), $status->getCode());
  }
}
