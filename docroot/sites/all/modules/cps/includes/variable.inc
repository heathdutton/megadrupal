<?php

/**
 * @file
 * variable.inc replacement to override how variables are managed in Drupal.
 *
 * This relies on a patch from https://drupal.org/node/1193396
 */

/**
 * Loads the persistent variable table.
 *
 * The variable table is composed of values that have been saved in the table
 * with variable_set() as well as those explicitly specified in the
 * configuration file.
 */
function variable_initialize($conf = array()) {
  // NOTE: caching the variables improves performance by 20% when serving
  // cached pages.
  if ($cached = cache_get('variables', 'cache_bootstrap')) {
    $variables = $cached->data;
  }
  else {
    // Cache miss. Avoid a stampede.
    $name = 'variable_init';
    if (!lock_acquire($name, 1)) {
      // Another request is building the variable cache.
      // Wait, then re-run this function.
      lock_wait($name);
      return variable_initialize($conf);
    }
    else {
      // Proceed with variable rebuild.
      $variables = array_map('unserialize', db_query('SELECT name, value FROM {variable}')->fetchAllKeyed());
      cache_set('variables', $variables, 'cache_bootstrap');
      lock_release($name);
    }
  }

  foreach ($conf as $name => $value) {
    $variables[$name] = $value;
  }

  return $variables;
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
 *   The value of the variable. Unserialization is taken care of as necessary.
 *
 * @see variable_del()
 * @see variable_set()
 */
function _variable_get($name, $default = NULL) {
  global $conf;

  return isset($conf[$name]) ? $conf[$name] : $default;
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
 *
 * @see variable_del()
 * @see variable_get()
 */
function variable_set($name, $value) {
  global $conf;
  $drupal_change = function () use ($name, $value) {
    db_merge('variable')
      ->key(array('name' => $name))
      ->fields(array('value' => serialize($value)))
      ->execute();
  };
  $cps_change = function () use ($name, $value) {
    $GLOBALS['cps_conf'][$name] = $value;
  };
  variable_changed($name, $drupal_change, $cps_change);
  $conf[$name] = $value;
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
 *
 * @see variable_get()
 * @see variable_set()
 */
function variable_del($name) {
  global $conf;
  $drupal_change = function () use ($name) {
    db_delete('variable')
      ->condition('name', $name)
      ->execute();
  };
  $cps_change = function () use ($name) {
    unset($GLOBALS['cps_conf'][$name]);
  };
  variable_changed($name, $drupal_change, $cps_change);
  unset($conf[$name]);
}

/**
 * Execute a variable change (set or delete).
 *
 * @param string $name
 *   The name of the variable being changed.
 * @param callable $drupal_change
 *   If Drupal handles this variable change then this callable is executed.
 * @param callable $cps_change
 *   If CPS handles this variable change then this callable is executed.
 */
function variable_changed($name, callable $drupal_change, callable $cps_change) {
  // The actual writing of these variables happens in hook_exit().
  // @see cps_exit()
  // Guarantee the .module file is loaded.
  drupal_load('module', 'cps');
  $change_set = cps_get_current_changeset();
  // Also check a whitelist. Drupal uses a lot of variables for runtime cache
  // and we shouldn't attach those to changesets.
  $whitelist = FALSE;
  if (strpos($name, 'cache') !== FALSE) {
    $whitelist = TRUE;
  }
  if ($change_set == 'published' || $whitelist) {
    $drupal_change();
    cache_clear_all('variables', 'cache_bootstrap');
  }
  else {
    $cps_change();
    // The module's hook_exit will read this and write just the once.
    global $cps_conf_changed;
    $cps_conf_changed = TRUE;
  }
}
