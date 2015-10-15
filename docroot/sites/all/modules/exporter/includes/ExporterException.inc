<?php
/**
 * Author: Vojtěch Kusý <vojtech.kusy@gmail.com> 
 * Date: 18.7.13
 * Time: 17:10
 */

class ExporterException extends RuntimeException {

  protected $variables;
  protected $severity;

  public function __construct($message = "", $variables = array(), $severity = WATCHDOG_ERROR, $code = 0, Exception $previous = NULL) {
    parent::__construct($message, $code, $previous);
    $this->variables = $variables;
    $this->severity = $severity;
  }

  public function getVariables() {
    return $this->variables;
  }

  public function getSeverity() {
    return $this->severity;
  }
}
