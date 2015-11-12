<?php
/**
 * @file
 */

class EntityCollectionQueryException extends Exception {}

class EntityCollectionQuery {
  protected $bundleConditions = array();
  protected $entityConditions = array();
  protected $collectionConditions = array();
  protected $contexts = array();

  /**
   * Limit the selection to a particular bundle.
   * @param $bundles
   *  One ore more bundles to limit the query to
   * @param operator
   *  Operator to use for this operation.
   *  Allowed values: 'IN','NOT IN'
   */
  public function bundleCondition($bundles, $operator = 'IN') {
    if (!is_array($bundles)) {
      $bundles = array($bundle);
    }
    $available_bundles = entity_collection_bundles();
    $bundles_by_backend = array();
    foreach ($bundles as $bundle) {
      $storage = $available_bundles[$bundle]->storage;
      if (!isset($bundles_by_backend[$storage])) {
        $bundles_by_backend[$storage] = array();
      }
      $bundles_by_backend[$storage] += array($bundle);
    }
    foreach ($bundles_by_backend as $backend => $bundles) {
      $this->bundleConditions[$backend][] = array('value' => $bundles, 'operator' => $operator);
    }
    return $this;
  }

  function collectionCondition($names, $operator = 'IN') {
    if (!is_array($names)) {
      $names = array($name);
    }
    if (!empty($names)) {
      $this->collectionConditions[] = array('value' => $names, 'operator' => $operator);
    }
    return $this;
  }

  /**
   * Limit the result to particular contexts.
   * @param array $contexts
   */
  public function useContexts(array $contexts) {
    $this->contexts = $contexts;
    return $this;
  }

  public function getContexts() {
    return $this->contexts;
  }

  /**
   * Select which backend to use.
   * @param $backend
   *   The name of the backend plugin to use.
   */
  public function useBackend($backend) {
    $this->backend = $backend;
    return $this;
  }

  /**
   * Get all current bundle conditions.
   * You needd to set the current backened to do this.
   */
  public function getBundleConditions() {
    if (!isset($this->backend)) {
      throw new EntityCollectionQueryException(t('No active backend has been set'));
    }
    return $this->bundleConditions[$this->backend];
  }

  /**
   * Get all current entity conditions.
   */
  public function getEntityConditions() {
    return $this->entityConditions;
  }

  /**
   * Get all current collection conditions.
   */
  public function getCollectionConditions() {
    return $this->collectionConditions;
  }

  /**
   * Returns the entity collections that contains a particular
   * entity
   * @param @entity_type
   *  The entity type of the entity.
   * @param @entity_ids
   *  The ids to look for.
   * @param operator
   *   Operator to use for this operation.
   *  Allowed values: 'IN', 'NOT IN'
   */
  public function entityCondition($entity_type, $entity_ids, $operator = 'IN') {
    if (!is_array($entity_ids)) {
      $entity_ids = array($entity_ids);
    }
    if (!empty($entity_ids)) {
      $this->entityConditions[] = array(
        'entity_type' => $entity_type,
        'entity_ids' => $entity_ids,
        'operator' => $operator
      );
    }
    return $this;
  }
  /**
   * Execute the query and return all matching Entity collections.
   * @return
   *  An array containing the entity names of all matching entity collections.
   */
  public function execute() {
    // Bundle conditions determines which backends we need to query.
    // Go though all backends and execute the query.
    $result = array();
    foreach (array_keys($this->bundleConditions) as $backend) {
      $this->useBackend($backend);
      $result += EntityCollectionStorage::getStorage($backend)->executeQuery($this);
    }
    return $result;
  }
}
