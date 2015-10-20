<?php

namespace eWAY\Adapter;

use eWAY\Message\RequestInterface;
use eWAY\Message\ResponseInterface;

interface TransactionInterface {
  /**
   * @return RequestInterface
   */
  public function getRequest();
  /**
   * @return ResponseInterface|null
   */
  public function getResponse();
  /**
   * Set a response on the transaction
   *
   * @param ResponseInterface $response Response to set
   */
  public function setResponse(ResponseInterface $response);
}