<?php

/**
 * @file
 * The sOnr API-class.
 */

class SemanticConnectorSonrApi {

  protected $id;
  protected $endpoint;
  protected $credentials;
  protected $customAttributes;
  protected $customAttributesSelected;
  protected $error;

  /**
   * The constructor of the SonrApi-class.
   *
   * @param string $endpoint
   *   URL of the endpoint of the PoolParty-server.
   * @param string $credentials
   *   Username and password if required (format: "username:password").
   */
  public function __construct($endpoint, $credentials='') {
    $this->endpoint = $endpoint;
    $this->credentials = $credentials;
    $this->customAttributes = array(
      'source' => 'dyn_lit_source',
      'author' => 'dyn_lit_author',
      'sentiment' => 'dyn_flt_sentiment',
      'tags' => 'dyn_uri_all_concepts',
    );
    $this->customAttributesSelected = array(
      'dyn_lit_source',
      'dyn_lit_author',
    );
    $this->error = '';
  }

  /**********************************************************
   * public methods
   **********************************************************/

  /**
   * Sets the sonr configuration id.
   *
   * @param int $id
   *  The id of the sonr configuration
   */
  public function setId($id) {
    $this->id = $id;
  }
  /**
   * Returns the ID.
   *
   * @return int
   *   The id of the sonr configuration.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Adds a specific attribute to the search property "customAttributes"
   *
   * @param string $attribute
   *   The name of the attribute
   */
  public function addCustomAttribute($attribute) {
    if (isset($this->customAttributes[$attribute])) {
      $this->customAttributesSelected[] = $this->customAttributes[$attribute];
    }
  }

  /**
   * This method checks if the sOnr service exists and is running.
   *
   * @return bool
   *   TRUE if the service is available, FALSE if not
   */
  public function available() {
    $resource_path = '/sonr-backend/api/heartbeat';

    return $this->get($resource_path) === FALSE ? FALSE : TRUE;
  }

  /**
   * This method searches in the sOnr index.
   *
   * @param array $facets
   *   A list of facets that should be used for faceting the
   *   search. [optional]
   *    array(facedID_1, facedID_2)
   *
   * @param array $filters
   *   A list of filter object parameters that define the query. [optional]
   *    array(
   *      object(
   *        'field'    => (string)  facedID   | 'date-to' | 'date-from',
   *        'value'    => (int)     conceptID | timestamp | timestamp,
   *        'optional' => (boolean) TRUE, (default: TRUE)
   *      ),
   *      ...
   *    )
   *
   * @param array $parameters
   *   A list of key value pairs [optional]
   *    array(
   *      'facetMinCount' => (int)    1,     (default:    1)
   *      'locale'        => (string) 'en',  (default: 'en')
   *      'start'         => (int)    0,     (default:    0)
   *      'count'         => (int)    10,    (default:   10)
   *      'sort'          => object(
   *        'field'     => (string) facetID | 'date',
   *        'direction' => (string) 'DESC' | 'ASC',
   *      ),   (default: object('field' => 'date', 'direction' => 'DESC')
   *    )
   *
   * @return array
   *   List of items
   */
  public function search($facets = array(), $filters = array(), $parameters = array()) {
    $resource_path = '/sonr-backend/api/search';

    $sort = new stdClass();
    $sort->field = 'date';
    $sort->direction = 'DESC';

    $data = array(
      'facets' => $this->prepareFacets($facets),
      'filters' => $this->prepareFilters($filters),
      'customAttributes' => $this->customAttributesSelected,
      'facetMinCount' => isset($parameters['facetMinCount']) ? $parameters['facetMinCount'] : 1,
      'maxFacetCount' => isset($parameters['maxFacetCount']) ? $parameters['maxFacetCount'] : 10,
      'locale' => isset($parameters['locale']) ? $parameters['locale'] : 'en',
      'start' => isset($parameters['start']) ? $parameters['start'] : 0,
      'count' => isset($parameters['count']) ? $parameters['count'] : 10,
      'format' => 'json',
      'sort' => isset($parameters['sort']) ? $parameters['sort'] : $sort,
    );

    $variables['data'] = drupal_json_encode($data);
    if (($result = $this->post($resource_path, $variables)) === FALSE) {
      return FALSE;
    }

    return drupal_json_decode($result);
  }

  /**
   * Get concept suggestions from a given search string.
   *
   * @param string $search_string
   *   The string to get suggestions for
   * @param array $parameters
   *   array(
   *    'locale' => (string) 'en',  (default: 'en')
   *    'count'  => (int)    10,    (default:   10)
   *  )
   *
   * @return array
   *   Array of concepts
   *   array(
   *    'id'      => (string) URI of concept
   *    'label'   => (string) prefLabel of concept
   *    'context' => (string) label of conceptScheme
   *    'field'   => (string) URI of conceptScheme
   *  )
   */
  public function suggest($search_string, $parameters = array()) {
    $resource_path = '/sonr-backend/api/suggest';

    $query = array(
      'searchString' => $search_string,
      'locale' => isset($parameters['locale']) ? $parameters['locale'] : 'en',
      'count' => isset($parameters['count']) ? $parameters['count'] : 10,
      'format' => 'json',
    );

    $variables['query'] = $query;
    if (($result = $this->get($resource_path, $variables)) === FALSE) {
      return FALSE;
    }

    return drupal_json_decode($result);
  }

  /**
   * Get all project dependent facets.
   *
   * @return array
   *   A key value pair list of facets
   */
  public function getFacets() {
    $resource_path = '/sonr-backend/api/config';

    if (($result = $this->get($resource_path)) === FALSE) {
      return FALSE;
    }

    $data = drupal_json_decode($result);
    $facets = array();

    // Add project independent facets.
    $facets['dyn_lit_source'] = 'Source';
//    $facets['type'] = 'Type';

    // Add project dependent facets.
    foreach ($data['searchFields'] as $field) {
      if ($field['type'] == 'URI' && $field['defaultFacet'] == TRUE) {
        $facets[$field['name']] = $field['label'];
      }
    }

    return $facets;
  }

  /**
   * Get similar content.
   *
   * @param int $item_id
   *   The uri of the item
   * @param array $parameters
   *   Array of the parameters
   *
   * @return array
   *   A key value pair list of facets
   */
  public function getSimilar($item_id, $parameters = array()) {
    $resource_path = '/sonr-backend/api/similar';

    $query = array(
      'id' => $item_id,
      'locale' => isset($parameters['locale']) ? $parameters['locale'] : 'en',
      'count' => isset($parameters['count']) ? $parameters['count'] : 10,
      'format' => 'json',
      'fields' => 'dyn_uri_all_concepts',
      'customAttributes' => implode(',', $this->customAttributes),
    );

    $variables['query'] = $query;
    if (($result = $this->get($resource_path, $variables)) === FALSE) {
      return FALSE;
    }

    return drupal_json_decode($result);
  }

  /**
   * Get all agents with their configuration and status.
   *
   * @return array
   *   A list of agents with their configuration and status
   */
  public function getAgents() {
    $resource_path = '/sonr-backend/api/agents/status';

    if (($result = $this->get($resource_path)) === FALSE) {
      return FALSE;
    }

    $data = drupal_json_decode($result);
    $agents = array();
    foreach ($data as $id => $agent) {
      $agents[$id] = new stdClass;
      $agents[$id]->id = $agent['agent']['id'];
      $agents[$id]->configuration = $agent['agent']['configuration'];
      $agents[$id]->status = $agent['status'];
    }
    usort($agents, array($this, 'sortAgents'));

    return $agents;
  }

  /**
   * Get all agents that have feed items stored in the search index
   *
   * @return array
   *   A list of agents
   */
  public function getIndexedAgents() {
    $agents = array();

    // Make a simple search call with the source facet only
    $facets = array('dyn_lit_source');
    $parameters = array(
      'count' => 1,
      'start' => 0,
      'maxFacetCount' => 10000,
    );
    $search = $this->search($facets, array(), $parameters);
    if ($search == FALSE) {
      return array();
    }

    // Get the agents from the facet list
    $source_facet = $search['facetList'][0]['facets'];
    foreach ($source_facet as $source) {
      $agents[$source['label']] = $source['value'];
    }

    return $agents;
  }

  /**
   * Get one agent with his configuration.
   *
   * @param int $agent_id
   *   The ID of the agent
   *
   * @return array
   *   List of agents with their configuration
   */
  public function getAgent($agent_id) {
    $resource_path = '/sonr-backend/api/agents/%id';

    $variables['parameters'] = array(
      '%id' => $agent_id,
    );

    if (($result = $this->get($resource_path, $variables)) === FALSE) {
      return FALSE;
    }

    $data = drupal_json_decode($result);
    $data['id'] = $agent_id;

    return $data;
  }

  /**
   * Add a new agent.
   *
   * @param array $config
   *   array(
   *    'source'          => (string) 'EIP Water',
   *    'url'             => (string) 'http://eip-water.eu/rss.xml'
   *    'username'        => (string) 'admin',
   *    'privateContent'  => (boolean) FALSE,
   *    'periodMillis'    => (int) 3600000,
   *    'spaceKey'        => (string) 'extern',
   *  )
   *
   * @return bool
   *   TRUE on success, FALSE on error
   */
  public function addAgent($config) {
    $resource_path = '/sonr-backend/api/agents';

    $variables['data'] = drupal_json_encode($config);

    return $this->post($resource_path, $variables) === FALSE ? FALSE : TRUE;
  }

  /**
   * Update an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param array $config
   *   array(
   *    'source'          => (string) 'EIP Water',
   *    'url'             => (string) 'http://eip-water.eu/rss.xml'
   *    'username'        => (string) 'admin',
   *    'privateContent'  => (boolean) FALSE,
   *    'periodMillis'    => (int) 3600000,
   *    'spaceKey'        => (string) 'extern',
   *  )
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updateAgent($agent_id, $config) {
    $resource_path = '/sonr-backend/api/agents/%id';

    $variables['parameters'] = array(
      '%id' => $agent_id,
    );
    $variables['data'] = drupal_json_encode($config);

    return $this->put($resource_path, $variables) === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteAgent($agent_id) {
    $resource_path = '/sonr-backend/api/agents/%id';

    $variables['parameters'] = array('%id' => $agent_id);

    return $this->delete($resource_path, $variables) === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete all indexed documents from an agent.
   *
   * @param string $source
   *   The name of the source.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteIndex($source) {
    $resource_path = '/sonr-backend/api/pings/deleteAll';

    $variables['query'] = array(
      'source' => $source,
    );

    return $this->get($resource_path, $variables) === FALSE ? FALSE : TRUE;
  }

  /**
   * Run an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function runAgent($agent_id) {
    $resource_path = '/sonr-backend/api/agents/runAgent';

    $variables['query'] = array(
      'id' => $agent_id,
    );

    return $this->get($resource_path, $variables) === FALSE ? FALSE : TRUE;
  }

  /**
   * Get trends from a list of concepts.
   *
   * @param array $uris
   *   A list of uris of concepts.
   *
   * @return array
   *   List of trends.
   */
  public function getTrends($uris) {
    $resource_path = '/sonr-backend/api/trend/histories';

    if (is_string($uris)) {
      $uris = array($uris);
    }
    $variables['query'] = array(
      'concepts' => implode(',', $uris),
    );

    if (($result = $this->get($resource_path, $variables)) === FALSE) {
      return FALSE;
    }

    return drupal_json_decode($result);
  }

  /**
   * Get the last request error.
   *
   * @return string
   *   error (code - message).
   */
  public function error() {
    return $this->error;
  }


  /**********************************************************
   * protected help functions
   **********************************************************/

  /**
   * Converts facet list into a list of object parameters for the sOnr.
   *
   * @param array $facet_ids
   *   The list of facets.
   *
   * @return array
   *   Array of facet objects.
   */
  protected function prepareFacets($facet_ids) {
    $facets = array();
    foreach ($facet_ids as $facet_id) {
      $facets[] = array('field' => $facet_id);
    }

    return $facets;
  }

  /**
   * Maps filters into the defined filters or the sOnr.
   *
   * @param array $filters
   *   The list of filters.
   *
   * @return array
   *   Array of filter object parameters.
   */
  protected function prepareFilters($filters) {
    $dates = array();
    foreach ($filters as $key => $filter) {
      if ($filter->field == 'date-from') {
        $dates['from'] = $filter->value . 'T00:00:00Z';
        unset($filters[$key]);
      }
      if ($filter->field == 'date-to') {
        $dates['to'] = $filter->value . 'T00:00:00Z';
        unset($filters[$key]);
      }
    }
    $value = '';
    $value .= isset($dates['from']) ? $dates['from'] : '*';
    $value .= ' TO ';
    $value .= isset($dates['to']) ? $dates['to'] : '*';

    $item = new stdClass();
    $item->field = 'date';
    $item->value = '[' . $value . ']';

    $filters[] = $item;
    $filters = array_values($filters);

    return $filters;
  }

  /**
   * Sort agends by their source.
   *
   * @param array $a
   *   The first agent.
   * @param array $b
   *   The second agent.
   *
   * @return int
   *   The sort-comparison-value.
   */
  protected function sortAgents($a, $b) {
    return strcasecmp($a->configuration['source'], $b->configuration['source']);
  }

  /**********************************************************
   * protected request functions
   **********************************************************/

  // TODO: move the following methods and the same methods in the semantic_connector_ppx_api.class.inc into a separate class

  /**
   * Make a GET request.
   *
   * @param string $resource_path
   *   The path to the REST method.
   *
   *   You can include wildcards in the string
   *   which will be filled in by the $parameters array.
   *   Ex: /Courses/%session
   *
   * @param array $variables
   *   An array of variables with the following keys:
   *
   *   parameters [optional] : Key/value pairs of parameters to inject into the
   *   resource path to replace dynamic values
   *   Ex: array('%session' => 20111)
   *
   *   query [optional] : Key/value pairs of query string parameters
   *   Ex: array('personid' => 2896263)
   *
   *   headers [optional] : Key/value pairs of extra header data to include
   *   in the request
   *
   *   retry [optional] : Number of retries. Defaults to 3
   *
   *   timeout [optional] : Timeout in seconds. Defaults to 30
   *
   * @return object
   *   Returns an object containing the response data, FALSE otherwise.
   */
  protected function get($resource_path, $variables = array()) {
    return $this->call($resource_path, $variables, 'GET');
  }

  /**
   * Make a POST request.
   *
   * @param string $resource_path
   *   The path to the REST method.
   * @param array $variables
   *   An array of variables.
   *
   * @return object
   *   Returns an object containing the response data, FALSE otherwise.
   */
  protected function post($resource_path, $variables = array()) {
    if (isset($variables['data'])) {
      if (is_string($variables['data'])) {
        $variables['headers']['Content-Length'] = strlen($variables['data']);
      }
    }
    else {
      $variables['data'] = array();
      $variables['headers']['Content-Length'] = 0;
    }

    return $this->call($resource_path, $variables, 'POST');
  }

  /**
   * Make a PUT request.
   *
   * @param string $resource_path
   *   The path to the REST method.
   * @param array $variables
   *   An array of variables with the following keys:
   *
   * @return object
   *   Returns an object containing the response data, FALSE otherwise.
   */
  protected function put($resource_path, $variables = array()) {
    if (isset($variables['data'])) {
      if (is_string($variables['data'])) {
        $variables['headers']['Content-Length'] = strlen($variables['data']);
      }
    }
    else {
      $variables['data'] = array();
      $variables['headers']['Content-Length'] = 0;
    }

    return $this->call($resource_path, $variables, 'PUT');
  }

  /**
   * Make a DELETE request.
   *
   * @param string $resource_path
   *   The path to the REST method.
   * @param array $variables
   *   An array of variables with the following keys:
   *
   * @return object
   *   Returns an object containing the response data, FALSE otherwise.
   */
  protected function delete($resource_path, $variables = array()) {
    return $this->call($resource_path, $variables, 'DELETE');
  }

  /**
   * Basic request (Compatible with GET and DELETE).
   *
   * @param string $resource_path
   *   The path to the REST method.
   * @param array $variables
   *   An array of variables with the following keys:
   * @param string $method
   *   The request-method (GET, POST, PUT, DELETE)
   *
   * @return object
   *   The response object or FALSE on error
   */
  protected function call($resource_path, $variables = array(), $method = 'GET') {
    $variables['method'] = $method;

    // Initialize the cURL request.
    $ch = curl_init();

    // Set the default parameters for the cURL request.
    $this->setRequestDefaults($ch, $variables);

    // Prepare the URL parameters.
    if (isset($variables['parameters']) && !empty($variables['parameters'])) {
      $this->prepareUrlParameters($resource_path, $variables['parameters']);
    }

    // Build the URL.
    $url = $this->buildUrl($resource_path, $variables);
    curl_setopt($ch, CURLOPT_URL, $url);

    switch ($method) {
      case 'GET':
        curl_setopt($ch, CURLOPT_HTTPGET, TRUE);
        break;

      case 'POST':
        curl_setopt($ch, CURLOPT_POST, 1);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $variables['data']);
        break;

      case 'PUT':
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'PUT');
//        curl_setopt($ch, CURLOPT_HTTPHEADER, array('X-HTTP-Method-Override: PUT'));
        curl_setopt($ch, CURLOPT_POSTFIELDS, $variables['data']);
        break;

      case 'DELETE':
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'DELETE');
        break;
    }

    // Make the request.
    $response = curl_exec($ch);
    $error_code = curl_errno($ch);
    $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);

    // There has been an error.
    if ($error_code != 0) {
      // Log the error.
      $this->watchdog($method, $error_code, curl_error($ch), $url);
      $response = FALSE;
    }
    if ($http_code != 200 && $http_code != 304) {
      $this->watchdog($method, $http_code, 'HTTP response code error', $url);
      $response = FALSE;
    }

    // Close the cURL request.
    curl_close($ch);

    // No error occurred, return the response.
    return $response;
  }

  /**
   * Set the default parameters for the cURL request.
   *
   * @param resource $ch
   *   The cURL request object.
   * @param array $variables
   *   Array of variables.
   */
  protected function setRequestDefaults($ch, array &$variables) {
    // Set the credentials.
    if (!empty($this->credentials)) {
      curl_setopt($ch, CURLOPT_USERPWD, $this->credentials);
    }

    // Set timeout.
    if (!(isset($variables['timeout']) && is_numeric($variables['timeout']) && intval($variables['timeout']) >= 0)) {
      $variables['timeout'] = 30;
    }
    curl_setopt($ch, CURLOPT_TIMEOUT, $variables['timeout']);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

    // Headers.
    $variables['headers']['Content-Type'] = 'application/json;charset=UTF-8';
    $variables['headers']['Accept'] = 'application/json';

    if (isset($variables['headers']) && is_array($variables['headers'])) {
      $headers = array();
      foreach ($variables['headers'] as $key => $value) {
        $headers[] = trim($key) . ": " . trim($value);
      }
      curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
    }
  }

  /**
   * Encodes and appends any URL parameters to the resource path.
   *
   * @param string $resource_path
   *   Resource .
   * @param array $parameters
   *   Array of URL parameters.
   */
  protected function prepareUrlParameters(&$resource_path, array $parameters) {
    // URL Encode all parameters.
    foreach ($parameters as $key => $param) {
      $parameters[$key] = urlencode($param);
    }

    // Add the parameters to the resource path.
    $resource_path = strtr($resource_path, $parameters);
  }

  /**
   * Build the URL to connect to.
   *
   * @param string $resource_path
   *   Base path.
   * @param array $variables
   *   Configuration variables.
   *
   * @return string
   *   Returns a full URL.
   */
  protected function buildUrl($resource_path, array $variables) {
    if (!empty($resource_path)) {
      $url = $this->endpoint . $resource_path;
    }
    else {
      $url = $this->endpoint;
    }

    // Set the options to be used by url().
    $options = array(
      'query' => isset($variables['query']) ? $variables['query'] : '',
      'absolute' => TRUE,
      'alias' => TRUE,
      'external' => TRUE,
    );

    // TODO: find a way to skip hook_url_outbound or migrate url() function
    // to internal function.
    $url = url($url, $options);

    return $url;
  }

  /**
   * Log error messages into the watchdog.
   *
   * @param string $method
   *   Request method.
   * @param int $code
   *   Error code.
   * @param string $error
   *   Error string.
   * @param string $url
   *   Request URL.
   * @param array $extra
   *   Extra data to show in the log.
   */
  protected function watchdog($method, $code, $error, $url, $extra = array()) {
    $debug = "";
    if (!empty($extra)) {
      $debug .= "\n" . '<pre>' . print_r($extra, TRUE) . '</pre>';
    }
    $this->error = $code . ' - ' . $error;

    watchdog(
      'sOnr API', 'Error making @method request (@code @message): @url !debug',
      array(
        '@method' => $method,
        '@code' => $code,
        '@message' => $error,
        '@url' => $url,
        '!debug' => $debug,
      ),
      WATCHDOG_ERROR
    );
  }
}
