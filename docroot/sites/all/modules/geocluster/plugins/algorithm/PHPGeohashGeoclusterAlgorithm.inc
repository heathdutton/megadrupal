<?php

/**
 * PHP based clustering for geocluster.
 */
class PHPGeohashGeoclusterAlgorithm extends GeohashGeoclusterAlgorithm {

  function __construct($config, $cluster_distance, $zoom, $field_handler) {
    parent::__construct($config, $cluster_distance, $zoom, $field_handler);
  }

  /**
   * No pre execution step needed for php based clustering.
   */
  function pre_execute() {
  }

  /**
   * Create clusters from geohash grid.
   */
  protected function preClusterByGeohash() {
    $this->total_items = count($this->values);
    // Prepare input data & parameters.
    $entities_by_type = $this->entities_by_type();

    // Generate geohash-based pre-clusters.
    $entities_by_geohash = $this->load_entity_fields($entities_by_type, $this->geohash_length);

    // Loop over geohash-based pre-clusters to create real clusters.
    foreach ($entities_by_geohash as $current_hash => &$entities) {
      $cluster_id = NULL;
      // Add all points within the current geohash to a cluster.
      foreach ($entities as $key => &$entity) {
        // Prepare data.
        $value = &$this->values[$key];
        $value->geocluster_ids = $value->{$this->field_handler->field_alias};
        $value->geocluster_count = 1;
        $value->geocluster_lon = $value->geocluster_geometry->getX();
        $value->geocluster_lat = $value->geocluster_geometry->getY();
        // Either create a new cluster, or
        if (!isset($cluster_id)) {
          // $entity->geocluster_geometry = $center;
          $this->initCluster($value);
          $cluster_id = $key;
        }
        else {
          $this->addCluster($cluster_id, $key, $current_hash, $current_hash, $entities_by_geohash);
        }
      }
    }

    return $entities_by_geohash;
  }

  /**
   * Use default neighbor check.
   */
  protected function clusterByNeighborCheck(&$results_by_geohash) {
    parent::clusterByNeighborCheck($results_by_geohash);
  }

  /**
   * Use default finalization.
   */
  protected function finalizeClusters() {
    parent::finalizeClusters();
  }


  /*** HELPERS ***/

  /**
   * see views_handler_field_field::post_execute()
   */
  function entities_by_type() {
    // Divide the entity ids by entity type, so they can be loaded in bulk.
    $entities_by_type = array();
    $revisions_by_type = array();
    foreach ($this->values as $key => $object) {
      if (isset($object->{$this->field_handler->field_alias}) && !isset($this->values[$key]->_field_data[$this->field_handler->field_alias])) {
        $entity_type = $object->{$this->field_handler->aliases['entity_type']};
        if (empty($this->field_handler->definition['is revision'])) {
          $entity_id = $object->{$this->field_handler->field_alias};
          $entities_by_type[$entity_type][$key] = $entity_id;
        }
        else {
          $revision_id = $object->{$this->field_handler->field_alias};
          $entity_id = $object->{$this->field_handler->aliases['entity_id']};
          $entities_by_type[$entity_type][$key] = array($entity_id, $revision_id);
        }
      }
    }
    return $entities_by_type;
  }

  function load_entity_fields($entities_by_type, $geohash_length) {
    // Load only the field data required for geoclustering.
    // This saves us unnecessary entity loads.
    foreach ($entities_by_type as $entity_type => $my_entities) {
      // Use EFQ for preparing entities to be used in field_attach_load().
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'node');
      $query->entityCondition('entity_id', $my_entities, 'IN');
      $result = $query->execute();
      $entities = $result[$entity_type];
      field_attach_load(
        $entity_type,
        $entities,
        FIELD_LOAD_CURRENT,
        array('field_id' => $this->field_handler->field_info['id'])
      );
      // @todo handle revisions?

      $keys = $my_entities;
      $entities_by_geohash = array();

      foreach ($keys as $key => $entity_id) {
        // If this is a revision, load the revision instead.
        if (isset($entities[$entity_id])) {
          $this->values[$key]->_field_data[$this->field_handler->field_alias] = array(
            'entity_type' => $entity_type,
            'entity' => $entities[$entity_id],
          );

          $geofield = $this->get_geofield_with_geometry($this->values[$key]);
          $geohash_key = substr($geofield['geohash'], 0, $geohash_length);
          if (!isset($entities_by_geohash[$geohash_key])) {
            $entities_by_geohash[$geohash_key] = array();
          }
          $entities_by_geohash[$geohash_key][$key] = $entities[$entity_id];
        }
      }
    }
    ksort($entities_by_geohash);
    return $entities_by_geohash;
  }

  /**
   * Helper function to get the geofield with its geometry for a given result.
   *
   * Geometry will only we loaded once and stored in the geofield.
   *
   * @param $entities all result entities that have been loaded
   * @param $value the current result row value set
   */
  function &get_geofield_with_geometry(&$value) {
    $entity = &$value->_field_data[$this->field_handler->field_alias]['entity'];

    $geofield = &$entity->{$this->field_handler->field_info['field_name']}[LANGUAGE_NONE][0];
    if (!isset($value->geocluster_geometry)) {
      $value->geocluster_geometry = geoPHP::load($geofield['geom']);
    }
    return $geofield;
  }


}


