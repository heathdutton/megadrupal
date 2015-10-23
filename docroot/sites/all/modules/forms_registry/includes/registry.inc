<?php
/**
 * @file
 * Provides a custom form registry.
 */

/**
 * A forms registry.
 * Keeps track of the forms and their required files.
 */
class FormsRegistry {

  const CACHE_BIN = 'cache';
  const CACHE_KEY = 'forms_registry';
  const HOOK_NAME = 'forms_registry';

  /**
   * Stores the registry data.
   * @var array
   */
  private static $data;

  /**
   * Returns the custom form registry.
   *
   * @param string $id
   */
  public static function get($id) {
    if (!self::$data) {
      self::load();
    }
    return isset(self::$data[$id]) ? self::$data[$id] : NULL;
  }

  /**
   * Loads the form registry from the database.
   */
  private static function load() {
    $cache = cache_get(self::CACHE_KEY, self::CACHE_BIN);
    if (isset($cache->data)) {
      self::$data = $cache->data;
    }
    else {
      self::rebuild();
    }
  }

  /**
   * Rebuilds the form registry.
   */
  public static function rebuild() {
    self::$data = module_invoke_all(self::HOOK_NAME);
    cache_set(self::CACHE_KEY, self::$data, self::CACHE_BIN);
  }

}