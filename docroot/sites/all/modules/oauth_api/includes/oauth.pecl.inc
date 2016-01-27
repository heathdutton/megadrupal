<?php
// $Id$

/**
 * A class adapter implementing DrupalOAuth. Since PHP doesn't support private
 * inheritence for implementation, we are using normal inheritence.
 *
 * This adapter class is not intended to be extended.
 */
final class OAuthPECLConsumer extends OAuth implements DrupalOAuthConsumer {
  private $aid;
  private $oid;
  private $oauth;

  public static function construct($oauth, $aid = NULL) {
    $consumer = array_pop(field_get_items('oauth', $oauth, 'consumer'));
    $that = new OAuthPECLConsumer($consumer['key'], $consumer['secret'], $consumer['signature_method'], $consumer['auth_type']);
    $that->aid = $aid;
    $that->oid = $oauth->oid;
    $that->oauth = $oauth;

    if (isset($aid)) {
      $token = oauth_token_load($oauth, $aid);
      $that->setToken($token['token'], $token['secret']);
    }

    return $that;
  }

  public function checkAccess() {
    return isset($_GET['state']) && $this->getPersistentData('state') == $_GET['state'];
  }

  public function authenticate($authname, $response) {
    oauth_authenticate($this->oauth, $authname, $response, $this->aid);
  }

  public function getRequestToken($request_url, $callback = NULL) {
    // Setting up variables
    $random = md5(uniqid(rand(), TRUE)); //CSRF protection
    if (is_null($callback)) {
      $callback = 'oauth/' . $this->oid . '/callback';
    }
    $callback = url($callback, array('query' => array('state' => $random), 'absolute' => TRUE));

    try {
      $request = parent::getRequestToken($request_url, $callback);

      // Put variables in a session.
      $this->setPersistentData('secret', $request['oauth_token_secret']);
      $this->setPersistentData('state',  $random);
      return $request;
    } catch(OAuthException $e) {
      watchdog_exception('oauth', $e);
      throw $e;
    }
  }

  public function getAccessToken($access_url, $auth_session_handle = NULL) {
    try {
      $this->setToken($_GET['oauth_token'], $this->getPersistentData('secret'));
      $access = parent::getAccessToken($access_url, $auth_session_handle, $_GET['oauth_verifier']);

      // Replace the requestToken by a fresh accessToken from Provider.
      $this->setToken($access['oauth_token'], $access['oauth_token_secret']);

      $this->clearPersistentData('secret');
      $this->clearPersistentData('state');
      return $access;
    } catch (OAuthException $e) {
      watchdog_exception('oauth', $e);
      throw $e;
    }
  }

  protected function setPersistentData($key, $value) {
    $_SESSION['oauth'][$this->oid][$key] = $value;
  }

  protected function getPersistentData($key, $default = FALSE) {
    return isset($_SESSION['oauth'][$this->oid][$key]) ?
      $_SESSION['oauth'][$this->oid][$key] : $default;
  }

  protected function clearPersistentData($key) {
    unset($_SESSION['oauth'][$this->oid][$key]);
  }
}