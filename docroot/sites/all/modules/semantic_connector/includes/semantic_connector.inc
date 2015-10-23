<?php

/**
 * @file
 * The PoolParty PPX API-class.
 */

/**
 * A collection of static functions offered by the PoolParty Semantic Connector.
 */
class SemanticConnector {
  /**
   * Get a connection of the PoolParty Semantic Connector by its ID.
   *
   * @param string $type
   *   The type of the connection to receive. Possible values: 'pp_server',
   *   'sonr_server' and 'sparql_endpoint'.
   * @param int $connection_id
   *   The ID of the Semantic Connector Connection.
   *
   * @return SemanticConnectorConnection
   *   The connection object, depending on the provided $type.
   */
  public static function getConnection($type, $connection_id = 0) {
    switch ($type) {
      case 'pp_server':
        return new SemanticConnectorPPServerConnection($connection_id);

      case 'sonr_server':
        return new SemanticConnectorSonrServerConnection($connection_id);

      case 'sparql_endpoint':
        return new SemanticConnectorSparqlEndpointConnection($connection_id);

      default:
        return NULL;
    }
  }

  /**
   * Get all connection of the PoolParty Semantic Connector by connection-type.
   *
   * @param string $type
   *   The type of the connections to receive. Possible values: 'pp_server',
   *   'sonr_server' and 'sparql_endpoint'.
   *
   * @return array
   *   Array of SemanticConnectorConnection-objects of the give type.
   */
  public static function getConnectionsByType($type) {
    $connection_ids = db_select('semantic_connector_connections', 'c')
      ->fields('c', array('connection_id'))
      ->condition('type', $type)
      ->execute()
      ->fetchCol();

    $connections = array();
    foreach ($connection_ids as $connection_id) {
      switch ($type) {
        case 'pp_server':
          $connections[$connection_id] = new SemanticConnectorPPServerConnection($connection_id);
          break;

        case 'sonr_server':
          $connections[$connection_id] = new SemanticConnectorSonrServerConnection($connection_id);
          break;

        case 'sparql_endpoint':
          $connections[$connection_id] = new SemanticConnectorSparqlEndpointConnection($connection_id);
          break;
      }
    }

    return $connections;
  }

  /**
   * Create a new connection for the PoolParty Semantic Connector.
   *
   * @param string $type
   *   The type of the connection to receive. Possible values: 'pp_server',
   *   'sonr_server' and 'sparql_endpoint'.
   * @param string $url
   *   The URL of the connection.
   * @param string $title
   *   The title of the connection.
   * @param array $credentials
   *   The credentials required for the connection in the format
   *   "username:password" if required.
   * @param array $config
   *   The config of the Semantic Connector Connection as an array.
   * @param int $fixed_id
   *   A fixed connection id to use instead of the one with the given $type and
   *   $url.
   *   WARNING: Using an ID that does not exist will result in an error.
   *
   * @return SemanticConnectorConnection
   *   The connection object, depending on the provided $type.
   */
  public static function createConnection($type, $url, $title, array $credentials = array('username' => '', 'password' => ''), array $config = array(), $fixed_id = 0) {
    $connection = NULL;

    if ($fixed_id <= 0) {
      $old_connection_id = db_select('semantic_connector_connections', 'c')
        ->fields('c', array('connection_id'))
        ->condition('type', $type)
        ->condition('url', $url)
        ->execute()
        ->fetchField();
    }
    else {
      $old_connection_id = $fixed_id;
    }

    // If there is a connection available with the url, load it.
    if ($old_connection_id !== FALSE) {
      switch ($type) {
        case 'pp_server':
          $connection = new SemanticConnectorPPServerConnection($old_connection_id);
          break;

        case 'sonr_server':
          $connection = new SemanticConnectorSonrServerConnection($old_connection_id);
          break;

        case 'sparql_endpoint':
          $connection = new SemanticConnectorSparqlEndpointConnection($old_connection_id);
          break;
      }

      // If there already is a connection available, change if data has changed.
      $has_changed = FALSE;
      if ($connection->getTitle() != $title) {
        $connection->setTitle($title);
        $has_changed = TRUE;
      }
      if ($connection->getURL() != $url) {
        $connection->setURL($url);
        $has_changed = TRUE;
      }
      if ($connection->getCredentials() != $credentials) {
        $connection->setCredentials($credentials);
        $has_changed = TRUE;
      }
      if (!empty($config)) {
        $connection->setConfig(array_merge($connection->getConfig(), $config));
        $has_changed = TRUE;
      }

      // Save the connection if its data has changed.
      if ($has_changed) {
        $connection->save();
      }
    }
    // Data was not found in the DB --> Really create a new Connection.
    else {
      switch ($type) {
        case 'pp_server':
          $connection = new SemanticConnectorPPServerConnection();
          break;

        case 'sonr_server':
          $connection = new SemanticConnectorSonrServerConnection();
          break;

        case 'sparql_endpoint':
          $connection = new SemanticConnectorSparqlEndpointConnection();
          break;

        default:
          // todo: throw an error.
          break;
      }

      // Set all the required variables and save the connection.
      $connection->setTitle($title);
      $connection->setURL($url);
      $connection->setCredentials($credentials);
      $connection->setConfig(array_merge($connection->getDefaultConfig(), $config));
      $connection->save();
    }

    return $connection;
  }

  /**
   * Delete one or multiple Semantic Module Connections.
   *
   * @param array $connection_ids
   *   A single connection_id or an array of connection_ids to remove.
   */
  public static function deleteConnections(array $connection_ids) {
    if (!empty($connection_ids)) {
      db_delete('semantic_connector_connections')
        ->condition('connection_id', $connection_ids)
        ->execute();
    }
  }
}

/**
 * An abstract class for all connections used by PoolParty Semantic Connector.
 */
abstract class SemanticConnectorConnection {
  protected $id;
  protected $type;
  protected $url;
  protected $credentials;
  protected $title;
  protected $config;

  /**
   * Get an already configured API of a connection.
   *
   * @param string $api_type
   *   Additional information on what API to get, if a connection supports more
   *   than one APIs.
   *
   * @return object
   *   An API-object, depending on the $api_type.
   */
  abstract public function getAPI($api_type = '');

  /**
   * Get the default config of the Semantic Connector Connection.
   *
   * @return array
   *   The default config of the Semantic Connector Connection.
   */
  abstract public function getDefaultConfig();

  /**
   * The constructor of the SemanticConnectorConnection-class.
   *
   * @param int $connection_id
   *   The ID of the Semantic Connector Connection.
   */
  public function __construct($connection_id = 0) {
    $this->id = $connection_id;

    // Set the default values.
    $this->credentials = array(
      'username' => '',
      'password' => '',
    );
    $this->config = $this->getDefaultConfig();
    $this->title = '';
    $this->url = '';

    if ($connection_id > 0) {
      $connection = db_select('semantic_connector_connections', 'c')
        ->fields('c')
        ->condition('connection_id', $connection_id)
        ->execute()
        ->fetchAssoc();

      if ($connection !== FALSE) {
        $this->type = $connection['type'];
        $this->url = $connection['url'];
        $this->credentials['username'] = $connection['username'];
        $this->credentials['password'] = $connection['password'];
        $this->title = $connection['title'];

        // Merge the config with the default values in case there was a new
        // property added.
        $this->config = array_merge($this->config, unserialize($connection['config']));
      }
      else {
        // todo: throw an error.
        $this->id = 0;
      }
    }
  }

  /**
   * Create / update the current Semantic Connector connection in the database.
   */
  public function save() {
    // Update an existing connection.
    if ($this->id > 0) {
      db_update('semantic_connector_connections')
        ->fields(array(
          'type' => $this->type,
          'url' => $this->url,
          'title' => $this->title,
          'username' => $this->credentials['username'],
          'password' => $this->credentials['password'],
          'config' => serialize($this->config),
        ))
        ->condition('connection_id', $this->id)
        ->execute();
    }
    // Create a new connection.
    else {
      if (!isset($this->url)) {
        // todo: throw an error.
        return;
      }

      $id = db_insert('semantic_connector_connections')
        ->fields(array(
          'type' => $this->type,
          'url' => $this->url,
          'title' => $this->title,
          'username' => $this->credentials['username'],
          'password' => $this->credentials['password'],
          'config' => serialize($this->config),
        ))
        ->execute();

      $this->id = $id;
    }
  }

  /**
   * Get the ID of the Semantic Connector Connection.
   *
   * @return int
   *   The ID of the Semantic Connector Connection.
   */
  public function getID() {
    return $this->id;
  }

  /**
   * Get the type of the Semantic Connector Connection.
   *
   * @return string
   *   The type of the Semantic Connector Connection.
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Set the type of the Semantic Connector Connection.
   *
   * @param string $type
   *   The type of the Semantic Connector Connection.
   */
  public function setType($type) {
    $this->type = $type;
  }

  /**
   * Get the URL of the Semantic Connector Connection.
   *
   * @return string
   *   The URL of the Semantic Connector Connection.
   */
  public function getURL() {
    return $this->url;
  }

  /**
   * Set the URL of the Semantic Connector Connection.
   *
   * @param string $url
   *   The URL of the Semantic Connector Connection.
   */
  public function setURL($url) {
    $this->url = $url;
  }

  /**
   * Get the credentials of the Semantic Connector Connection.
   *
   * @return array
   *   An array with the credentials of the Semantic Connector Connection,
   *   including properties "username" and "password" .
   */
  public function getCredentials() {
    return $this->credentials;
  }

  /**
   * Set the credentials of the Semantic Connector Connection.
   *
   * @param array $credentials
   *   An array with the credentials of the Semantic Connector Connection,
   *   including properties "username" and "password" .
   */
  public function setCredentials(array $credentials) {
    if (!isset($credentials['username']) || !isset($credentials['password'])) {
      // todo: throw an error.
    }
    $this->credentials = $credentials;
  }

  /**
   * Get the title of the Semantic Connector Connection.
   *
   * @return string
   *   The title of the Semantic Connector Connection.
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Set the title of the Semantic Connector Connection.
   *
   * @param string $title
   *   The title of the Semantic Connector Connection.
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Get the config of the Semantic Connector Connection.
   *
   * @return array
   *   The config of the Semantic Connector Connection.
   */
  public function getConfig() {
    return $this->config;
  }

  /**
   * Set the config of the Semantic Connector Connection.
   *
   * @param array $config
   *   The config of the Semantic Connector Connection as an array.
   */
  public function setConfig(array $config) {
    $this->config = array_merge($this->getDefaultConfig(), $config);
  }
}

/**
 * A class for a connection to a PoolParty-server.
 */
class SemanticConnectorPPServerConnection extends SemanticConnectorConnection {

  public function __construct($connection_id = 0) {
    $this->type = 'pp_server';
    parent::__construct($connection_id);

    // Incorrect connection was loaded --> Throw an error.
    if ($this->type != 'pp_server') {
      // todo: throw an error.
    }
  }

  public function save() {
    // Add the projects to the config before saving the PoolParty Server.
    $ppt_api = $this->getAPI('PPT');
    $projects = $ppt_api->getProjects();

    $sparql_endpoints_to_remove = array();
    $new_project_urls = array();
    foreach ($projects as $project) {
      if (property_exists($project, 'sparql_endpoint_url')) {
        $new_project_urls[] = $project->sparql_endpoint_url;
      }
    }
    if (isset($this->config['projects'])) {
      foreach ($this->config['projects'] as $project) {
        if (property_exists($project, 'sparql_endpoint_url') && !in_array($project->sparql_endpoint_url, $new_project_urls)) {
          $sparql_endpoints_to_remove[] = $project->sparql_endpoint_url;
        }
      }
    }
    $this->config['projects'] = $projects;

    // Add a SPARQL-endpoint connection for every project.
    foreach ($this->config['projects'] as $project) {
      if (property_exists($project, 'sparql_endpoint_url')) {
        SemanticConnector::createConnection('sparql_endpoint', $project->sparql_endpoint_url, $project->title, $this->credentials, array());
      }
    }

    // Remove SPARQL-endpoints, that do not exist anymore.
    if (!empty($sparql_endpoints_to_remove)) {
      $delete_connection_ids = db_select('semantic_connector_connections', 'c')
        ->fields('c', array('connection_id'))
        ->condition('type', 'sparql_endpoint')
        ->condition('url', $sparql_endpoints_to_remove)
        ->execute()
        ->fetchCol();

      SemanticConnector::deleteConnections($delete_connection_ids);
    }

    parent::save();
  }

  public function getDefaultConfig() {
    return array();
  }

  public function getAPI($api_type = 'PPX') {
    switch ($api_type) {
      case 'PPX':
        return new SemanticConnectorPPXApi($this->url, ((!empty($this->credentials['username'])) ? $this->credentials['username'] . ':' . $this->credentials['password'] : ''));

      case 'PPT':
        return new SemanticConnectorPPTApi($this->url, ((!empty($this->credentials['username'])) ? $this->credentials['username'] . ':' . $this->credentials['password'] : ''));

      default:
        return NULL;
    }
  }
}

/**
 * A class for a connection to a sOnr-server.
 */
class SemanticConnectorSonrServerConnection extends SemanticConnectorConnection {
  public function __construct($connection_id = 0) {
    $this->type = 'sonr_server';
    parent::__construct($connection_id);

    // Incorrect connection was loaded --> Throw an error.
    if ($this->type != 'sonr_server') {
      // todo: throw an error.
    }
  }

  public function getDefaultConfig() {
    return array();
  }

  public function getAPI($api_type = '') {
    $sonrApi = new SemanticConnectorSonrApi($this->url, ((!empty($this->credentials['username'])) ? $this->credentials['username'] . ':' . $this->credentials['password'] : ''));
    $sonrApi->setId($this->id);
    return $sonrApi;
  }
}

/**
 * A class for a connection to a SPARQL-endpoint.
 */
class SemanticConnectorSparqlEndpointConnection extends SemanticConnectorConnection {
  public function __construct($connection_id = 0) {
    $this->type = 'sparql_endpoint';
    parent::__construct($connection_id);

    // Incorrect connection was loaded --> Throw an error.
    if ($this->type != 'sparql_endpoint') {
      // todo: throw an error.
    }
  }

  public function getDefaultConfig() {
    return array(
      'pp_server_id' => 0,
    );
  }

  public function getAPI($api_type = '') {
    if (!class_exists('ARC2')) {
      $arc2_path = 'sites/all/libraries/arc2';
      if (module_exists('libraries')) {
        $arc2_path = libraries_get_path('arc2');
      }

      // Includes the ARC2 library if it exists.
      if (!empty($arc2_path) && file_exists($arc2_path . '/ARC2.php')) {
        require_once $arc2_path . '/ARC2.php';
      }
      else {
        drupal_set_message(t('ARC2-Library is required for using a SPARQL endpoint.'), 'error');
        return NULL;
      }
    }

    $sparql_config = array(
      'remote_store_endpoint' => $this->url,
      'remote_store_timeout'  => 3);
    if (!empty($this->credentials['username'])) {
      $sparql_config['arc_reader_credentials'] = array($this->url => $this->credentials['username'] . '::' . $this->credentials['password']);
    }

    return ARC2::getRemoteStore($sparql_config);
  }
}
