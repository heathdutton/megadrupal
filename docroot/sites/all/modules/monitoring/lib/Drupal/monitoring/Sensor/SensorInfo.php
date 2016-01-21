<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\SensorInfo.
 */

namespace Drupal\monitoring\Sensor;

/**
 * Represents a sensor info as defined in hook_monitoring_sensor_info().
 *
 * @todo more
 */
class SensorInfo {

  /**
   * The sensor name.
   *
   * @var string
   */
  protected $sensorName;

  /**
   * The sensor info array.
   *
   * @var array
   */
  protected $sensorInfo;

  /**
   * Instantiates sensor info object.
   *
   * Note that sensor_info is unchecked and needs to be well defined.
   *
   * @see hook_monitoring_sensor_info().
   *
   * @param string $sensor_name
   *   Sensor name.
   * @param array $sensor_info
   *   Sensor info.
   */
  function __construct($sensor_name, array $sensor_info) {
    $this->sensorName = $sensor_name;
    $this->sensorInfo = $sensor_info;
  }

  /**
   * Gets sensor name.
   *
   * @return string
   *   Sensor name.
   */
  public function getName() {
    return $this->sensorName;
  }

  /**
   * Gets sensor label.
   *
   * The sensor label might not be self-explaining enough or unique without
   * the category, the category should always be present when the label is
   * displayed.
   *
   * @return string
   *   Sensor label.
   */
  public function getLabel() {
    return $this->sensorInfo['label'];
  }

  /**
   * Gets sensor description.
   *
   * @return string
   *   Sensor description.
   */
  public function getDescription() {
    return $this->sensorInfo['description'];
  }

  /**
   * Gets sensor class.
   *
   * @return string
   *   Sensor class
   */
  public function getSensorClass() {
    return $this->sensorInfo['sensor_class'];
  }

  /**
   * Gets sensor result class.
   *
   * @return string
   *   Result class.
   */
  public function getResultClass() {
    return $this->sensorInfo['result_class'];
  }

  /**
   * Gets sensor categories.
   *
   * @return string
   *   Categories.
   */
  public function getCategory() {
    return $this->getSetting('category');
  }

  /**
   * Gets sensor value label.
   *
   * In case the sensor defines value_type, it will use the label provided for
   * that type by monitoring_value_types().
   *
   * Next it searches for the label within the sensor definition value_label.
   *
   * If nothing is defined, it returns NULL.
   *
   * @return string|null
   *   Sensor value label.
   */
  public function getValueLabel() {
    if (isset($this->sensorInfo['value_label'])) {
      return $this->sensorInfo['value_label'];
    }
    if ($value_type = $this->getValueType()) {
      $value_types = monitoring_value_types();
      return $value_types[$value_type]['label'];
    }
  }

  /**
   * Gets sensor value type.
   *
   * @return string|null
   *   Sensor value type.
   *
   * @see monitoring_value_types().
   */
  public function getValueType() {
    return isset($this->sensorInfo['value_type']) ? $this->sensorInfo['value_type'] : NULL;
  }

  /**
   * Determines if the sensor value is numeric.
   *
   * @return bool
   *   TRUE if the sensor value is numeric.
   */
  public function isNumeric() {
    return isset($this->sensorInfo['numeric']) ? $this->sensorInfo['numeric'] : TRUE;
  }

  /**
   * Gets sensor caching time.
   *
   * @return int
   *   Caching time in seconds.
   */
  public function getCachingTime() {
    return $this->getSetting('caching_time');
  }

  /**
   * Gets threshold type.
   *
   * @return string|null
   *   Threshold type.
   */
  public function getThresholdsType() {
    $thresholds = $this->getSetting('thresholds');
    if (!empty($thresholds['type'])) {
      return $thresholds['type'];
    }

    // We assume the default threshold type.
    return 'exceeds';
  }

  /**
   * Returns a given threshold if one is configured.
   *
   * @param $key
   *   Name of the threshold, for example warning or critical.
   *
   * @return int|null
   *   The threshold value or NULL if not-configured.
   */
  public function getThresholdValue($key) {
    $thresholds = $this->getSetting('thresholds');
    if (isset($thresholds[$key]) && $thresholds[$key] !== '') {
      return $thresholds[$key];
    }
  }

  /**
   * Returns all settings.
   *
   * @return array
   *   Settings as an array.
   */
  public function getSettings() {
    return $this->sensorInfo['settings'];
  }

  /**
   * Gets time interval value.
   *
   * @return int
   *   Number of seconds of the time interval.
   *   NULL in case the sensor does not define the time interval.
   */
  public function getTimeIntervalValue() {
    return $this->getSetting('time_interval_value', NULL);
  }

  /**
   * Gets setting.
   *
   * @param string $key
   *   Setting key.
   * @param mixed $default
   *   Default value if the setting does not exist.
   *
   * @return mixed
   *   Setting value.
   */
  public function getSetting($key, $default = NULL) {
    return isset($this->sensorInfo['settings'][$key]) ? $this->sensorInfo['settings'][$key] : $default;
  }

  /**
   * Checks if sensor is enabled.
   *
   * @return bool
   */
  public function isEnabled() {
    return (boolean) $this->getSetting('enabled');
  }

  /**
   * Checks if sensor is configurable.
   *
   * @return bool
   */
  public function isConfigurable() {
    return in_array('Drupal\monitoring\Sensor\SensorConfigurableInterface', class_implements($this->getSensorClass()));
  }

  /**
   * Checks if sensor provides extended info.
   *
   * @return bool
   */
  public function isExtendedInfo() {
    return in_array('Drupal\monitoring\Sensor\SensorExtendedInfoInterface', class_implements($this->getSensorClass()));
  }

  /**
   * Checks if sensor defines thresholds.
   *
   * @return bool
   */
  public function isDefiningThresholds() {
    return in_array('Drupal\monitoring\Sensor\SensorThresholdsInterface', class_implements($this->getSensorClass()));
  }

  /**
   * Compiles sensor info values to an associative array.
   *
   * @return array
   *   Sensor info associative array.
   */
  public function toArray() {
    $info_array = array(
      'sensor' => $this->getName(),
      'label' => $this->getLabel(),
      'category' => $this->getCategory(),
      'description' => $this->getDescription(),
      'numeric' => $this->isNumeric(),
      'value_label' => $this->getValueLabel(),
      'caching_time' => $this->getCachingTime(),
      'time_interval' => $this->getTimeIntervalValue(),
      'enabled' => $this->isEnabled(),
    );

    if ($this->isDefiningThresholds()) {
      $info_array['thresholds'] = $this->getSetting('thresholds');
    }

    return $info_array;
  }

}
