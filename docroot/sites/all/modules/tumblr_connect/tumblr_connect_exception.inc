<?php


/**
  * @file
  * exception definitions.
 */

/*
 * Represents base exception for this module.
 */
interface TumblrConnectExceptionInterface {
}

/*
 * Represents base exception for this module.
 *
 * Default implementation of TumblrConnectExceptionInterface.
 */
class TumblrConnectException extends Exception implements TumblrConnectExceptionInterface {

}


/*
 * Represents connection exception.
 */
interface TumblrConnectRemoteExceptionInterface extends TumblrConnectExceptionInterface {

}

/*
 * Represents connection exception.
 *
 * Default implementation of TumblrConnectExceptionInterface.
 */

class TumblrConnectTypeException implements TumblrConnectExceptionInterface {

}

/*
 * Represents exceptions returned by remote tumblr service.
 */
class TumblrConnectRemoteException implements TumblrConnectRemoteExceptionInterface {

  protected $severity;

  function __construct() {
  }

  public function getSeverity() {
    return $this->severity;
  }

  public function setSeverity($severity) {
    $this->severity = $severity;
  }

}

/*
 * Represents database exceptions for this module.
 */
interface TumblrConnectDatabaseExceptionInterface extends TumblrConnectExceptionInterface {

}

/*
 * Represents database exceptions for this module.
 *
 * Default implementation of TumblrConnectDatabaseExceptionInterface.
 */
class TumblrConnectDatabaseException extends Exception implements TumblrConnectDatabaseExceptionInterface {

}

/**
 * Represents no record found exception for this module.
 */
class TumblrConnectDatabaseNoRecordException extends TumblrConnectDatabaseException {

}

/*
 * Represents missing values and parameters for this module.
 */
class TumblrConnectMissingElementException extends TumblrConnectException {

  protected $element;

  function __construct($element = NULL) {
    if (isset($element)) {
      $this->element = $element;
    }
  }

}