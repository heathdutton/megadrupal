<?php

/**
 * @file
 * The PoolParty PPT API-class.
 */

class SemanticConnectorPPTApi {

  protected $connection;

  /**
   * The constructor of the PoolParty Thesaurus class.
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

  /**
   * Get a list of available projects of a PoolParty server.
   *
   * @return bool
   *   TRUE if the service is available, FALSE if not.
   */
  public function getProjects() {
    // Offer the possibility to support a different value for this function.
    $projects = NULL;

    $resource_path = '/PoolParty/api/projects';
    $result = $this->connection->get($resource_path);
    $projects = json_decode($result);

    if (!is_null($projects)) {
      foreach ($projects as &$project) {
        if (isset($project->uriSupplement)) {
          $project->sparql_endpoint_url = $this->connection->getEndpoint() . '/PoolParty/sparql/' . $project->uriSupplement;
        }
      }
    }
    else {
      $projects = array();
    }

    return $projects;
  }

  /**
   * Export data of a project as a file and store it on the server.
   *
   * @param string $project_id
   *   The ID of the PoolParty project to export and store.
   * @param string $format
   *   The returned RDF format.
   *   Possible values are: TriG, N3, Turtle, N-Triples, RDF/XML, TriX
   * @param string $export_modules
   *   A list of the export modules for the data that should be exported.
   *   Possible values are:
   *   - concepts - includes concept schemes, concepts, collections and notes
   *   - workflow - workflow status for all concepts
   *   - history - all history events
   *   - freeConcepts - all free concepts
   *   - void - the project VoiD graph
   *   - adms - the project ADMS graph
   *
   * @return string
   *   The URL of the stored file or an empty string if an error occurred.
   */
  public function storeProject($project_id, $format = 'RDF/XML', $export_modules = 'concepts') {
    $resource_path = '/PoolParty/api/projects/' . $project_id . '/store';

    $get_parameters = array(
      'format' => $format,
      'exportModules' => $export_modules,
    );

    $file_path = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
      'headers' => array('Accept' => 'text/plain')
    ));

    return (filter_var($file_path, FILTER_VALIDATE_URL) !== FALSE) ? $file_path : '';
  }
}
