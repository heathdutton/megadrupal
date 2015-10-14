<?php

/**
 * @file
 * Main services client connection class which exposes simple API
 */

class ServicesClientConnection {

  /**
   * Connection definition
   *
   * @var stdClass
   */
  protected $connection;

  /**
   * Authentication plugin
   *
   * @var ServicesClientConnectionAuth
   */
  protected $auth;

  /**
   * Remote server
   *
   * @var ServicesClientConnectionServer
   */
  protected $server;

  /**
   * Request for handling HTTP request / response
   *
   * @var ServicesClientConnectionRequest
   */
  protected $request;

  /**
   * HTTP Request
   *
   * @var ServicesClientConnectionHttpRequest
   */
  protected $http_request;

  /**
   * Response from server for debugging purposes
   *
   * @var ServicesClientConnectionResponse
   */
  protected $response;

  /**
   * Initialize connection to remote Drupal site
   *
   * @param $connection
   *   Connection object from DB or File
   */
  public function __construct($connection) {
    $this->connection = $connection;

    // Create authentication plugin
    $this->auth = $this->getPlugin('auth');

    // Create server plugin
    $this->server = $this->getPlugin('server');

    // Create
    $this->request = $this->getPlugin('request');

    // Connect
    $this->auth->connect();

    // Authenticate
    $this->auth->login();
  }

  /**
   * Destructor
   */
  public function __destruct() {
    $this->close();
  }

  /**
   * Close connection properly
   */
  public function close() {
    $this->auth->logout();
  }

  /**
   * Retrieve connection definition object.
   *
   * @return stdClass
   *   Exportable definition of connection.
   */
  public function getDetails() {
    return $this->connection;
  }

  /**
   * Loads required class, initialized object by connection configuration.
   *
   * @param string $type
   *   Type of required plugin.
   * @return ServicesClientConnectionPlugin
   */
  protected function getPlugin($type) {
    // Load plugin class
    $name = $this->connection->config[$type]['plugin'];

    return services_client_connection_get_plugin($type, $name, $this->connection, $this->connection->config[$type]['config'], $this);
  }

  /**
   * Helper method for creating
   *
   * @param array $options
   *   Options for Request. @see ServicesClientConnectionRequest for
   *   all keys that can be passed. Each request attribute can be passed
   *   as option key.
   * @return ServicesClientConnectionHttpRequest
   *   Initialized request
   */
  protected function createRequest($options = array()) {
    // Create new request
    $request = new ServicesClientConnectionHttpRequest();

    // Set options
    foreach ($options as $key => $val) {
      if (property_exists($request, $key)) {
        $request->{$key} = $val;
      }
    }

    return $request;
  }

  /**
   * Process HTTP Request. This method will fire all hooks on different plugins
   * which will create final request. Final request is sent to Request plugin.
   *
   * @param $http_request
   */
  protected function processRequest($http_request = NULL) {

    // Ensure we have proper http request that is processed
    if (empty($http_request)) {
      $http_request = $this->http_request;
    }

    $this->server->prepareRequest($http_request);
    $this->request->prepareRequest($http_request);
    $this->auth->prepareRequest($http_request);

    $this->http_request = $http_request;

    // Allow other modules to react
    drupal_alter('services_client_connection_request', $this->http_request);

    $this->response = $this->request->call($this->http_request);

    $this->server->processResponse($this->response);
    $this->auth->processResponse($this->response);

    // Log data to watchdog
    $this->debug();

    // If response is error generate new exception
    if ($this->response->error_code) {
      throw new ServicesClientConnectionResponseException($this->response, $this->http_request);
    }

    drupal_alter('services_client_connection_response', $this->response, $this->http_request);

    return $this->response;
  }

  /**
   * Debug last request and response
   */
  protected function debug() {
    if ($this->connection->debug) {
      watchdog('scc_debug', 'DEBUG: <pre>!data</pre>', array(
        '!data' => print_r($this->getDebug(), TRUE),
      ));
    }
  }

  /**
   * Return last response for debugging purposes
   *
   * @return ServicesClientConnectionResponse
   */
  public function getDebug() {
    return array(
      'request' => $this->http_request,
      'response' => $this->response,
    );
  }

  /**
   * Returns last HTTP request made by connection class.
   *
   * @return ServicesClientConnectionHttpRequest
   */
  public function getRequest() {
    return $this->http_request;
  }

  /**
   * Retrieve response from last operation
   *
   * @return ServicesClientConnectionResponse
   */
  public function getResponse() {
    return $this->response;
  }

  /**
   * Create data on remote endpoint.
   *
   * @param string $resource
   *   Resource type like 'node' or 'user'
   * @param mixed $data
   *   Optionally additional data that should be added to query
   * @param array $query
   *   Additional data passed as URL param
   * @return
   *   Response from remote API
   */
  public function create($resource, $data = array(), $query = array()) {
    $this->http_request = $this->createRequest(array(
      'resource' => $resource,
      'data' => $data,
      'query' => $query,
      'action' => 'create'
    ));

    return $this->processRequest()->data;
  }

  /**
   * Retrieve data from remote endpoint.
   *
   * @param string $resource
   *   Resource type like 'node' or 'user'
   * @param int $id
   *   Id of remote resource
   * @param mixed $data
   *   Optionally additional data that should be added to query
   * @return
   *   Response from remote API
   */
  public function get($resource, $id = NULL, $data = array()) {
    $this->http_request = $this->createRequest(array(
      'resource' => $resource,
      'id' => $id,
      'data' => $data,
      'action' => 'retrieve'
    ));

    return $this->processRequest()->data;
  }

  /**
   * Update data from remote endpoint.
   *
   * @param string $resource
   *   Resource type like 'node' or 'user'
   * @param int $id
   *   Id of remote resource
   * @param mixed $data
   *   Optionally additional data that should be added to query
   * @return
   *   Response from remote API
   */
  public function update($resource, $id = NULL, $data = array()) {
    $this->http_request = $this->createRequest(array(
      'resource' => $resource,
      'id' => $id,
      'data' => $data,
      'action' => 'update'
    ));

    return $this->processRequest()->data;
  }

  /**
   * Delete data from remote endpoint.
   *
   * @param string $resource
   *   Resource type like 'node' or 'user'
   * @param int $id
   *   Id of remote resource
   * @return
   *   Response from remote API
   */
  public function delete($resource, $id = NULL) {
    $this->http_request = $this->createRequest(array(
      'resource' => $resource,
      'id' => $id,
      'action' => 'delete'
    ));

    return $this->processRequest()->data;
  }

  /**
   * List resources from remote endpoint.
   *
   * @param $resource
   *   Resource type i.e. 'user' or 'node'
   * @param $fields
   *   List of fields that should be retrieved for resource. Can be either string
   *   of comma separted values like 'nid,title,uid,created' or array which will
   *   be automatically converted to list. Default is all fields '*'.
   * @param $parameters
   *   Filter resources by params.
   * @param $page
   *   Page number - default 0
   * @return
   *   List of remote resources
   */
  public function index($resource, $fields = '*', $parameters = array(), $page = 0) {
    $data = array();

    if (!empty($parameters)) {
      $data['parameters'] = $parameters;
    }

    if (is_array($fields)) {
      $fields = implode(',', $fields);
    }
    $data['fields'] = $fields;

    if (!empty($page)) {
      $data['page'] = $page;
    }

    $this->http_request = $this->createRequest(array(
      'resource' => $resource,
      'http_method' => 'GET',
      'data' => $data,
    ));

    return $this->processRequest()->data;
  }

  /**
   * Execute action on remote resource
   *
   * @param $resource
   *   Resource type i.e. 'user' or 'node'
   * @param $action
   *   Action name i.e. 'login' or 'publish'
   * @param $data
   *   Associative array of additional data that should be passed to call
   * @param $options
   *   Extra options of request
   */
  public function action($resource, $action, $data = array(), $options = array()) {
    $this->http_request = $this->createRequest(array(
      'resource' => $resource,
      'action' => $action,
      'data' => $data
    ));

    return $this->processRequest()->data;
  }

  /**
   * Target action on specific resource
   *
   * @param $resource
   *   Resource type i.e. 'user' or 'node'
   * @param $id
   *   Optionally this can be id of the resource, call would look like
   *   $client->action('node', 23, 'publish');
   * @param $action
   *   Action name i.e. 'login' or 'publish'
   * @param $data
   *   Associative array of additional data that should be passed to call
   * @param $options
   *   Extra options of request
   */
  public function targetAction($resource, $id, $action, $data = array(), $options = array()) {
    $this->http_request = $this->createRequest(array(
      'resource' => $resource,
      'action' => $action,
      'id' => $id,
      'data' => $data
    ));

    return $this->processRequest()->data;
  }

  /**
   * Load resource related data
   *
   * @param $resource
   *   Resource type i.e. 'user' or 'node'
   * @param $id
   *   Optionally this can be id of the resource, call would look like
   *   $client->action('node', 23, 'publish');
   * @param $relation
   *   Related data i.e. for node comments
   * @param $data
   *   Associative array of additional data that should be passed to call
   * @param $options
   *   Extra options of request
   */
  public function relationships($resource, $id, $relation, $data = array()) {
    $this->http_request = $this->createRequest(array(
      'resource' => $resource,
      'relation' => $relation,
      'id' => $id,
      'data' => $data,
    ));

    return $this->processRequest()->data;
  }

  /**
   * Make low level HTTP request which allows to make calls to non-services
   * REST only endpoints.
   *
   * @param $method
   *   HTTP method - GET, POST, DELETE, PUT
   * @param $path
   *   Requested path
   * @param $query
   *   GET query parameters
   * @param $data
   *   DATA that needs to be posted - all except GET requests
   * @param $headers
   *   Optionally additional HTTP headers
   * @return
   */
  protected function httpRequest($method, $path, $query = array(), $data = array(), $headers = array()) {
    // Raw HTTP requests will only work with REST Request plugin
    if (!is_a($this->server, 'ServicesClientConnectionRestServer')) {
      throw new ServicesClientConnectionException(t("Raw HTTP requests require ServicesClientConnectionRestServer plugin."));
    }

    // Normalize http method
    $method = strtoupper($method);

    // Some methods shoudln't carry any data
    if (!empty($data) && in_array($method, array('GET', 'DELETE', 'OPTIONS'))) {
      throw new ServicesClientConnectionException(t("Method @name can't carry data in request body, only in query.", array(
        '@name' => $method
      )));
    }

    $this->http_request = $this->createRequest(array(
      'http_method' => $method,
      'url' => $path,
      'query' => $query,
      'data' => $data,
      'http_headers' => $headers,
    ));

    return $this->processRequest()->data;
  }

  /**
   * HTTP GET method
   *
   * @param $path
   *   Requested path relative to endpoint configuration
   * @param $query
   *   URL query parameters
   * @param $headers
   *   Optionally additional headers
   * @return
   *   Response data
   */
  public function rawGet($path, $query = array(), $headers = array()) {
    return $this->httpRequest('GET', $path, $query, array(), $headers);
  }

  /**
   * HTTP POST method
   *
   * @param $path
   *   Requested path relative to endpoint configuration
   * @param $query
   *   URL query parameters
   * @param $data
   *   Request data
   * @param $headers
   *   Optionally additional headers
   * @return
   *   Response data
   */
  public function rawPost($path, $query = array(), $data = array(), $headers = array()) {
    return $this->httpRequest('POST', $path, $query, $data, $headers);
  }

  /**
   * HTTP PUT method
   *
   * @param $path
   *   Requested path relative to endpoint configuration
   * @param $query
   *   URL query parameters
   * @param $data
   *   Request data
   * @param $headers
   *   Optionally additional headers
   * @return
   *   Response data
   */
  public function rawPut($path, $query = array(), $data = array(), $headers = array()) {
    return $this->httpRequest('PUT', $path, $query, $data, $headers);
  }

  /**
   * HTTP DELETE method
   *
   * @param $path
   *   Requested path relative to endpoint configuration
   * @param $query
   *   URL query parameters
   * @param $headers
   *   Optionally additional headers
   * @return
   *   Response data
   */
  public function rawDelete($path, $query = array(), $headers = array()) {
    return $this->httpRequest('DELETE', $path, $query, array(), $headers);
  }

  /**
   * HTTP PATCH method
   *
   * @param $path
   *   Requested path relative to endpoint configuration
   * @param $query
   *   URL query parameters
   * @param $data
   *   Request data
   * @param $headers
   *   Optionally additional headers
   * @return
   *   Response data
   */
  public function rawPatch($path, $query = array(), $data = array(), $headers = array()) {
    return $this->httpRequest('PATCH', $path, $query, $data, $headers);
  }

  /**
   * HTTP OPTIONS method
   *
   * @param $path
   *   Requested path relative to endpoint configuration
   * @param $query
   *   URL query parameters
   * @param $headers
   *   Optionally additional headers
   * @return
   *   Response data
   */
  public function rawOptions($path, $query = array(), $headers = array()) {
    return $this->httpRequest('OPTIONS', $path, $query, array(), $headers);
  }
}
