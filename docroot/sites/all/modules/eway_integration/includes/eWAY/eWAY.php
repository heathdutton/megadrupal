<?php

/**
 * @file
 * eWAY Rapid API 3.1
 * Requires PHP 5.3 or greater with the cURL extension
 * @see http://api-portal.anypoint.mulesoft.com/eway/api/eway-rapid-31-api/docs/
 * @version 1.0
 * @package eWAY
 * @author XiNG
 * @copyright (c) 2014, XiNG Digital
 */

namespace eWAY;

use eWAY\Adapter\Curl\CurlAdapter;
use eWAY\Adapter\Transaction;
use eWAY\Message\Request;
use eWAY\Message\RequestInterface;
use \eWAY\Message\MessageFactory;
use \eWAY\Message\MessageFactoryInterface;

class eWAY {

  private $messageFactory;

  /**
   * @var
   */
  private $adapter;

  /**
   * @param array $config
   */
  function __construct(array $config = array()) {
    $this->configureAdapter($config);
  }

  /**
   * @param $method
   * @param $payment
   * @param array $configs
   * @return \eWAY\Message\Request
   */
  public function createRequest($method, $payment, $configs = array()) {
    $request = new Request($method, $payment, $configs);
    return $request;
  }

  public function send(RequestInterface $request) {
    $transaction = new Transaction($request);
    if ($response = $this->adapter->send($transaction)) {
      return $response;
    } else {
      return FALSE;
    }
  }

  /**
   * Config adapter.
   * @param $config
   */
  private function configureAdapter(&$config) {
    if (isset($config['message_factory'])) {
      $this->messageFactory = $config['message_factory'];
    } else {
      $this->messageFactory = new MessageFactory();
    }
    if (isset($config['adapter'])) {
      $this->adapter = $config['adapter'];
    }
    else {
      $this->getDefaultAdapter();
    }
  }

  /**
   * Get default adapter.
   */
  private function getDefaultAdapter() {
    if (extension_loaded('curl')) {
      $this->adapter = new CurlAdapter($this->messageFactory);
    }
    else {
      throw new \RuntimeException('eWAY integration requires cURL.');
    }
  }
}
