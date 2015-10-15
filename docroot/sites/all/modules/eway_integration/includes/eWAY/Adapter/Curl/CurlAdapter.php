<?php

namespace eWAY\Adapter\Curl;

use \eWAY\Adapter\AdapterInterface;
use \eWAY\Adapter\TransactionInterface;
use \eWAY\Message\MessageFactoryInterface;

class CurlAdapter implements AdapterInterface {
  private $curlFactory;
  private $messageFactory;

  public function __construct(MessageFactoryInterface $messageFactory,
                              array $options = array()) {
    $this->messageFactory = $messageFactory;
    $this->curlFactory = new Curl();
  }

  public function send(TransactionInterface $transaction) {
    if ($response = $transaction->getResponse()) {
      return $response;
    }

    $request = $transaction->getRequest();
    $configs = $request->getConfigs();
    $method = $request->getMethod();
    $base = $request->getBase();
    $path = $request->getPath();
    $url = $base . $path;
    $payment = $request->getPayment();

    $factory = $this->curlFactory;
    $factory->setHeader('Content-Type', 'application/json');
    if (isset($configs['auth'])) {
      $factory->setBasicAuthentication($configs['auth']['user'], $configs['auth']['pass']);
    }

    switch ($method) {
      default:
        $factory->post($url, json_encode($payment));
        break;
    }
    $factory->close();

    if ($factory->error) {
      $response = $this->messageFactory->createResponse(
        $factory->error_code,
        is_array($factory->response_headers->value()) ? $factory->response_headers->value() : array(),
        $factory->response,
        array(
          'url' => $factory->url,
        )
      );
    }
    else {
      $response = $this->messageFactory->createResponse(
        $factory->http_status_code,
        is_array($factory->response_headers->value()) ? $factory->response_headers->value() : array(),
        $factory->response,
        array(
          'url' => $factory->url,
        )
      );
    }

    $transaction->setResponse($response);
    return $transaction->getResponse();
  }
}
