<?php

/**
 * @file
 * Smartling context authorize.
 */

/**
 * Smartling context authorize.
 *
 * @return array
 *   Return response array.
 */
function smartling_translation_context_authorize($username) {
  global $user;

  $auth = new SmartlingContextAuthorize($user);
  return $auth->authorize($username);
}

/**
 * SmartlingContextAuthorize.
 */
class SmartlingContextAuthorize {
  protected $user;

  protected static $authError = array(
    'response' => array(
      'data' => NULL,
      'code' => "AUTHENTICATION_ERROR",
      "messages" => array("Bad credentials"),
    ),
  );
  protected static $validationError = array(
    'response' => array(
      'data' => NULL,
      'code' => "AUTHENTICATION_ERROR",
      "messages" => array("Bad credentials"),
    ),
  );

  /**
   * Construct.
   *
   * @param object $user
   *   User object.
   */
  public function __construct($user) {
    $this->user = $user;
  }

  /**
   * Authorize.
   *
   * @param string $name
   *   Name.
   *
   * @return array
   *   Return response array.
   */
  public function authorize($name) {
    $cookie = $this->authByName($name);
    if (!$cookie) {
      return self::$authError;
    }


    return array(
      'response' => array(
        'data' => array('cookie' => $cookie),
        'code' => "SUCCESS",
        "messages" => array(),
      ),
    );
  }

  /**
   * Auth by name.
   *
   * @param string $name
   *   Name.
   *
   * @return int|string
   *   Return result.
   */
  protected function authByName($name) {
    //$user = $this->user;
    //watchdog('smartling', print_r($_SESSION, TRUE));
    //return 'SESSeb675e98e3e4272deb136d7d8b77a083=ItnXI30Ok_B8VNdGbfpPjA5smlD6ky2nCuFqQ2IHsRk';
    global $user;

    if (isset($name) && $account = user_load_by_name($name)) {
      $session_name = session_name();
      $session_id = session_id();
      $session_value = $_SESSION;
      $params = session_get_cookie_params();
      $old_user = clone $user;
      session_write_close();

      session_id(drupal_random_key());
      session_start();

      $user = $account;
      $user->timestamp = time() - 9999;

      drupal_session_regenerate();
      drupal_session_commit();

      $credentials = session_name() . "=" . session_id();


      $user = clone $old_user;

      session_start();
      session_id($session_id);
      session_name($session_name);
      $user = $old_user;

      $expire = $params['lifetime'] ? REQUEST_TIME + $params['lifetime'] : 0;
      setcookie(session_name(), session_id(), $expire, $params['path'], $params['domain'], $params['secure'], $params['httponly']);

      $_SESSION = $session_value;

      drupal_session_commit();
      session_write_close();
      session_start();

      return $credentials;
    }
    return 0;
  }
}
