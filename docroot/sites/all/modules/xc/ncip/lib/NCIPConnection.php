<?php
/**
 * @file
 * NCIP Connection object
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

class NCIPConnection {
  const NCIP_SELF_SERVICE_CONNECTION = 0x0000;
  const NCIP_INITIATING_CONNECTION = 0x0001;
  const NCIP_RESPONDING_CONNECTION = 0x0002; // Don't use yet

  const NCIP_HTTP_CONNECTION = 'http';
  const NCIP_HTTPS_CONNECTION = 'https'; // Don't use yet
  const NCIP_TCP_CONNECTION = 'tcp'; // Don't use yet

  const NCIP_STATELESS = 0x0000;
  const NCIP_IDLE_STATE = 0x0001;
  const NCIP_WAITING_STATE = 0x0002;
  const NCIP_PROCESSING_STATE = 0x0003;

  private $connection_id;

  // General information
  private $type;
  private $protocol;
  private $application;
  private $state;

  // Connection information
  private $host;
  private $port;
  private $path;

  private $timeout = 20; // Request timeout
  public $session = array();
  public $cookies = array();

  // Remote application information
  private $to_system_id = array();
  private $to_agency_id = array();

  // Basic connection settings
  public $use_session = TRUE;
  public $use_cookies = TRUE;

  private $last_modification;

  // Message stack
  private $messages = array();
  private $errormsg;

  private $raw_response = FALSE;

  /**
   * NCIPConnection Constructor
   */
  public function __construct(NCIPApplication $application, $connection_id) {
    static $sql;
    if (!isset($sql)) {
      $sql = "SELECT connection_id, type, protocol, state, host, port, path,
              timeout, session, cookies, to_system_id, to_agency_id, use_session,
              use_cookies, last_modification
              FROM {ncip_connection}
              WHERE connection_id = %d AND application = '%s'";
    }

    $this->connection_id = $connection_id;
    $this->application = $application;
    $namespace = $this->application->get_namespace();
    $connection = db_query("SELECT connection_id, type, protocol, state, host, port, path,
              timeout, session, cookies, to_system_id, to_agency_id, use_session,
              use_cookies, last_modification
              FROM {ncip_connection}
              WHERE connection_id = :connection_id AND application = :application", array(':connection_id' => $connection_id, ':application' => $namespace))->fetchObject();

    if ($connection) {
      $this->type = $connection->type;
      $this->protocol = $connection->protocol;
      $this->state = $connection->state;
      $this->host = $connection->host;
      $this->port = $connection->port;
      $this->path = $connection->path;
      $this->timeout = $connection->timeout;
      $this->session = unserialize($connection->session);
      $this->cookies = unserialize($connection->cookies);
      $this->to_system_id = unserialize($connection->to_system_id);
      $this->to_agency_id = unserialize($connection->to_agency_id);
      $this->use_session = $connection->use_session;
      $this->use_cookies = $connection->use_cookies;
      $this->update_state(NCIPConnection::NCIP_STATELESS);
      $this->last_modification = $connection->last_modification;
    }
    else {
      drupal_set_message(t('Unexpected error. Cannot create NCIP connection.'), 'error');
    }
  }

  public function get_url() {
    return $this->protocol . '://' . $this->host . ':' . $this->port . $this->path;
  }

  /**
   * NCIPConnection destructor, may be useful for any future TCP implementation
   */
  public function __destruct() {
    // TODO: instead of desctuct we should use something different thing, because in Ajax calls it cased an error
    // this line is just a dirty hack. The problem is, that an update do some error, which
    // calls watchdog, but without user id, and the triggered error message in HTML format
    // is concatenated to the output, which is the result of the Ajax reponse.

    // FIXME: ok. perhaps we can do the updates a different place instead...
    // destruct and disconnect are probably not necessary anyways
    // xc_log_info('ncip', 'fake');
    $this->disconnect();
    $this->update();
  }

  private function _func_if_exists($function, $args) {
    if (method_exists($this, $function[1])) {
      return call_user_func_array($function, $args);
    }
    elseif (function_exists($function[1])) {
      return call_user_func_array($function[1], $args);
    }
  }

  /**
   * Establish a network connection to the host.
   *
   * Note: NCIPConnection object must have first been created from an
   * NCIPApplication object.
   */
  public function connect() {
    $this->update_state(NCIPConnection::NCIP_IDLE_STATE);
    $function = array($this, '_ncip_' . $this->protocol . '_connect');
    $args = func_get_args();
    return $this->_func_if_exists($function, $args);
  }

  /**
   * Finish/end network connection
   *
   * Note: NCIPConnection object will continue to persist until it is destroyed
   * from its related NCIPApplication object, or until its current state timeout
   * for inactivity is reached
   */
  public function disconnect() {
    $this->update_state(NCIPConnection::NCIP_STATELESS);
    $function = array($this, '_ncip_' . $this->protocol . '_disconnect');
    $args = func_get_args();
    return $this->_func_if_exists($function, $args);
  }

  /**
   * Updates all properties of the NCIP connection, saves all values to the
   * database
   */
  public function update() {
    static $sql;
    if (!isset($sql)) {
      $sql = "UPDATE {ncip_connection}
              SET type = %d, protocol = '%s', application = '%s', state = %d,
                host = '%s', port = '%s', path = '%s', timeout = %d,
                timestamp = %d, session = '%s', cookies = '%s',
                to_system_id = '%s', to_agency_id = '%s', use_session = %d,
                use_cookies = %d, last_modification = %d
              WHERE connection_id = %d AND application = '%s'";
    }
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql, $this->type, $this->protocol, $this->application->get_namespace(), $this->state, $this->host, $this->port, $this->path, $this->timeout, time(), serialize($this->session), serialize($this->cookies), serialize($this->to_system_id), serialize($this->to_agency_id), $this->use_session, $this->use_cookies, time(), $this->connection_id, $this->application->get_namespace()) */
    db_update('ncip_connection')
      ->fields(array(
          'type' => $this->type,
          'protocol' => $this->protocol,
          'application' => $this->application->get_namespace(),
          'state' => $this->state,
          'host' => $this->host,
          'port' => $this->port,
          'path' => $this->path,
          'timeout' => $this->timeout,
          'timestamp' => time(),
          'session' => serialize($this->session),
          'cookies' => serialize($this->cookies),
          'to_system_id' => serialize($this->to_system_id),
          'to_agency_id' => serialize($this->to_agency_id),
          'use_session' => $this->use_session,
          'use_cookies' => $this->use_cookies,
          'last_modification' => time(),
        ))
      ->condition('connection_id', $this->connection_id)
      ->condition('application', $this->application->get_namespace())
      ->execute();
  }

  /**
   * Updates the NCIP connection timestamp to track connection activity, saves
   * value to the database
   */
  public function update_timestamp() {
    static $sql;
    if (!isset($sql)) {
      $sql = "UPDATE {ncip_connection}
              SET timestamp = %d
              WHERE connection_id = %d AND application = '%s'";
    }
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql, time(), $this->connection_id, $this->application->get_namespace()) */
    db_update('ncip_connection')
      ->fields(array(
          'timestamp' => time(),
        ))
      ->condition('connection_id', $this->connection_id)
      ->condition('application', $this->application->get_namespace())
      ->execute();
  }

  /**
   * Updates the NCIP connection state, saves value to the database
   */
  private function update_state($state) {
    static $sql;
    if (!isset($sql)) {
      $sql = "UPDATE {ncip_connection}
              SET state = %d, timestamp = %d
              WHERE connection_id = %d AND application = '%s'";
    }
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query($sql, $this->state, time(), $this->connection_id, $this->application->get_namespace()) */
    db_update('ncip_connection')
      ->fields(array(
          'state' => $this->state,
          'timestamp' => time(),
        ))
      ->condition('connection_id', $this->connection_id)
      ->condition('application', $this->application->get_namespace())
      ->execute();
  }

  /**
   * Sends an NCIP message object over the connection
   *
   * @param $message
   *    NCIPMessage object
   * @return
   *    TRUE if NCIP message was sent or FALSE
   */
  public function send(NCIPMessage $message) {
    switch ($this->type) {
      case NCIPConnection::NCIP_INITIATING_CONNECTION:
        if ($this->state == NCIPConnection::NCIP_IDLE_STATE) {
          $this->update_state(NCIPConnection::NCIP_WAITING_STATE);
        }
        break;
      case NCIPConnection::NCIP_RESPONDING_CONNECTION:
        if ($this->state == NCIPConnection::NCIP_PROCESSING_STATE) {
          $this->update_state(NCIPConnection::NCIP_IDLE_STATE);
        }
        break;
    }
    // _ncip_http_send'
    if ($this->protocol == 'http') {
      $return = $this->_ncip_http_send($message);
    }
    else {
      $function = array($this, '_ncip_' . $this->protocol . '_send');
      $return = $this->_func_if_exists($function, $message);
    }
    $this->update();
    return $return;
  }

  /**
   * Receives an incoming NCIP message over the connection
   *
   * @return
   *    NCIPMessage object from or NULL
   */
  public function receive() {
    switch ($this->type) {
      case NCIPConnection::NCIP_INITIATING_CONNECTION:
        if ($this->state == NCIPConnection::NCIP_WAITING_STATE) {
          $this->update_state(NCIPConnection::NCIP_IDLE_STATE);
        }
        break;
      case NCIPConnection::NCIP_RESPONDING_CONNECTION:
        if ($this->state == NCIPConnection::NCIP_IDLE_STATE) {
          $this->update_state(NCIPConnection::NCIP_PROCESSING_STATE);
        }
        break;
    }
    $function = array($this, '_ncip_' . $this->protocol . '_receive');
    $args = func_get_args();
    $return = $this->_func_if_exists($function, $args);
    $this->update();
    return $return;
  }

  /** Getters and Setters **/
  public function set_to_system_id($scheme, $value) {
    $this->to_system_id = array(
      'scheme' => $scheme,
      'value' => $value,
    );
  }

  public function get_to_system_id() {
    return $this->to_system_id;
  }

  public function set_to_agency_id($scheme, $value) {
    $this->to_agency_id = array(
      'scheme' => $scheme,
      'value' => $value,
    );
  }

  public function get_to_agency_id() {
    return $this->to_agency_id;
  }

  public function get_connection_id() {
    return $this->connection_id;
  }

  public function get_type() {
    return $this->type;
  }

  /**
   * Get the NCIPApplication associated with the connection
   *
   * @return (NCIPApplication)
   *   The NCIPApplication associated with the connection
   */
  public function get_application() {
    return $this->application;
  }

  public function get_protocol() {
    return $this->protocol;
  }

  public function get_timestamp() {
    static $sql;
    //TODO from Tom, need to compare this query
    if (isset($sql)) {
      $sql = "SELECT timestamp
              FROM {ncip_connection}
              WHERE connection_id = %d AND application = '%s'";
    }
    return db_query("SELECT timestamp
              FROM {ncip_connection}
              WHERE connection_id = :connection_id AND application = :application", array(':connection_id' => $this->connection_id, ':application' => $this->application->get_namespace()))->fetchField();
  }

  public function get_curr_state() {
    return $this->state;
  }

  public function get_curr_state_time() {
    return time() - $this->get_timestamp();
  }

  /** Supported protocol implementations **/
  private function &_ncip_http_connect($connect = TRUE) {
    static $curl = array();

    if (!isset($curl[$this->connection_id])
        || is_null($curl[$this->connection_id]) && $connect) {
      $curl[$this->connection_id] = curl_init($this->get_url());
      curl_setopt($curl[$this->connection_id], CURLOPT_HTTPHEADER, array(
        'Content-Type: application/xml; charset="utf-8"',
        'Connection: Keep-Alive',
        'Keep-Alive: 120',
      ));
      curl_setopt($curl[$this->connection_id], CURLOPT_FORBID_REUSE, TRUE);
      curl_setopt($curl[$this->connection_id], CURLOPT_USERAGENT, 'eXtensible Catalog PHP cURL');
      curl_setopt($curl[$this->connection_id], CURLOPT_CONNECTTIMEOUT, $this->timeout);
      curl_setopt($curl[$this->connection_id], CURLOPT_TIMEOUT, $this->timeout);
      curl_setopt($curl[$this->connection_id], CURLOPT_BINARYTRANSFER, TRUE);
      curl_setopt($curl[$this->connection_id], CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($curl[$this->connection_id], CURLOPT_FAILONERROR, FALSE);
      curl_setopt($curl[$this->connection_id], CURLOPT_URL, $this->get_url());
      curl_setopt($curl[$this->connection_id], CURLOPT_CUSTOMREQUEST, 'POST');
      curl_setopt($curl[$this->connection_id], CURLOPT_POST, TRUE);

      if (variable_get('ncip_use_proxy', FALSE)) {
        $proxy_url = 'http://' . variable_get('ncip_proxy_host', 'localhost') . ':' . variable_get('ncip_proxy_port', '8080');
        curl_setopt($curl[$this->connection_id], CURLOPT_PROXY, $proxy_url);
        curl_setopt($curl[$this->connection_id], CURLOPT_HTTPPROXYTUNNEL, TRUE);
        switch (variable_get('ncip_proxy_type', 'http')) {
          case 'socks':
            curl_setopt($curl[$this->connection_id], CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
            break;
          case 'http':
          default:
            curl_setopt($curl[$this->connection_id], CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
            break;
        }
        if (variable_get('ncip_proxy_auth', '') == 'basic') {
          curl_setopt($curl[$this->connection_id], CURLOPT_PROXYAUTH, CURLAUTH_BASIC);
          curl_setopt($curl[$this->connection_id], CURLOPT_PROXYUSERPWD, variable_get('ncip_proxy_username', NULL) . ':' . variable_get('ncip_proxy_password', ''));
        }
      }

      if ($this->use_cookies) {
        $cookies = array();
        foreach ($this->cookies as $n => $v) {
          $cookies[$n] = $v;
        }
        curl_setopt($curl[$this->connection_id], CURLOPT_COOKIE, implode('; ', $cookies));
        curl_setopt($curl[$this->connection_id], CURLOPT_HEADERFUNCTION, array($this, '_ncip_http_curl_parse_cookies'));
      }
    }

    return $curl[$this->connection_id];
  }

  private function _ncip_http_disconnect() {
    $curl = &$this->_ncip_http_connect(FALSE);
    if ($curl) {
      curl_close($curl);
      unset($curl);
    }
  }

  private function _ncip_http_send(NCIPMessage $message) {
    $curl = &$this->_ncip_http_connect();
    if ($this->use_cookies) {
      $cookies = array();
      foreach ($this->cookies as $n => $v) {
        $cookies[$n] = $v;
      }
      curl_setopt($curl, CURLOPT_COOKIE, implode('; ', $cookies));
    }

    // POST message to server and receive XML response or return FALSE
    curl_setopt($curl, CURLOPT_POSTFIELDS, $message->to_xml());
    if (variable_get('ncip_debugging', 0) == 1) {
      xc_log_info('ncip request', $this->get_url());
      xc_log_info('ncip request', htmlspecialchars($message->to_xml()));
    }

    $this->raw_response = curl_exec($curl);

    if ($this->raw_response === FALSE) {
      $this->errormsg = curl_error($curl);
      // xc_log_info('ncip', 'ERROR: '); // . $this->errormsg);
      return FALSE;
    }

    // Parse into an NCIPMessage object and push into message stack
    // xc_log_info('track', $this->raw_response);
    $response = NCIPMessage::from_xml($this->raw_response);
    array_push($this->messages, $response);

    // Debugging
    if (variable_get('ncip_debugging', 0) == 1) {
      // xc_log_info('ncip request', htmlspecialchars($message->to_xml()));
      xc_log_info('ncip response', htmlspecialchars($response->to_xml()));
    }

    // Send was successful, then return TRUE
    return TRUE;
  }

  private function _ncip_http_receive() {
    return array_shift($this->messages);
  }

  private function _ncip_http_curl_parse_cookies($curl, $line) {
    $length = strlen($line);
    if (!strncmp($line, "Set-Cookie:", 11)) {
      $string = trim(substr($line, 11, -1));
      $cookie = explode(';', $string);
      $cookie = explode('=', $cookie[0]);
      $n = trim(array_shift($cookie));
      $cookies[$n] = trim(implode('=', $cookie));
      $this->cookies = $cookies;
    }
    return $length;
  }

  /** Currently unsupported protocol implementations **/
  private function _ncip_https_connect() { } // Currently not supported

  private function _ncip_https_disconnect() { } // Currently not supported

  private function _ncip_https_send() { } // Currently not supported

  private function _ncip_https_receive() { } // Currently not supported

  private function _ncip_tcp_connect() { } // Currently not supported

  private function _ncip_tcp_disconnect() { } // Currently not supported

  private function _ncip_tcp_send() { } // Currently not supported

  private function _ncip_tcp_receive() { } // Currently not supported

  public function get_errormsg() {
    return $this->errormsg;
  }

  public function get_raw_response() {
    return $this->raw_response;
  }

  public function get_last_modification() {
    return $this->last_modification;
  }

  public function set_timeout($timeout) {
    $this->timeout = $timeout;
  }
}
