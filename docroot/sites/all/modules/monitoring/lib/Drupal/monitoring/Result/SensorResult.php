<?php
/**
 * @file
 * Contains \Drupal\monitoring\Result\SensorResult.
 */

namespace Drupal\monitoring\Result;

use Drupal\monitoring\Sensor\SensorCompilationException;
use Drupal\monitoring\Sensor\SensorInfo;
use Drupal\monitoring\Sensor\Thresholds;

/**
 * Generic container for the sensor result.
 *
 * @todo more
 *
 * @see \Drupal\monitoring\Sensor\SensorInfo
 * @see \Drupal\monitoring\SensorRunner
 */
class SensorResult implements SensorResultInterface {

  /**
   * The sensor info instance.
   *
   * @var \Drupal\monitoring\Sensor\SensorInfo
   */
  protected $sensorInfo;

  /**
   * If the current result was constructed from a cache.
   *
   * @var bool
   */
  protected $isCached = FALSE;

  /**
   * The sensor result data.
   *
   * @var array
   */
  protected $data = array();

  /**
   * Additional status messages from addStatusMessage().
   *
   * @var string[]
   */
  protected $statusMessages = array();

  /**
   * The main sensor message from setMessage().
   *
   * @var string[]
   */
  protected $sensorMessage = array();

  /**
   * The verbose output of the sensor execution.
   *
   * @var string
   */
  protected $verboseOutput;

  /**
   * Instantiates a sensor result object.
   *
   * By default, the sensor status is STATUS_UNKNOWN with empty message.
   *
   * @param SensorInfo $sensor_info
   *   Sensor info object.
   * @param array $cached_data
   *   Result data obtained from a cache.
   */
  function __construct(SensorInfo $sensor_info, array $cached_data = array()) {
    $this->sensorInfo = $sensor_info;
    if ($cached_data) {
      $this->data = $cached_data;
      $this->isCached = TRUE;
    }

    // Merge in defaults in case there is nothing cached for given sensor yet.
    $this->data += array(
      'sensor_status' => SensorResultInterface::STATUS_UNKNOWN,
      'sensor_message' => NULL,
      'sensor_expected_value' => NULL,
      'sensor_value' => NULL,
      'execution_time' => 0,
      'timestamp' => REQUEST_TIME,
    );
  }

  /**
   * Sets result data.
   *
   * @param string $key
   *   Data key.
   * @param mixed $value
   *   Data to set.
   */
  protected function setResultData($key, $value) {
    $this->data[$key] = $value;
    $this->isCached = FALSE;
  }

  /**
   * Gets result data.
   *
   * @param string $key
   *   Data key.
   *
   * @return mixed
   *   Stored data.
   */
  protected function getResultData($key) {
    return $this->data[$key];
  }

  /**
   * {@inheritdoc}
   */
  public function getStatus() {
    return $this->getResultData('sensor_status');
  }


  /**
   * {@inheritdoc}
   */
  public function getStatusLabel() {
    $labels = array(
      self::STATUS_CRITICAL => t('Critical'),
      self::STATUS_WARNING => t('Warning'),
      self::STATUS_INFO => t('Info'),
      self::STATUS_OK => t('OK'),
      self::STATUS_UNKNOWN => t('Unknown'),
    );
    return $labels[$this->getResultData('sensor_status')];
  }

  /**
   * {@inheritdoc}
   */
  public function getMessage() {
    return $this->getResultData('sensor_message');
  }

  /**
   * {@inheritdoc}
   */
  public function setMessage($message, array $variables = array()) {
    $this->sensorMessage = array(
      'message' => $message,
      'variables' => $variables,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function addStatusMessage($message, array $variables = array()) {
    $this->statusMessages[] = array(
      'message' => $message,
      'variables' => $variables,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function compile() {
    // If the status is unknown we do the value assessment through
    // configurable thresholds.
    $threshold_message = NULL;
    if ($this->isUnknown()) {
      if ($this->getSensorInfo()->isDefiningThresholds()) {
        $threshold_message = $this->assessThresholds();
      }
      // If there are no thresholds, look for an expected value and compare it.
      // A sensor can not have both, as an expected value implies and exact
      // match and then thresholds are not needed.
      // @todo Check expected value first?
      elseif ($this->getExpectedValue() !== NULL) {
        $this->assessComparison();
      }
    }

    if ($this->getSensorInfo()->getValueType() == 'bool') {
      $msg_expected = $this->getExpectedValue() ? 'TRUE' : 'FALSE';
    }
    else {
      $msg_expected = $this->getExpectedValue();
    }

    if (!empty($this->sensorMessage)) {
      // A message has been set by the sensor, use that as is and only do
      // placeholder replacements with the provided variables.
      $message = format_string($this->sensorMessage['message'], $this->sensorMessage['variables']);
    }
    else {

      // No message has been provided, attempt to build one.

      // Set the default message variables.
      $default_variables = array(
        '@sensor' => $this->getSensorName(),
        '!formatted_value' => $this->getFormattedValue(),
        '@time' => $this->getTimestamp(),
        '!expected' => $msg_expected,
        '!time_interval' => format_interval($this->getSensorInfo()->getTimeIntervalValue()),
      );

      // Build an array of message parts.
      $messages = array();

      // Add the sensor value if provided.
      if ($this->getValue() !== NULL) {

        // If the sensor defines time interval we append the info to the
        // message.
        if ($this->getSensorInfo()->getTimeIntervalValue()) {
          $messages[] = format_string('!formatted_value in !time_interval', $default_variables);
        }
        else {
          $messages[] = $default_variables['!formatted_value'];
        }
      }
      // Avoid an empty sensor message.
      elseif (empty($this->statusMessages)) {
        $messages[] = 'No value';
      }

      // Set the expected value message if the sensor did not match.
      if ($this->isCritical() && $this->getExpectedValue() !== NULL) {
        $messages[] = format_string('expected !expected', $default_variables);
      }
      // Set the threshold message if there is any.
      if ($threshold_message !== NULL) {
        $messages[] = $threshold_message;
      }

      // Append all status messages which were added by the sensor.
      foreach ($this->statusMessages as $msg) {
        $messages[] = format_string($msg['message'], array_merge($default_variables, $msg['variables']));
      }

      $message = implode(', ', $messages);
    }

    $this->setResultData('sensor_message', $message);
  }

  /**
   * Performs comparison of expected and actual sensor values.
   */
  protected function assessComparison() {
    if ($this->getValue() != $this->getExpectedValue()) {
      $this->setStatus(SensorResultInterface::STATUS_CRITICAL);
    }
    else {
      $this->setStatus(SensorResultInterface::STATUS_OK);
    }
  }

  /**
   * Deal with thresholds.
   *
   * Set the sensor value  based on threshold configuration.
   *
   * @return string
   *   The message associated with the threshold.
   *
   * @see \Drupal\monitoring\Sensor\Thresholds
   */
  protected function assessThresholds() {
    $thresholds = new Thresholds($this->sensorInfo);
    $matched_threshold = $thresholds->getMatchedThreshold($this->getValue());

    // Set sensor status based on matched threshold.
    $this->setStatus($matched_threshold);
    // @todo why not just set the status message?
    return $thresholds->getStatusMessage();
  }

  /**
   * Formats the value to be human readable.
   *
   * @return string
   *   Formatted value.
   *
   * @throws \Drupal\monitoring\Sensor\SensorCompilationException
   */
  protected function getFormattedValue() {
    // If the value type is defined we have the formatter that will format the
    // value to be ready for display.
    if ($value_type = $this->getSensorInfo()->getValueType()) {
      $value_types = monitoring_value_types();
      if (!isset($value_types[$value_type])) {
        throw new SensorCompilationException(format_string('Invalid value type @type', array('@type' => $value_type)));
      }
      elseif (isset($value_types[$value_type]['formatter_callback']) && !function_exists($value_types[$value_type]['formatter_callback'])) {
        throw new SensorCompilationException(format_string('Formatter callback @callback for @type does not exist',
          array('@callback' => $value_types[$value_type]['formatter_callback'], '@type' => $value_type)));
      }
      else {
        $callback = $value_types[$value_type]['formatter_callback'];
        return $callback($this);
      }
    }

    // If there is no value formatter we try to provide something human readable
    // by concatenating the value and label.

    if ($label = $this->getSensorInfo()->getValueLabel()) {
      // @todo This assumption will no longer work when non-english messages
      // supported.
      $label = drupal_strtolower($label);
      return format_string('!value !label', array('!value' => $this->getValue(), '!label' => $label));
    }

    return format_string('Value !value', array('!value' => $this->getValue()));
  }

  /**
   * {@inheritdoc}
   */
  public function getValue() {
    return $this->getResultData('sensor_value');
  }

  /**
   * {@inheritdoc}
   */
  public function getExpectedValue() {
    return $this->getResultData('sensor_expected_value');
  }

  /**
   * {@inheritdoc}
   */
  public function getExecutionTime() {
    return round($this->getResultData('execution_time'), 2);
  }

  /**
   * {@inheritdoc}
   */
  public function setStatus($sensor_status) {
    $this->setResultData('sensor_status', $sensor_status);
  }

  /**
   * {@inheritdoc}
   */
  public function setValue($sensor_value) {
    $this->setResultData('sensor_value', $sensor_value);
  }

  /**
   * {@inheritdoc}
   */
  public function setExpectedValue($sensor_value) {
    $this->setResultData('sensor_expected_value', $sensor_value);
  }

  /**
   * {@inheritdoc}
   */
  public function setExecutionTime($execution_time) {
    $this->setResultData('execution_time', $execution_time);
  }

  /**
   * {@inheritdoc}
   */
  public function toNumber() {
    $sensor_value = $this->getValue();

    if (is_numeric($sensor_value)) {
      return $sensor_value;
    }

    // Casting to int should be good enough as boolean will get casted to 0/1
    // and string as well.
    return (int) $sensor_value;
  }

  /**
   * {@inheritdoc}
   */
  public function isWarning() {
    return $this->getStatus() == SensorResultInterface::STATUS_WARNING;
  }

  /**
   * {@inheritdoc}
   */
  public function isCritical() {
    return $this->getStatus() == SensorResultInterface::STATUS_CRITICAL;
  }

  /**
   * {@inheritdoc}
   */
  public function isUnknown() {
    return $this->getStatus() == SensorResultInterface::STATUS_UNKNOWN;
  }

  /**
   * {@inheritdoc}
   */
  public function isOk() {
    return $this->getStatus() == SensorResultInterface::STATUS_OK;
  }

  /**
   * Returns sensor result data as array.
   *
   * @return array
   *   An array with data having following keys:
   *   - sensor_name
   *   - value
   *   - expected_value
   *   - numeric_value
   *   - status
   *   - message
   *   - execution_time
   *   - timestamp
   */
  public function toArray() {
    return array(
      'sensor_name' => $this->getSensorName(),
      'value' => $this->getValue(),
      'expected_value' => $this->getExpectedValue(),
      'numeric_value' => $this->toNumber(),
      'status' => $this->getStatus(),
      'message' => $this->getMessage(),
      'execution_time' => $this->getExecutionTime(),
      'timestamp' => $this->getTimestamp(),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function isCached() {
    return $this->isCached;
  }

  /**
   * {@inheritdoc}
   */
  public function getTimestamp() {
    return $this->getResultData('timestamp');
  }

  /**
   * {@inheritdoc}
   */
  function getSensorName() {
    return $this->sensorInfo->getName();
  }

  /**
   * {@inheritdoc}
   */
  function getSensorInfo() {
    return $this->sensorInfo;
  }

  /**
   * {@inheritdoc}
   */
  public function setVerboseOutput($verbose_output) {
    $this->verboseOutput = $verbose_output;
  }

  /**
   * {@inheritdoc}
   */
  public function getVerboseOutput() {
    return $this->verboseOutput;
  }


}
