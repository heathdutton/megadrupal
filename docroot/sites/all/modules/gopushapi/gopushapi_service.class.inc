<?php

/**
 * Service connector class for the GoPushAPI service.
 */
class GoPushAPI {
  /**
   * TRUE if the class is initilized.
   *
   * @var bool
   */
  private static $initialized = FALSE;

  /**
   * Initializes the class.
   *
   * Adjusts the include path to the needs of phpseclib, and loads the RSA class.
   *
   * @param $path
   *   Path to phpseclib.
   *
   * @return bool
   *   TRUE if the initialization happened, FALSE if the class is already initialized.
   */
  public static function init($path) {
    if (!self::$initialized) {
      set_include_path(trim(get_include_path(), PATH_SEPARATOR) . PATH_SEPARATOR . $path);
      require_once 'Crypt/RSA.php';
      self::$initialized = TRUE;
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Generates a key pair.
   *
   * @param int $bits
   *   Number of bits to use for the RSA key.
   *   Important to note that the recommended value is 1024. If you use a long
   *   key, it might happen that the signature gets truncated when sent as a
   *   HTTP header, which makes all request fail.
   *
   * @return array
   *   Array with two keys: 'privatekey' and 'publickey'.
   *
   * @throws GoPushAPIClassNotInitializedException
   */
  public static function generateKeyPair($bits = 1024) {
    if (!self::$initialized) {
      throw new GoPushAPIClassNotInitializedException();
    }

    $rsa = new Crypt_RSA();
    $rsa->setPrivateKeyFormat(CRYPT_RSA_PRIVATE_FORMAT_PKCS1);
    $rsa->setPublicKeyFormat(CRYPT_RSA_PUBLIC_FORMAT_PKCS1);
    return $rsa->createKey($bits);
  }

  /**
   * Server address.
   *
   * @var string
   */
  protected $server;

  /**
   * Private key.
   *
   * @var string
   */
  protected $private_key;

  /**
   * Email address to verify the user.
   *
   * @var string
   */
  protected $mail;

  /**
   * RSA class, prepared to sign requests.
   *
   * @var Crypt_RSA
   */
  protected $rsa;

  /**
   * @constructor
   *
   * @param $server
   *   Server address.
   * @param $mail
   *   Email address to identify the user.
   * @param $private_key
   *   Private key of the user.
   *
   * @throws GoPushAPIClassNotInitializedException
   *   Make sure that you call GoPushAPI::init() before constructing this class.
   * @throws GoPushAPIKeyLoadingException
   *   The given private key is invalid.
   */
  public function __construct($server, $mail, $private_key) {
    if (!self::$initialized) {
      throw new GoPushAPIClassNotInitializedException();
    }

    $this->server = rtrim($server, '/');
    $this->mail = $mail;
    $this->private_key = $private_key;

    $this->rsa = new Crypt_RSA();
    if (!$this->rsa->loadKey($private_key, CRYPT_RSA_PRIVATE_FORMAT_PKCS1)) {
      throw new GoPushAPIKeyLoadingException();
    }
    $this->rsa->setSignatureMode(CRYPT_RSA_SIGNATURE_PKCS1);
  }

  /**
   * Signs the content and sends the request.
   *
   * @param string $path
   *   Service path to call.
   * @param string $post
   *   Post content.
   * @param string|null $center
   *   The 'notify' call requires the center= GET parameter.
   *
   * @return array|string
   *   String if the http connection failed.
   *   Array with keys 'code' and 'response' if a HTTP response was received.
   */
  protected function sendRequest($path, $post, $center = NULL) {
    $request_path = "{$this->server}/{$path}?mail={$this->mail}" . ($center ? "&center={$center}" : '');
    $signature = bin2hex($this->rsa->sign($post));

    $ch = curl_init($request_path);
    curl_setopt($ch, CURLOPT_AUTOREFERER, TRUE);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    curl_setopt($ch, CURLOPT_HEADER, FALSE);
    curl_setopt($ch, CURLOPT_POST, TRUE);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Expect:', "Authorization: GoPush {$signature}"));

    $response = curl_exec($ch);
    $http_status = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    if ($err = curl_error($ch)) {
      return $err;
    }
    curl_close($ch);

    return array(
      'code' => $http_status,
      'response' => $response,
    );
  }

  /**
   * Converts $this->sendRequest() errors to exceptions.
   *
   * @param string $path
   *   Service path to call.
   * @param string $post
   *   Post content.
   * @param string|null $center
   *   Some calls requires the center= GET parameter.
   *
   * @return string
   *   The HTTP response body on success.
   *
   * @throws GoPushAPIHTTPException
   *   Non-2xx response.
   * @throws GoPushAPICurlException
   *   HTTP connection failure.
   */
  protected function requestHelper($path, $post, $center = NULL) {
    $resp = $this->sendRequest($path, $post, $center);
    if (is_array($resp)) {
      if ($resp['code'] < 300 && $resp['code'] >= 200) {
        return $resp['response'];
      }
      else {
        throw new GoPushAPIHTTPException($resp['response'], $resp['code']);
      }
    }
    else {
      throw new GoPushAPICurlException($resp);
    }
  }

  /**
   * Creates a new notification center.
   *
   * @param string $name
   *   Name of the notification center.
   *
   * @return string
   *   Absolute name of the notification center.
   */
  public function createCenter($name) {
    return $this->requestHelper('newcenter', $name);
  }

  /**
   * Removes a notification center.
   *
   * @param $name
   *   Name of the notification center.
   */
  public function removeCenter($name) {
    return $this->requestHelper('removecenter', $name);
  }

  /**
   * Broadcasts a notification to the clients.
   *
   * @param $center
   *   Name of the notification center.
   * @param $message
   *   Message to broadcast.
   */
  public function notify($center, $message) {
    return $this->requestHelper('notify', $message, $center);
  }

  /**
   * Simple echo service to ensure that the service is working correctly.
   *
   * @param $message
   *   Message to expect to receive back.
   *
   * @return string
   *   $message
   */
  public function test($message) {
    return $this->requestHelper('test', $message);
  }
}

class GoPushAPIClassNotInitializedException extends Exception {}
class GoPushAPIKeyLoadingException extends Exception {}
class GoPushAPICurlException extends Exception {}
class GoPushAPIHTTPException extends Exception {}
