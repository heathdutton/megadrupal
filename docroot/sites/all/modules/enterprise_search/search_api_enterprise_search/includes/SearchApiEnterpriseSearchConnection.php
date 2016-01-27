<?php

/**
 * @file
 * Connection class for Solr.
 */

/**
 * Establishes a connection to the Acquia Search service.
 */
class SearchApiEnterpriseSearchConnection extends SearchApiSolrConnection {

  /**
   * Overrides SearchApiSolrConnection::__construct().
   *
   * Uses the Acquia Search HTTP transport.
   */
  public function __construct(array $options) {
    parent::__construct($options);
    $this->newClient = trim(parent::SVN_REVISION, '$ :A..Za..z') > 40;
    if ($this->newClient) {
      $this->_httpTransport = new SearchApiEnterpriseSearchHttpTransport($this->http_auth);
    }
    else {
      throw new Exception(t('This module only works with the latest version of SolrPhpClient'));
    }
  }
}
