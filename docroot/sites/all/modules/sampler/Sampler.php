<?php

/**
  * Number of seconds before a lock on a running metric times out.
  *
  * This should be longer than than the longest time it will take to finish
  * a full processing run of a metric. Set the lockInterval parameter of a
  * SamplerMetric object to customize this interval.
  *
  * Note that locking is only enforced for the process method of the
  * SamplerMetric object -- if lower level methods are used, lock management
  * must be provided by the calling code.
 */
define('LOCK_DEFAULT_INTERVAL', 300);

/**
 * @file
 * Base classes for Sampler API.
 */

/**
 * Builds an object which is used to invoke a sampling object.
 *
 * Also responsible for initial loading of plugins and plugin defaults.
 */
class Sampler {

  private $pluginTypes = array();
  private $plugins = array();

  public function __construct() {

    // Load plugins.
    $this->pluginTypes = sampler_load_plugin_types();
    foreach ($this->pluginTypes as $type => $settings) {
      if (isset($settings['load'])) {
        $loader_function = $settings['load'];
        $this->plugins[$type]['plugins'] = $loader_function();
      }
      if (isset($settings['default'])) {
        $default_function = $settings['default'];
        $this->plugins[$type]['default'] = $default_function();
      }
    }
  }

  /**
   * Builds a new sampling object.
   *
   * Sets up the default options for the sample, loads the metric class, and
   * returns an object that can be used for sampling.
   *
   * @param $module
   *   The module providing the metric.
   * @param
   *   The metric name.
   * @param
   *   An arbitrary array of custom options to pass to the sampler.
   *
   * @return SamplerMetric
   *   A sampling object.
   */
  public function newSampler($module, $metric, $custom_options = array()) {

    $settings = sampler_load_metrics($module, $metric);
    if (!empty($settings)) {

      $settings['default_options'] = isset($settings['default_options']) ? $settings['default_options'] : array();
      // Override default options with the ones that were passed in.
      $settings['default_options'] = array_merge($settings['default_options'], $custom_options);

      // Load the metric plugin.
      if ($metric_class = ctools_plugin_get_class($settings, 'handler')) {
        $sampler = new $metric_class($module, $metric, $settings, $this->plugins);
        // Set up the data type here, so that it will be available when
        // ensuring storage.  Use the stored schema information if we have it,
        // otherwise fall back to any default setting the metric itself
        // provided, then the site default value.
        $metric_schema = $sampler->getMetricStorageData();
        if (empty($metric_schema)) {
          $sampler->dataType = sampler_get_metric_data_type($module, $metric);
        }
        else {
          $sampler->dataType = $metric_schema->data_type;
        }

        return $sampler;
      }
    }
    return FALSE;
  }

  /**
   * Returns plugin information for all plugins.
   *
   * @return
   *   An array of plugin information.
   */
  public function getPlugins() {
    return $this->plugins;
  }
}

/**
 * Base class for metrics.
 *
 * This does most of the heavy lifting to build a sampling object.  For the
 * default plugins, only the compute() and trackObjectIDs() methods need
 * to be overridden to build a metric.
 */
class SamplerMetric {

  // Global options to control sampling.
  public $module = '';
  public $metric = '';
  public $dataType = '';
  public $storagePlugin = '';
  public $methodPlugin = '';
  public $adjustmentPlugin = '';
  public $options = array();
  public $lockInterval = LOCK_DEFAULT_INTERVAL;

  // These are generated during sampling, and can be set.
  public $samplesSaved = 0;
  public $objectsSaved = 0;
  public $pluginOutput = array();
  public $highestDiscarded = array();
  public $lowestDiscarded = array();

  // Objects for plugins to advertise things to the sampler object.
  public $storagePluginData = array();
  public $methodPluginData = array();
  public $adjustmentPluginData = array();

  // These are metric-wide settings exposed as read-only.
  private $title = '';
  private $description = '';
  private $objectBaseTable = '';
  private $objectPrimaryKey = '';
  private $plugins = array();
  private $lockName = NULL;
  private $metricLocked = FALSE;


  // Read only properties set during sampling.
  private $currentSample = FALSE;
  private $sampleSet = array();  // This can be changed using adjustment plugins.
  private $samplesComputed = 0;
  private $lastSampleTime = 0;

  public function __construct($module, $metric, $settings, $plugins) {

    $this->module = $module;
    $this->metric = $metric;
    $this->lockName = "sampler_metric_{$this->module}_{$this->metric}";

    $this->title = $settings['title'];
    $this->description = $settings['description'];
    $this->objectBaseTable = isset($settings['object_base_table']) ? $settings['object_base_table'] : 'node';
    $this->objectPrimaryKey = isset($settings['object_primary_key']) ? $settings['object_primary_key'] : 'nid';

    // Set up plugins.
    $this->plugins = $plugins;
    $sampler_plugins = array_keys($this->plugins);

    $metric_schema = $this->getMetricStorageData();

    // Set up plugins.  For Sampler API plugin types, if the metric has state
    // information stored in {sampler_schema_state}, use that.  Otherwise, fall
    // back to the metric setting, then to the site default for the plugin.
    $plugin_map = $this->getPluginMap();
    foreach ($this->plugins as $type => $data) {
      $plugin_type = $plugin_map[$type]['property'];
      $db_column = $plugin_map[$type]['db_column'];
      if (in_array($type, $sampler_plugins) && !empty($metric_schema)) {
        $this->$plugin_type = $metric_schema->$db_column;
      }
      else {
        if (isset($settings[$type])) {
          $this->$plugin_type = $settings[$type];
        }
        else {
          $this->$plugin_type = $data['default'];
        }
      }
    }

    // Set up the options after the plugins have been set up, to allow for
    // overrides.  Merge the options with some sensible global defaults.
    $defaults = $this->defaultOptions();
    // Allowing the user to override data_type would be bad.
    unset($defaults['data_type']);
    $options = isset($settings['default_options']) ? $settings['default_options'] : array();
    $all_options = $options + $defaults;
    $this->options = array();
    foreach ($all_options as $key => $value) {
      $this->options[$key] = $value;
    }
  }

  public function __get($variable) {
    return $this->$variable;
  }

  public function __set($variable, $value) {
    // Stub function -- we're not allowing any private variables to be set.
  }

  /**
   * Default options that get passed to all sampler objects.
   *
   * @return
   *   An associative array of default options, key = option name.
   */
  private function defaultOptions() {
    return array(
      'object_ids' => array(),
    );
  }

  /**
   * Maps the plugin type to the class property.
   *
   * @return
   *   An associative array, key is plugin type, value is class property.
   */
  protected function getPluginMap() {
    return array(
      'storage' => array(
        'db_column' => 'storage_plugin',
        'property' => 'storagePlugin',
      ),
      'method' => array(
        'db_column' => 'method_plugin',
        'property' => 'methodPlugin',
      ),
      'adjustment' => array(
        'db_column' => 'adjustment_plugin',
        'property' => 'adjustmentPlugin',
      ),
    );
  }

  /**
   * Make sure that the metric has a valid storage plugin.
   *
   * @return
   *   TRUE if a valid storage plugin exists, FALSE otherwise.
   */
  public function ensureStorage() {
    // In case the schema state information isn't present, check here for it
    // add it if necessary.
    if (!$this->getMetricStorageData()) {
      $state_data = $this->buildMetricStateData();
      sampler_update_schema_state('update', $this->module, $this->metric, $state_data);
    }
    if ($storage = $this->loadPlugin('storage')) {
      return $storage->ensureStorage();
    }
    return FALSE;
  }

  /**
   * Ensures that the number of sample values for a sample being stored by the
   * storage plugin are consistent with the number of data types the metric
   * shows in the {sampler_metric_state} table.
   *
   * Note that this function does not ensure consistency of data types, only
   * number of values.
   *
   * @param $samples
   *   An array of samples as returned by computeSamples().
   * @return
   *   TRUE if the number of values is consistent with the schema state, FALSE
   *   otherwise.
   */
  public function ensureDataStorageConsistency($samples) {
    if (is_array($samples) && (current($samples) !== FALSE)) {
      $first_sample = current($samples);
      $values = $first_sample->values;
      if (is_array($first_sample->values) && (current($values) !== FALSE)) {
        $first_object_values = current($values);
        if ($schema = $this->getMetricStorageData()) {
          if (count($first_object_values) == count($schema->data_type)) {
            return TRUE;
          }
        }
      }
    }
    return FALSE;
  }

  /**
   * Builds information needed to maintain metric state.
   *
   * @return
   *   An array of metric state information related to the current sampling
   *   object for the metric.
   */
  public function buildMetricStateData() {
    return array(
      'data_type' => $this->dataType,
      'storage_plugin' => $this->storagePlugin,
      'method_plugin' => $this->methodPlugin,
      'adjustment_plugin' => $this->adjustmentPlugin,
    );
  }

  /**
   * Retrives schema information for a metric.
   *
   * @return
   *   An object containing the schema infomation for the metric, or FALSE if
   *   not found.
   */
  public function getMetricStorageData() {
    $schema = sampler_load_metric_schema($this->module, $this->metric);
    return $schema;
  }

  /**
   * Updates the storage plugin for a metric.
   *
   * @return
   *   TRUE if the storage plugin was updated, FALSE otherwise.
   */
  public function updateMetricStoragePlugin() {
    // Make sure the plugin we're updating to exists.
    if ($this->loadPlugin('storage')) {
      return sampler_update_metric_storage_plugin($this->module, $this->metric, $this->storagePlugin);
    }
    return FALSE;
  }

  /**
   * Updates the method plugin for a metric.
   *
   * @return
   *   TRUE if the method plugin was updated, FALSE otherwise.
   */
  public function updateMetricMethodPlugin() {
    // Make sure the plugin we're updating to exists.
    if ($this->loadPlugin('method')) {
      return sampler_update_metric_method_plugin($this->module, $this->metric, $this->methodPlugin);
    }
    return FALSE;
  }

  /**
   * Updates the adjustment plugin for a metric.
   *
   * @return
   *   TRUE if the adjustment plugin was updated, FALSE otherwise.
   */
  public function updateMetricAdjustmentPlugin() {
    // Make sure the plugin we're updating to exists.
    if ($this->loadPlugin('adjustment')) {
      return sampler_update_metric_adjustment_plugin($this->module, $this->metric, $this->adjustmentPlugin);
    }
    return FALSE;
  }

  /**
   * Deletes the metric storage completely, including all data and the stored
   * schema information -- use with caution!
   *
   * @return
   *   TRUE if the metric schema was deleted, FALSE otherwise.
   */
  public function deleteMetricFromSchema() {
    if ($this->getMetricStorageData()) {
      if ($engine = $this->loadPlugin('storage')) {
        return $engine->deleteMetricFromSchema($this->module, $this->metric);
      }
    }
    return FALSE;
  }

  /**
   * Loads a plugin.
   *
   * @param $type
   *   The plugin type.
   * @param $custom_plugin
   *   Optional.  If set, load this plugin instead of the default.
   *
   * @return
   *   The plugin object on success, or FALSE otherwise.
   */
  public function loadPlugin($type, $custom_plugin = NULL) {
    $plugin_map = $this->getPluginMap();
    $plugin_type = $plugin_map[$type]['property'];
    if (isset($custom_plugin)) {
      $plugin_name = $custom_plugin;
    }
    else {
      $plugin_name = isset($this->$plugin_type) ? $this->$plugin_type : NULL;
    }
    if (isset($plugin_name)) {
      if ($plugin_class = ctools_plugin_load_class('sampler', $type, $plugin_name, 'handler')) {
        $plugin = new $plugin_class($this);
        return $plugin;
      }
    }
    return FALSE;
  }

  /**
   * Retrieves the last sample time for a metric.
   *
   * @return
   *   The last sample time if one exists.
   */
  public function getLastSampleTime() {
    if ($storage = $this->loadPlugin('storage')) {
      return $storage->getLastSampleTime();
    }
  }

  /**
   * Builds a list of needed sample points.
   *
   * @return
   *   An array of sample points if they are successfully built, or FALSE
   *   otherwise.
   */
  public function buildSampleSet() {
    // Check for the previous sample.
    $this->lastSampleTime = $this->getLastSampleTime();
    if ($method = $this->loadPlugin('method')) {
      $sample_set = $method->buildSampleSet();
      return $sample_set;
    }
    return FALSE;
  }

  /**
   * Computes values for a given metric on a given sample set.
   *
   * @see adjustSampleSet()
   * @see buildSampleSet()
   * @see computeSample()
   * @see adjustSampleResults()
   *
   * @return
   *   An associative array of computed values for the metric, key is the
   *   sample point of the computation, and value is an associative array, key
   *   is the object ID of of the object ID, values is an array of computed
   *   values, in the order they should be stored.
   */
  public function computeSamples() {

    $samples = array();

    // Build the sample set.
    $this->sampleSet = $this->buildSampleSet();

    if ($this->sampleSet !== FALSE) {
      // Add default values for each sample now, so that the adjustment
      // plugins have a chance to tweak them if desired.
      foreach ($this->sampleSet as $key => $sample) {
        $this->sampleSet[$key] = $this->sampleAddDefaultValues($sample);
      }
      reset($this->sampleSet);

      // Allow plugins to adjust the sample set before computing the metrics.
      $this->sampleSet = $this->adjustSampleSet($this->sampleSet);

      if (!empty($this->sampleSet) && is_array($this->sampleSet)) {
        while (current($this->sampleSet) !== FALSE) {
          $this->currentSample = current($this->sampleSet);

          // Compute the sample.  The compute function may not want to load
          // this sample into the results, which it can prevent by returning
          // FALSE.
          if ($this->computeSample() !== FALSE) {
            $samples[] = $this->currentSample;
          }
          next($this->sampleSet);
        }
        // Expose the number of samples computed.
        $this->samplesComputed = count($samples);
      }

      // Allow plugins to adjust the results before they go into the database.
      $samples = $this->adjustSampleResults($samples);

      return $samples;
    }
    return FALSE;
  }

  /**
   * Ensures that a sample contains the minimum required values.
   *
   * The timestamp and value properties are required, so this sets them to
   * a default of the current time and an empty array respectively.
   *
   * @return
   *   A sample object.
   */
  public function sampleAddDefaultValues($sample) {
    $sample->timestamp = isset($sample->timestamp) ? $sample->timestamp : REQUEST_TIME;
    $sample->values = isset($sample->values) ? $sample->values : array();
    // Each sample gets a copy of the global options.  The metrics can look
    // here for the options when computing each sample.
    $sample->options = $this->options;
    return $sample;
  }

  /**
   * Adjusts a sample set prior to computation.
   *
   * @see buildSampleSet()
   *
   * @param $sample_set
   *   An array of sample points, as built by buildSampleSet().
   *
   * @return
   *   The array of sample points, with any adjustments.
   */
  public function adjustSampleSet($sample_set) {
    if ($adjustment = $this->loadPlugin('adjustment')) {
      if (!empty($sample_set)) {
        $sample_set = $adjustment->adjustSampleSet($sample_set);
      }
    }
    return $sample_set;
  }

  /**
   * Adjusts sample results prior to storage.
   *
   * @see computeSamples()
   *
   * @param $samples
   *   An array of computed values for the metric, as built by
   *   computeSamples().
   *
   * @return
   *   The array metric values, with any adjustments.
   */
  public function adjustSampleResults($samples) {
    if ($adjustment = $this->loadPlugin('adjustment')) {
      if (!empty($samples)) {
        $samples = $adjustment->adjustSampleResults($samples);
      }
    }
    return $samples;
  }

  /**
   * Save calculated metric values to storage.
   *
   * @see computeSamples()
   *
   * This method handles the basic storage checks prior to actually storing the
   * data:
   *
   *  - Makes sure the storage medium is available.
   *  - Checks the consistency of the data against what the storage medium
   *    expects.
   *
   * @param $samples
   *   An array of $metric values as returned by computeSamples().
   *
   * @return
   *   TRUE if the save operation was successful, FALSE otherwise.
   */
  public function saveSamples($samples) {
    if (empty($samples)) {
      return TRUE;
    }
    elseif ($storage = $this->loadPlugin('storage')) {
      if ($storage->ensureStorage()) {
        if ($this->ensureDataStorageConsistency($samples)) {
          return $storage->insertSamples($samples);
        }
      }
    }
    return FALSE;
  }

  /**
   * Calculate and store a metric.
   *
   * @see batchProcess()
   * @see computeSamples()
   * @see saveSamples()
   *
   * @return
   *   TRUE if the process operation was successful, FALSE otherwise.
   */
  public function process() {
    $return = FALSE;
    if(lock_acquire($this->lockName, $this->lockInterval)) {
      // Only do batch processing if the metric can provide the objects it
      // wants sampled.
      if (isset($this->options['object_batch_size']) && method_exists($this, 'trackObjectIDs')) {
        $return = $this->batchProcess();
      }
      else {
        $samples = $this->computeSamples();
        if ($samples !== FALSE) {
          $return = $this->saveSamples($samples);
        }
      }
      lock_release($this->lockName);
    }
    else {
      $this->metricLocked = TRUE;
    }
    return $return;
  }

  /**
   * Calculate and store a metric containing a very large number of objects.
   *
   * To keep the sample set consistent, but allow the objects in the sample
   * to be done in smaller batches, a different workflow is needed.  Sample
   * sets are first generated, then the samples for each batch of objects is
   * first computed then saved.
   *
   * @see adjustSampleSet()
   * @see buildSampleSet()
   * @see computeSample()
   * @see adjustSampleResults()
   * @see saveSamples()
   *
   * @return
   *   TRUE if the process operation was successful, FALSE otherwise.
   */
  public function batchProcess() {
    // Start by assuming things will succeed -- this will be set FALSE if a
    // batch fails.
    $return = TRUE;
    $sample_count = 0;

    // Build the sample set.
    $this->sampleSet = $this->buildSampleSet();

    if ($this->sampleSet !== FALSE) {
      // Add default values for each sample now, so that the adjustment
      // plugins have a chance to tweak them if desired.
      foreach ($this->sampleSet as $key => $sample) {
        $this->sampleSet[$key] = $this->sampleAddDefaultValues($sample);
      }
      reset($this->sampleSet);

      // Allow plugins to adjust the sample set before computing the metrics.
      $this->sampleSet = $this->adjustSampleSet($this->sampleSet);

      if (!empty($this->sampleSet) && is_array($this->sampleSet)) {
        // Discover the objects that the metric cares about.
        $object_ids = $this->trackObjectIDs();
        $batch = array();
        // Break up the object IDs into batches.
        while (!empty($object_ids)) {
          if (count($object_ids) > $this->options['object_batch_size']) {
            $object_count = $this->options['object_batch_size'];
          }
          else {
            $object_count = count($object_ids);
          }
          // Array splice pulls out the specified range from the array and
          // returns it, so $object_ids will reduce in size until it's empty.
          $batch[] = array_splice($object_ids, 0, $object_count);
        }
        // Don't continue if an error occurred.
        while ($return === TRUE && current($this->sampleSet) !== FALSE) {
          $sample_count++;
          reset($batch);
          foreach ($batch as $batch_object_ids) {
            // Clone is required here as we want a fresh copy of the bare
            // sample object with the new set of object IDs.
            $this->currentSample = clone current($this->sampleSet);
            $this->currentSample->options['object_ids'] = $batch_object_ids;

            // Compute the sample.  The compute function may not want to load
            // this sample into the results, which it can prevent by returning
            // FALSE.
            if ($this->computeSample() !== FALSE) {
              // Adjust/save expects an array of samples.
              $samples = array($this->currentSample);
              // Allow plugins to adjust the results before they go into the database.
              $samples = $this->adjustSampleResults($samples);
              if ($this->saveSamples($samples)) {
                unset($samples);
                if (isset($this->options['verbose'])) {
                  $message = t("Batched !count objects in sample !sample", array('!count' => count($batch_object_ids), '!sample' => $sample_count));
                  print "$message\n";
                }
              }
              else {
                // The save failed, so bail on the batch.
                $return = FALSE;
                break;
              }
            }
            // Skip the other batches if the sample was rejected.
            else {
              break;
            }
          }
          next($this->sampleSet);
        }
        // Expose the number of samples computed.
        $this->samplesComputed = $sample_count;
      }
    }
    return $return;
  }

  /**
   * Gets a sample point relative to the current sample point.
   *
   * @param $count
   *   The number of sample points to count from the current sample.
   * @param $direction
   *   The direction to count, 'prev', or 'next'.
   *
   * @return
   *   The relative sample point if it exists, FALSE otherwise.
   */
  private function getRelativeSample($count = 1, $direction = 'next') {
    $sample_set = $this->sampleSet;
    for ($i = 0; $i < $count; $i++) {
      if (!($sample = $direction($sample_set))) {
        return FALSE;
      }
    }
    return $sample;
  }

  /**
   * Gets a sample point relative to the current sample point in a future
   * sample.
   *
   * @param $count
   *   The number of sample points to count from the current sample.
   *
   * @return
   *   The relative sample point if it exists, FALSE otherwise.
   */
  public function getFollowingSample($count = 1) {
    return $this->getRelativeSample($count);
  }

  /**
   * Gets a sample point relative to the current sample point in a previous
   * sample.
   *
   * @param $count
   *   The number of sample points to count from the current sample.
   *
   * @return
   *   The relative sample point if it exists, FALSE otherwise.
   */
  public function getPreviousSample($count = 1) {
    return $this->getRelativeSample($count, 'prev');
  }

  /**
   * Compute a single sample point for a metric.
   *
   * @see computeSamples()
   *
   * See the computeSample() method in sampler.api.php for usage.
   */
  public function computeSample() {
    return array();
  }

  /**
   * Return a list of object IDs to be tracked for sampling.
   *
   * @see sampler.api.php
   *
   * See the trackObjectIDs method in sampler.api.php for usage.
   */
  public function trackObjectIDs() {
    return array();
  }
}

/**
 * Interfaces for plugins.
 */

/**
 * Storage handler interface.
 */
interface SamplerStorageHandlerInterface {
  public function __construct($sampler);

  /**
   * Options for the plugin.
   *
   * @return
   *   An array of defaults for all options the plugin supports.
   */
  public function options();

  /**
   * The identifier for the metric's storage in the storage plugin.
   *
   * @return
   *   The identifier as a string.
   */
  public function schemaIdentifier();

  /**
   * Boolean indicating if Drupal should be informed of the metric's schema.
   *
   * @return
   *   TRUE if it should, FALSE otherwise.
   */
  public function reportSchemaToDrupal();

  /**
   * Make sure that the storage medium can store data for the metric.
   *
   * @return
   *   TRUE if it can store data, FALSE otherwise.
   */
  public function ensureStorage();

  /**
   * Add the metric's schema to the storage plugin.
   *
   * This function should also update the {sampler_metric_state} table so that
   * the API is aware of the change.
   *
   * @return
   *   TRUE if the operation was successful, FALSE otherwise.
   */
  public function addMetricToSchema();

  /**
   * Delete the metric's schema from the storage plugin.
   *
   * This function should also update the {sampler_metric_state} table so that
   * the API is aware of the change.
   *
   * @return
   *   TRUE if the operation was successful, FALSE otherwise.
   */
  public function deleteMetricFromSchema();

  /**
   * Builds a Schema API table structure for metric tables.
   *
   * If the storage handler doesn't report a schema to Drupal, an empty array
   * can be returned.
   *
   * @return
   *   An array describing the table structure to Schema API.
   */
  public function buildMetricSchema();

  /**
   * Pulls the last sample time for a metric.
   *
   * @return
   *   The last sample time, or FALSE if no samples have been taken.
   */
  public function getLastSampleTime();

  /**
   * Inserts data for a single metric to the database.
   *
   * @param $samples
   *   An array of metric data to save, in the format returned by
   *   computeSamples().
   */
  public function insertSamples($samples);
}

/**
 * Method handler interface.
 */
interface SamplerMethodHandlerInterface {
  public function __construct($sampler);

  /**
   * Options for the plugin.
   *
   * @return
   *   An array of defaults for all options the plugin supports.
   */
  public function options();

  /**
   * Builds a list of needed sample points.
   *
   * @return
   *   An array of sample objects to be computed, with at least the following
   *   properties (other properties may be included as needed by the plugin):
   *     timestamp:
   *       A Unix timestamp representing when what point the sampling period
   *       begins and/or ends at.
   *     values:
   *       An empty array that will hold the computed values for the sample.
   */
  public function buildSampleSet();
}

/**
 * Adjustment handler interface.
 */
interface SamplerAdjustmentHandlerInterface {
  public function __construct($sampler);

  /**
   * Options for the plugin.
   *
   * @return
   *   An array of defaults for all options the plugin supports.
   */
  public function options();

  /**
   * Adjust the sample set before the sample is computed.
   *
   * @param $samples
   *   The sample set that is to be computed.  This will be an array of sample
   *   objects, with at least the following properties:
   *     timestamp:
   *       A Unix timestamp representing when the sample is taken, or in the
   *       case of the periodic method plugin, what point the period begins
   *       and/or ends at.
   *     options:
   *       An array of all options that have been passed to the sampler.  This
   *       is a combination of global defaults, plugin defaults, and
   *       user-provided options.
   *     values:
   *       An empty array that will hold the computed values for the sample.
   *
   * @return
   *   The adjusted sample set.  The standard compute function expects this
   *   value to be an array in the same form as it was passed to the function.
   *   Return an empty array to skip all sampling.
   */
  public function adjustSampleSet($sample_set);

  /**
   * Adjust the sample results before they are saved.
   *
   * @param $samples
   *   The sample values to be saved.  The standard compute function will
   *   pass an array of sample objects, with at least the following properites:
   *     timestamp:
   *       A Unix timestamp representing when the sample is taken, or in the
   *       case of the periodic method plugin, what point the period begins
   *       and/or ends at.
   *     options:
   *       An array of all options that have been passed to the sampler.  This
   *       is a combination of global defaults, plugin defaults, and
   *       user-provided options.
   *     values:
   *       An associative array of computed values for the sample, key is
   *       object ID, value is an array of computed values in the order they
   *       are to be stored.
   *
   * @return
   *   The adjusted sample values.  The standard save function expects this
   *   to be an array of objects in the same form as the function received.
   *   Return an empty array to skip saving any of the samples.
   */
  public function adjustSampleResults($samples);
}

