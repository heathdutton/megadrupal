<?php

/**
 * @file
 * request command from drupal to tumblr.
 */

/**
 * Represents tumblr remote service publishing states definitions.
 */
class TumblrPublishingStates {

  const PUBLISHED = 'published';
  const DRAFT = 'draft';
  const QUEUE = 'queue';
}

/**
 * Represents tumblr remote service request message parameters.
 */
interface TumblrRequestParamsInterface {

  /**
   * Prepares class properties as http post parameters.
   *
   * @return string
   *   The request parameters formatted for an http post message.
   */
  public function toPostParms();
}

/**
 * Represents tumblr web service request message content.
 */
interface TumblrRequestInterface extends TumblrRequestParamsInterface {

  /**
   * Gets the format request message.
   *
   * @return string
   *   The format of this message.
   */
  public function getFormat();

  /**
   * Sets the format for request message.
   *
   * @param $format
   *   The format type as defined by remote tumblr service.
   *   Example: html
   */
  public function setFormat($format);

  /**
   * Gets the tumblr remote service content type.
   *
   * @return string
   *   The tumblr content type of this message.
   */
  public function getType();

  /**
   * Sets the tumblr remote service content type.
   *
   * @param $type
   *   The content type as defined by remote tumblr service.
   *   Example: regular
   */
  public function setType($type);

  /**
   * Gets the tumblr remote service publishing state.
   *
   * @return string
   *   The tumblr publishing state of this message.
   */
  public function getState();

  /**
   * Sets the tumblr remote publishing state.
   *
   * @param $state
   *   The content type as defined by remote tumblr service.
   */
  public function setState($state);

  /**
   * Gets the tumblr remote term tags.
   *
   * @return string
   *   The tumblr remote term tags.
   */
  public function getTags();

  /**
   * Sets the tumblr remote term tags.
   *
   * @param $tags
   *   The tumblr remote term tags.
   */
  public function setTags($tags);

  /**
   * Gets the properties of this message as an array.
   *
   * @return array
   *   The properties of this message.
   */
  public function toArray();
}

/**
 * Represents tumblr web service request message content.
 *
 * Default implementation of TumblrRequestInterface.
 */
class TumblrRequest implements TumblrRequestInterface {

  /**
   * Format for tumblr remote service content.
   *
   * @var string.
   *   html or plain.
   */
  protected $format;

  /**
   * Tumblr remote service content type.
   *
   * @var string.
   *   regular
   */
  protected $type;

  /**
   * Tumblr remote service publish state.
   *
   * @var string.
   *   comma delimited list of terms.
   */
  protected $state;

  /**
   * Tumblr remote tag terms.
   *
   * @var string.
   *   published
   */
  protected $tags;

  /**
   * Default constructor.
   */
  function __construct() {

  }

  public function getFormat() {
    return $this->format;
  }

  public function setFormat($format) {
    $this->format = $format;
  }

  public function getType() {
    return $this->type;
  }

  public function setType($type) {
    $this->type = $type;
  }

  public function getState() {
    return $this->state;
  }

  public function setState($state) {
    $this->state = $state;
  }

  public function getTags() {
    return $this->tags;
  }

  public function setTags($tags) {
    $this->tags = $tags;
  }

  /**
   * Implements TumblrRequestInterface::toArray().
   */
  public function toArray() {
    return array('format' => $this->format,
        'type' => $this->type,
        'state' => $this->state,
        'tags' => $this->tags,
    );
  }

  /**
   * Implements TumblrRequestParamsInterface::toPostParms().
   */
  public function toPostParms() {
    $str = '&type=' . $this->type;
    if (isset($this->state)) {
      $str .= '&state=' . $this->state;
    }

    if (isset($this->format)) {
      $str .= '&format=' . $this->format;
    }

    if (isset($this->tags)) {
      $str .= '&tags=' . $this->tags;
    }
    return $str;
  }

}

/*
 * Represents tumblr request message tumblr regular content type.
 *
 *
 * Note tumblr defines different types of command requests
 * this module supports the tumblr "regular" request type, thus our class
 * name is "regular"
 */
class TumblrRequestRegular extends TumblrRequest {

  /**
   * Tumblr remote service content title property.
   *
   * @var string.
   */
  protected $title;

  /**
   * Tumblr remote service content body property.
   *
   * @var string.
   */
  protected $body;


  function __construct() {

  }

  public function getTitle() {
    return $this->title;
  }

  public function setTitle($title) {
    $this->title = $title;
  }

  public function getBody() {
    return $this->body;
  }

  public function setBody($body) {
    $this->body = $body;
  }

  /**
   * Implements TumblrRequestInterface::toArray().
   */
  public function toArray() {
    $instance_array = array(
        'type' => $this->type,
        'format' => $this->format,
        'type' => $this->type,
        'state' => $this->state,
        'title' => $this->title,
        'body' => $this->body);
    return $instance_array;
  }

  /**
   * Implements TumblrRequestParamsInterface::toPostParms().
   */
  public function toPostParms() {
    $str = parent::toPostParms();
    if (isset($this->title)) {
      $str .= '&title=' . $this->title;
    }

    if (isset($this->body)) {
      $str .= '&body=' . $this->body;
    }
    return $str;
  }

}

/*
 * Represents a request command message bound for the remote tumblr service.
 */
interface TumblrRequestCommandInterface extends TumblrRequestParamsInterface {

  /**
   * Gets the tumblr remote service unique content identifier.
   *
   * @return integer
   *   The tumblr remote service unique content identifier.
   */
  public function getPostId();

  /**
   * Sets the tumblr remote service unique content identifier.
   *
   * @param $postId
   *   The tumblr remote service unique content identifier.
   */
  public function setPostId($postId);

  /**
   * Gets the request message content property.
   *
   * @return object
   *   The request content.
   */
  public function getRequest();

   /**
   * Sets the request message content property.
   *
   * @param object $request
   *   The request message content.
   */
  public function setRequest($request);
}

/*
 * Represents a request command message bound for the remote tumblr service.
 *
 * Default implementation of TumblrRequestCommandInterface.
 */

class TumblrRequestCommand implements TumblrRequestCommandInterface {

  /**
   * Tumblr remote content identifier.
   *
   * @var string.
   *   regular
   */
  protected $postId;

  /**
   * Request message property..
   *
   * @var TumblrRequest
   *
   */
  protected $request;

  public function getPostId() {
    return $this->postId;
  }

  public function setPostId($postId) {
    $this->postId = $postId;
  }

  public function getRequest() {
    return $this->request;
  }

  public function setRequest($request) {
    $this->request = $request;
  }

  /**
   * Implements TumblrRequestParamsInterface::toPostParms().
   */
  public function toPostParms() {
    $str = NULL;
    // some commands like delete don't need a request message
    if (isset($this->request)) {
      $str = $this->request->toPostParms();
    }

    if (isset($this->postId)) {
      $str .= '&post-id=' . $this->postId;
    }
    return $str;
  }

}
