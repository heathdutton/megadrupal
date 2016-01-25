<?php
/**
 * @file
 * Openlayers Solr Layer type.
 */

class openlayers_solr_type extends openlayers_layer_type {

  /**
   * Provide initial values for options.
   */
  function options_init() {
    return array(
      'projection' => array('EPSG:3857'),
      'layer_type' => 'openlayers_solr_type',
      'layer_handler' => 'openlayers_solr',
      'vector' => TRUE,
      'isBaseLayer' => FALSE,
      'solr_env' => apachesolr_default_environment(),
    );
  }

  /**
   * Options form which generates layers
   */
  function options_form($defaults = array()) {
    $environments = apachesolr_load_all_environments();
    $apachesolr_servers = array();
    foreach ($environments as $key => $data) {
      $apachesolr_servers[$key] = $data['name'];
    }

    return array(
      'solr_env' => array(
        '#title' => t('Apachesolr Environment'),
        '#type' => 'select',
        '#default_value' => isset($this->data['solr_env']) ? $this->data['solr_env'] : apachesolr_default_environment(),
        '#options' => $apachesolr_servers
      ),
      'layer_type' => array(
        '#type' => 'hidden',
        '#value' => 'openlayers_solr_type'
      ),
    );
  }

  /**
   * Get features for the current search
   *
   * @return array $features
   */
  function get_features() {
    $results = array();
    $env = apachesolr_default_environment($this->data['solr_env']);
    $query = apachesolr_current_query($env);
    if ($query) {
      $searcher = $query->getSearcher();
      $results = apachesolr_static_response_cache($searcher);
    }

    $features = $this->_get_points($results);
    $this->data['features'] = isset($features) ? $features : array();
    return $this->data['features'];
  }

  /**
   * Returns an array of $features from a set of solr results.
   * @param  array $results solr results
   * @return array $features the map features.
   */
  protected function _get_points($results) {
    $features = array();
    // Return if we have no response.
    if (empty($results->response->docs)) {
      return $features;
    }

    foreach ($results->response->docs as $doc) {
      // Continue if we don't have a point.
      if (empty($doc->ss_geo_wkt)) {
        continue;
      }
      // Create the feature.
      $features[] = array(
        'projection' => 'EPSG:4326',
        'wkt' => $doc->ss_geo_wkt,
        'attributes' => array(
          'name' => l($doc->label, $doc->path),
          'description' => $doc->teaser,
        ),
      );
    }
    return $features;
  }

  /**
   * Render.
   */
  function render(&$map) {
    drupal_add_js(drupal_get_path('module', 'openlayers_solr') . '/plugins/layer_types/openlayers_solr.layer.js');
    $this->get_features();
  }
}
