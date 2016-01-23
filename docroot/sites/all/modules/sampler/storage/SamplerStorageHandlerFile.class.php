<?php

/**
 * @file
 * Handler class for file storage plugin.
 */

class SamplerStorageHandlerFile implements SamplerStorageHandlerInterface {

  public function __construct($sampler) {
    $this->sampler = $sampler;

    // Dump in plugin option defaults.
    $this->sampler->options = $this->sampler->options + $this->options();
  }

  public function options() {
  // Add periodic defaults to the global options.
    return array(
      'file_storage_path' => variable_get('file_public_path', conf_path() . '/files'),
      'file_storage_file' => "sampler.csv",
      'file_storage_type' => "csv",
      'file_write_mode' => "append",
    );
  }

  public function schemaIdentifier() {
    return "{$this->sampler->options['file_storage_path']}/{$this->sampler->options['file_storage_file']}";
  }

  public function reportSchemaToDrupal() {
    return FALSE;
  }

  public function ensureStorage() {

    // Make sure the directory containing the file is writable.
    if (is_writable($this->sampler->options['file_storage_path'])) {
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
    // Inform the API about the new storage.
    sampler_update_schema_state('update', $this->sampler->module, $this->sampler->metric, $state_data);
    // CSV file doesn't really have a schema, so just create the file.
    return touch($this->schemaIdentifier());
  }

  public function deleteMetricFromSchema() {
    if (is_writable($this->sampler->options['file_storage_path']) && is_file($this->schemaIdentifier())) {
      if (unlink($this->schemaIdentifier())) {
        sampler_update_schema_state('drop', $this->sampler->module, $this->sampler->metric);
        return TRUE;
      }
    }
    return FALSE;
  }

  public function buildMetricSchema() {
    // CSV file doesn't really have a schema, but this function is required,
    // so just return nothing.
    return array();
  }

  public function getLastSampleTime() {
    if (is_file($this->schemaIdentifier())) {
      // Check for the file type, and call the appropriate method.
      $method = "getLastSampleTime_{$this->sampler->options['file_storage_type']}";
      if (method_exists($this, $method)) {
        return $this->$method();
      }
    }
    return FALSE;
  }

  /**
   * Pulls the last sample time for a metric in a CSV file.
   *
   * @return
   *   The last sample time, or FALSE if no samples have been taken.
   */
  private function getLastSampleTime_csv() {
    $fp = fopen($this->schemaIdentifier(), 'r');
    $offset = filesize($this->schemaIdentifier()) - 1;
    // Find the last line in the file.
    fseek($fp, $offset - 1);
    // Wind backwards to the beginning of the line.
    while (fgetc($fp) != "\n" && $offset > 0) {
      $offset--;
      fseek($fp, $offset);
    }

    // Pull the timestamp.
    $last_line = fgetcsv($fp);
    // Third field should be the timestamp.
    if (isset($last_line[3]) && is_numeric($last_line[3])) {
      return intval($last_line[3]);
    }
    return FALSE;
  }

  public function insertSamples($samples) {
    // Check for the file type, and call the appropriate write function.
    $method = "writeFile_{$this->sampler->options['file_storage_type']}";
    if (method_exists($this, $method)) {
      return $this->$method($samples);
    }
    return FALSE;
  }

  /**
   * Inserts data for a single metric to a CSV file.
   *
   * @param $samples
   *   An array of metric data to save, in the format returned by
   *   sampler_compute_metric().
   */
  public function writeFile_csv($samples) {

    // Figure out the write mode, default to append.
    switch ($this->sampler->options['file_write_mode']) {
      case "append":
        $mode = 'a';
        break;
      case "overwrite":
        $mode = 'w';
        break;
      default:
        $mode = 'a';
        break;
    }
    $fp = fopen($this->schemaIdentifier(), $mode);

    $samples_count = 0;
    $objects = 0;
    foreach ($samples as $sample) {
      $samples_count++;
      foreach ($sample->values as $object_id => $sample_values) {
        // Add module, metric, object_id, timestamp.
        $primary_key_args = array($this->sampler->module, $this->sampler->metric, $object_id, $sample->timestamp);
        // Merge in sample values.
        $fields = array_merge($primary_key_args, $sample_values);
        if (fputcsv($fp, $fields)) {
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

