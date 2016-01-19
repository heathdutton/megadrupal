<?php

namespace eWAY\Message;

interface MessageInterface {
  public function __toString();
  public function setBody($body = null);
  public function getBody();
  public function getHeaders();
  public function getHeader($header, $asArray = false);
  public function hasHeader($header);
  public function removeHeader($header);
  public function addHeader($header, $value);
  public function addHeaders(array $headers);
  public function setHeader($header, $value);
  public function setHeaders(array $headers);
}
