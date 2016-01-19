<?php

/**
 * @file
 * Contains the entity classes for Past Simpletest backend.
 */
/**
 * The main Event class for the Past Simpletest backend.
 */
class PastEventSimpletest implements PastEventInterface {
  public $event_id;
  public $module;
  public $machine_name;
  public $message;
  public $severity;
  public $timestamp;
  public $uid;

  protected $arguments;
  protected $child_events = array();
  protected $max_recursion;

  protected static $event_id_counter = 1;

  public function __construct(array $values = array()) {
    $this->max_recursion = variable_get('past_max_recursion', 10);

    foreach ($values as $key => $value) {
      $this->$key = $value;
    }
  }

  public function save() {
    // Prepare all arguments.
    foreach ($this->getArguments() as $argument) {
      $argument->ensureType();
    }

    // Create a pretty debug string to show on the Simpletest view
    $out  = 'Module: ' . $this->getModule() . chr(10);
    $out .= 'Machine name: ' . $this->getMachineName() . chr(10);
    $out .= 'Message: ' . $this->getMessage() . chr(10);

    // Attach all Arguments
    $out .= 'Arguments: ' . chr(10);
    foreach ($this->getArguments() as $name => $argument) {
      $out .= $this->formatArgument($name, $argument);
    }

    // Assign an event id.
    $this->event_id = self::$event_id_counter++;

    // Debug outputs are shown on the Simpletest frontend.
    debug($out);
  }

  public function addArgument($key, $data, array $options = array()) {
    if (!is_array($this->arguments)) {
      $this->arguments = array();
    }

    // If it is an object, clone it to avoid changing the original and log it
    // at the current state. Except when it can't, like e.g. exceptions.
    if (is_object($data) && !($data instanceof Exception)) {
      $data = clone $data;
    }

    // Special support for exceptions, convert them to something that can be
    // stored.
    if (isset($data) && $data instanceof Exception) {
      $data = _drupal_decode_exception($data) + array('backtrace' => $data->getTraceAsString());
    }

    // Remove values which were explicitly added to the exclude filter.
    if (!empty($options['exclude'])) {
      foreach ($options['exclude'] as $exclude) {
        if (is_array($data)) {
          unset($data[$exclude]);
        }
        elseif (is_object($data)) {
          unset($data->$exclude);
        }
      }
      unset($options['exclude']);
    }

    $this->arguments[$key] = new PastEventSimpletestArgument(array('name' => $key, 'original_data' => $data) + $options);
    return $this->arguments[$key];
  }

  public function addArgumentArray($key_prefix, array $data, array $options = array()) {
    $arguments = array();
    foreach ($data as $key => $value) {
      $arguments[$key] = $this->addArgument($key_prefix . ':' . $key, $value, $options);
    }
    return $arguments;
  }

  protected function loadArguments() {
    if (!is_array($this->arguments)) {
      $this->arguments = array();
    }
  }

  public function getArgument($key) {
    $this->loadArguments();
    return isset($this->arguments[$key]) ? $this->arguments[$key] : NULL;
  }

  public function getArguments() {
    $this->loadArguments();
    return $this->arguments;
  }

  public function addException(Exception $exception, array $options = array()) {
    $this->addArgument('exception', $exception, $options);
  }

  public function getMachineName() {
    return $this->machine_name;
  }

  public function getModule() {
    return $this->module;
  }

  public function getSeverity() {
    return $this->severity;
  }

  public function getMessage() {
    return $this->message;
  }

  public function getTimestamp() {
    return $this->timestamp;
  }

  public function id() {
    return $this->event_id;
  }

  public function setParentEventId($event_id) {
    $this->parent_event_id = $event_id;
  }

  public function setSeverity($severity) {
    $this->severity = $severity;
  }

  public function setMessage($message) {
    $this->message = $message;
  }

  public function setTimestamp($timestamp) {
    $this->timestamp = $timestamp;
  }

  public function setUid($uid) {
    $this->uid = $uid;
    return $this;
  }

  public function getUid() {
    return $this->uid;
  }

  public function setMachineName($machine_name) {
    $this->machine_name = $machine_name;
  }

  public function setModule($module) {
    $this->module = $module;
  }

  public function addChildEvent($event_id) {
    $this->child_events[] = $event_id;
  }

  public function getChildEvents() {
    return $this->child_events;
  }

  protected function formatArgument($name, $argument) {
    $back = '';
    $data = $argument->getData();
    if (is_array($data) || is_object($data)) {
      foreach ($data as $k => $v) {
        $back .= '   [' . strip_tags($k) . '] => (' . gettype($v) . ') ' . $this->parseObject($v) . chr(10);
      }
      $back = chr(10) . $back;
    }
    else {
      $back = (is_string($data) ? strip_tags($data) : $data) . chr(10);
    }
    $back = ' ' . strip_tags($name) . ' (' . gettype($data) . '): ' . $back;
    return $back;
  }

  protected function parseObject($obj, $recursive = 0) {
    if ($recursive > $this->max_recursion) {
      return t('_Too many nested objects ( @recursion )_', array('@recursion' => $this->max_recursion));
    }
    if (is_scalar($obj) || is_null($obj)) {
      return is_string($obj) ? trim(strip_tags($obj)) : $obj;
    }

    $back = '';
    foreach ($obj as $k => $v) {
      $back .= chr(10) . str_repeat(' ', $recursive * 2) . '     [' . strip_tags($k) . '] => (' . gettype($v) . ') ' . $this->parseObject($v, $recursive + 1);
    }
    return $back;
  }
}

/**
 * The Arguments Entity class for the Past Simpletest backend.
 */
class PastEventSimpletestArgument implements PastEventArgumentInterface {

  public $argument_id;
  public $event_id;
  protected $original_data;
  public $name;
  public $type;
  public $raw;

  public function __construct(array $values = array()) {
    foreach ($values as $key => $value) {
      $this->$key = $value;
    }
  }

  public function getData() {
    return $this->original_data;
  }

  public function getKey() {
    return $this->name;
  }

  public function getRaw() {
    return $this->raw;
  }

  public function getType() {
    return $this->type;
  }

  public function setRaw($data, $json_encode = TRUE) {
    $this->raw = $json_encode ? drupal_json_encode($data) : $data;
  }

  public function getOriginalData() {
    return $this->original_data;
  }

  public function ensureType() {
    if (isset($this->original_data)) {
      if (is_object($this->original_data)) {
        $this->type = get_class($this->original_data);
      }
      else {
        $this->type = gettype($this->original_data);
      }
    }
  }

  public function defaultLabel() {
    return $this->getKey();
  }
}
