<?php

/**
 * @file
 * The PoolParty PPX API-class.
 */

class SemanticConnectorPPXApi {

  protected $connection;

  /**
   * The constructor of the PoolParty Extractor class.
   *
   * @param string $endpoint
   *   URL of the endpoint of the PoolParty-server.
   * @param string $credentials
   *   Username and password if required (format: "username:password").
   */
  public function __construct($endpoint, $credentials = '') {
    $this->connection = new SemanticConnectorCurlConnection($endpoint, $credentials);
  }

  /**
   * Get the configured cURL-connection.
   *
   * @return SemanticConnectorCurlConnection
   *   The connection object.
   */
  public function getConnection() {
    return $this->connection;
  }

  /**********************************************************
   * public methods
   **********************************************************/

  /**
   * This method checks if the PoolParty server exists and is running.
   *
   * @return array
   *   Associative array which following properties:
   *   - success (boolean): TRUE if a connection to the server can be
   *     established.
   *   - message (string): This property is optional, but if it exists it
   *     includes more details about why the connection could not be
   *     established.
   */
  public function available() {
    $is_available = NULL;
    drupal_alter('semantic_connector_ppx_available', $this, $is_available);

    if (is_null($is_available)) {
      $is_available = array(
        'success' => NULL,
      );
      $resource_path = '/extractor/api/heartbeat';
      $result = (json_decode($this->connection->get($resource_path)));

      if (is_null($result)) {
        $is_available['success'] = FALSE;
      }
      else {
        $is_available['success'] = $result->success;
        if (property_exists($result, 'message')) {
          $is_available['message'] = $result->message;
        }
      }
    }

    return $is_available;
  }

  /**
   * Get a list of available projects of a PoolParty server.
   *
   * @return bool
   *   TRUE if the service is available, FALSE if not.
   */
  public function getProjects() {
    // Offer the possibility to support a different value for this function.
    $projects = NULL;
    drupal_alter('semantic_connector_ppx_getProjects', $this, $projects);

    if (is_null($projects)) {
      $resource_path = '/extractor/api/projects';
      $result = $this->connection->get($resource_path);
      $projects = json_decode($result);
      if (!is_null($projects) && property_exists($projects, 'projects')) {
        $projects = $projects->projects;
      }
    }

    $default_project = array(
      'label' => 'Default project',
      'uuid' => '',
      'defaultLanguage' => 'en',
      'languages' => array('en'),
    );

    if (!is_null($projects)) {
      foreach ($projects as &$project) {
        $project = (object) (array_merge($default_project, (array) $project));
      }
    }
    else {
      $projects = array();
    }

    return $projects;
  }

  /**
   * Get a list of available projects of a PoolParty server.
   *
   * @param object $data
   *   Can be either a string for normal text-extraction of a file-object for
   *   text extraction of the file content.
   * @param string $language
   *   The iso-code of the text's language.
   * @param array $parameters
   *   Additional parameters to forward to the API (e.g. projectId).
   *
   * @return object
   *   Object of concepts.
   */
  public function extractConcepts($data, $language, array $parameters = array()) {
    // Offer the possibility to support a different value for this function.
    $concepts = NULL;

    $input = array(
      'data' => $data,
      'language' => $language,
      'parameters' => $parameters,
    );
    drupal_alter('semantic_connector_ppx_extractConcepts', $this, $concepts, $input);

    if (is_null($concepts)) {
      $resource_path = '/extractor/api/extract';
      if (is_string($data)) {
        $post_parameters = array_merge(array(
          'text' => $data,
          'language' => $language,
        ), $parameters);
        $result = $this->connection->post($resource_path, array(
          'data' => $post_parameters,
        ));
      }
      elseif (is_object($data) && property_exists($data, 'fid')) {
        $post_parameters = array_merge(array(
          'file' => '@' . drupal_realpath($data->uri),
          'language' => $language,
        ), $parameters);
        $result = $this->connection->post($resource_path, array(
          'data' => $post_parameters,
        ));
      }
      else {
        // Not supported --> Throw error.
      }

      $concepts = json_decode($result);
    }

    // Files have additional information we don't need --> remove it.
    if (is_object($concepts) && property_exists($concepts, 'document')) {
      $concepts = $concepts->document;
    }

    return $concepts;
  }

  /**
   * Get a list of of concepts / free terms matching a string.
   *
   * @param string $string
   *   The string to search matching concepts / freeterms for.
   * @param string $language
   *   The iso-code of the text's language.
   * @param string $project_id
   *   The ID of the PoolParty project to use.
   * @param array $parameters
   *   Additional parameters to forward to the API (e.g. projectId).
   *
   * @return array
   *   An associative array of concepts and freeterms.
   */
  public function suggest($string, $language, $project_id, array $parameters = array()) {
    $suggestion = NULL;

    $input = array(
      'string' => $string,
      'language' => $language,
      'project_id' => $project_id,
      'parameters' => $parameters,
    );
    // Offer the possibility to support a different value for this function.
    drupal_alter('semantic_connector_ppx_suggest', $this, $suggestion, $input);

    if (is_null($suggestion)) {
      $resource_path = '/extractor/api/suggest';
      $post_parameters = array_merge(array(
        'searchString' => $string,
        'language' => $language,
        'projectId' => $project_id,
      ), $parameters);

      $result = $this->connection->post($resource_path, array(
        'data' => $post_parameters,
      ));

      $suggestion = json_decode($result);
    }

    return ((isset($suggestion->suggestedConcepts) && is_array($suggestion->suggestedConcepts)) ? $suggestion->suggestedConcepts : array());
  }
}
