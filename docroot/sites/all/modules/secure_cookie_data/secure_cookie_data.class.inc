<?php
/**
 * @file   secure_cookie.class.inc
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Wed Dec 21 03:17:53 2011
 *
 * @brief Class implementation of the generator and validator functions for
 *        the secure cookie protocol.
 *
 * @see   http://www.cs.utexas.edu/~gouda/papers/conference/cookie.pdf.
 *
 */

// This class could be used out of a Drupal context. In that case we need to
// define the REQUEST_TIME constant.
if (!defined('REQUEST_TIME')) {
  define('REQUEST_TIME', $_SERVER['REQUEST_TIME']);
}

class secureCookieBasic {

  // This is the secret key that it will be shared with the server.
  static public $__secret = '9704da10e0cdc0481fc51d323665a25fdd1a487faed2fa4a9eadb2a9c94a6164';

  // The hashing algorithm to be used.
  static protected $__algorithm = 'tiger160,3';

  // The default cookie path.
  static public $__cookie_path = '/';

  // The default cookie name.
  static public $__cookie_name = 'SecureDataCookie';

  // The cookie duration.
  static public $__cookie_duration = 0;

  /**
   * Predicate to check if the current session is secure or not.
   *
   * @return boolean
   *   TRUE if it is, FALSE if not.
   */
  static public function secure_session_p() {
    return !!(ini_get('session.cookie_secure') === '1');
  }

  /**
   * Get the current cookie domain.
   *
   * @return string
   *   The cookie domain.
   */
  static public function get_domain() {
    return ini_get('session.cookie_domain');
  }

  /**
   * Returns the secret that is used as a key for the HMAC.
   *
   * @return string
   *   The secret to be used as a key for the HMAC.
   */
  static public function get_secret() {
    // Check to see if the the drupal_hash_salt is defined: Which is always
    // for a Drupal site. The class can be used outside of a Drupal context
    // nevertheless.
    return function_exists('drupal_get_hash_salt') ? sprintf('%sXYX%s', static::$__secret, drupal_get_hash_salt()) : static::$__secret;
  }

  /**
   * Generates the ciphered hash for the given data, algorithm and secret key.
   *
   * @param object|array $data
   *   The data that constitutes the message to be digested.
   * @return string
   *   The message digest in hexadecimal.
   */
  static public function set_hmac($data = NULL) {
    // Compute the HMAC.
    return hash_hmac(static::$__algorithm,
                     $data,
                     hash_hmac(static::$__algorithm, $data, static::get_secret()));
  }

  /**
   * Verifies the message digest for the given data, algorithm and secret key.
   *
   * @param $hmac string
   *   The message digest that is going to be verified against the data.
   * @param object $data
   *   The data that constitutes the message to be digested.
   * @return boolean
   *   TRUE if it validates, FALSE if not.
   */
  static public function validate($hmac, $data = NULL) {
    return self::set_hmac(json_encode($data)) === $hmac;
  }

  /**
   * Encode the data as a safe string to be in a cookie.
   *
   * @param object $data
   *   The data object to be encoded.
   * @return string
   *   The encoded data serialized.
   */
  static public function encode($data = NULL) {
    return base64_encode(json_encode($data));
  }

  /**
   * Decodes the data as stored in the cookie.
   *
   * @param string $data
   *   The data as stored in the cookie.
   * @return object
   *   The object
   */
  static public function decode($data = NULL) {
    return json_decode(base64_decode(urldecode($data)));
  }

  /**
   * Setter for the class.
   *
   * @param array|object $data
   *   The data to be stored in the cookie.
   * @return boolean
   *   TRUE if the cookie was modified/set, FALSE if not.
   */
  static public function set($data) {

    // Decode the given data.
    $cookie_data = (object) $data;
    // Compute the HMAC right away.
    $cookie_data->hmac = secureCookieBasic::set_hmac(json_encode($cookie_data));
    // Encode the data.
    $encoded_data = secureCookieBasic::encode($cookie_data);
    // Set the cookie.
    if (setcookie(static::$__cookie_name,
                  $encoded_data,
                  static::$__cookie_duration,
                  static::$__cookie_path,
                  secureCookieBasic::get_domain(),
                  secureCookieBasic::secure_session_p(),
                  TRUE)) {
      // Set the superglobal if the cookie setting was successful.
      $_COOKIE[static::$__cookie_name] = $encoded_data;
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Getter for the class.
   *
   * @return object
   *   The stored data or NULL if the cookie is missing
   *   or the HMAC doesn't validate.
   */
  static public function get() {

    if (isset($_COOKIE[static::$__cookie_name])) {
      // Decode the JSON.
      $cookie_data = secureCookieBasic::decode($_COOKIE[static::$__cookie_name]);

      // Next we get the HMAC.
      if (isset($cookie_data->hmac)) {
        $hmac = $cookie_data->hmac;
        // Unset the hmac entry so that we get the 'raw' data only.
        unset($cookie_data->hmac);
        // Validate the cookie using the HMAC stored on the data.
        if (secureCookieBasic::validate($hmac, $cookie_data)) {
          // Return the data.
          return $cookie_data;
        }
      }
    }
    return NULL;
  }

  /**
   * Destroyer for the class. Deletes the data cookie if it exists.
   *
   * @return boolean
   *   TRUE if the cookie is set, FALSE otherwise.
   */
  static public function delete() {
    // Check if the cookie is set.
    if (isset($_COOKIE[static::$__cookie_name])) {
      // Unset it at once.
      unset($_COOKIE[static::$__cookie_name]);
      // Create a cookie with an lifetime in the past.
      return setcookie(static::$__cookie_name,
                       secureCookieBasic::encode($cookie_data),
                       REQUEST_TIME - 3600,
                       static::$__cookie_path,
                       secureCookieBasic::get_domain(),
                       secureCookieBasic::secure_session_p(),
                       TRUE);
    }
    return FALSE;
  }
}

