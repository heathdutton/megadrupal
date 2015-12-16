<?php
/**
 * @file
 * Contains \Drupal\monitoring\SensorRunner.
 */

namespace Drupal\monitoring;

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\DisabledSensorException;
use Drupal\monitoring\Sensor\SensorInfo;

/**
 * Instantiate and run requested sensors.
 *
 * @todo more
 */
class SensorRunner implements \IteratorAggregate {

  /**
   * Sensor manager.
   *
   * @var \Drupal\monitoring\Sensor\SensorManager
   */
  protected $sensorManager;

  /**
   * Internal sensor result cache.
   *
   * @var array
   */
  protected $cache = array();

  /**
   * List of sensors info keyed by sensor name that are meant to run.
   *
   * @var \Drupal\monitoring\Sensor\SensorInfo[]
   */
  protected $sensors = array();

  /**
   * Flag to force sensor run.
   *
   * @var bool
   */
  protected $forceRun = FALSE;

  /**
   * Flag to switch the collecting of verbose output.
   *
   * @var bool
   */
  protected $verbose = FALSE;

  /**
   * Result logging mode.
   *
   * Possible values: none, on_request, all
   *
   * @var string
   */
  protected $loggingMode = 'none';

  /**
   * Constructs a SensorRunner.
   *
   * @param \Drupal\monitoring\Sensor\SensorInfo[] $sensors
   *   Associative array of sensor names => sensor info. Only enabled sensors
   *   may be passed.
   */
  public function __construct(array $sensors = array()) {
    $this->sensors = $sensors;
    $this->sensorManager = monitoring_sensor_manager();
    if (empty($sensors)) {
      $this->sensors = $this->sensorManager->getEnabledSensorInfo();
    }
    // @todo LOW Cleanly variable based installation should go into a factory.
    $this->loggingMode = variable_get('monitoring_sensor_call_logging', 'on_request');
    $this->loadCache();
  }

  /**
   * Forces to run sensors even there is cached data available.
   *
   * @param bool $force
   */
  public function forceRun($force = TRUE) {
    $this->forceRun = $force;
  }

  /**
   * Sets flag to collect verbose info.
   *
   * @param bool $verbose
   *   Verbose flag.
   */
  public function verbose($verbose = TRUE) {
    $this->verbose = $verbose;
  }

  /**
   * Sets result logging mode.
   *
   * @param string $mode
   *   (NULL, on_request, all)
   */
  public function setLoggingMode($mode) {
    $this->loggingMode = $mode;
  }

  /**
   * Loads available sensor results from cache.
   */
  public function loadCache() {
    $cids = array();
    // Only load sensor caches if they define caching.
    foreach ($this->sensors as $name => $sensor_info) {
      if ($sensor_info->getCachingTime()) {
        $cids[] = $this->getSensorCid($name);
      }
    }
    if ($cids) {
      foreach (cache_get_multiple($cids) as $cache) {
        if ($cache->expire > REQUEST_TIME) {
          $this->cache[$cache->data['name']] = $cache->data;
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getIterator() {
    $results = $this->runSensors();
    return new \ArrayIterator($results);
  }

  /**
   * Runs the defined sensors.
   *
   * @return \Drupal\monitoring\Result\SensorResultInterface[]
   *   Array of sensor results.
   *
   * @throws \Drupal\monitoring\Sensor\DisabledSensorException
   *   Thrown if any of the passed sensors is not enabled.
   *
   * @see \Drupal\monitoring\SensorRunner::runSensor()
   */
  public function runSensors() {
    $results = array();
    foreach ($this->sensors as $name => $info) {
      if ($result = $this->runSensor($info)) {
        $results[$name] = $result;
      }
    }
    $this->logResults($results);
    $this->cacheResults($results);
    return $results;
  }

  /**
   * Run a single given sensor.
   *
   * @param SensorInfo $sensor_info
   *   Sensor info
   *
   * @return SensorResultInterface
   *   Sensor result.
   *
   * @throws \Drupal\monitoring\Sensor\DisabledSensorException
   *   Thrown if the passed sensor is not enabled.
   *
   * @see \Drupal\monitoring\Sensor\SensorInterface::runSensor()
   */
  protected function runSensor(SensorInfo $sensor_info) {
    $sensor = $this->getSensorObject($sensor_info);
    // Check if sensor is enabled.
    if (!$sensor->isEnabled()) {
      throw new DisabledSensorException(format_string('Sensor @sensor_name is not enabled and must not be run.', array('@sensor_name' => $sensor_info->getName())));
    }

    $result = $this->getResultObject($sensor_info);

    // In case result is not yet cached run sensor.
    if (!$result->isCached()) {

      timer_start($sensor_info->getName());
      try {
        $sensor->runSensor($result);
      } catch (\Exception $e) {
        // In case the sensor execution results in an exception, mark it as
        // critical and set the sensor status message.
        $result->setStatus(SensorResultInterface::STATUS_CRITICAL);
        $result->setMessage(get_class($e) . ': ' . $e->getMessage());
        // Log the error to watchdog.
        watchdog_exception('monitoring_exception', $e);
        // @todo Improve logging by e.g. integrating with past or save the
        //   backtrace as part of the sensor verbose output.
      }

      $timer = timer_stop($sensor_info->getName());
      $result->setExecutionTime($timer['time']);

      // Capture verbose output if requested and if we are able to do so.
      if ($this->verbose && $sensor_info->isExtendedInfo()) {
        $result->setVerboseOutput($sensor->resultVerbose($result));
      }

      try {
        $result->compile();
      }
      catch (\Exception $e) {
        $result->setStatus(SensorResultInterface::STATUS_CRITICAL);
        $result->setMessage(get_class($e) . ': ' . $e->getMessage());
      }
    }


    return $result;
  }

  /**
   * Log results if needed.
   *
   * @param \Drupal\monitoring\Result\SensorResultInterface[] $results
   *   Results to be saved.
   */
  protected function logResults(array $results) {
    foreach ($results as $result) {
      // Skip if the result is cached.
      if ($result->isCached()) {
        continue;
      }

      $old_status = NULL;
      // Try to load the previous log result for this sensor.
      if ($last_result = monitoring_sensor_result_last($result->getSensorName())) {
        $old_status = $last_result->sensor_status;
      }

      // Check if we need to log the result.
      if ($this->needsLogging($result, $old_status, $result->getStatus())) {
        monitoring_sensor_result_save($result);
      }
    }
  }

  /**
   * Checks if sensor results should be logged.
   *
   * @param \Drupal\monitoring\Result\SensorResultInterface $result
   *   The sensor result.
   * @param string $old_status
   *   The old sensor status.
   * @param string $new_status
   *   Thew new sensor status.
   *
   * @return bool
   *   TRUE if the result should be logged, FALSE if not.
   */
  protected function needsLogging($result, $old_status = NULL, $new_status = NULL) {
    $log_activity = $result->getSensorInfo()->getSetting('result_logging', FALSE);

    // We log if requested or on status change.
    if ($this->loggingMode == 'on_request') {
      return $log_activity || ($old_status != $new_status);
    }

    // We are logging all.
    if ($this->loggingMode == 'all') {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Cache results if caching applies.
   *
   * @param \Drupal\monitoring\Result\SensorResultInterface[] $results
   *   Results to be cached.
   */
  protected function cacheResults(array $results) {
    // @todo: Cache in a single array, with per item expiration?
    foreach ($results as $result) {
      $definition = $result->getSensorInfo();
      if ($definition->getCachingTime() && !$result->isCached()) {
        $data = array(
          'name' => $result->getSensorName(),
          'sensor_status' => $result->getStatus(),
          'sensor_message' => $result->getMessage(),
          'sensor_expected_value' => $result->getExpectedValue(),
          'sensor_value' => $result->getValue(),
          'execution_time' => $result->getExecutionTime(),
          'timestamp' => $result->getTimestamp(),
        );
        cache_set($this->getSensorCid($result->getSensorName()), $data, 'cache', REQUEST_TIME + $definition->getCachingTime());
      }
    }
  }

  /**
   * Instantiates sensor object.
   *
   * @param SensorInfo $sensor_info
   *   Sensor info.
   *
   * @return \Drupal\monitoring\Sensor\SensorInterface
   *   Instantiated sensor.
   */
  protected function getSensorObject(SensorInfo $sensor_info) {
    $sensor_class = $sensor_info->getSensorClass();
    $sensor = new $sensor_class($sensor_info);
    return $sensor;
  }

  /**
   * Instantiates sensor result object.
   *
   * @param SensorInfo $sensor_info
   *   Sensor info.
   *
   * @return \Drupal\monitoring\Result\SensorResultInterface
   *   Instantiated sensor result object.
   */
  protected function getResultObject(SensorInfo $sensor_info) {
    $result_class = $sensor_info->getResultClass();

    if (!$this->forceRun && isset($this->cache[$sensor_info->getName()])) {
      $result = new $result_class($sensor_info, $this->cache[$sensor_info->getName()]);
    }
    else {
      $result = new $result_class($sensor_info);
    }
    return $result;
  }

  /**
   * Gets sensor cache id.
   *
   * @param string $sensor_name
   *
   * @return string
   *   Cache id.
   */
  protected static function getSensorCid($sensor_name) {
    return 'monitoring_sensor_result:' . $sensor_name;
  }

  /**
   * Reset sensor result caches.
   *
   * @param array $sensor_names
   *   (optional) Array of sensors to reset the cache for. An empty array clears
   *   all results, which is the default.
   */
  public static function resetCache(array $sensor_names = array()) {
    if (empty($sensor_names)) {
      // No sensor names provided, clear all caches.
      cache_clear_all('monitoring_sensor_result:', 'cache', TRUE);
    }
    else {
      foreach ($sensor_names as $sensor_name) {
        cache_clear_all(self::getSensorCid($sensor_name), 'cache');
      }
    }
  }

}
