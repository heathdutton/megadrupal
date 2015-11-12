<?php

/**
 * @file Contains class iDEALMethodNotImplementedxception.
 */

/**
 * Exception thrown if an iDEAL controller has not implemented (overriden) an
 * iDEALPaymentMethodController method.
 */
class iDEALMethodNotImplementedException extends PaymentException {

  function __construct($message = '', $code = 0, Exception $previous = NULL) {
    if (!$message) {
      $message = t('This method should be implemented by the child class.');
    }
    parent::__construct($message, $code, $previous);
  }
}