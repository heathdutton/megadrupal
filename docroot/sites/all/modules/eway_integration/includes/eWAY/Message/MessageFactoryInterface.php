<?php

namespace eWAY\Message;

interface MessageFactoryInterface {
  public function createResponse(
    $statusCode,
    array $headers = array(),
    $body = null,
    array $options = array()
  );
  public function createRequest($method, $payment, array $configs = array());
}
