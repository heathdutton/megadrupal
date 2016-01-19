<?php
/**
 * @file
 * Tealium Tag Manager stores Tealium Tags.
 */

final class TealiumTagManager {
  /**
   * @var array
   *   array of (key => array(value,value)) pairs
   */
  private static $tags = array();

  /**
   * @var null
   *   self
   */
  private static $instance = NULL;

  /**
   * Singleton, Do Not Want.
   */
  private function __construct() {}

  /**
   * Singleton, Do Not Want.
   */
  private function __clone() {}

  /**
   * Singleton factory.
   *
   * @return object
   *   self
   */
  public static function getInstance() {
    if (!isset(self::$instance)) {
      self::$instance = new self();
    }
    return self::$instance;
  }

  /**
   * Add key/value pair to store.
   *
   * @param string $key
   *   Key to store against.
   * @param string $value
   *   Value to store.
   * @param bool $overwrite
   *   If TRUE, overwrite existing value(s).
   */
  public function add($key, $value, $overwrite = FALSE) {
    if (!isset(self::$tags[$key]) || $overwrite == TRUE) {
      self::$tags[$key] = array();
    }

    // Avoid duplicate values being added to the tag.
    if (!in_array($value, self::$tags[$key])) {
      self::$tags[$key][] = $value;
    }
  }

  /**
   * Add key/array value pair to store.
   *
   * @param string $key
   *   Key to store against.
   * @param array $values
   *   Array of values to store.
   */
  public function addArray($key, $values) {
      self::$tags[$key] = $values;
  }

  /**
   * Return comma-delimited list of tags for the given key.
   *
   * @param string $key
   *   Tag to get values for.
   *
   * @return mixed
   *   String of tags, else NULL.
   */
  public function get($key) {
    if (!isset(self::$tags[$key])) {
      return NULL;
    }
    return self::$tags[$key];
  }

  /**
   * Delete the given key and its values.
   *
   * @param string $key
   *   Tag to delete
   */
  public function delete($key) {
    unset(self::$tags[$key]);
  }

  /**
   * Get all stored tags as comma-delimited lists.
   *
   * @return array
   *   Array of tags.
   */
  public function getAll() {
    // Return a copy of the values.
    return self::$tags;
  }
}
