<?php
/**
 * @file
 * Provides an API for the lazy vars backend.
 */

/**
 * Provides an API for persistent variables that should be loaded on demand.
 */
class LazyVars implements LazyVarsInterface {

  /**
   * The storage table.
   */
  const TABLE = 'lazy_vars';

  /**
   * A static cache container.
   * @var array
   */
  private static $cached_vars = array();

  /**
   * Sets a persistent variable into the storage table.
   *
   * @param string $var_name
   *   The name of the variable to set.
   * @param mixed $value
   *   The value to set. This can be scalar or compound data type as the data
   *   will be serialized.
   * @param bool $cache
   *   Decides if the var-value pair should be added to the static cache or not.
   */
  public static function set($var_name, $value, $cache = FALSE) {
    db_merge(self::TABLE)
      ->key(array('name' => $var_name))
      ->fields(array('value' => serialize($value)))->execute();
    if ($cache) {
      self::$cached_vars[$var_name] = $value;
    }
  }

  /**
   * Returns a persistent variable.
   *
   * @param string $name
   *   The name of the variable to return.
   * @param mixed $default
   *   The default value to use if this variable has never been set.
   * @param bool $cache
   *   Decides if the var-value pair should be added to the static cache or not
   *   after it has been retrieved from the database.
   *
   * @return
   *   The value of the variable.
   */
  public static function get($var_name, $default = NULL, $cache = TRUE) {
    $value = '';
    if (isset(self::$cached_vars[$var_name])) {
      return self::$cached_vars[$var_name];
    }
    else {
      $query = db_select(self::TABLE, 'cs')
        ->fields('cs', array('value'))
        ->condition('name', $var_name);
      $result = $query->execute()->fetchAssoc();
      if ($result) {
        $value = unserialize($result['value']);
        if ($cache) {
          self::$cached_vars[$var_name] = $value;
        }
        return $value;
      }
      else {
        return $default;
      }
    }
  }

  /**
   * Unsets a persistent variable.
   *
   * @param string $name
   *   The name of the variable to delete.
   */
  public static function del($var_name) {
    $num_deleted = db_delete(self::TABLE)
      ->condition('name', $var_name)
      ->execute();

    if (isset(self::$cached_vars[$var_name])) {
      unset(self::$cached_vars[$var_name]);
    }
  }
}