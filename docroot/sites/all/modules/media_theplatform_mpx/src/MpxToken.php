<?php

/**
 * Class MpxToken
 *
 * @todo How can we respect the token lifetime and idle_timeout values separately?
 */
class MpxToken {

  /**
   * Maximum possible token TTL is one week, expressed in seconds.
   *
   * @todo Should this value be lower?
   *
   * @var int
   */
  const MAX_TTL = 604800;

  /**
   * The account username linked to the token.
   *
   * @var string
   */
  public $username;

  /**
   * The token string.
   *
   * @var string
   */
  public $value;

  /**
   * The UNIX timestamp of when the token expires.
   *
   * @var int
   */
  public $expire;

  /**
   * Construct an MPX token object.
   *
   * @param string $username
   *   The account username linked to the token.
   * @param string $value
   *   The token string.
   * @param int $expire
   *   The UNIX timestamp of when the token expires.
   */
  public function __construct($username, $value, $expire) {
    $this->username = $username;
    $this->value = $value;
    $this->expire = $expire;
  }

  /**
   * Load a token from the cache.
   *
   * In most cases, using MpxAccount::acquireToken() is recommended since this
   * may return an expired token object.
   *
   * @param string $username
   *   The mpx account username.
   *
   * @return MpxToken|bool
   *   The token object if available, otherwise FALSE if no token was available.
   */
  public static function load($username) {
    $tokens = &drupal_static('media_theplatform_mpx_tokens', array());
    $cid = 'token:' . $username;

    if (!isset($tokens[$cid])) {
      $tokens[$cid] = FALSE;
      if ($cache = cache_get($cid, 'cache_mpx')) {
        /** @var object $cache */
        $tokens[$cid] = new static($username, $cache->data, $cache->expire);
      }
    }

    return $tokens[$cid];
  }

  /**
   * Save the token to the cache.
   */
  public function save() {
    $tokens = &drupal_static('media_theplatform_mpx_tokens', array());
    $cid = 'token:' . $this->username;
    $tokens[$cid] = $this;
    cache_set($cid, $this->value, 'cache_mpx', $this->expire);
  }

  /**
   * Delete the token from the cache.
   */
  public function delete() {
    $tokens = &drupal_static('media_theplatform_mpx_tokens', array());
    $cid = 'token:' . $this->username;
    if ($tokens[$cid] && $tokens[$cid]->value == $this->value) {
      $tokens[$cid] = FALSE;
    }

    // Only clear from the cache if this is the same token currently stored.
    /** @var object $cache */
    if ($cache = cache_get($cid, 'cache_mpx')) {
      if ($cache->data == $this->value) {
        cache_clear_all($cid, 'cache_mpx');
      }
    }

    // If the token is still valid, expire it using the API.
    if ($this->isValid()) {
      $this->expire();
    }
  }

  /**
   * Checks if a token is valid.
   *
   * @param int $duration
   *   The number of seconds for which the token should be valid. Otherwise
   *   this will just check if the token is still valid for the current time.
   *
   * @return bool
   *   TRUE if the token is valid, or FALSE otherwise.
   */
  public function isValid($duration = NULL) {
    return $this->value && $this->expire > (time() + $duration);
  }

  /**
   * Fetch a fresh authentication token using thePlatform API.
   *
   * In most cases, using MpxAccount::acquireToken() is recommended since this
   * does not save the token to the cache.
   *
   * @param string $username
   *   The mpx account username.
   * @param string $password
   *   The mpx account password.
   * @param int $duration
   *   The number of seconds for which the token should be valid.
   *
   * @return MpxToken
   *   The token object if a token was fetched.
   *
   * @throws MpxApiException
   */
  public static function fetch($username, $password, $duration = NULL) {
    if (!isset($duration)) {
      $duration = variable_get('media_theplatform_mpx__token_ttl');
    }

    $query = array(
      'schema' => '1.0',
      'form' => 'json',
    );
    if (!empty($duration)) {
      // API expects this value in milliseconds, not seconds.
      $query['_duration'] = $duration * 1000;
      $query['_idleTimeout'] = $duration * 1000;
    }

    $data = MpxApi::request(
      'https://identity.auth.theplatform.com/idm/web/Authentication/signIn',
      $query,
      array(
        'headers' => array(
          'Authorization' => 'Basic ' . base64_encode($username . ':' . $password),
        ),
      )
    );

    $lifetime = floor(min($data['signInResponse']['duration'], $data['signInResponse']['idleTimeout']) / 1000);
    $token = new MpxToken(
      $username,
      $data['signInResponse']['token'],
      REQUEST_TIME + $lifetime
    );
    watchdog('media_theplatform_mpx', 'Fetched new mpx token @token for account @username that expires on @date (in @duration).', array('@token' => $token->value, '@username' => $username, '@date' => format_date($token->expire, 'custom', DATE_ISO8601), '@duration' => format_interval($lifetime)), WATCHDOG_INFO);
    return $token;
  }

  /**
   * Expire an authentication token using thePlatform API.
   *
   * In most cases, using MpxAccount::releaseToken() is recommended instead.
   * This method only interacts with the thePlatform API and does not delete
   * the token from the cache.
   *
   * @throws MpxApiException
   */
  public function expire() {
    MpxApi::request(
      'https://identity.auth.theplatform.com/idm/web/Authentication/signOut',
      array(
        'schema' => '1.0',
        'form' => 'json',
        '_token' => $this->value,
      )
    );

    watchdog('media_theplatform_mpx', 'Expired mpx authentication token @token for account @account.', array('@token' => $this->value, '@account' => $this->username), WATCHDOG_INFO);
    $this->value = NULL;
    $this->expire = NULL;
  }

  /**
   * @return string
   */
  public function __toString() {
    return $this->value;
  }
}
