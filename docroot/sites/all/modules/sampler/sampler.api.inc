<?php

/**
 * @file
 * API functions for Sampler API module.
 */

/**
 * Update the schema state for metrics in the {sampler_metric_state} table.
 *
 * @param $action
 *   The action to perform, 'update' or 'drop'.
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $state_data
 *   Optional. An associative array of state data to save for the metric, with
 *   the following keys:
 *   - data_type:
 *       An array of the Schema API data types used in the columns to store the
 *       metric, in the order they are stored.
 *   - storage_plugin:
 *       The name of the storage plugin.
 *   - method_plugin:
 *       The name of the method plugin.
 *   - adjustment_plugin:
 *       The name of the adjustment plugin.
 *
 *   Plugin names are derived from the name of the file implementing the
 *   plugin.  For example, if a plugin is declared in a file named 'foo.inc',
 *   then the plugin name is 'foo'.
 *
 *   The easiest way to get the full list of current names for the plugins is
 *   to execute 'drush sampler-list-plugins' via the Drush plugin.
 *
 *   Site defaults will be provided for any values not passed.
 *
 * @return
 *   TRUE if the operation succeeds, FALSE otherwise.
 */
function sampler_update_schema_state($action, $module, $metric, $state_data = array()) {
  // Make sure we have sensible defaults if something wasn't passed.
  $defaults = array(
    'data_type' => sampler_default_data_type(),
    'storage_plugin' => sampler_default_storage_plugin(),
    'method_plugin' => sampler_default_method_plugin(),
    'adjustment_plugin' => sampler_default_adjustment_plugin(),
  );
  $state_data = $state_data + $defaults;
  $state_data = (object) $state_data;

  // Normalize data type.
  $state_data->data_type = is_array($state_data->data_type) ? $state_data->data_type : array($state_data->data_type);
  $serialized_types = serialize($state_data->data_type);

  switch ($action) {
    case 'update':
      // Check for an existing entry, and insert/update as appropriate.
      $result = db_query("SELECT metric FROM {sampler_metric_state} WHERE module = :module AND metric = :metric", array(':module' => $module, ':metric' => $metric))->fetchField();
      if ($result) {
        $return = db_update('sampler_metric_state')
          ->fields(array(
            'data_type' => $serialized_types,
            'storage_plugin' => $state_data->storage_plugin,
            'method_plugin' => $state_data->method_plugin,
            'adjustment_plugin' => $state_data->adjustment_plugin,
          ))
          ->condition('module', $module)
          ->condition('metric', $metric)
          ->execute();
      }
      else {
        $return = db_insert('sampler_metric_state')
          ->fields(array(
            'module' => $module,
            'metric' => $metric,
            'data_type' => $serialized_types,
            'storage_plugin' => $state_data->storage_plugin,
            'method_plugin' => $state_data->method_plugin,
            'adjustment_plugin' => $state_data->adjustment_plugin,
          ))
          ->execute();
      }
      return $return;

    case 'drop':
      return db_delete('sampler_metric_state')
        ->condition('module', $module)
        ->condition('metric', $metric)
        ->execute();
  }
}

/**
 * Loads the metric schema information from {sampler_metric_state}.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $storage
 *   Optional. The storage plugin to search by. Defaults to all plugins.
 * @return
 *   An object containing the schema infomation for the metric, or FALSE if the
 *   metric doesn't exist.
 */
function sampler_load_metric_schema($module, $metric, $storage = NULL) {

  $schema_select = db_select('sampler_metric_state', 'sms')
    ->fields('sms', array(
      'module',
      'metric',
      'data_type',
      'storage_plugin',
      'method_plugin',
      'adjustment_plugin',
    ))
    ->condition('module', $module)
    ->condition('metric', $metric);
  if (isset($storage)) {
    $schema_select->condition('storage', $storage);
  }
  $schema = $schema_select->execute()->fetchObject();

  // Normalize data type.
  if ($schema) {
    $schema->data_type = unserialize($schema->data_type);
  }

  return $schema;
}

/**
 * Builds Schema API information for metrics.
 *
 * We support multiple storage plugins, and not all storage plugins will want
 * or need to report their schema to drupal. Therefore, until a storage
 * plugin is actually selected for a metric, we cannot know if we should be
 * reporting the schema to Drupal. So, only look at metrics that already have
 * state information stored.
 *
 * @param $reset
 *   TRUE to reset the cache of metric schema information.  Default is FALSE.
 *
 * @return
 *   An associative array of Schema API information, key is module, value is an
 *   associative array, key is metric name, value is an associative array with
 *   the following keys:
 *     table_name:
 *       The name of the table.
 *     schema:
 *       An array of information that describes the table to Schema API.
 */
function sampler_build_drupal_metric_schema($reset = FALSE) {
  $schema = &drupal_static(__FUNCTION__);

  if (!isset($schema) || $reset) {
    $schema = array();
    sampler_load_sampler();
    $object = new Sampler();

    $metrics_schemas = db_query('SELECT module, metric, data_type, storage_plugin FROM {sampler_metric_state}');
    foreach ($metrics_schemas as $metric_schema) {
      $sampler = $object->newSampler($metric_schema->module, $metric_schema->metric);
      // It's possible that a module providing a metric may have been disabled,
      // in which case we can't load the sampler for the metric, so check to
      // make sure a sampler object exists before proceeding.
      if (is_object($sampler)) {
        $sampler->dataType = unserialize($metric_schema->data_type);
        $sampler->storagePlugin = $metric_schema->storage_plugin;
        // If we can't find the storage plugin, just ignore reporting the schema
        // for the metric.
        if ($storage = $sampler->loadPlugin('storage')) {
          // Make sure this plugin actually wants its schema reported.
          if (method_exists($storage, 'reportSchemaToDrupal') && $storage->reportSchemaToDrupal()) {
            $schema[$metric_schema->module][$metric_schema->metric] = array(
              'table_name' => $storage->schemaIdentifierDrupal(),
              'schema' => $storage->buildMetricSchemaDrupal(),
            );
          }
        }
      }
    }
  }
  return $schema;
}

/**
 * Creates a default entry in {sampler_metric_state} for a metric if necessary.
 *
 * The very first time any plugin is updated in the schema, there may not be an
 * entry in the schema state table, so check this and insert the default values
 * if neccessary.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 */
function sampler_ensure_schema_entry($module, $metric) {
  if (!sampler_load_metric_schema($module, $metric)) {
    $defaults = array(
      'data_type' => sampler_get_metric_data_type($module, $metric),
    );
    sampler_update_schema_state('update', $module, $metric, $defaults);
  }
}

/**
 * Determines the data type for a metric.
 *
 * The data type can be provided as an associative array, a single value, or
 * not specified.  This function inspects the metric settings and determines
 * the correct data type(s).
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 */
function sampler_get_metric_data_type($module, $metric) {
  // Pull the metric settings.
  $settings = sampler_load_metrics($module, $metric);
  if (isset($settings['data_type'])) {
    // Normalize data_type.
    $data_type = is_array($settings['data_type']) ? $settings['data_type'] : array('value' => $settings['data_type']);
  }
  else {
    $data_type = sampler_default_data_type();
  }
  return $data_type;
}

/**
 * Updates the storage plugin for a metric in {sampler_metric_state}.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $storage
 *   The storage plugin to use for the metric.
 * @return
 *   TRUE if the update succeeded, FALSE otherwise.
 */
function sampler_update_metric_storage_plugin($module, $metric, $storage) {
  sampler_ensure_schema_entry($module, $metric);
  return db_update('sampler_metric_state')
    ->fields(array(
      'storage_plugin' => $storage,
    ))
    ->condition('module', $module)
    ->condition('metric', $metric)
    ->execute();
}

/**
 * Updates the method plugin for a metric in {sampler_metric_state}.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $method
 *   The method plugin to use for the metric.
 * @return
 *   TRUE if the update succeeded, FALSE otherwise.
 */
function sampler_update_metric_method_plugin($module, $metric, $method) {
  sampler_ensure_schema_entry($module, $metric);
  return db_update('sampler_metric_state')
    ->fields(array(
      'method_plugin' => $method,
    ))
    ->condition('module', $module)
    ->condition('metric', $metric)
    ->execute();
}

/**
 * Updates the adjustment plugin for a metric in {sampler_metric_state}.
 *
 * @param $module
 *   The module providing the metric.
 * @param $metric
 *   The metric name.
 * @param $adjustment
 *   The adjustment plugin to use for the metric.
 * @return
 *   TRUE if the update succeeded, FALSE otherwise.
 */
function sampler_update_metric_adjustment_plugin($module, $metric, $adjustment) {
  sampler_ensure_schema_entry($module, $metric);
  return db_update('sampler_metric_state')
    ->fields(array(
      'adjustment_plugin' => $adjustment,
    ))
    ->condition('module', $module)
    ->condition('metric', $metric)
    ->execute();
}

/**
 * Computes and stores values for all metrics.
 *
 * By default, this function will compute and store all metrics using each
 * metric's default settings.
 *
 * @param $options
 *   An array of options to pass to the sampler object, as used by the various
 *   plugins. Optional.
 *
 * @return
 *   An array of associative arrays, key is module name, values is an
 *   associative array, key is metric name, values are an associative array
 *   with the following keys:
 *   - samples_computed:
 *       The total number of samples that were computed in the sample.
 *   - samples_saved:
 *       The number of computed samples saved to the database.
 *   - objects_saved:
 *       The total number of objects sampled across the entire sample.
 */
function sampler_process_all_metrics($options = array()) {
  $metrics = sampler_load_metrics();

  sampler_load_sampler();
  $object = new Sampler();

  $results = array();
  $not_specified = t("not specified");

  foreach ($metrics as $module => $metric_data) {
    $t_args = array('%module' => $module);
    foreach ($metric_data as $metric => $data) {

      $sampler = $object->newSampler($module, $metric, $options);
      // This handles computation and storage all at once.
      $success = $sampler->process();

      // Prepare some helpful data about the sample.
      $properties = array();
      $t_args['%metric'] = $metric;
      $t_args['%storage'] = $sampler->storagePlugin;
      $sample_data = array(
        'samples_computed' => 'samplesComputed',
        'samples_saved' => 'samplesSaved',
        'objects_saved' => 'objectsSaved',
      );
      foreach ($sample_data as $key => $property) {
        $prop = $sampler->$property;
        if (isset($prop)) {
          $properties[$key] = $sampler->$property;
          $t_args["%$key"] = $sampler->$property;
        }
        else {
          $t_args["%$property"] = $not_specified;
        }
      }
      if ($success) {
        watchdog('sampler', 'Successfully processed metric %metric from module %module using storage %storage. Samples computed: %samples_computed, Samples saved: %samples_saved, Objects sampled: %objects_saved', $t_args);
      }
      else {
        watchdog('sampler', 'Error processing metric %metric from module %module using storage %storage. Samples computed: %samples_computed, Samples saved: %samples_saved, Objects sampled: %objects_saved', $t_args, WATCHDOG_ERROR);
      }

      $results[$module][$metric] = $properties;
    }
  }
  return $results;
}

/**
 * Displays sample results in a summary table.
 *
 * @see sampler_process_all_metrics()
 *
 * @param $results
 *   An associtive array in the form returned by sampler_process_all_metrics().
 *
 * @return
 *   An HTML string, which is a summary table of the sampling results.
 */
function sampler_display_all_metrics_results($results) {
  $metrics_settings = sampler_load_metrics();
  $header = array(
    t('Module'),
    t('Metric'),
    t('Sample summary'),
  );
  $build = array();
  $rows = array();
  foreach ($results as $module => $module_metrics) {
    foreach ($module_metrics as $metric => $sample_stats) {
      $rows[] = array(
        $module,
        $metrics_settings[$module][$metric]['title'],
        t('Samples computed: %samples_computed, Samples saved: %samples_saved, Objects sampled: %objects_saved', array('%samples_computed' => $sample_stats['samples_computed'], '%samples_saved' => $sample_stats['samples_saved'], '%objects_saved' => $sample_stats['objects_saved'])),
      );
    }
  }
  drupal_set_message(t('All metrics processed.'));
  $build['metrics_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}

