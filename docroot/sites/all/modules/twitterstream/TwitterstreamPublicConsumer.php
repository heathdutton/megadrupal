<?php

/**
 * Consume tweet data from the Steaming API and store in the database.
 */
class TwitterstreamPublicConsumer extends Phirehose {

  protected $consumer;
  protected $token;

  protected $signatureMethod;

  /**
   * Create a new Streaming API consumer.
   */
  public function __construct() {
    $this->signatureMethod = new OAuthSignatureMethod_HMAC_SHA1();

    parent::__construct(null, null, Phirehose::METHOD_FILTER);
  }

  /**
   * Set the application consumer key and secret for OAuth access.
   * @param string $key
   * @param string $secret
   */
  public function setConsumerKey($key, $secret) {
    $this->consumer = new OAuthConsumer($key, $secret);
  }

  /**
   * Set the Twitter account token and secret for OAuth access.
   *
   * @param string $token
   * @param string $secret
   */
  public function setAccessToken($token, $secret) {
    $this->token = new OAuthConsumer($token, $secret);
  }

  /**
   * Set the minimum period between writing status updates to the log.
   *
   * @param int $value
   *   Number of seconds
   */
  public function setAvgPeriod($value = 60) {
    $this->avgPeriod = $value;
  }

  /**
   * Set the minimum period between checking for changes to the filter
   * predicates.
   *
   * The stream is only updated at most every 120 seconds, even if this period
   * is shorter.
   *
   * @param int $value
   *   Number of seconds
   */
  public function setFilterCheckMin($value = 5) {
    $this->filterCheckMin = $value;
  }

  /**
   * Put the provided raw status in the processing queue.
   *
   * @see Phirehose::enqueueStatus()
   */
  public function enqueueStatus($status) {
    DrupalQueue::get('twitterstream_process')
      ->createItem($status);
  }

  /**
   * Fetch the keywords to track and users to follow.
   *
   * @see Phirehose::checkFilterPredicates()
   */
  public function checkFilterPredicates() {

    // TODO Connecting to the Streaming API will fail if neither parameter is
    // specified (causing the daemon to exit after several retries), so sleep
    // and check again if nothing is available?

    $track = array();
    $follow = array();

    $result = Database::getConnection()->query("SELECT params FROM {twitterstream_params}");
    foreach ($result as $row) {
      $params = unserialize($row->params);
      if (!empty($params['track'])) {
        $track = array_merge($track, $params['track']);
      }
      if (!empty($params['follow'])) {
        $follow = array_merge($follow, $params['follow']);
      }
    }

    $this->setTrack($track);
    $this->setFollow($follow);
  }

  /**
   * Connects to the stream URL using the configured method.
   *
   * Phirehose::connect() only attempts to resolve the IP address of the
   * Streaming API endpoint once, so this method catches the exception and
   * attempts to connect again after a delay.
   *
   * @throws ErrorException
   *
   * @see Phirehose::connect()
   */
  protected function connect() {
    $networkFailures = 0;

    while (true) {
      try {
        parent::connect();
        return;
      }
      catch (PhirehoseNetworkException $ex) {

        $networkFailures++;

        if ($networkFailures >= 5) {
          throw $ex;
        }

        $this->log($ex->getMessage());

        sleep(pow(2, $networkFailures));
      }
    }
  }

  protected function getAuthorizationHeader() {
    $url = self::URL_BASE . $this->method . '.' . $this->format;

    // Setup params appropriately
    $requestParams = array('delimited' => 'length');

    // Filter takes additional parameters
    if (count($this->trackWords) > 0) {
      $requestParams['track'] = implode(',', $this->trackWords);
    }
    if (count($this->followIds) > 0) {
      $requestParams['follow'] = implode(',', $this->followIds);
    }

    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, 'POST', $url, $requestParams);
    $request->sign_request($this->signatureMethod, $this->consumer, $this->token);

    $header = $request->to_header();
    // Strip off "Authorization: ", since Phirehose prepends it itself.
    return substr($header, 15);
  }

  /**
   * Pass log messages through to System_Daemon::log().
   *
   * @param string $message
   * @param string $level
   */
  public function log($message, $level = 'notice') {
    System_Daemon::log(System_Daemon::LOG_INFO, 'Phirehose: ' . $message);
  }
}
