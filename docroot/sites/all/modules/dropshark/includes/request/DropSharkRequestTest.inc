<?php

/**
 * Class DropSharkRequestTest.
 */
class DropSharkRequestTest extends DropSharkRequestDefault {

  /**
   * {@inheritdoc}
   */
  protected function execute($url, $options = array()) {
    $options = $this->setAuth($options);

    if (isset($options['method']) && $options['method'] == 'POST') {
      return $this->handlePost($url, $options);
    } else {
      return $this->handleGet($url, $options);
    }

  }

  /**
   * Route GET requests.
   *
   * @param $url
   * @param $options
   *
   * @return \stdClass
   */
  protected function handleGet($url, $options) {

    $parts = parse_url($url);
    $result = new stdClass();
    $result->code = 200;

    switch ($parts['path']) {
      case '/sites/token':
        return $this->getSiteToken($options, $result);
        break;
    }

    $result->code = 404;
    $result->error = 'Not Found';

    return $result;
  }

  /**
   * Route POST requests.
   *
   * @param $url
   * @param $options
   *
   * @return \stdClass
   */
  protected function handlePost($url, $options) {
    $parts = parse_url($url);
    $result = new stdClass();
    $result->code = 200;

    if (!empty($options['data'])) {
      parse_str($options['data'], $options['data']);
    } else {
      $options['data'] = array();
    }

    switch ($parts['path']) {
      case '/data':
        return $this->postData($options, $result);
        break;
      case '/sites/token':
        return $this->postSiteToken($options, $result);
        break;
    }

    $result->code = 404;
    $result->error = 'Not Found';

    return $result;
  }

  /**
   * Simulate a request submit site data.
   */
  protected function postData($options, $result) {
    if (empty($options['headers']['Authorization']) || $options['headers']['Authorization'] != 'valid_token') {
      $result->code = 400;
      $result->error = 'Bad Request';
      return $result;
    }

    $data = new stdClass();
    $result->data = $data;
    return $result;
  }

  /**
   * Simulate a request to verify a site token.
   */
  protected function getSiteToken($options, $result) {
    if (empty($options['headers']['Authorization']) || $options['headers']['Authorization'] != 'valid_token') {
      $result->code = 400;
      $result->error = 'Bad Request';
      return $result;
    }

    $data = new stdClass();
    $data->site_id = '0bb250ce-ffd5-4563-81fa-c8a165cd6af8';
    $result->data = $data;
    return $result;
  }

  /**
   * Simulate a request to obtain a site token.
   */
  protected function postSiteToken($options, $result) {
    if (empty($options['data']['user']) || empty($options['data']['password']) || empty($options['data']['site_id'])) {
      $result->code = 400;
      $result->error = 'Bad Request';
      return $result;
    }

    if ($options['data']['user'] != 'test_user' || $options['data']['password'] != 'password' ||  $options['data']['site_id'] != '0bb250ce-ffd5-4563-81fa-c8a165cd6af8') {
      $result->code = 403;
      $result->error = 'Forbidden';
      return $result;
    }

    $data = new stdClass();
    $data->token = 'valid_token';
    $result->data = $data;
    return $result;
  }

}
