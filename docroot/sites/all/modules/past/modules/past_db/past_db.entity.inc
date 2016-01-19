<?php

/**
 * @file
 * Contains the entity classes for Past DB.
 */
class PastEvent extends Entity implements PastEventInterface {
  public $event_id;
  public $module;
  public $machine_name;
  public $type;
  public $message;
  public $severity;
  public $timestamp;
  public $parent_event_id;
  public $uid;

  protected $arguments;
  protected $child_events = array();
  protected $max_recuresion;

  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, $entityType);
    $this->max_recursion = variable_get('past_max_recursion', 10);
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

    $this->arguments[$key] = entity_create('past_event_argument', array('name' => $key, 'original_data' => $data) + $options);
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
      foreach (entity_load_multiple_by_name('past_event_argument', FALSE, array('event_id' => $this->event_id)) as $argument) {
        $this->arguments[$argument->name] = $argument;
      }
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

  public function getUid() {
    return (int)$this->uid;
  }

  public function id() {
    return $this->event_id;
  }

  public function setParentEventId($event_id) {
    $this->parent_event_id = $event_id;
    return $this;
  }

  public function setSeverity($severity) {
    $this->severity = $severity;
    return $this;
  }

  public function setMessage($message) {
    $this->message = $message;
    return $this;
  }

  public function setTimestamp($timestamp) {
    $this->timestamp = $timestamp;
    return $this;
  }

  public function setMachineName($machine_name) {
    $this->machine_name = $machine_name;
    return $this;
  }

  public function setModule($module) {
    $this->module = $module;
    return $this;
  }

  public function setUid($uid) {
    $this->uid = $uid;
    return $this;
  }

  public function addChildEvent($event_id) {
    $this->child_events[] = $event_id;
    return $this;
  }

  public function getChildEvents() {
    return $this->child_events;
  }

  /**
   * Overrides Entity::defaultUri().
   * @return array
   */
  public function defaultUri() {
    return array(
      'path' => 'admin/reports/past/' . $this->event_id,
      'options' => array(),
    );
  }

  /**
   * Overrides Entity::defaultLabel().
   */
  public function defaultLabel() {
    if (!empty($this->defaultLabel)) {
      return $this->defaultLabel;
    }

    $this->defaultLabel = strip_tags($this->getMessage());

    if (empty($this->defaultLabel)) {
      $this->defaultLabel = t('Event #@id', array('@id' => $this->event_id));
    }

    return $this->defaultLabel;
  }

  /**
   * Overrides Entity::buildContent().
   * @param string $view_mode
   * @param null $langcode
   * @return mixed
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content = array();

    // global information about the event
    $content['message'] = array(
      '#type' => 'item',
      '#title' => t('Message'),
      '#markup' => $this->getMessage(),
    );
    $content['module'] = array(
      '#type' => 'item',
      '#title' => t('Module'),
      '#markup' => $this->getModule(),
    );
    $content['machine_name'] = array(
      '#type' => 'item',
      '#title' => t('Machine name'),
      '#markup' => $this->getMachineName(),
    );
    $content['timestamp'] = array(
      '#type' => 'item',
      '#title' => t('Date'),
      '#markup' => format_date($this->getTimestamp(), 'long'),
    );

    $account = user_load($this->getUid());
    $content['user'] = array(
      '#type' => 'item',
      '#title' => t('User'),
      '#markup' => theme('username', array('account' => $account)),
    );

    // show all arguments in a vertical_tab
    $content['arguments'] = array(
      '#type' => 'vertical_tabs',
      '#tree' => TRUE,
      '#weight' => 99,
    );

    foreach ($this->getArguments() as $key => $argument) {
      $content['arguments']['fieldset_' . $key] = array(
        '#type' => 'fieldset',
        '#title' => ucfirst($key),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#group' => 'arguments',
        '#tree' => TRUE,
        '#weight' => -2,
      );
      $content['arguments']['fieldset_' . $key]['argument_' . $key] = array(
        '#type' => 'item',
        '#markup' => $this->formatArgument($key, $argument),
      );
    }

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  protected function formatArgument($name, $argument) {
    $back = '';
    $data = $argument->getData();
    if (is_array($data) || is_object($data)) {
      foreach ($data as $k => $v) {
        $back .= '<div style="padding-left:10px;">[<strong>' . check_plain($k) . '</strong>] (<em>' . gettype($v) . '</em>): ' . $this->parseObject($v) . '</div>';
      }
    }
    else {
      $back = nl2br(check_plain($data));
    }
    $back = '<div><strong>' . check_plain($name) . '</strong> (<em>' . gettype($data) . '</em>): ' . $back . '</div>';
    return $back;
  }

  protected function parseObject($obj, $recursive = 0) {
    if ($recursive > $this->max_recursion) {
      return t('<em>Too many nested objects ( @recursion )</em>', array('@recursion' => $this->max_recursion));
    }
    if (is_scalar($obj) || is_null($obj)) {
      return is_string($obj) ? nl2br(trim(check_plain($obj))) : $obj;
    }

    $back = '';
    $css = 'style="padding-left:' . ($recursive + 10) . 'px;"';
    foreach ($obj as $k => $v) {
      $back .= '<div ' . $css . ' >[<strong>' . check_plain($k) . '</strong>] (<em>' . gettype($v) . '</em>): ' . $this->parseObject($v, $recursive + 1) . '</div>';
    }
    return $back;
  }

}

/**
 * Contains the entity classes for Past DB.
 */
class PastEventArgument extends Entity implements PastEventArgumentInterface {

  public $argument_id;
  public $event_id;
  protected $original_data;
  public $name;
  public $type;
  public $raw;

  public function getData() {
    $return = NULL;
    $result = db_query('SELECT * FROM {past_event_data} WHERE argument_id = :argument_id', array(':argument_id' => $this->argument_id));
    if ($this->type == 'array') {
      $return = array();
      foreach ($result as $row) {
        $return[$row->name] = $row->serialized ? unserialize($row->value) : $row->value;
      }
    }
    elseif (!in_array($this->type, array('integer', 'string', 'double', 'boolean'))) {
      $return = new stdClass();
      foreach ($result as $row) {
        $return->{$row->name} = $row->serialized ? unserialize($row->value) : $row->value;
      }
    }
    else {
      if ($row = $result->fetchObject()) {
        $return = $row->value;
      }
    }
    return $return;
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

/**
 * Past event data entity.
 */
class PastEventData extends Entity implements PastEventDataInterface {
  public $data_id;
  public $argument_id;
  public $name;
  public $type;

  public function id() {
    return $this->data_id;
  }

  public function getKey() {
    return $this->name;
  }

  public function getType() {
    return $this->type;
  }
}

/**
 * Use a separate class for past_event types so we can specify some defaults
 * modules may alter.
 */
class PastEventType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  public function __construct($values = array()) {
    parent::__construct($values, 'past_event_type');
  }

  /**
   * Returns whether the past_event type is locked, thus may not be deleted or renamed.
   *
   * PastEvent types provided in code are automatically treated as locked, as well
   * as any fixed past_event type.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

