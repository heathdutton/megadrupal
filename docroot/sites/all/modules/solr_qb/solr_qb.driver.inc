<?php
/**
 * @file
 * Main Solr QB driver interface.
 */

/**
 * Interface SolrQbConnectionInterface.
 */
interface SolrQbDriverInterface {

  /**
   * Send query to Solr and get response.
   *
   * @param array|string $values
   *   Values array or string from Solr QB forms.
   *
   * @return mixed
   *   Array for display.
   */
  public function query($values);

}
