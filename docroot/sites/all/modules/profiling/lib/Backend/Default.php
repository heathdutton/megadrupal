<?php

/**
 * Default implementation.
 */
class Profiling_Backend_Default implements Profiling_Backend_Interface
{
  /**
   * Currently collected timers.
   * 
   * @var array
   */
  protected $_timers = array();

  /**
   * Delta tracking.
   * 
   * @var array
   */
  protected $_delta = array();

  /**
   * @see Profiling_Backend_Interface::init()
   */
  public function init() {}

  /**
   * @see Profiling_Backend_Interface::startTimer()
   */
  public function startTimer($name, $collection = PROFILING_DEFAULT_COLLECTION) {

    if (isset($this->_delta[$name])) {
      $delta = (++$this->_delta[$name]);
    }
    else {
      $delta = $this->_delta[$name] = 0;
    }

    Database::startLog($name . ':' . $delta);

    $this->_timers[$name][$delta] = array(
      'time_start' => microtime(TRUE),
      'memory_start' => memory_get_usage(TRUE),
      'name' => $name,
      'collection' => $collection,
      //'query_count' => count($queries),
      'finished' => 1,
      'delta' => $delta,
    );
    return $delta;
  }

  /**
   * Stop single timer.
   * 
   * @param array &$timer
   *   Internal timer array structure.
   */
  protected function _stopTimer(&$timer) {
    if (!isset($timer['time_stop'])) {
      $queries = Database::getLog($timer['name'] . ':' . $timer['delta']);
      $timer['time_stop'] = microtime(TRUE);
      $timer['memory_stop'] = memory_get_usage(TRUE);
      $timer['query_count'] = count($queries);
      $timer['finished'] = 0;
    }
  }

  /**
   * @see Profiling_Backend_Interface::stopTimer()
   */
  public function stopTimer($name, $delta = NULL) {
    // Ensure delta is numeric.
    if (!is_numeric($delta)) {
      $delta = NULL;
    }

    // Ensure timer exists.
    if (!isset($this->_timers[$name])) {
      // FIXME: Log that timer do not exists.
      return FALSE;
    }

    // User did not provide a delta, attempt to find the latest one.
    if ($delta === NULL) {
      $delta = isset($this->_delta[$name]) ? $this->_delta[$name] : 0;
    }

    $this->_stopTimer($this->_timers[$name][$delta]);
    return TRUE;
  }

  /**
   * @see Profiling_Backend_Interface::stopAllTimers()
   */
  public function stopAllTimers() {
    foreach ($this->_timers as $name => &$timers) {
      foreach ($timers as $delta => &$timer) {
        $this->_stopTimer($timer);
      }
    }
  }

  /**
   * @see Profiling_Backend_Interface::getTimers()
   */
  public function getTimers() {
    return $this->_timers;
  }

  /**
   * @see Profiling_Backend_Interface::checkEnv()
   */
  public function checkEnv() {
    return TRUE;
  }
}
