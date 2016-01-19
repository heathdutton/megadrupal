<?php

namespace eWAY\Message;

class MessageFactory implements MessageFactoryInterface {
  public function createResponse(
    $statusCode,
    array $headers = array(),
    $body = NULL,
    array $options = array()
  ) {
    return new Response($statusCode, $headers, $body, $options);
  }

  public function createRequest($method, $payment, array $configs = array()) {

  }
}
