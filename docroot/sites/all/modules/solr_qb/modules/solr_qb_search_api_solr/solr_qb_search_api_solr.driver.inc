<?php
/**
 * @file
 * Main class for Solr QB Search Api Solr driver.
 */

/**
 * Class SolrQbSearchApiSolrDriver
 */
class SolrQbSearchApiSolrDriver extends SolrQbCustomDriver {

  protected $connection;
  protected $query;
  protected $params;

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
      $this->connection = new SearchApiSolrConnection($server->options);
    }
    else {
      throw new Exception(t('Can not get connection parameters.'));
    }
  }

  /**
   * Prepare form values for sending to Solr.
   *
   * @param array|string $values
   *   Solr QB form values.
   *
   * @throws Exception
   */
  protected function prepareQuery($values) {
    if (is_array($values)) {
      $this->query = $values['common']['q'];

      $query_values = array_filter($values['common']);
      $additional = array(
        'dismax',
        'edismax',
        'hl',
        'facet',
        'spatial',
        'spellcheck',
      );
      foreach ($additional as $name) {
        if (!empty($values[$name])) {
          $additional_values = array();
          $additional_values[$name] = 'true';
          $additional_values = array_merge($additional_values, $values[$name . '_wrapper']);
          $query_values = array_merge($query_values, array_filter($additional_values));
        }
      }
      $this->params = $query_values;
    }
    elseif (is_string($values)) {
      $array = array();
      parse_str($values, $array);
      if (isset($array['q'])) {
        $this->query = $array['q'];
        unset($array['q']);
        $this->params = $array;
      }
    }
    else {
      throw new Exception(t('Values should be array or string.'));
    }
  }

  /**
   * Send query to Solr and return response.
   *
   * @param array|string $values
   *   Solr QB form values.
   *
   * @return null|array
   *   Parsed response.
   *
   * @throws \Exception
   */
  public function query($values) {
    $this->prepareQuery($values);
    $result = NULL;
    if ($response = $this->connection->search($this->query, $this->params)) {
      $result = $this->parseResponse($response);
    }
    return $result;
  }

}
