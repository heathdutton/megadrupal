<?php
/**
 * @file go1_base.misc.inc
 *
 * Mostly hack here, to be improved/removed.
 */

/**
 * Return if key|upercase/variable is not FALSE|NULL|0.
 */
function go1_valid($key, $get_value = FALSE) {
  $c = strtoupper($key);
  if (defined($c)) $return = constant($c);
  else $return = variable_get($key, FALSE);
  return $get_value ? $return : (boolean)($return);
}

/**
 * Check is Drupal system on dev or not.
 *
 * @return boolean
 */
function go1_debug() {
  return defined('GO1_DEBUG');
}

if (!function_exists('yaml_parse')) {
  /**
   * Read YAML file.
   *
   * @param  string $path Path to yaml file.
   * @return mixed
   */
  function yaml_parse_file($path) {
    if (!is_file(DRUPAL_ROOT . '/sites/all/libraries/spyc/Spyc.php')) {
      throw new \RuntimeException('Missing library: spyc');
    }

    if (!function_exists('spyc_load_file')) {
      require_once DRUPAL_ROOT . '/sites/all/libraries/spyc/Spyc.php';
    }

    return spyc_load_file($path);
  }
}

if (!function_exists('yaml_emit')) {
  function yaml_emit($data) {
    return spyc_dump($data);
  }
}

/**
 * Override default callback of $fn.
 *
 * @see  go1_fn()
 * @param  string   $fn
 * @param  callable $callback
 */
function go1_fn_fake($fn, $callback) {
  global $conf;
  $conf["go1fn:{$fn}"] = $callback;
}

/**
 * Similar to go1_fn_fake(). Usage:
 *
 *  $time = time();
 *  go1_fake::time(function() use ($time) { return $time; });
 *  echo go1_fn::time(); // same to $time
 *
 *  // Next 1 hour
 *  go1_fake::time(function() use ($time) { return $time + 3600; });
 *  echo go1_fn::time(); // same to $time + 3600
 */
class go1_fake {
  public static function __callStatic($fn, $args) {
    $GLOBALS['conf']["go1fn:{$fn}"] = $args[0];
  }
}

/**
 * Wrapper for class based forms.
 */
function go1_form_validate($form, &$form_state) {
  // Build the form
  list($class, $args) = $form['#go1_form'];

  $obj = go1_newv($class, $args);
  $obj->setForm($form);
  $obj->setFormState($form_state);
  $obj->validate();
}

/**
 * Wrapper for class based forms.
 */
function go1_form_submit($form, &$form_state) {
  // Build the form
  list($class, $args) = $form['#go1_form'];

  $obj = go1_newv($class, $args);
  $obj->setForm($form);
  $obj->setFormState($form_state);
  $obj->submit();
}

/**
 * Wrapper for class based forms.
 *
 * @todo  Test me.
 */
function go1_form($form, &$form_state) {
  // Get the variables from arguments
  $args       = func_get_args();
  $form       = array_shift($args);
  $form_state = array_shift($args);
  $class      = array_shift($args);
  $args       = reset($args);

  // Build the form
  $obj = go1_newv($class, $args);
  $obj->setForm($form);
  $obj->setFormState($form_state);

  $form = $obj->get();
  $form['#go1_form'] = array($class, $args);

  return $form;
}

/**
 * Shortcut to expression_language:evaluate.
 */
function go1_eval($expression) {
  return go1_container('expression_language')->evaluate($expression);
}
