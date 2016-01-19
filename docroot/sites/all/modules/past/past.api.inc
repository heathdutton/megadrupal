<?php

/**
 * @file
 * Contains the interface definition.
 */

/**
 * Represents a logged event.
 */
interface PastEventInterface {

  /**
   * Returns the ID of the event, if saved.
   *
   * @return integer
   *   The event ID.
   */
  public function id();

  /**
   * Sets the module of the event.
   *
   * @param string $module
   *   The name of the module which is logging the event.
   *
   * @return PastEventInterface
   *   The current object.
   */
  public function setModule($module);

  /**
   * Set the machine name of this event.
   *
   * Has no special meaning other than providing an easy way to group and
   * identify past events.
   *
   * @param string $machine_name
   *
   * @return PastEventInterface
   *   The current object.
   */
  public function setMachineName($machine_name);

  /**
   *
   * @param type $message
   *
   * @return PastEventInterface
   *   The current object.
   */
  public function setMessage($message);

  /**
   * Returns the event message.
   *
   * @return string
   *   The logged event message. Contrary to watchdog, past event messages are
   *   not translated.
   */
  public function getMessage();

  /**
   * Returns the severity of this event.
   *
   * @return integer
   *   One of severities listed in past_event_severities().
   */
  public function getSeverity();

  /**
   * Sets the severity of this event.
   *
   * @param integer $severity
   *   One of severities listed in past_event_severities().
   *
   * @return PastEventInterface
   *   The current object.
   */
  public function setSeverity($severity);

  /**
   * Add an argument to the event.
   *
   * Each argument is identified by a key and has arbitrary data. The data is
   * normalized and persisted in a separate table so that it can be queried.
   *
   * @param string $key
   *   The name of the argument, must be unique for the event.
   * @param mixed $data
   *   The data that belongs to the argument and should be logged. Supported are
   *   scalar values, arrays, and objects. Exceptions are automatically decoded
   *   with _drupal_decode_exception() including a backtrace.
   * @param array $options
   *   Options for the argument. Currently supported:
   *     - exclude: Array of keys/properties that should be ignored on the data
   *       structure and not logged.
   *
   * @return PastEventArgumentInterface
   *   The created past event argument object.
   */
  public function addArgument($key, $data, array $options = array());

  /**
   * Add an array of arguments to the event.
   *
   * The key for each will consist of the $key_prefix_$array_key
   *
   * @param string $key_prefix
   *   The name prefix of the arguments,
   * @param array $data
   *   Array of arguments.
   * @param array $options
   *   Options for the argument. Currently supported:
   *     - exclude: Array of keys/properties that should be ignored on the data
   *       structure and not logged.
   *
   * @return array
   *   Array of the created past event argument objects.
   *
   * @see PastEventInterface::addArgument()
   */
  public function addArgumentArray($key_prefix, array $data, array $options = array());

  /**
   * Returns all arguments of this event.
   *
   * @return array
   *   Array of all arguments of this event, keyed by key.
   */
  public function getArguments();

  /**
   * Returns a specific argument based on the key.
   *
   * @param $key
   *   The key of the argument.
   *
   * @return PastEventArgumentInterface
   *   The argument object.
   */
  public function getArgument($key);

  /**
   * Adds an exception.
   *
   * @param $exception
   *   The exception.
   * @param array $options
   *   Options for the argument. Currently supported:
   *     - exclude: Array of keys/properties that should be ignored on the data
   *       structure and not logged.
   *
   * @return PastEventArgumentInterface
   *   The created past event argument object.
   */
  public function addException(Exception $exception, array $options = array());

  /**
   * Allows to reference this to a parent event.
   *
   * @param $event_id
   *   The event id of the event this belongs to.
   *
   * @return PastEventInterface
   *   The current object.
   */
  public function setParentEventId($event_id);

  /**
   * Sets the event timestamp.
   *
   * @param integer $timestamp
   *   The unix timestamp when this event happened.
   *
   * @return PastEventInterface
   *   The current object.
   */
  public function setTimestamp($timestamp);

  /**
   * Sets Drupal user id.
   *
   * @param int $uid
   *   Drupal user id.
   *
   * @return PastEventInterface
   *   The current object.
   */
  public function setUid($uid);

  /**
   * Gets Drupal user id.
   *
   * @return int
   *   Drupal user id.
   */
  public function getUid();

  /**
   * Returns the timestamp when the event happened.
   *
   * @return integer
   *   The timestamp when the event happened.
   */
  public function getTimestamp();

  /**
   * Returns the machine name of this event.
   *
   * @return string
   *   The machine name of this event.
   */
  public function getMachineName();

  /**
   * Returns the module that created this event.
   *
   * @return string
   *   The module name.
   */
  public function getModule();

  /**
   * Will flag the child event to use this as the parent once saved.
   *
   * @param int $event_id
   *   The event id that should use this event as the parent.
   */
  public function addChildEvent($event_id);

  /**
   * Saves the event.
   */
  public function save();
}

/**
 * Represents an event argument.
 */
interface PastEventArgumentInterface {

  /**
   * Returns the argument key.
   *
   * @return string
   *   The key of this argument.
   */
  public function getKey();

  /**
   * Returns the data of the event argument.
   *
   * @return mixed
   *   Based on the type, this returns either an array, if the data was an array
   *   or an object, a stdClass if it was an object or a scalar value.
   *
   * @todo. Standardize the return value, allow access to the metadata.
   */
  public function getData();

  /**
   * Returns the type of the argument.
   *
   * This is detected automatically on save and is either a native php type or
   * the name of the class if data was an object.
   *
   * @return string
   *   The type of the argument data.
   */
  public function getType();

  /**
   * Returns the raw data assigned with the argument.
   *
   * @return mixed
   *   The raw data as passed to PastEventArgumentInterface::setRaw().
   */
  public function getRaw();

  /**
   * Add raw data to the argument, which will be saved as a serialized string.
   *
   * Use this if it is required to have acess to the original data structure
   * later on.
   *
   * @param mixd $raw
   *   Any serializable data structure.
   */
  public function setRaw($raw);
}

/**
 * Represents event data.
 */
interface PastEventDataInterface {

  /**
   * Returns the ID of the data, if saved.
   *
   * @return integer
   *   The event ID.
   */
  public function id();

  /**
   * Returns the data key.
   *
   * @return string
   *   The key of this argument.
   */
  public function getKey();

  /**
   * Returns the type of the data.
   *
   * This is detected automatically on save and is either a native php type or
   * the name of the class if data was an object.
   *
   * @return string
   *   The type of the argument data.
   */
  public function getType();

}
