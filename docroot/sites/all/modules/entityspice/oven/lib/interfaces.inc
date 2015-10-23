<?php

interface __OvenFileInterface {
  public function __construct($file, $operation);

  public function write($content);

  public function read();
}


interface __OvenFileHandlerInterface {
  public function write($data);

  public function read();

  public function open($file);
}
