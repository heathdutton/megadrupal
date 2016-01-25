<?php
/**
 * @file
 * NCIP Application objects
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

class NCIPApplication {
  // Contains ALL loaded applications
  private static $applications = array();

  // General information
  private $namespace;
  private $version;
  private $profile;
  private $module;

  private $services = array();
  private $data_elements = array();
  private $schemes = array();
  private $connections = array();

  public $title = '';
  public $description = '';

  // Local application information
  private $from_system_id = array();
  private $from_system_authentication = NULL;
  private $from_agency_id = array();
  private $from_agency_authentication = NULL;
  private $on_behalf_of_agency = array();
  private $application_profile_type = array();

  /**
   * Install an installed NCIP application, adding it to the database
   *
   * @param $namespace
   *    NCIP application namespace
   * @param $profile
   *    Profiles to use for this application
   * @param $module
   *    Module implementing this application
   * @param $title
   *    NCIP application title
   * @param $description
   *    NCIP application description
   */
  public static function install($namespace, $profile = 'ncip', $version = 2,
    $module = 'ncip', $title = '', $description = '') {

    if (!in_array($version, array(1, 1.01, 2, 2.01))) { // valid versions
      drupal_set_message(t('NCIP Version Invalid. Unable to install namespace %ns.',
        array('%ns' => $namespace)), 'error');
      return false;
    }

    $record = new stdClass();
    $record->namespace   = $namespace;
    $record->profile     = $profile;
    $record->version     = $version;
    $record->module      = $module;
    $record->title       = $title;
    $record->description = $description;
    $result = drupal_write_record('ncip_application', $record);
    if ($result) {
      return TRUE;
    }
    else {
      drupal_set_message(t('Unexpected error. Unable to install NCIP Application namespace: %ns',
        array('%ns' => $namespace)), 'error');
    }
    return FALSE;
  }

  /**
   * Uninstall an installed NCIP applicaiton, removing it from the database
   *
   * @param $namespace
   *    NCIP application namespace
   */
  public static function uninstall($namespace) {
    if (db_delete('ncip_application')
          ->condition('namespace', $namespace)
          ->execute()) {
      return TRUE;
    }
    else {
      drupal_set_message(t('Unexpected error. Unable to uninstall namespace %ns',
        array('%ns' => $namespace)), 'error');
    }
  }

  /**
   * Load an installed NCIP applicaiton
   *
   * @param $namespace
   *    NCIP application namespace
   *
   * @return
   *    NCIPApplication object
   */
  public static function load($namespace) {
    if (db_query("SELECT namespace FROM {ncip_application} WHERE namespace = :namespace", array(':namespace' => $namespace))->fetchField()) {
      self::$applications[$namespace] = empty(self::$applications[$namespace])
        ? new NCIPApplication($namespace)
        : self::$applications[$namespace];
      return self::$applications[$namespace];
    }
    return FALSE;
  }

  protected function __construct($namespace) {
    // this function is protected, to prevent loading of an application without
    // using the static factory method load() above; ensures that all running
    // NCIP applications are registered in the static $applications array
    $application = db_query("SELECT namespace, profile, version, module, title, description,
              from_system_id, from_system_authentication, from_agency_id,
              from_agency_authentication, on_behalf_of_agency,
              application_profile_type
            FROM {ncip_application}
            WHERE namespace = :namespace", array(':namespace' => $namespace))->fetchObject();

    if ($application) {
      $this->namespace = $application->namespace;
      $this->profile = $application->profile;
      $this->version = $application->version;
      $this->module = $application->module;
      $this->title = $application->title;
      $this->description = $application->description;
      $this->from_system_id = unserialize($application->from_system_id);
      $this->from_system_authentication = $application->from_system_authentication;
      $this->from_agency_id = unserialize($application->from_agency_id);
      $this->from_agency_authentication = $application->from_agency_authentication;
      $this->on_behalf_of_agency = unserialize($application->on_behalf_of_agency);
      $this->application_profile_type = unserialize($application->application_profile_type);
      $this->services = isset($this->profile['services']) ? $this->profile['services'] : NULL;
      $this->data_elements = isset($this->profile['data elements']) ? $this->profile['data elements'] : NULL;
      $this->schemes = isset($this->profile['schemes']) ? $this->profile['schemes'] : NULL;
    }
    else {
      drupal_set_message(t('Unexpected error. Cannot create NCIP application.'),
        'error');
    }
  }

  /**
   * Updates all properties of the NCIP application, saves all values to the
   * database
   */
  public function update() {
    // Update application
    db_update('ncip_application')
      ->fields(array(
          'title' => $this->title,
          'description' => $this->description,
          'from_system_id' => serialize($this->from_system_id),
          'from_system_authentication' => $this->from_system_authentication,
          'from_agency_id' => serialize($this->from_agency_id),
          'from_agency_authentication' => $this->from_agency_authentication,
          'on_behalf_of_agency' => serialize($this->on_behalf_of_agency),
          'application_profile_type' => serialize($this->application_profile_type),
        ))
      ->condition('namespace', $this->namespace)
      ->execute();

    // Destroy all connections
    db_delete('ncip_connection')
      ->condition('application', $this->namespace)
      ->execute();
  }

  /**
   * Create an NCIP connection by inserting it into the database
   *
   * @param $host
   *    Host for remote NCIP application
   * @param $port
   *    Port for remote NCIP applicaiton
   * @param $path
   *    Path for remote NCIP application
   * @param $type
   *    NCIP_INITIATING_CONNECTION or NCIP_RESPONDING_CONNECTION
   * @param $protocol
   *    NCIP_HTTP_CONNECTION, since both NCIP_HTTPS_CONNECTION
   *    and NCIP_TCP_CONNECTION are unsupported at this time
   * @return
   *    NCIPConnection object
   */
  public function create_connection($host = 'localhost', $port = '', $path = '',
      $type = NCIPConnection::NCIP_INITIATING_CONNECTION,
      $protocol = NCIPConnection::NCIP_HTTP_CONNECTION) {

    $record = new stdClass();
    $record->host         = $host;
    $record->port         = $port;
    $record->path         = $path;
    $record->application  = $this->namespace;
    $record->type         = $type;
    $record->protocol     = $protocol;
    $record->state        = NCIPConnection::NCIP_STATELESS;
    $record->timeout      = 20;
    $record->timestamp    = time();
    $record->session      = serialize(array());
    $record->cookies      = serialize(array());
    $record->to_system_id = serialize(array());
    $record->to_agency_id = serialize(array());
    $record->use_session  = TRUE;
    $record->use_cookies  = TRUE;
    $record->last_modification = time();
    $result = drupal_write_record('ncip_connection', $record);
    if ($result) {
      $connection_id = $record->connection_id;
      return $this->load_connection($connection_id);
    }
    else {
      return FALSE;
    }
  }

  /**
   * Destroy an NCIP connection, if possible, by removing it from the database
   *
   * @param $connection_id
   *    Connection identifier
   */
  public function destroy_connection($connection_id) {
    if ($this->connections[$connection_id] instanceof NCIPConnection) {
      $this->connections[$connection_id]->disconnect();
    }
    db_delete('ncip_connection')
      ->condition('connection_id', $connection_id)
      ->condition('application', $this->namespace)
      ->execute();
    unset($this->connections[$connection_id]);
  }

  /**
   * Load an NCIP connection, if possible, by loading it from the database
   *
   * @param $connection_id
   *    Connection identifier
   * @return
   *    NCIPConnection object
   */
  public function load_connection($connection_id) {
    if (!isset($this->connections[$connection_id])
        || !($this->connections[$connection_id] instanceof NCIPConnection)) {
      $sql = "SELECT connection_id
              FROM {ncip_connection}
              WHERE connection_id = :connection_id AND application = :application";
      if (!db_query($sql, array(
          ':connection_id' => $connection_id,
          ':application' => $this->namespace)
          )->fetchField()) {
        return FALSE;
      }
      $this->connections[$connection_id] = new NCIPConnection($this, $connection_id);
    }
    $this->connections[$connection_id]->update_timestamp();
    return $this->connections[$connection_id];
  }

  /** Getters and setters **/
  public function set_from_system_id($scheme, $value) {
    $this->from_system_id = array(
      'scheme' => $scheme,
      'value' => $value,
    );
  }

  public function get_from_system_id() {
    return $this->from_system_id;
  }

  public function set_from_system_authentication($value) {
    $this->from_system_authentication = $value;
  }

  public function get_from_system_authentication() {
    return $this->from_system_authentication;
  }

  public function set_from_agency_id($scheme, $value) {
    $this->from_agency_id = array(
      'scheme' => $scheme,
      'value' => $value,
    );
  }

  public function get_from_agency_id() {
    return $this->from_agency_id;
  }

  public function set_from_agency_authentication($value) {
    $this->from_agency_authentication = $value;
  }

  public function get_from_agency_authentication() {
    return $this->from_agency_authentication;
  }

  public function set_application_profile_type($scheme, $value) {
    $this->application_profile_type = array(
      'scheme' => $scheme,
      'value' => $value,
    );
  }

  public function get_application_profile_type() {
    return $this->application_profile_type;
  }

  public function set_on_behalf_of_agency($scheme, $value) {
    $this->on_behalf_of_agency = array(
      'scheme' => $scheme,
      'value' => $value,
    );
  }

  public function get_on_behalf_of_agency() {
    return $this->on_behalf_of_agency;
  }

  public function get_connection($connection_id) {
    return $this->connections[$connection_id];
  }

  public function get_namespace() {
    return $this->namespace;
  }

  public function get_module() {
    return $this->module;
  }

  public function get_profile() {
    return $this->profile;
  }

  public function get_version() {
    return $this->version;
  }

  public function get_services() {
    return $this->services;
  }

  public function get_data_elements() {
    return $this->data_elements;
  }

  public function get_schemes() {
    return $this->schemes;
  }

  public function get_connections() {
    return $this->connections;
  }

}
