<?php
/**
 * @file
 * Librato helper, A bridge between drupal and the 3 party service.
 */

/**
 * Librato Helper.
 */
class LibratoHelper {
  protected static $instance;
  protected static $logs = array();
  protected static $url = 'metrics-api.librato.com/v1/metrics';
  protected static $isSent = FALSE;
  protected $email = '';
  protected $token = '';

  /**
   * Return the obj instance of class.
   */
  public static function getInstance() {
    if (!isset(self::$instance)) {
      $c = __CLASS__;
      self::$instance = new $c();
    }
    return self::$instance;
  }

  /**
   * Add the count value to librato summary table.
   *
   * @param string $name
   *   The name to be used when listed in librato.
   *
   * @param int $value
   *   The count of values to be send.
   */
  public static function count($name = '', $value = 0) {
    db_merge('librato_metric_count')
    ->key(array('name' => $name))
    ->fields(array(
      'last_update' => REQUEST_TIME,
      'count' => (!empty($value) ? $value : 1),
    ))
    ->insertFields(array(
      'last_update' => REQUEST_TIME,
      'created' => REQUEST_TIME,
      'count' => (!empty($value) ? $value : 1),
      'name' => $name,
    ))
    ->expression('count', 'count + :inc', array(':inc' => (!empty($value) ? $value : 1)))
    ->execute();
  }

  /**
   * Increment the count by name.
   *
   * @param string $name
   *   The name to be used when listed in librato.
   */
  public static function increment($name = '') {
    self::count($name, 1);
  }

  /**
   * Adding a gauge counter.
   *
   * @see addLog()
   */
  public static function gauges($name = '', $value = '', $source = '', $timestamp = 0) {
    self::addLog('gauges', $name, $value, $source, $timestamp);
  }

  /**
   * Adding a counter.
   *
   * @see addLog()
   */
  public static function counters($name = '', $value = '', $source = '', $timestamp = 0) {
    self::addLog('counters', $name, $value, $source, $timestamp);
  }

  /**
   * Save the log entry to array so it can be written to disk later.
   *
   * @param string $key
   *   The type of action to be send.
   *
   * @param string $name
   *   The name to be used when listed in librato.
   *
   * @param int $value
   *   The count of values to be send.
   *
   * @param string $source
   *   The source of the request default is site host.
   *
   * @param int $timestamp
   *   The current time the event was created.
   */
  public static function addLog($key = 'gauges', $name = '', $value = '', $source = '', $timestamp = 0) {
    if (empty(self::$instance)) {
      self::getInstance();
    }

    self::$logs[$key][] = array(
      'name' => $name,
      'value' => $value,
      'source' => $source,
      'measure_time' => !empty($timestamp) ? $timestamp : REQUEST_TIME,
    );
  }

  /**
   * Destroy the instance and send data to librato.
   */
  public function __destruct() {
    if (!self::$isSent) {
      self::send();
    }
  }

  /**
   * Send the statics to librato.
   */
  public static function send() {
    if (!empty(self::$logs)) {

      $email = variable_get('librato_config_email', '');
      $token = variable_get('librato_config_token', '');

      if (!empty($email) and !empty($token)) {

        $url = 'https://' . urlencode($email) . ':' . $token . '@' . self::$url;
        $fields_string = http_build_query(self::$logs);

        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_POST, count(self::$logs));
        curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);

        $result = curl_exec($ch);

        curl_close($ch);
        self::$isSent = TRUE;

        return $result;
      }
      else {
        watchdog('librato', 'Account information missing', array(), WATCHDOG_ERROR);
      }
    }
  }
}
