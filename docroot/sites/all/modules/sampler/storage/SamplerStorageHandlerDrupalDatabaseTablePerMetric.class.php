<?php

/**
 * @file
 * Handler class for drupal_database_table_per_metric storage plugin.
 */

class SamplerStorageHandlerDrupalDatabaseTablePerMetric extends SamplerStorageHandlerDrupalDatabaseTable {

  public function schemaIdentifier() {
    return "sampler_{$this->sampler->module}_{$this->sampler->metric}";
  }

  public function deleteMetricFromSchema() {
    $this->dropTable($this->schemaIdentifier());
    if (!$this->tableExists($this->schemaIdentifier())) {
      sampler_update_schema_state('drop', $this->sampler->module, $this->sampler->metric);
      return TRUE;
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
   * Builds a schema structure for a metric, in the storage plugin's form.
   *
   * @return
   *   Information describing the schema to the storage plugin.
   */
  public function buildMetricSchema() {
    // The single table plugin already has the basic definition we need, so
    // just grab it and make the necessary tweaks.
    $schema = parent::buildMetricSchema();
    unset($schema['fields']['module'], $schema['fields']['metric']);
    $schema['primary key'] = array('object_id', 'timestamp');
    return $schema;
  }

  public function getLastSampleTime() {
    $identifier = $this->schemaIdentifier();
    if ($this->tableExists($identifier)) {
      $table_name = $this->escapeTable($identifier);
      $result = $this->db->query("SELECT MAX(timestamp) FROM {$table_name}");
      if ($result && ($timestamp = $result->fetchField())) {
        return intval($timestamp);
      }
    }
    return FALSE;
  }

  public function insertSamples($samples) {
    $samples_count = 0;
    $objects = 0;
    foreach ($samples as $sample) {
      $samples_count++;
      foreach ($sample->values as $object_id => $sample_values) {
        $fields = array(
          'object_id' => $object_id,
          'timestamp' => $sample->timestamp,
        );
        reset($sample_values);
        foreach ($this->sampler->dataType as $key => $type) {
          $fields["value_$key"] = current($sample_values);
          next($sample_values);
        }
        $result = $this->db->insert($this->schemaIdentifier())
          ->fields($fields)
          ->execute();
        if ($result) {
          $objects++;
        }
      }
    }
    // Inject some helpful data about the save operation into the sampler
    // object.
    $this->sampler->samplesSaved = $samples_count;
    $this->sampler->objectsSaved = $objects;

    return TRUE;
  }
}

