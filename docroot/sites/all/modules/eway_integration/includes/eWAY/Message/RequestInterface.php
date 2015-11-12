<?php

namespace eWAY\Message;

interface RequestInterface {
  public function setMethod($method);
  public function getMethod();
  public function setSandbox($sandbox);
  public function getSandbox();
  public function setConfigs($configs);
  public function getConfigs();
  public function setBase($base);
  public function getBase();
  public function getPath();
}
