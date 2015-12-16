<?php
/**
 * @file
 * Main class for Solr QB Acquia Search driver.
 */

/**
 * Class SolrQbAcquiaSearchDriver
 */
class SolrQbAcquiaSearchDriver extends SolrQbApachesolrDriver {
  /**
   * Class constructor..
   */
  public function __construct() {
    $this->solr = apachesolr_get_solr(ACQUIA_SEARCH_ENVIRONMENT_ID);
  }
}
