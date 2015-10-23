<?php

/**
 * @file
 * Handler class for mongodb_collection storage plugin.
 */

class SamplerStorageHandlerMongoDBCollection implements SamplerStorageHandlerInterface {

  public $mongoDBUsername;
  public $mongoDBPassword;
  public $mongoDBHost;
  public $mongoDBPort;
  public $mongoDBDatabase;
  private $connection;

  public function __construct($sampler) {
    $this->sampler = $sampler;

    // We want to accept command-line options to connect to alternate databases
    // but don't want this information floating around in the global options.
    // So extract login information and put it into our object properties.
    $mongo_connection_info = array(
      'mongodb_username' => array('property' => 'mongoDBUsername', 'value' => NULL),
      'mongodb_password' => array('property' => 'mongoDBPassword', 'value' => NULL),
      'mongodb_host' => array('property' => 'mongoDBHost', 'value' => 'localhost'),
      'mongodb_port' => array('property' => 'mongoDBPort', 'value' => '27017'),
      'mongodb_database' => array('property' => 'mongoDBDatabase', 'value' => 'sampler_metrics'),
    );

    foreach ($mongo_connection_info as $option => $data) {
      if (isset($this->sampler->options[$option])) {
        $final_value = $this->sampler->options[$option];
        unset($this->sampler->options[$option]);
      }
      else {
        $final_value = $data['value'];
      }
      $property = $data['property'];
      $this->$property = $final_value;
    }

    // Dump in plugin option defaults.
    $this->sampler->options = $this->sampler->options + $this->options();
  }

  public function options() {
    return array();
  }

  /**
   * Connects to the Mongo database, and stores the connection for later use.
   *
   * @return
   *   TRUE if the connection was successful, FALSE otherwise.
   */
  public function connect() {
    // Only need to connect once per object.
    if (isset($this->connection)) {
      return TRUE;
    }
    else {
      if (class_exists("Mongo")) {
        // Authenticated connections use a different connection string.
        if (isset($this->mongoDBUsername) && isset($this->mongoDBPassword)) {
          $connection_string = "mongodb://$this->mongoDBUsername:$this->mongoDBPassword@$this->mongoDBHost:$this->mongoDBPort/$this->mongoDBDatabase";
        }
        else {
          $connection_string = "mongodb://$this->mongoDBHost:$this->mongoDBPort/$this->mongoDBDatabase";
        }
        // Try to connect to the database.
        try {
          $this->connection = new Mongo($connection_string);
        }
        catch (MongoConnectionException $e) {
          return FALSE;
        }
        return isset($this->connection) && is_object($this->connection);
      }
    }
    return FALSE;
  }

  /**
   * Builds the current collection object.
   *
   * @return
   *   The collection object on success, FALSE otherwise.
   */
  public function getCurrentCollection() {
    // Make sure we can connect to the database.
    if (isset($this->connection)) {
      $database = $this->mongoDBDatabase;
      $schema_identifier = $this->schemaIdentifier();
      return $this->connection->$database->$schema_identifier;
    }
    return FALSE;
  }

  public function schemaIdentifier() {
    $number_of_values = count($this->sampler->dataType);
    return "sampler_metrics_values_$number_of_values";
  }

  public function reportSchemaToDrupal() {
    return FALSE;
  }

  public function ensureStorage() {

    // Make sure we can connect to the database.
    if ($this->connect()) {
      // Sampler API already knows about the storage, nothing more to do.
      if (sampler_load_metric_schema($this->sampler->module, $this->sampler->metric)) {
        return TRUE;
      }
      else {
        return $this->addMetricToSchema();
      }
    }
    return FALSE;
  }

  public function addMetricToSchema() {
    module_load_include('inc', 'sampler', 'sampler.api');
    $state_data = $this->sampler->buildMetricStateData();
    // Make the Sampler API aware of the schema update.  Since Mongo will
    // create the collection on the fly upon first insert, this is all we need
    // to do here.
    sampler_update_schema_state('update', $this->sampler->module, $this->sampler->metric, $state_data);
    return TRUE;
  }

  public function deleteMetricFromSchema() {

    // Make sure we can connect to the database.
    if ($this->connect()) {
      $collection = $this->getCurrentCollection();
      // Remove all objects for the metric.
      $where = array(
        'module' => $this->sampler->module,
        'metric' => $this->sampler->metric,
      );
      $collection->remove($where);
      // If the collection is now empty, remove it.
      if ($collection->count() == 0) {
        $collection->drop();
      }
      // Delete the metric from the API's state table.
      sampler_update_schema_state('drop', $this->sampler->module, $this->sampler->metric);
      return TRUE;
    }
  }

  public function buildMetricSchema() {
    // Mongo really has no schema.
    return array();
  }

  public function getLastSampleTime() {
    // Make sure we can connect to the database.
    if ($this->connect()) {
      $collection = $this->getCurrentCollection();
      $query = array(
        '$query' => array(
          'module' => $this->sampler->module,
          'metric' => $this->sampler->metric,
        ),
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

  public function insertSamples($samples) {
    // Make sure we can connect to the database.
    if ($this->connect()) {

      $collection = $this->getCurrentCollection();

      // Build the unique key on the primary key columns.
      $primary_keys = $this->getUniqueKeyColumns();
      $index = array();
      foreach ($primary_keys as $key => $value) {
        $index[$key] = 1;
      }
      // Make sure we have indexes on this collection.
      $collection->ensureIndex($index, array('unique' => TRUE));
      $collection->ensureIndex(array('timestamp' => -1));

      // Store the names of the value keys for later use.
      $value_keys = array();
      $count = 0;
      foreach ($this->sampler->dataType as $key => $type) {
        $count++;
        $value_keys[] = $this->buildValueColumnName($key, $count);
      }

      $samples_count = 0;
      $objects = 0;

      foreach ($samples as $sample) {
        $samples_count++;
        foreach ($sample->values as $object_id => $sample_values) {
          // These are the elements of the unique index.
          $primary_key_args = $this->getUniqueKeyColumns($object_id, $sample->timestamp);

          // These are the values of the sampled object.
          $values_doc = array();
          while (current($sample_values) !== FALSE) {
            $value = current($sample_values);
            $value_key = current($value_keys);
            $values_doc[$value_key] = $value;
            next($sample_values);
            next($value_keys);
          }
          // Rewind the value keys to use for the next trip around the loop.
          reset($value_keys);

          $insert_args = array_merge($primary_key_args, $values_doc);
          if ($collection->insert($insert_args, array('safe' => TRUE))) {
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
    return "value_$count";
  }

  /**
   * Builds the unique key columns for the metric.
   *
   * @param $object_id
   *   Optional.  Includes the passed object_id in the return value.
   * @param $timestamp
   *   Optional.  Includes the passed timestamp in the return value.
   *
   * @return
   *   An array keyed on column names of the unique key.  Values are the
   *   current module and metric, and the passed object_id and timestamp.
   */
  public function getUniqueKeyColumns($object_id = NULL, $timestamp = NULL) {
    return array(
      'module' => $this->sampler->module,
      'metric' => $this->sampler->metric,
      'object_id' => $object_id,
      'timestamp' => $timestamp,
    );
  }
}

