<?php

/**
 * @file
 */

interface IException {
  /*
   * Protected methods inherited from Exception class.
   */
  // Exception message.
  public function getMessage();
  // User-defined Exception code.
  public function getCode();
  // Source filename.
  public function getFile();
  // Source line.
  public function getLine();
  // An array of the backtrace().
  public function getTrace();
  // Formated string of trace.
  public function getTraceAsString();

  /*
   * Overrideable methods inherited from Exception class.
   */
  // Formated string for display.
  public function __toString();
  public function __construct($message = null, $code = 0);
}

abstract class CustomException extends Exception implements IException {
  // Exception message.
  protected $message = 'Unknown exception';
  // Unknown.
  private $string;
  // User-defined exception code.
  protected $code  = 0;
  // Source filename of exception.
  protected $file;
  // Source line of exception.
  protected $line;
  // Unknown.
  private $trace;

  public function __construct($message = null, $code = 0) {
    if (!$message) {
      throw new $this('Unknown ' . get_class($this));
    }
    parent::__construct($message, $code);
  }

  public function __toString() {
    return get_class($this) . " '{$this->message}' in {$this->file}({$this->line})\n"
      . "{$this->getTraceAsString()}";
  }
}
