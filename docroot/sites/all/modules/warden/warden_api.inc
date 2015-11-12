<?php

/**
 * @file
 * The API for communicating with the Warden server application.
 */

class WardenAPI {

  /**
   * @var string
   */
  protected $wardenUrl;

  /**
   * @var string
   */
  protected $wardenPublicKey = '';

  /**
   * @var string
   */
  protected $username = '';

  /**
   * @var string
   */
  protected $password = '';

  /**
   * @param string $warden_url
   *   The URL to the server.
   * @param string $username
   *   (optional) The basic HTTP username of warden if set.
   * @param string $password
   * (optional) The basic HTTP password of warden if set.
   */
  public function __construct($warden_url, $username = '', $password = '') {
    $this->wardenUrl = $warden_url;
    $this->username = $username;
    $this->password = $password;
  }

  /**
   * Get the public key.
   */
  public function getPublicKey() {

    if (empty($this->wardenPublicKey)) {
      $result = $this->request('/public-key');
      $this->wardenPublicKey = base64_decode($result->data);
    }

    return $this->wardenPublicKey;
  }

  /**
   * Check the validity of a token sent from Warden.
   *
   * To prove a request came from the Warden application, Warden encrypts
   * the current timestamp using its private key which can be decrypted with
   * its public key. Only the true Warden can produce the encrypted message.
   * Since it is possible to reply the token, the token only lasts for
   * 20 seconds.
   *
   * @param string $encryptedRemoteToken
   *   The token sent from the warden site which has been encrypted
   *   with Warden's private key.
   *
   * @return bool
   *   TRUE if we can trust the token.
   */
  public function isValidWardenToken($encryptedRemoteToken, $timestamp) {
    $envelope = json_decode(base64_decode($encryptedRemoteToken));

    if (!is_object($envelope) || empty($envelope->time) || empty($envelope->signature)) {
      return FALSE;
    }

    $remoteTimestamp = base64_decode($envelope->time);

    if (!is_numeric($remoteTimestamp)
      || ($remoteTimestamp > $timestamp + 20)
      || ($remoteTimestamp < $timestamp - 20)) {
      return FALSE;
    }

    $result = openssl_verify($remoteTimestamp, base64_decode($envelope->signature), $this->getPublicKey());
    return $result === 1;
  }

  /**
   * Encrypt a plaintext message.
   *
   * @param mixed $data
   *   The data to encrypt for transport.
   *
   * @return string
   *   The encoded message
   *
   * @throws
   *   Exception if there is a problem
   */
  public function encrypt($data) {
    $plaintext = json_encode($data);

    $public_key = $this->getPublicKey();

    $result = openssl_seal($plaintext, $message, $keys, array($public_key));

    if ($result === FALSE || empty($keys[0]) || empty($message) || $message === $plaintext) {
      throw new Exception('Unable to encrypt a message: ' . openssl_error_string());
    }

    $envelope = (object) array(
      'key' => base64_encode($keys[0]),
      'message' => base64_encode($message),
    );

    return base64_encode(json_encode($envelope));
  }

  /**
   * Decrypt a message which was encrypted with the Warden private key.
   *
   * @param string $cypherText
   *   The encrypted text
   * @return mixed
   *   The original data
   *
   * @throws Exception
   */
  public function decrypt($cypherText) {
    $envelope = json_decode(base64_decode($cypherText));

    if (!is_object($envelope) || empty($envelope->key) || empty($envelope->message)) {
      throw new Exception('Encrypted message is not understood');
    }

    $key = base64_decode($envelope->key);
    $message = base64_decode($envelope->message);

    $decrypted = '';
    $result = openssl_open($message, $decrypted, $key, $this->getPublicKey());

    if ($result === FALSE) {
      throw new Exception('Unable to decrypt a message: ' . openssl_error_string());
    }

    return json_decode($decrypted);
  }

  /**
   * Send the site data to Warden.
   *
   * @param array $data
   * @throws Exception
   */
  public function postSiteData(array $data) {
    $encrypted_message = $this->encrypt($data);
    $this->request('/site-update', $encrypted_message);
  }

  /**
   * Send a message to warden
   *
   * @param string $path
   *   The query path including the leading slash (e.g. '/public-key')
   * @param string $content
   *   The body of the request. If this is not empty, the request is a post.
   * @return object
   *   The response object
   *
   * @throws Exception
   *   If the response status was not 200
   */
  public function request($path, $content = '') {
    $url = $this->wardenUrl . $path;

    $options = array();

    if (!empty($this->username)) {
      $options['headers']['Authorization'] = 'Basic ' . base64_encode($this->username . ':' . $this->password);
    }

    if (!empty($content)) {
      $options['data'] = $content;
      $options['method'] = 'post';
    }

    $warden_context_options = variable_get('warden_context_options', array());

    if (!empty($warden_context_options)) {
      $options['context'] = stream_context_create($warden_context_options);
    }

    $result = drupal_http_request($url, $options);

    if ($result->code != 200) {
      watchdog('warden', '@url : @code : @message', array(
        '@url' => $url,
        '@code' => $result->code,
        '@message' => $result->error,
        ), WATCHDOG_ERROR);
      throw new Exception('Unable to communicate with Warden');
    }

    return $result;
  }

}
