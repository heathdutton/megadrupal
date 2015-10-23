<?php

/**
 * @file
 */

/**
 * Plugin for "apachesolr_geo" query types.
 */
class FacetapiApachesolrGeoQueryType extends FacetapiQueryType implements FacetapiQueryTypeInterface {
  /**
   * Returns the query type associated with the plugin.
   *
   * @return string
   *   The query type.
   */
  static public function getType() {
    return 'apachesolr_geo';
  }

  /**
   * Adds the filter to the query object.
   *
   * @param DrupalSolrQueryInterface $query
   *   An object containing the query in the backend's native API.
   */
  public function execute($query) {
    // Retrieve settings of the facet.
    // We should be able to get all constants as facet options.
    $settings = $this->adapter->getFacet($this->facet)->getSettings();
    $active_items = $this->adapter->getActiveItems($this->facet);

    if (empty($active_items)) {
      $distance = APACHESOLR_GEO_DEFAULT_RADIUS;
    }
    else {
      $active_item = array_pop($active_items);
      $distance = substr($active_item['value'], 1);
      // Add current selected distance to have possibility to unselect it.
      $facet_distances[] = 1;
    }
    if ($settings->enabled) {
      apachesolr_geo_proximity_filter($query, APACHESOLR_GEO_SEARCH_CENTER_POINT, $distance, $this->facet['field']);

      // Set facets.
      foreach ($facet_distances as $facet_option) {
        $facet_distance = $distance * $facet_option;
        $query->addParam('facet.query', '{!geofilt d=' . $facet_distance . ' key=d' . $facet_distance . '}');
      }
    }
  }

  /**
   * Initializes the facet's build array.
   *
   * @return array
   *   The initialized render array.
   */
  public function build() {
    $build = array();
    if ($response = apachesolr_static_response_cache($this->adapter->getSearcher())) {
      if (isset($response->facet_counts->facet_queries)) {
        foreach ($response->facet_counts->facet_queries as $value => $count) {
          // Skip zero results values.
          if ($count > 0) {
            $build[$value] = array('#count' => $count);
          }
        }
      }
    }
    return $build;
  }
}