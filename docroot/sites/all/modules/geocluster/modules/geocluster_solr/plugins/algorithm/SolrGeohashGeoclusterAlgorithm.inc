<?php

/**
 * Search API Solr based clustering for geocluster.
 */
class SolrGeohashGeoclusterAlgorithm extends GeohashGeoclusterAlgorithm {

  function __construct($config, $cluster_distance, $zoom, $field_handler) {
    parent::__construct($config, $cluster_distance, $zoom, $field_handler);
  }

  function pre_execute() {
  }

  function post_execute() {
    parent::post_execute();
  }

  public function clusterByNeighborCheck(&$results_by_geohash) {
    parent::clusterByNeighborCheck($results_by_geohash);
  }


  /**
   * Cluster two given rows.
   *
   * @param $row_id the first row to be clustered
   * @param $row_id2 the second row to be clustered
   */
  protected function addCluster($row_id, $row_id2, $hash, $hash2, &$entities_by_geohash) {
    $result1 = &$this->values[$row_id]; $result2 = &$this->values[$row_id2];

    // Merge cluster data.
    $result1['geocluster_ids'] .= ',' . $result2['geocluster_ids'];
    $result1['geocluster_count'] += $result2['geocluster_count'];

    // Calculate new center from all points.
    $center = GeoclusterHelper::getCenter(array($result1['geocluster_geometry'], $result2['geocluster_geometry']), array($result1['geocluster_count'], $result2['geocluster_count']));
    $result1['geocluster_geometry'] = $center;
    $result1['geocluster_lat'] = $center->getY();
    $result1['geocluster_lon'] = $center->getX();

    // Remove other result data that has been merged into the cluster.
    unset($this->values[$row_id2]);
    unset($entities_by_geohash[$hash2][$row_id2]);
    if (count($entities_by_geohash[$hash2]) == 0) {
      unset($entities_by_geohash[$hash2]);
    }
  }

  protected function getGeometry($result) {
    return $result['geocluster_geometry'];
  }

  }


