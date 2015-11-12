<?php

class SearchApiSolrReplicationConnection extends SearchApiSolrConnection {

  protected $primary_connections = array();

  protected $secondary_connections = array();

  protected $selected_primary;

  protected $selected_secondary;

  public function __construct(array $options) {
    // Note we do not call our parent constructor, because we're just pretending
    // to be a Solr connection.

    $primaries = array_filter($options['primary']);
    foreach (search_api_server_load_multiple($primaries) as $server) {
      $this->primary_connections[] = $server->getSolrConnection();
    }

    $secondaries = array_filter($options['secondary']);
    foreach (search_api_server_load_multiple($secondaries) as $server) {
      $this->secondary_connections[] = $server->getSolrConnection();
    }
  }



  protected function getPrimaryConnection() {
    if (!isset($this->selected_primary)) {
      $key = array_rand($this->primary_connections);
      $this->selected_primary = $this->primary_connections[$key];
    }
    return $this->selected_primary;
  }

  protected function getSecondaryConnection() {
    if (!isset($this->selected_secondary)) {
      $key = array_rand($this->secondary_connections);
      $this->selected_secondary = $this->secondary_connections[$key];
    }
    return $this->selected_secondary;
  }


  // Proxy off to the correct connection

  public function ping($timeout = 2) {
    return $this->getPrimaryConnection()->ping($timeout);
  }

  public function getLuke($num_terms = 0) {
    return $this->getPrimaryConnection()->getLuke($num_terms);
  }

  public function commit($expungeDeletes = FALSE, $waitFlush = TRUE, $waitSearcher = TRUE, $timeout = 3600) {
    return $this->getPrimaryConnection()->commit($expungeDeletes, $waitFlush, $waitSearcher, $timeout);
  }

  public function add($rawPost) {
    return $this->getPrimaryConnection()->add($rawPost);
  }

  public function delete($rawPost, $timeout = 3600) {
    return $this->getPrimaryConnection()->delete($rawPost, $timeout);
  }

  public function optimize($waitFlush = TRUE, $waitSearcher = TRUE, $timeout = 3600) {
    return $this->getPrimaryConnection()->optimize($waitFlush, $waitSearcher, $timeout);
  }

  public function threads() {
    return $this->primary_connections->threads();
  }

  public function search($query, $offset = 0, $limit = 10, $params = array(), $method = self::METHOD_GET) {
    return $this->getSecondaryConnection()->search($query, $offset, $limit, $params, $method);
  }

  public function extractFromString($data, $params = array(), $document = NULL, $mimetype = 'application/octet-stream') {
    return $this->getSecondaryConnection()->extractFromString($data, $params, $document, $mimetype);
  }
}
