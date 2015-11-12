<?php

/**
 * @file
 * Handler class for mongodb_collection_per_metric storage plugin.
 */

class SamplerStorageHandlerMongoDBCollectionPerMetric extends SamplerStorageHandlerMongoDBCollection {

  public function schemaIdentifier() {
    return "sampler_{$this->sampler->module}_{$this->sampler->metric}";
  }

  public function deleteMetricFromSchema() {

    // Make sure we can connect to the database.
    if ($this->connect()) {
      $collection = $this->getCurrentCollection();
      $collection->drop();
      // Delete the metric from the API's state table.
      sampler_update_schema_state('drop', $this->sampler->module, $this->sampler->metric);
      return TRUE;
    }
  }

  public function getLastSampleTime() {
    // Make sure we can connect to the database.
    if ($this->connect()) {
      $collection = $this->getCurrentCollection();
      $query = array(
        '$query' => array(),
        '$orderby' => array(
          'timestamp' => -1,
        ),
      );
      // We assume the last sample has the most recent timestamp.
      $timestamp = $collection->findOne($query, array('timestamp'));
      if (isset($timestamp['timestamp'])) {
        return intval($timestamp['timestamp']);
      }
    }
    return FALSE;
  }

  /**
   * Builds the column name for a value column.
   *
   * @param $key
   *   The key name of the column.
   * @param $count
   *   The ordered column number.
   */
  protected function buildValueColumnName($key, $count) {
    return "value_$key";
  }

  /**
   * Builds the unique key columns for the metric.
   *
   * @param $object_id
   *   Optional.  Includes the passed object_id in the return value.
   * @param $timestamp
   *   Optional.  Includes the passed timestamp in the return value.
   * @return
   *   An array keyed on column names of the unique key.  Values are the passed
   *   object_id and timestamp.
   */
  public function getUniqueKeyColumns($object_id = NULL, $timestamp = NULL) {
    return array(
      'object_id' => $object_id,
      'timestamp' => $timestamp,
    );
  }
}

