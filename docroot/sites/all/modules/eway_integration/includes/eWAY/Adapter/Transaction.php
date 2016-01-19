<?php

namespace eWAY\Adapter;

use eWAY\Message\RequestInterface;
use eWAY\Message\ResponseInterface;

class Transaction implements TransactionInterface {
  /**
   * @var RequestInterface
   */
  private $request;
  /**
   * @var ResponseInterface
   */
  private $response;

  public function __construct(RequestInterface $request) {
    $this->request = $request;
  }

  public function getRequest() {
    return $this->request;
  }

  public function getResponse() {
    return $this->response;
  }

  public function setResponse(ResponseInterface $response) {
    $this->response = $response;
  }
}
