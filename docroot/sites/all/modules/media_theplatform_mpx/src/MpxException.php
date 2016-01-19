<?php

class MpxException extends Exception {

  /**
   * Set the exception message.
   *
   * @param string $message
   */
  public function setMessage($message){
    $this->message = $message;
  }

}
