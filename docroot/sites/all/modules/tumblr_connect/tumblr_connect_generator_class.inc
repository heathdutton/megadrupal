<?php

/**
 * @file
 * generates requests to tumblr and retrieves tumblr response
 */

/*
 * Represents a request and response dispatcher.
 *
 * @interface
 *   tumblr message request and response dispatcher.
 */
interface TumblrConnectGeneratorInterface {

  /**
   * Sets an add content request command..
   *
   * @param TumblrRequestInterface $request
   *  Tumblr request content object.
   *
   * @return
   *   Response object from tunblr remote service.
   */
  public function addNew(TumblrRequestInterface $request);

  /**
   * Sets a modify content request command..
   *
   * @param TumblrRequestInterface $request
   *  Tumblr request content object.
   *
   * @return
   *   Response object from tunblr remote service.
   *
   * @throws TumblrConnectMissingElementException
   */
  public function update(TumblrRequestCommandInterface $request);

  /**
   * Sets a delete content request command..
   *
   * @param TumblrRequestInterface $request
   *  Tumblr request content object.
   *
   * @return
   *   Response object from tunblr remote service.
   *
   * @throws TumblrConnectMissingElementException
   */
  public function delete(TumblrRequestCommandInterface $request);
}

/*
 * Represents a request and response dispatcher.
 *
 * Default implementation of TumblrConnectGeneratorInterface.
 */

class TumblrConnectGenerator implements TumblrConnectGeneratorInterface {

  /**
   * Email credential required to connect to the tubmlr remote service.
   *
   * @var string.
   */
  protected $email;

  /**
   * Password credential required to connect to the tubmlr remote service.
   *
   * @var string.
   */
  protected $password;

  /**
   * String identified of generator entity as required by tubmlr remote service.
   *
   * @var string.
   */
  protected $generator;

  const POST_HTTP_HEADER_CONTENT_TYPE ='Content-Type';
  const APPLICATION_HEADER = 'application/x-www-form-urlencoded; charset=utf-8';

  const POST_METHOD = 'POST';

  function __construct() {

  }

  public function getEmail() {
    return $this->email;
  }

  public function setEmail($email) {
    $this->email = $email;
  }

  public function getPassword() {
    return $this->password;
  }

  public function setPassword($password) {
    $this->password = $password;
  }

  public function getGenerator() {
    return $this->generator;
  }

  public function setGenerator($generator) {
    $this->generator = $generator;
  }

  /**
   * Prepares message content for http post to tumblr remote service.
   *
   * @param
   *   tumblr command request
   * tumblr for some reason fails with standard post param preparation
   * therefore we perform simple markup here.
   */
  function prepareMessage(TumblrRequestParamsInterface $request) {

    $parms = 'email=' . $this->email . '&password=' . $this->password;
    $parms .= $request->toPostParms() . '&generator=' . $this->generator;

    $headers = array(
        TumblrConnectGenerator::POST_HTTP_HEADER_CONTENT_TYPE =>
        TumblrConnectGenerator::APPLICATION_HEADER
    );

    $msg_array = array('headers' => $headers,
        'method' => TumblrConnectGenerator::POST_METHOD,
        'data' => $parms,
    );
    return $msg_array;
  }

  /**
   * Implements TumblrConnectGeneratorInterface::addNew().
   */
  public function addNew(TumblrRequestInterface $request) {
    $message_array = $this->prepareMessage($request);
    $response = drupal_http_request(TumblrGateway::WRITE_POSTS, $message_array);
    return $response;
  }

  /**
   * Implements TumblrConnectGeneratorInterface::update().
   */
  public function update(TumblrRequestCommandInterface $request_command) {
    if (is_null($request_command->getPostId())) {
      throw new TumblrConnectMissingElementException('postId');
    }
    else {
      $message_array = $this->prepareMessage($request_command);
      $response = drupal_http_request(TumblrGateway::WRITE_POSTS, $message_array);
    }
    return $response;
  }

  /**
   * Implements TumblrConnectGeneratorInterface::delete().
   */
  public function delete(TumblrRequestCommandInterface $request_command) {
    if (is_null($request_command->getPostId())) {
      throw new TumblrConnectMissingElementException('postId');
    }
    else {
      $message_array = $this->prepareMessage($request_command);
      $response = drupal_http_request(TumblrGateway::DELETE_POSTS, $message_array);
    }

    return $response;
  }

}

