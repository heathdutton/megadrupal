<?php

define('LYRIS_CONN_FAIL',          -3);
define('LYRIS_CONN_INVALID_API',   -2);
define('LYRIS_CONN_MISSING_CREDS', -1);
define('LYRIS_CONN_NO_ATTEMPT',     0);
define('LYRIS_CONN_SUCCESS',        1);
define('LYRIS_CONN_LIST_ADMIN',     2);
define('LYRIS_CONN_SITE_ADMIN',     3);
define('LYRIS_CONN_SERVER_ADMIN',   4);

/**
 * @file
 * Class to manage the connection to Lyris and load the appropriate API
 * functions.
 */
class LyrisConn extends Base {
  // The API connection to Lyris.
  protected $mlapi;
  protected $apiClient;

  // The location of the WSDL files.
  protected $wsdl_loc;

  // The status of the object's connection to lyris.
  public $connection_status = LYRIS_CONN_NO_ATTEMPT;

  // The connection mode.  Defaults to 'production'
  public $mode;

  // The Drupal account or 'admin' used to connect to Lyris.
  protected $account;

  // The current user's information from the people_ table
  public $user = array();

  // The current user's username.
  public $user_mail;

  // The name of the current Lyris user.
  public $username;

  // The current user's passowrd.
  protected $userpass;

  // The available Lyris API version.
  public $api_version;

  // The major API version, used to determine structs and API calls.
  public $api_major_version;

  // APIs that we have coded for.
  private $valid_apis = array('1.5.2a', '10', '11', '12');

  // The object key used to define this instance of the Lyris connection.
  public $object_key;

  // An array of the lists this user has administrative permission to access.
  protected $allowed_lists = array();

  // Constructor
  public function LyrisConn($account, $mode) {
    parent::__construct();

    $this->account = $account;
    $this->mode = $mode;

    try {
      // Do not attempt a connection if the server mode is set to offline.
      if ($this->mode == LYRIS_MODE_OFFLINE) {
        throw new LyrisModeException($this);
      }

      // Create the connection using the stored data.
      $this->wsdl_loc = variable_get('lyris_wsdl_' . $this->mode);

      $this->username = $this->getUserName($account);
      $this->userpass = $this->getUserPass($account);

      // Establish the connection.
      $this->connect();

      // Get the current user's e-mail address
      $this->user_mail = $this->call('CurrentUserEmailAddress');

    } catch (Exception $e) {$this->handleException($e);}
  }

  /**
   * Determine if the current user is the Lyris server admin.
   */
  protected function accountIsAdmin() {
    return (is_string($this->account) && $this->account == 'admin');
  }

  /**
   * Establish a connection to Lyris.
   */
  private function connect() {
    // Prevent multiple connections per request.
    if ($this->isConnected()) {
      return;
    }

    // Log connection attempt.
    if (variable_get('lyris_log_connection', 0)) {
      $this->watchdog('Connection attempt for Lyris user %name to %server server.', WATCHDOG_NOTICE, array('%name' => $this->username, '%server' => $this->mode), NULL, 'lyris_connect');
    }

    // Make sure the settings are in place.
    if (empty($this->wsdl_loc)) {
      $this->connection_status = LYRIS_CONN_MISSING_CREDS;
      throw new Exception(t('Lyris has not yet been properly configured.  WSDL not set for %mode mode. Please !l.', array('%mode' => $this->mode, '!l' => l('check your settings', 'admin/config/services/lyris', array('query' => drupal_get_destination())))), WATCHDOG_CRITICAL);
    }

    // Create a new connection to Lyris.
    $this->apiClient = (PHP_MINOR_VERSION >= 3 ? new nusoap_client($this->wsdl_loc, TRUE) : new nusoapclient($this->wsdl_loc, TRUE));
    $this->apiClient->setCredentials($this->username, password_field_decrypt($this->userpass), 'basic');

    // Check for errors on the connection.
    if ($err = $this->apiClient->getError()) {
      $this->connection_status = LYRIS_CONN_FAIL;
      throw new LyrisConnectionException($this, $err);
    }

    // Establish a proxy connection to the actual API.
    $this->mlapi = $this->apiClient->getProxy();

    // Check for errors on the connection.
    if (!is_object($this->mlapi)) {
      $this->connection_status = LYRIS_CONN_FAIL;
      throw new LyrisConnectionException($this);
    }

    $this->mlapi->setCredentials($this->username, password_field_decrypt($this->userpass), 'basic');

    // To connect to the Lyris API, the user must have at least List Admin
    // permissions as of 10.x.  If this call succeeds, then the user has at
    // least List Admin level access.  Otherwise, we can't establish a
    // connection.
    if (!$this->getApiVersion()) {
      $this->connection_status = LYRIS_CONN_FAIL;
      throw new LyrisConnectionException($this);
    }
  }

  /**
   * Get the current API version.
   */
  private function getApiVersion($major = FALSE) {
    if (is_object($this->mlapi)) {
      if ($this->api_version = $this->mlapi->ApiVersion()) {
        preg_match('/^([0-9]{1,2})\.[a-z0-9\.]+$/', $this->api_version, $matches);
        if ($matches[1] < 10) {
          $this->api_major_version = $this->api_version;
        }
        else {
          $this->api_major_version = $matches[1];
        }

        // Set a variable to make api recognition less process intesive.
        variable_set('lyris_api_major_' . $this->mode, $this->api_major_version);
        variable_set('lyris_api_full_'  . $this->mode, $this->api_version);

        if (!in_array($this->api_major_version, $this->valid_apis)) {
          $this->connection_status = LYRIS_CONN_INVALID_API;
          throw new LyrisApiException($this, 'Your Lyris API version (%_version) is currently not supported.');
        }
        else {
          $this->connection_status = LYRIS_CONN_SUCCESS;
          return ($major ? $this->api_major_version : $this->api_version);
        }
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the display name of the current Lyris user.
   */
  private function getUserName($account = NULL) {
    if ($account == 'admin') {
      $creds = variable_get('lyris_connection', '');
      if (isset($creds[$this->mode])) {
        return $creds[$this->mode]['admin_name'];
      }
      else {
        throw new LyrisConnectionException($this, _lyris_options('lyris_mode', $this->mode) . ' admin credentials are not set.  A Lyris server admin must be defined in the settings.php file in order to establish a connection to Lyris. !_readme');
      }
    }

    $account = (is_object($account) ? $account : lyris_global_user());

    if (!isset($account->data['lyris_username']) || empty($account->data['lyris_username'])) {
      $this->connection_status = LYRIS_CONN_MISSING_CREDS;
      throw new Exception(t('Lyris username not set for %name.', array('%name' => $account->name)), WATCHDOG_ERROR);
    }
    else {
      return $account->data['lyris_username'];
    }
  }

  /**
   * Get the active user's Lyris password.
   */
  private function getUserPass($account = NULL) {
    if ($account == 'admin') {
      $creds = variable_get('lyris_connection', '');
      if (isset($creds[$this->mode])) {
        return lyris_password_encrypt($creds[$this->mode]['admin_pass']);
      }
      else {
        throw new LyrisConnectionException($this, _lyris_options('lyris_mode', $this->mode) . ' admin credentials are not set.  A Lyris server admin must be defined in the settings.php file in order to establish a connection to Lyris. !_readme');
      }
    }

    $account = (is_object($account) ? $account : lyris_global_user());

    if (!isset($account->data['lyris_userpass']) || empty($account->data['lyris_userpass'])) {
      $this->connection_status = LYRIS_CONN_MISSING_CREDS;
      throw new Exception(t('Lyris password not set for %name.', array('%name' => $account->name)), WATCHDOG_ERROR);
    }
    else {
      return $account->data['lyris_userpass'];
    }
  }

  /**
   * Get a string of the connection credentials for debugging.
   */
  public function getDebugCreds() {
    if ($this->userIsDrupalAdmin()) {
      return t('Connecting to !wsdl as !name.', array('!wsdl' => $this->wsdl_loc, '!name' => $this->username));
    }
  }

  /**
   * Determine whether we have established a connection to Lyris.
   */
  public function isConnected() {
    switch ($this->connection_status) {
      case LYRIS_CONN_SUCCESS:
      case LYRIS_CONN_LIST_ADMIN:
      case LYRIS_CONN_SITE_ADMIN:
      case LYRIS_CONN_SERVER_ADMIN:
        return TRUE;
        break;

      case LYRIS_CONN_FAIL:
      case LYRIS_CONN_INVALID_API:
      case LYRIS_CONN_MISSING_CREDS:
      case LYRIS_CONN_NO_ATTEMPT:
      default:
        return FALSE;
        break;
    }
  }

  /**
   * Test the connection.
   */
  public function testConnection() {
    try {
      // Make the connection.
      $this->connect();

      // Retrieve test data.
      $this->email = $this->mlapi->CurrentUserEmailAddress();
      $this->setMessage(t('SUCCESS! You have successfully connected to Lyris as %email.', array('%email' => $this->email)), 'status');
      $this->setMessage(t('Connected to %l as %u.', array('%l' => $this->wsdl_loc, '%u' => $this->username)), 'debug');
    } catch (Exception $e) {$this->handleException($e);}

    return TRUE;
  }
}
