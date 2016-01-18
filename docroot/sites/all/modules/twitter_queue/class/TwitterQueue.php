<?php
/**
 * Class TwitterQueue
 *
 * Interface for thmOAth
 */
class TwitterQueue {
  const VERSION = '0.1.1';

  private static $api;

  public function __construct($config) {

    static::$api = new tmhOAuth($config);

    return static::$api;
  }

  public static function &api() {
    if (static::$api !== NULL) {
      return static::$api;
    }
  }

  public static function create($tweet = '') {
    $params = array(
      'status' => $tweet,
    );

    static::api()->request('POST', static::api()->url('1.1/statuses/update'), $params);

    $response['message'] = static::api()->response['response'];
    $response['code']    = static::api()->response['code'];

    return $response;
  }
}
