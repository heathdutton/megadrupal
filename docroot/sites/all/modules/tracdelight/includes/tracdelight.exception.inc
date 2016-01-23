<?php
class TracdelightImageException extends Exception {

  public $ein;

  public $imageType;


  public function __construct($message, $ein, $imageType) {

    parent::__construct($message);

    $this->ein = $ein;
    $this->imageType = $imageType;
  }
}

