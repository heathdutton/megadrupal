<?php

/**
 * Class SolrQbApachesolrDriver
 */
class SolrQbApachesolrDriver extends SolrQbCustomDriver {

  protected $solr;
  protected $query;
  protected $params;

  /**
   * Class constructor.
   *
   * @param string $env_id
   *   Apachesolr environment identifier.
   *
   * @throws \Exception
   */
  public function __construct($env_id) {
    if ($env_id) {
      $this->solr = apachesolr_get_solr($env_id);
    }
    else {
      throw new Exception(t('You need to set used environment on settings page'));
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
   */
  public function query($values) {
    $this->prepareQuery($values);
    $query = apachesolr_drupal_query("custom", array('q' => $this->query), '', '', $this->solr);
    foreach ($this->params as $key => $param) {
      $query->addParam($key, $param);
    }
    $result = NULL;
    if ($response = $query->search()) {
      $result = $this->parseResponse($response);
    }
    return $result;
  }
}
