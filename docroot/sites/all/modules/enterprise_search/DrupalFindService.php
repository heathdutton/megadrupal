<?php
/**
 * @file
 * Extends the Drupal_Apache_Solr_Service to make it compatible with
 * the A12 Find service.
 *
 * @author Axis12 Ltd. <technical@axistwelve.com>
 */

class DrupalFindService extends DrupalApacheSolrService {
  /**
   * Servlet mappings
   */
  const DELETE_SERVLET = 'delete';
  const DELETE_QUERY_SERVLET = 'delete/query';
  const SOLR_NAMESPACE = 'druA12';
  const ADMIN_SERVLET = 'admin/servers';
  const ELEVATE_SERVLET = 'admin/elevate';
  const SYNONYMS_SERVLET = 'admin/synonyms';

  protected $connector;
  protected $core;
  protected $index;

  /**
   * Override the default constructor.
   *
   * Append the currently selected index and core to the Solr URL.
   */
  public function __construct($url, $env_id = NULL) {
    parent::__construct($url, $env_id);
    $connector = a12_connect_get_connector();
    $this->setConnector($connector);
    $index = variable_get("enterprise_search_apachesolr_index", "default");
    $core = variable_get("enterprise_search_apachesolr_core", "DEV");
    $this->setIndex($index, $core);
    $this->setUrl($this->getUrl() . "/$index/$core");
  }

  /**
   * Set the connector.
   */
  public function setConnector(A12Connector $connector) {
    $this->connector = $connector;
  }

  /**
   * Set the index and core.
   */
  public function setIndex($index, $core) {
    $this->index = $index;
    $this->core = $core;
  }

  /**
   * Get all the indicies that the user controls.
   */
  public function getIndicies() {
    $indicies = array();
    try {
      $response = $this->makeServletRequest('admin/servers');
      $data = $this->decodeResponse($response);
      if (isset($data->indexes)) {
        foreach ($data->indexes as $index) {
          $indicies[$index->name] = array(
            'label' => $index->label,
            'description' => $index->description,
          );
        }
      }
    }
    catch (A12ConnectorException $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
    catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');  
    }

    return $indicies;
  }

  /**
   * Helper function to decode the response from Solr.
   */
  public function decodeResponse($response) {
    return json_decode($response->data);
  }

  /**
   * Override the ping() method.
   *
   * This needs to happen because it is called immediately on initalization by
   * the ApacheSolr module. At this point we have not been able to set the
   * A12 Connector we can't send a request successfully. To get around this we
   * check if the connector has been set and if it has we can execute a ping.
   */
  public function ping($timeout = 2) {
    if ($this->connector) {
      parent::ping($timeout);
    }
    return TRUE;
  }

  /**
   * Override _makeHttpRequest method.
   *
   * Attach authentication headers and check the response for Find specific
   * errors.
   *
   * @param string $url
   *   The URL we are making the request to
   * @param array $options
   *   An array of options to be passed to drupal_http_request()
   */
  protected function _makeHttpRequest($url, array $options = array()) {
    $options['headers'] = $this->connector->getHeaders($url);
    $options['User-Agent'] = ENTERPRISE_SEARCH_USER_AGENT;
    $response = parent::_makeHttpRequest($url, $options);
    if (isset($response->status) && $response->status == "error") {
      $result = $this->decodeResponse($response);
      throw new A12ConnectorException($result->error_code);
    }

    return $response;
  }

  /**
   * Override addDocument method.
   *
   * @todo Extend to include all schema type and cast as appropriate
   */
  public function addDocuments($documents, $overwrite = NULL, $commit_within = NULL) {
    $records = array();

    foreach ($documents as $document) {
      $field = array();
      $xml = simplexml_load_string(ApacheSolrDocument::documentToXml($document));
      foreach ($xml->children() as $element) {
        $attributes = $element->attributes();
        $name = $attributes['name'];
        $key = self::SOLR_NAMESPACE . '$' . (string) $name;
        $value = (string) $element;
        $solr_type = explode('_', $name);
        switch ($solr_type[0]) {
          case "bs":
            $field[$key] = (bool) $value;
            break;

          case 'bm':
            $field[$key][] = (bool) $value;
            break;

          case 'access':
            $field[$key][] = intval($value);
            break;

          case 'fm':
          case 'ftm':
          case 'fsm':
            $field[$key][] = floatval($value);
            break;

          case 'ghm':
          case 'hsm':
          case 'htm':
          case 'im':
          case 'ism':
          case 'itm':
            $field[$key][] = intval($value);
            break;

          case 'pm':
          case "ptm":
          case "psm":
            $field[$key][] = (double) $value;
            break;

          case 'sm':
            $field[$key][] = (string) $value;
            break;

          case 'tid':
            $field[$key][] = intval($value);
            break;

          case 'dm':
          case 'ddm':
          case 'taxonomy':
          case 'tem':
          case 'tom':
          case 'tm':
          case 'tum':
          case 'twm':
          case 'xm':
            $field[$key][] = (string) $value;
            break;

          case 'ss':
            $field[$key] = (string) $value;
            break;

          case 'is':
            if (!empty($value)) {
              $field[$key] = intval($value);
            }
            break;

          default:
            if (is_numeric($value) && $key != 'druA12$comment_count') {
              $field[$key] = intval($value);
            }
            else {
              $field[$key] = $value;
            }
        }

        if ($name == 'entity_id') {
          $field[$key] = (string) $element;
          $entity_id = intval((string) $element);
        }

        if ($name == 'id') {
          $field[$key] = (string) $element;
          $id = (string) $element;
        }
      }

      $record = array();
      $record['fields'] = (object) $field;

      if (isset($id)) {
        $record['id'] = $id;
      }

      if (isset($entity_id)) {
        $doc_data = new stdClass();
        $doc_data->nid = $document->entity_id;
        $doc_data->language = $document->language;
        $record['id'] = $this->generateDocumentId($doc_data);
        $record['fields']->id = $record['id'];
      }
      $records[] = $record;
    }

    $raw_post = array();
    $raw_post['namespaces'] = array(
      'com.axistwelve.drupalschema' => self::SOLR_NAMESPACE,
    );
    $raw_post['records'] = $records;
    $this->update(json_encode($raw_post));
  }

  /**
   * Override setStats method.
   *
   * Send and receive data through JSON.
   */
  protected function setStats() {
    $data = $this->getLuke();
    $cid = "$this->env_id:stats:$this->index:$this->core";
    $cache = cache_get($cid, 'cache_apachesolr');
    if (isset($cache->data) && !$flush) {
      $this->stats = $cache->data;
    }
    else {
      $response = $this->makeServletRequest(self::STATS_SERVLET);
      $this->stats = $this->decodeResponse($response);
      cache_set($cid, $this->stats, 'cache_apachesolr');
    }
  }

  /**
   * Sets $this->luke with the meta-data about the index from admin/luke.
   */
  protected function setLuke($num_terms = 0) {
    if (empty($this->luke[$num_terms])) {
      $params = array(
        'numTerms' => "$num_terms",
        'wt' => 'json',
        'json.nl' => self::NAMED_LIST_FORMAT,
      );
      $cid = "$this->env_id:luke:$this->index:$this->core";
      $cache = cache_get($cid, 'cache_apachesolr');
      if (isset($cache->data)) {
        $this->luke = $cache->data;
      }
      else {
        $this->luke[$num_terms] = $this->makeServletRequest(self::LUKE_SERVLET, $params);
        cache_set($cid, $this->luke, 'cache_apachesolr');
      }
    }
  }

  /**
   * Override getStatsSummary method.
   */
  public function getStatsSummary() {
    $stats = $this->getStats();
    return (array) $stats;
  }

  /**
   * Create a delete document based on document ID.
   *
   * @param string $id
   *   Expected to be utf-8 encoded
   * @param float $timeout
   *   Maximum expected duration of the delete operation on the server
   *   (otherwise, will throw a communication exception)
   *
   * @return object
   *   Response object.
   *
   * @throws Exception If an error occurs during the service call
   */
  public function deleteById($id, $timeout = 3600) {
    return $this->deleteByMultipleIds(array($id), $timeout);
  }

  /**
   * Raw update Method.
   *
   * @param string $raw_put
   *   The raw put data.
   * @param float $timeout
   *   Maximum expected duration (in seconds).
   *
   * @return object
   *   Response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function update($raw_put, $timeout = FALSE) {
    // @todo: throw exception if updates are disabled.
    if (empty($this->update_url)) {
      // Store the URL in an instance variable since many updates may be sent
      // via a single instance of this class.
      $this->update_url = $this->_constructUrl(self::UPDATE_SERVLET, array('wt' => 'json'));
    }
    $options['method'] = 'PUT';
    $options['data'] = $raw_put;
    if ($timeout) {
      $options['timeout'] = $timeout;
    }
    return $this->_makeHttpRequest($this->update_url, $options);
  }

  /**
   * Create and post a delete document based on multiple document IDs.
   *
   * @param array $ids
   *   Expected to be utf-8 encoded strings
   * @param float $timeout
   *   Maximum expected duration of the delete operation on the server
   *   (otherwise, will throw a communication exception)
   *
   * @return object
   *   Response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function deleteByMultipleIds($ids, $timeout = 3600) {
    $record_ids = array();
    foreach ($ids as $id) {
      $node = new stdClass();
      $node->id = $id;
      $record_ids[] = $this->generateDocumentId($node);
    }
    if (count($record_ids) == 1) $record_ids = $record_ids[0];

    $options['method'] = 'DELETE';
    $options['data'] = json_encode($record_ids);
    if ($timeout) {
      $options['timeout'] = $timeout;
    }
    $url = $this->_constructUrl(self::DELETE_SERVLET);
    return $this->_makeHttpRequest($url, $options);
  }

  /**
   * Create a delete document based on a query and submit it.
   *
   * @param string $raw_query
   *   Expected to be utf-8 encoded
   * @param float $timeout
   *   Maximum expected duration of the delete operation on the server
   *   (otherwise, will throw a communication exception)
   *
   * @return object
   *   Response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function deleteByQuery($raw_query, $timeout = 3600) {
    $query_parts = explode(' ', $raw_query);
    foreach ($query_parts as $pos => $part) {
      if (strstr($part, 'sm_parent_document_id:')) {
        $nd = new stdClass();
        $nd->id = str_replace('sm_parent_document_id:', '', $part);
        $nd->id = str_replace('"', '', $nd->id);
        $query_parts[$pos] = sprintf('%s%s', 'sm_parent_document_id:', $this->generateDocumentId($nd));
        $id_parts = explode('/', $nd->id);
        if ($id_parts[1] != 'node') {
          $query_parts[] = 'OR';
          $add_node_id = new stdClass();
          $add_node_id->id = sprintf('%s/node/%s', $id_parts[0], $id_parts[2]);
          $query_parts[] = sprintf('%s%s', 'sm_parent_document_id:', $this->generateDocumentId($add_node_id));
        }
      }
      elseif (strstr($part, 'id:')) {
        $nd = new stdClass();
        $nd->id = str_replace('id:', '', $part);
        $nd->id = str_replace('"', '', $nd->id);
        $query_parts[$pos] = sprintf('%s%s', 'id:', $this->generateDocumentId($nd));
        $id_parts = explode('/', $nd->id);
        if ($id_parts[1] != 'node') {
          $query_parts[] = 'OR';
          $add_node_id = new stdClass();
          $add_node_id->id = sprintf('%s/node/%s', $id_parts[0], $id_parts[2]);
          $query_parts[] = sprintf('%s%s', 'id:', $this->generateDocumentId($add_node_id));
        }
      }
    }
    $raw_query = implode(' ', $query_parts);
    $options['method'] = 'DELETE';
    $options['data'] = $raw_query;
    if ($timeout) {
      $options['timeout'] = $timeout;
    }
    $url = $this->_constructUrl(self::DELETE_QUERY_SERVLET);
    return $this->_makeHttpRequest($url, $options);
  }

  /**
   * Generate a unique Solr document id for the document.
   */
  public function generateDocumentId($node, $entity_type = 'node') {
    $vars = array();
    if (!empty($node->language)) {
      $vars[] = 'language=' . $node->language;
    }
    else {
      $vars[] = 'language=und';
    }

    $vars[] = "name=$this->index";
    $vars[] = "type=$this->core";

    if (isset($node->id)) {
      $id = $node->id;
    }
    else {
      $id = apachesolr_document_id($node->nid, $entity_type);
    }

    $id = str_ireplace('/', '-', $id);
    $vars = implode(',', $vars);
    return sprintf('%s.%s', $id, $vars);
  }

  /**
   * Set elevation method.
   *
   * @param string $elevation_payload
   *   The json encoded data.
   * @param float $timeout
   *   Maximum expected duration (in seconds).
   *
   * @return object
   *   Response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function setElevate($elevation_payload, $timeout = FALSE) {
    if (empty($this->elevation_url)) {
      // Store the URL in an instance variable since many updates may be sent
      // via a single instance of this class.
      $this->elevation_url = $this->_constructUrl(self::ELEVATE_SERVLET, array('wt' => 'json'));
    }
    $options['method'] = 'PUT';
    $options['data'] = $elevation_payload;
    if ($timeout) {
      $options['timeout'] = $timeout;
    }
    return $this->_makeHttpRequest($this->elevation_url, $options);
  }

  /**
   * Set synonyms method.
   *
   * @param string $synonyms_payload
   *   The json encoded data.
   * @param float $timeout
   *   Maximum expected duration (in seconds).
   *
   * @return object
   *   Response object
   *
   * @throws Exception If an error occurs during the service call
   */
  public function setSynonyms($synonyms_payload, $timeout = FALSE) {
    if (empty($this->synonyms_url)) {
      // Store the URL in an instance variable since many updates may be sent
      // via a single instance of this class.
      $this->elevation_url = $this->_constructUrl(self::SYNONYMS_SERVLET, array('wt' => 'json'));
    }
    $options['method'] = 'PUT';
    $options['data'] = $synonyms_payload;
    if ($timeout) {
      $options['timeout'] = $timeout;
    }
    return $this->_makeHttpRequest($this->synonyms_url, $options);
  }

  /**
   * Simple Search interface.
   *
   * @param string $query
   *   The raw query string
   * @param array $params 
   *   key / value pairs for other query parameters (see Solr documentation),
   *   use arrays for parameter keys used more than once (e.g. facet.field).
   *
   * @return object
   *   The search response.
   *
   * @throws Exception If an error occurs during the service call
   */
  public function search($query = '', array $params = array(), $method = 'GET') {
    // Make an exception for CSV.
    if (isset($params['wt']) && is_array($params['wt']) && $params['wt'][0] ==
    'csv') {
      $params['wt'] = 'csv';
    }
    else {
      $params['wt'] = 'json';
    }

    // Additional default params.
    $params += array(
      'json.nl' => self::NAMED_LIST_FORMAT,
    );
    if ($query) {
      $params['q'] = $query;
    }

    // PHP's built in http_build_query() doesn't give us the format Solr wants.
    $query_string = $this->httpBuildQuery($params);
    // Check string length of the query string, change method to POST.
    $len = strlen($query_string);
    // Fetch our threshold to find out when to flip to POST.
    $max_len = apachesolr_environment_variable_get($this->env_id, 'apachesolr_search_post_threshold', 32768);

    // If longer than $max_len (default 8192) characters
    // we should switch to POST (a typical server handles 4096 max).
    // If this class is used independently (without environments), we switch
    // automatically to POST at an limit of 1800 chars.
    if (($len > 32768) && (empty($this->env_id) || ($len > $max_len))) {
      $method = 'POST';
    }

    if ($method == 'GET') {
      $search_url = $this->_constructUrl(self::SEARCH_SERVLET, array(), $query_string);
      return $this->_sendRawGet($search_url);
    }
    elseif ($method == 'POST') {
      $search_url = $this->_constructUrl(self::SEARCH_SERVLET);
      $options['data'] = $query_string;
      $options['headers']['Content-Type'] = 'application/x-www-form-urlencoded; charset=UTF-8';
      return $this->_sendRawPost($search_url, $options);
    }
    else {
      throw new Exception("Unsupported method '$method' for search(), use GET or POST");
    }
  }

}
