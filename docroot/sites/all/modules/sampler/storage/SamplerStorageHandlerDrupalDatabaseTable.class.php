<?php

/**
 * @file
 * Handler class for drupal_database_table storage plugin.
 */

class SamplerStorageHandlerDrupalDatabaseTable implements SamplerStorageHandlerInterface {

  public function __construct($sampler) {
    $this->sampler = $sampler;

    // Dump in plugin option defaults.
    $this->sampler->options = $this->sampler->options + $this->options();

    $this->setupDatabase();

  }

  public function options() {
    return array();
  }

  protected function setupDatabase() {
    $this->db = Database::getConnection();
  }

  protected function setupDatabaseExternal() {
    if ($this->isExternalDatabase()) {
      $this->db = Database::getConnection('default', $this->sampler->options['database']);
    }
  }

  protected function isExternalDatabase() {
    return isset($this->sampler->options['database']) && $this->sampler->options['database'] != 'default';
  }

  /**
   * The Drupal table name for the metric.
   *
   * @return
   *   The Drupal table name.
   */
  public function schemaIdentifierDrupal() {
    return $this->schemaIdentifier();
  }

  public function schemaIdentifier() {
    $number_of_values = count($this->sampler->dataType);
    return "sampler_metrics_values_$number_of_values";
  }

  public function reportSchemaToDrupal() {
    return TRUE;
  }

  /**
   * Wrapper functions around Drupal's database layer functions.  These allow
   * other classes to use Drupal's external database functionality via the
   * setDatabaseConnection() method.
   */

  public function tableExists($table) {
    return $this->db->schema()->tableExists($table);
  }

  public function createTable($table, $schema) {
    return $this->db->schema()->createTable($table, $schema);
  }

  public function dropTable($table) {
    return $this->db->schema()->dropTable($table);
  }

  public function escapeTable($table) {
    return $this->db->escapeTable($table);
  }

  public function ensureStorage() {
    if ($this->tableExists($this->schemaIdentifier())) {
      return TRUE;
    }
    else {
      return $this->addMetricToSchema();
    }
  }

  public function addMetricToSchema() {
    module_load_include('inc', 'sampler', 'sampler.api');
    $state_data = $this->sampler->buildMetricStateData();
    // Make the Sampler API aware of the schema update first.
    sampler_update_schema_state('update', $this->sampler->module, $this->sampler->metric, $state_data);
    if ($this->tableExists($this->schemaIdentifier())) {
      return TRUE;
    }
    else {
      $schema = $this->buildMetricSchema();
      $this->createTable($this->schemaIdentifier(), $schema);
      return $this->tableExists($this->schemaIdentifier());
    }
    return FALSE;
  }

  public function deleteMetricFromSchema() {
    $table_name = $this->escapeTable($this->schemaIdentifier());
    // Clear out all entries for the metric in the shared table.
    $this->db->delete($this->schemaIdentifier())
      ->condition('module', $this->sampler->module)
      ->condition('metric', $this->sampler->metric)
      ->execute();
    // Check for remaining rows -- if any exist, assume that other metrics are
    // still using the table, and don't drop it.
    $remaining_rows = $this->db->query("SELECT COUNT(timestamp) FROM {$table_name}")->fetchField();
    if ($remaining_rows == 0) {
      $this->dropTable($this->schemaIdentifier());
    }
    sampler_update_schema_state('drop', $this->sampler->module, $this->sampler->metric);
    return TRUE;
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
    return "value_$count";
  }

  /**
   * Builds the Schema API description for the metric.
   *
   * @return
   *   An array that describes the schema to Schema API.
   */
  public function buildMetricSchemaDrupal() {
    return $this->buildMetricSchema();
  }

  /**
   * Builds a schema structure for a metric, in the storage plugin's form.
   *
   * @return
   *   Information describing the schema to the storage plugin.
   */
  public function buildMetricSchema() {
    $schema = array(
      'description' => "Data for {$this->sampler->metric} metric from {$this->sampler->module} module.",
      'fields' => array(
        'module' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'default' => '',
          'description' => 'The module providing the metric.',
        ),
        'metric' => array(
          'type' => 'varchar',
          'length' => 50,
          'not null' => TRUE,
          'default' => '',
          'description' => 'The metric name.',
        ),
        'object_id' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => 0,
          'description' => 'Unique identifier for the object type.',
        ),
        'timestamp' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'default' => 0,
          'description' => 'Unix timestamp representing the time the metric was calculated.',
        ),
      ),
      'primary key' => array('module', 'metric', 'object_id', 'timestamp'),
    );
    // Build as many columns as we need depending on the data types declared.
    $count = 0;
    foreach ($this->sampler->dataType as $key => $type) {
      $count++;
      $column = $this->buildValueColumnName($key, $count);
      $schema['fields'][$column] = array(
        'type' => $type,
        'description' => 'The calculated value of the metric for the specified time period.',
      );
    }
    return $schema;
  }

  public function getLastSampleTime() {
    $identifier = $this->schemaIdentifier();
    if ($this->tableExists($identifier)) {
      $table_name = db_escape_table($identifier);
      // We assume the last sample has the most recent timestamp.
      $result = $this->db->query("SELECT MAX(timestamp) FROM {$table_name} WHERE module = :module AND metric = :metric", array(':module' => $this->sampler->module, ':metric' => $this->sampler->metric));
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
          'module' => $this->sampler->module,
          'metric' => $this->sampler->metric,
          'object_id' => $object_id,
          'timestamp' => $sample->timestamp,
        );
        $count = 0;
        reset($sample_values);
        foreach ($this->sampler->dataType as $type) {
          $count++;
          $fields["value_$count"] = current($sample_values);
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

