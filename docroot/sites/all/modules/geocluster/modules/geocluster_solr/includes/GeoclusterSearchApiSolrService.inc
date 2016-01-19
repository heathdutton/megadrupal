<?php

/**
 * Search service class using Solr server.
 */
class GeoclusterSearchApiSolrService extends SearchApiSolrService {

  /*
   * @var SolrGeohashGeoclusterAlgorithm
   */
  protected $algorithm;

  protected $group_field;

  /**
   * Apply custom changes before the query
   * is sent to Solr. Works exactly like hook_search_api_solr_query_alter().
   *
   * @param array $call_args
   *   An associative array containing all four arguments to the
   *   Apache_Solr_Service::search() call ("query", "offset", "limit" and
   *   "params") as references.
   * @param SearchApiQueryInterface $query
   *   The SearchApiQueryInterface object representing the executed search query.
   */
  protected function preQuery(array &$call_args, SearchApiQueryInterface $query) {
    // Get geocluster from query option, as we can't access the view object.
    // @see GeoclusterSearchApiViewsQuery::build().
    $geocluster = $query->getOption("geocluster");
    if ($geocluster != NULL) {
      $this->addGeoclusterParams($call_args, $query, $geocluster);
    }
  }

  protected function addGeoclusterParams(array &$call_args, SearchApiQueryInterface $query, SolrGeohashGeoclusterAlgorithm &$algorithm) {
    $this->algorithm = &$algorithm;

    $this->group_field = $algorithm->getFieldHandler()->field . ':geocluster_index_' . $algorithm->getGeohashLength();
    $group_limit = 1;

    // Add bbox filter, if present.
    if (isset($_GET['bbox'])) {
      $field_name = $this->algorithm->getFieldHandler()->real_field;
      $index = $query->getIndex();
      $fields = $this->getFieldNames($index);
      $field = $fields[$field_name . ':latlon'];
      $field = SearchApiSolrConnection::escapeFieldName($field);
      list($left, $bottom, $right, $top) = explode(',', $_GET['bbox']);
      $fq = &$call_args['params']['fq'];
      $query = $field . ':[' . $bottom . ',' . $left . ' TO ' . $top . ',' . $right . ']';
      $fq[] = $query;
    }

    // Solr params.
    $params = array(
      'group'           => 'true',
      'group.limit'     => $group_limit,
      'group.field'     => array('ss_' . $this->group_field),
      // Use Solr Stats component to calculate mean of grouped lat/lon values.
      'stats' => 'true',
      'stats.facet' => array('ss_' . $this->group_field),
      'stats.field' => array(
        'fs_' . $algorithm->getFieldHandler()->field . ':lat',
        'fs_' . $algorithm->getFieldHandler()->field . ':lon'
      ),
    );

    $call_args['params'] += $params;
    //$call_args['limit'] = 10;
  }

  /**
   * Empty method to allow subclasses to apply custom changes before search results are returned.
   *
   * Works exactly like hook_search_api_solr_search_results_alter().
   *
   * @param array $results
   *   The results array that will be returned for the search.
   * @param SearchApiQueryInterface $query
   *   The SearchApiQueryInterface object representing the executed search query.
   * @param Apache_Solr_Response $response
   *   The response object returned by Solr.
   */
  protected function postQuery(array &$results, SearchApiQueryInterface $query, Apache_Solr_Response $response) {
    if (module_exists('devel')) {
      dd(timer_read("geocluster") . "ms: started clustering");
    }

    // Post-process clustered results.
    if ($this->algorithm) {
      geophp_load();
      $total_items = 0;
      $results_by_geohash = array();
      $index = $query->getIndex();
      $fields = $this->getFieldNames($index);
      $grouped_field = $fields[$this->group_field];
      $groups = $response->grouped->{$grouped_field}->groups;
      if (module_exists('devel')) {
        dd(timer_read("geocluster") . "ms: items from database: " . count($groups));
      }

      // Prepare data for getting cluster centers from solr stats component.
      $stats_field_lat = 'fs_' . $this->algorithm->getFieldHandler()->field . ':lat';
      $stats_field_lon = 'fs_' . $this->algorithm->getFieldHandler()->field . ':lon';
      $stats = $response->stats->stats_fields;
      $stats_lat = $stats->{$stats_field_lat}->facets->{$grouped_field};
      $stats_lon = $stats->{$stats_field_lon}->facets->{$grouped_field};

      foreach ($groups as $key => &$group) {

        // Clustered results.
        // @todo: create cluster entity instead of fake result
        $doc = $group->doclist->docs[0];
        $id = $doc->{$fields['search_api_item_id']};
        $result = &$results['results'][$id];

        // Set geocluster specific result data.
        $result['clustered'] = $group->doclist->numFound > 1;
        $result['geocluster_ids'] = $id;
        $result['geocluster_count'] = $group->doclist->numFound;
        $total_items += $group->doclist->numFound;
        $geohash = $group->groupValue;
        // TODO: improve accuracy?
        //$location = geoPHP::load($geohash, 'geohash');
        $location = new Point(
          $stats_lon->{$geohash}->mean,
          $stats_lat->{$geohash}->mean
        );
        $result['geocluster_geometry'] = $location;
        $result['geocluster_lat'] = $location->getY();
        $result['geocluster_lon'] = $location->getX();

        $results_by_geohash[$geohash] = array(
          $id => &$result,
        );
      }

      if (module_exists('devel')) {
        dd(timer_read("geocluster") . "ms: pre-clustered by geohash: " . count($groups));
      }

      $this->algorithm->setValues(&$results['results']);
      $this->algorithm->clusterByNeighborCheck($results_by_geohash);

      if (module_exists('devel')) {
        dd(timer_read("geocluster") . "ms: merged & finalized clusters: " . count($results['results']));
        dd(timer_read("geocluster") . "ms: total items within clusters: " . $total_items);
        timer_stop("geocluster");
      }

    }
    $i = 1;
  }

  /**
   * Extract results from a Solr response.
   *
   * @param Apache_Solr_Response $response
   *   A response object from SolrPhpClient.
   *
   * @return array
   *   An array with two keys:
   *   - result count: The number of total results.
   *   - results: An array of search results, as specified by
   *     SearchApiQueryInterface::execute().
   */
  protected function extractResults(SearchApiQueryInterface $query, Apache_Solr_Response $response) {
    if ($this->algorithm) {
      // Set grouping options, so search api will process single result groups correctly.
      $options = array(
        'use_grouping' => TRUE,
        'fields' => array($this->group_field),
      );
      $query->setOption('search_api_grouping', $options);
    }

    // Let search api retrieve the results as usual.
    $results = parent::extractResults($query, $response);
    return $results;
  }

  public function getFieldNames(SearchApiIndex $index, $reset = FALSE) {
    $field_names = parent::getFieldNames($index, $reset);
    // $field_names[$this->group_field] = array();
    return $field_names;
  }

  }
