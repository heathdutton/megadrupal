<?php
/**
 * @file
 * Main class for Solr QB Acquia Search Api driver.
 */

/**
 * Class SolrQbAcquiaSearchSearchApiDriver
 */
class SolrQbSearchApiAcquiaDriver extends SolrQbSearchApiSolrDriver {

  /**
   * Class constructor. Get connection parameters from server object.
   *
   * @param SearchApiServer $server
   *   Search API server object.
   *
   * @throws Exception
   */
  public function __construct($server) {
    if (!empty($server->options)) {
      $this->connection = new SearchApiAcquiaSearchConnection($server->options);
    }
    else {
      throw new Exception(t('Can not get connection parameters.'));
    }
  }

}
