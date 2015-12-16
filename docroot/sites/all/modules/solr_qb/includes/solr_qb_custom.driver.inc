<?php
/**
 * @file
 * Solr Query Builder. Custom connection.
 */

/**
 * Class SolrQbCustomDriver.
 */
class SolrQbCustomDriver implements SolrQbDriverInterface {

  private $server;
  private $query;
  private $format = 'json';

  /**
   * Get values from variable and fill object data.
   *
   * @throws Exception
   *   Throw exception if variable does not exist.
   */
  public function __construct($credentials) {
    if ($credentials) {
      $this->server = $credentials['protocol'] . '://' . $credentials['host'] . ':' . $credentials['port'] . $credentials['path'];
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
      $this->query = $this->server . $values['qt'] . '?';
      $values['common']['wt'] = $this->format;
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
      $this->query .= http_build_query($query_values);
    }
    elseif (is_string($values)) {
      $this->query = $this->server . '/select?' . $values . '&wt=' . $this->format;
    }
    else {
      throw new Exception(t('Values should be array or string.'));
    }
  }

  /**
   * Parse response data to array.
   */
  protected function parseResponse($response) {
    $result = NULL;

    if ($response->code == 200) {
      $result = json_decode($response->data);
    }
    else {
      throw new Exception("$response->error ($response->code)");
    }

    return $result;
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
    if ($response = drupal_http_request($this->query)) {
      $result = $this->parseResponse($response);
    }
    return $result;
  }

}
