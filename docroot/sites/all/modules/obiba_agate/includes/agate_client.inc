<?php
/**
 * @file
 * Obiba Agate Module.
 *
 * Copyright (c) 2015 OBiBa. All rights reserved.
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Class ObibaAgateClient.
 */
class ObibaAgateClient {

  const SET_COOKIE_HEADER = 'Set-Cookie';

  const OBIBA_COOKIE = 'obibaid';

  const HEADER_JSON = 'application/json';

  const AUTHORIZATION_HEADER = 'Authorization';
  const AUTHORIZATION_HEADER_AGATE = 'X-App-Auth';

  const COOKIE_HEADER = 'Cookie';

  protected $agateUrl;

  protected $applicationName;

  protected $applicationKey;

  protected $lastResponse;


  /**
   * Check if the user was authenticated by Agate.
   *
   * @return bool
   *   Return the stored Obiba Cookie.
   */
  public static function hasTicket() {
    return isset($_SESSION[self::OBIBA_COOKIE]);
  }

  /**
   * ObibaAgateClient constructor.
   *
   * If no parameters are provided, default are extracted from Drupal variables.
   *
   * @param string $agate_url
   *   The url of the Agate server.
   * @param string $application_name
   *   The application name also defined in agate (drupal).
   * @param string $application_key
   *   The key credential to allow the application(drupal) to connect
   *   to Agate server.
   */
  public function __construct($agate_url = NULL, $application_name = NULL, $application_key = NULL) {
    $this->agateUrl = (isset($agate_url) ? $agate_url : variable_get_value('agate_url')) . '/ws';
    $this->applicationName = (isset($application_name) ? $application_name : variable_get_value('agate_application_name'));
    $this->applicationKey = (isset($application_key) ? $application_key : variable_get_value('agate_application_key'));
  }

  /**
   * Get the last response (if any).
   *
   * @return mixed
   *   The last server response.
   */
  public function getLastResponse() {
    return $this->lastResponse;
  }

  /**
   * Get the last response headers (if any).
   *
   * @return array
   *   Parse and format the last response.
   */
  public function getLastResponseHeaders() {
    if ($this->lastResponse != NULL) {
      $result = array();
      foreach (explode("\r\n", $this->lastResponse->headers) as $header) {
        $h = explode(":", $header, 2);
        if (count($h) == 2) {
          if (!array_key_exists($h[0], $result)) {
            $result[$h[0]] = array();
          }
          array_push($result[$h[0]], trim($h[1]));
        }
      }
      return $result;
    }
    return '';
  }

  /**
   * Get the header value(s) from the last response.
   *
   * @param string $header_name
   *   The header name.
   *
   * @return array
   *   The value of the header'
   */
  public function getLastResponseHeader($header_name) {
    $headers = $this->getLastResponseHeaders();

    if (array_key_exists($header_name, $headers)) {
      return $headers[$header_name];
    }
    return array();
  }


  /**
   * Get form registration from agate.
   *
   * @return string
   *   A json of the form configuration.
   */
  public function getConfigFormJoin() {
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/config/join';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => array(
        'Accept' => array('application/json'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
    ));

    $client = $this->client();
    try {
      $body = $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      return json_decode($body, TRUE);
    }
    catch (HttpClientException $e) {
      watchdog('Agate Client', 'Request failed, @code: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      return array();
    }
  }

  /**
   * Get form registration from agate.
   *
   * @return string
   *   The google key of ReCaptcha configuration sorted on the agate server.
   */
  public function getReCaptchaKeyConfigFormJoin() {
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/config/client';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => array(
        'Accept' => array('application/json'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
    ));

    $client = $this->client();
    try {

      $body = $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      $response_google = json_decode($body);
      return $response_google->reCaptchaKey;
    }
    catch (HttpClientException $e) {
      watchdog('Agate Client', 'Request failed, @code: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      return array();
    }
  }

  /**
   * Append defaults user role.
   *
   * @return string
   *   The roles configuration in auto-roles parameters, to add to user.
   */
  public function appendAutoRoles($username) {
    $groups = '';
    if (!empty($username)) {
      $account = user_external_load($username);
      if (!empty($account->roles)) {
        foreach ($account->roles as $role) {
          if (!empty($role) && strstr($role, 'mica')) {
            $groups .= '&group=' . $role;
          }
        }
        if (!empty($groups) && $groups != '') {
          return $groups;
        }
      }
    }
    $db_roles = user_roles();
    foreach (variable_get('obiba_auto_assigned_role', array('mica-user')) as $group) {
      if (!empty($group) && !empty($db_roles[$group]) && strstr($db_roles[$group], 'mica')) {
        $groups .= '&group=' . $db_roles[$group];
      }
      // Default group user mica-user.
      else {
        $groups .= '&group=mica-user';
      }
    }
    return $groups;
  }

  /**
   * Add user to Agate.
   *
   * @param array $user_data
   *   The user profile to register in Agate.
   *
   * @return TRUE
   *   An error if user not added or true if OK.
   */
  public function addAgateUser(array $user_data) {
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/users/_join';
    $data = http_build_query($user_data);
    $roles = $this->appendAutoRoles(empty($user_data['username']) ? NULL : $user_data['username']);
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_POST,
      'headers' => array(
        'Accept' => array('application/json'),
        'Content-Type' => array('application/x-www-form-urlencoded'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
      'data' => $data . $roles,
    ));
    $client = $this->client();
    try {
      $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      $this->setLastResponseCookies();
      return TRUE;
    }
    catch (HttpClientException $e) {
      watchdog('Agate Client', 'Request failed, @code: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      return array(
        'codeError' => $e->getCode(),
        'message' => $e->getResponse()->body,
      );
    }
  }

  /**
   * Get form registration from agate.
   *
   * @param array $data
   *   An array key/value username/email to recover.
   *
   * @return array
   *   Server response.
   */
  public function passwordRequest(array $data) {
    $data = http_build_query($data);
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/users/_forgot_password';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_POST,
      'headers' => array(
        'Accept' => array('application/json'),
        'Content-Type' => array('application/x-www-form-urlencoded'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
      'data' => $data,
    ));
    $client = $this->client();
    try {
      $body = $client->execute($request);
      if (!empty($body)) {
        $this->lastResponse = $client->lastResponse;
      }
      return json_decode($body, TRUE);
    }
    catch (HttpClientException $e) {
      watchdog('Agate Client', 'Request failed, @code: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      return array();
    }
  }

  /**
   * Confirm user registration.
   *
   * @return array
   *   The server errors message.
   */
  public function sendPassword($data, $request_resource = 'confirm') {
    $data = http_build_query($data);
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/users/_' . $request_resource;
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_POST,
      'headers' => array(
        'Accept' => array('application/json'),
        'Content-Type' => array('application/x-www-form-urlencoded'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
      'data' => $data,
    ));
    $client = $this->client();
    try {
      $body = $client->execute($request);
      if (!empty($body)) {
        $this->lastResponse = $client->lastResponse;
      }
      return json_decode($body, TRUE);
    }
    catch (HttpClientException $e) {
      $body_response_message = json_decode($e->getResponse()->body);
      watchdog('Agate Client', 'Request failed, @code: @message',
        array(
          '@code' => $body_response_message->code,
          '@message' => $body_response_message->message,
        ), WATCHDOG_WARNING);
      return array(
        'code' => $body_response_message->code,
        'message' => $body_response_message->message,
      );
    }
  }


  /**
   * Perform authentication.
   *
   * Authentication by user name and forwards any cookie from Agate
   * to the drupal client.
   *
   * @param string $username
   *   The user name.
   * @param string $password
   *   The password.
   *
   * @return bool
   *   TRUE/FALSE authentication
   */
  public function authenticate($username, $password) {
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/tickets';
    $data = 'username=' . $username . '&password=' . $password;
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_POST,
      'headers' => array(
        'Accept' => array('application/json'),
        'Content-Type' => array('application/x-www-form-urlencoded'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
      'data' => $data,
    ));

    $client = $this->client();
    try {
      $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      $this->setLastResponseCookies();

      return TRUE;
    }
    catch (HttpClientException $e) {
      watchdog('Agate Client', 'Request failed, @code: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      return FALSE;
    }
  }

  /**
   * Get the user name matching the Agate token.
   *
   * @param string $token
   *   The stored id token.
   *
   * @return mixed|null
   *   The server response.
   */
  public function validate($token) {
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/ticket/' . $token . '/username';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => array(
        'Accept' => array('text/plain', 'application/json'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
    ));

    $client = $this->client();
    try {
      $body = $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      $_SESSION[self::OBIBA_COOKIE] = $token;
      return $body;
    }
    catch (HttpClientException $e) {
      watchdog('Agate Client', 'Request failed, @code: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      return NULL;
    }
  }

  /**
   * Get the subject from the current Agate ticket.
   *
   * User needs to have been authenticated first.
   *
   * @return array
   *   The Subject of the current ticket.
   */
  public function getSubject() {
    if (!ObibaAgateClient::hasTicket()) {
      return array();
    }
    $this->lastResponse = NULL;
    $url = $this->agateUrl . '/ticket/' . $_SESSION[self::OBIBA_COOKIE] . '/subject';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => array(
        'Accept' => array('application/json'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
    ));

    $client = $this->client();
    try {
      $body = $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      return json_decode($body, TRUE);
    }
    catch (HttpClientException $e) {
      watchdog('Agate Client', 'Request failed, @code: @message',
        array(
          '@code' => $e->getCode(),
          '@message' => $e->getMessage(),
        ), WATCHDOG_WARNING);
      return array();
    }
  }

  /**
   * Send a logout request to Agate and clean drupal client cookies.
   */
  public function logout() {
    if (!ObibaAgateClient::hasTicket()) {
      return;
    }

    $url = $this->agateUrl . '/ticket/' . $_SESSION[self::OBIBA_COOKIE];
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_DELETE,
      'headers' => array(
        'Accept' => array('application/json'),
        ObibaAgateClient::AUTHORIZATION_HEADER_AGATE => array($this->authorizationHeader()),
      ),
    ));

    $client = $this->client();
    try {
      $client->execute($request);
      $this->lastResponse = $client->lastResponse;
      $this->setLastResponseCookies();
      unset($_SESSION[self::OBIBA_COOKIE]);
    }
    catch (HttpClientException $e) {
      // Ignore.
    }
  }

  /**
   * Forwards the 'Set-Cookie' directive to the drupal client.
   */
  protected function setLastResponseCookies() {
    foreach ($this->getLastResponseHeader(self::SET_COOKIE_HEADER) as $cookie_str) {
      $cookie = $this->parseCookie($cookie_str);
      $keys = array_keys($cookie);
      $name = $keys[0];
      $value = $cookie[$name];
      watchdog('Agate Client', 'Cookie: name=@name, value=@value',
        array('@name' => $name, '@value' => $value), WATCHDOG_DEBUG);
      if (empty($value)) {
        if (!empty($_SESSION[$name])) {
          unset($_SESSION[$name]);
        }
      }
      else {
        $_SESSION[$name] = $value;
      }
    }
  }

  /**
   * Explode a cookie string in a array.
   *
   * @param string $cookie_str
   *   The stored in a string format cookie.
   *
   * @return array
   *   The cookies on a array format.
   */
  protected function parseCookie($cookie_str) {
    $cookie = array();
    foreach (explode(';', $cookie_str) as $entry_str) {
      $entry = explode('=', $entry_str);
      $cookie[$entry[0]] = $entry[1];
    }
    return $cookie;
  }

  /**
   * The override of the  client method.
   *
   * @return object
   *   The returned object of the client method.
   */
  protected function client() {
    $client = new HttpClient();

    if (!isset($client->options['curlopts'])) {
      $client->options['curlopts'] = array();
    }

    $client->options['curlopts'] += array(
      CURLOPT_SSL_VERIFYHOST => FALSE,
      CURLOPT_SSL_VERIFYPEER => FALSE,
    );

    return $client;
  }


  /**
   * The Header  authorization constructor.
   *
   * @return string
   *   The encrypted authorization header.
   */
  protected function authorizationHeader() {
    return 'Basic ' . base64_encode($this->applicationName . ':' . $this->applicationKey);
  }

}
