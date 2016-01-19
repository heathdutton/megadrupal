<?php

/**
 * Simple static container class to avoid using $GLOBALS...
 */
class VariablesThatSuckLess {
  public static $variables = array();

  public static $cache_keys_to_clear = array();

  public static $conf = array();

  public static $prefetch = array(
    'default_values' => array(),
    'db_keys' => array(),
  );

  public static $initial_prefetch = array();

  public static function initialize($conf) {
    self::$conf = $conf;
    if ($cache = cache_get('vtsl_prefetch', 'cache_bootstrap')) {
      self::$prefetch = $cache->data;
      self::$initial_prefetch = self::$prefetch;
      $cids = array_keys(self::$prefetch['db_keys']);
      foreach (cache_get_multiple($cids, 'cache_variable') as $key => $value) {
        self::$variables[$key] = $value->data;
      }
    }
    drupal_register_shutdown_function(array('VariablesThatSuckLess', 'shutdown'));
    if (empty(self::$conf['cache_class_cache_variable'])) {
      self::$conf['cache_class_cache_variable'] = 'DrupalDatabaseCache';
    }
    return self::$conf + self::$variables;
  }

  public static function shutdown() {
    if (self::$prefetch != self::$initial_prefetch) {
      cache_set('vtsl_prefetch', self::$prefetch, 'cache_bootstrap');
    }
    // Clearing during shutdown decreases the likelyhood that another process
    // will read the value we're clearing from the database, then set it in the
    // cache just after a write operation would clear it.
    foreach (self::$cache_keys_to_clear as $name) {
      cache_clear_all($name, 'cache_variable');
    }
  }

  public static function delete($name) {
    db_delete('variable')
      ->condition('name', $name)
      ->execute();
    self::$cache_keys_to_clear[] = $name;
    unset(self::$variables[$name]);
    unset(self::$conf[$name]);
    unset(self::$prefetch['default_values'][$name]);
  }

  public static function set($name, $value) {
    db_merge('variable')->key(array('name' => $name))->fields(array('value' => serialize($value)))->execute();
    self::$cache_keys_to_clear[] = $name;
    self::$variables[$name] = $value;
    unset(self::$prefetch['default_values'][$name]);
  }
}

/**
 * Returns a persistent variable.
 *
 * Case-sensitivity of the variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to return.
 * @param $default
 *   The default value to use if this variable has never been set.
 *
 * @return
 *   The value of the variable.
 */
function variable_get($name, $default = NULL) {
  if (isset(VariablesThatSuckLess::$conf[$name]) || array_key_exists($name, VariablesThatSuckLess::$conf)) {
    return VariablesThatSuckLess::$conf[$name];
  }

  if (isset(VariablesThatSuckLess::$prefetch['default_values'][$name])) {
    return $default;
  }

  if (drupal_bootstrap(NULL, FALSE) <= DRUPAL_BOOTSTRAP_VARIABLES) {
    return $default;
  }

  if (isset(VariablesThatSuckLess::$variables[$name]) || array_key_exists($name, VariablesThatSuckLess::$variables)) {
    return VariablesThatSuckLess::$variables[$name];
  }

  $value = db_query('SELECT value FROM {variable} WHERE name = :name', array(':name' => $name))->fetchField();
  if ($value !== FALSE) {
    VariablesThatSuckLess::$variables[$name] = unserialize($value);
    cache_set($name, VariablesThatSuckLess::$variables[$name], 'cache_variable');
    VariablesThatSuckLess::$prefetch['db_keys'][$name] = TRUE;
    unset(VariablesThatSuckLess::$prefetch['default_values'][$name]);
    return VariablesThatSuckLess::$variables[$name];
  }
  else {
    VariablesThatSuckLess::$prefetch['default_values'][$name] = TRUE;
    return $default;
  }
}

/**
 * Sets a persistent variable.
 *
 * Case-sensitivity of the variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to set.
 * @param $value
 *   The value to set. This can be any PHP data type; these functions take care
 *   of serialization as necessary.
 */
function variable_set($name, $value) {
  VariablesThatSuckLess::set($name, $value);
}

/**
 * Unsets a persistent variable.
 *
 * Case-sensitivity of the variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to undefine.
 */
function variable_del($name) {
  VariablesThatSuckLess::delete($name);
}

/**
 * Initializes the Drupal variable system.
 *
 * @param $name
 *   An array of values from the site's settings.php file.
 */
function variable_initialize($conf) {
  return VariablesThatSuckLess::initialize($conf);
}

