<?php

class MapfishPrintException extends Exception {
  public function __construct($message) {
    $this->message = $message;
  }
}

// vim: ft=php tabstop=2 shiftwidth=2
