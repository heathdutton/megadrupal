<?php
/**
 * Load a xu utility.
 *
 * This is the main utility loader for xu. It is preferential to use this over
 * xu_include, as it is possible that the method for loading libraries may
 * change in the future, and this will work consistently.
 *
 * E.g: Load the form helper....
 * <code>
 * <?php
 *   xu('form');
 * ?>
 * </code>
 *
 * @param $utility
 *  The name of the utility to load.
 *
 * @return array
 *  If the utility is a library, the utilities plugin configuration. If the
 *  utility is a helper, an instance of that helpers Class.
 */
function xu($utility) {
  static $cache = array();

  if (!isset($cache[$utility])) {
    ctools_include('plugins');
    $cache[$utility] = ctools_get_plugins('xu', 'helper', $utility);
  }

  return $cache[$utility];
}

/**
 * Load an xu helper
 *
 * @param $helper
 *  The name of the helper to load
 *
 * @return
 *  An instance of the helper's object
 */
function xu_helper($helper) {
  $helper_config = xu($helper);
  if (!empty($helper_config) && isset($helper_config['class'])) {
    return new $helper_config['class'];
  }
}


/**
 * Execute a function in an Xu library or file
 *
 * @param $function
 *  The name of the callback
 * @param $library
 *  The library the callback can be found in
 * @param $args
 *  Any arguments to pass to the callback
 *
 * @return bool|mixed
 *  Either the result of the callback, or FALSE.
 *
 */
function xu_callback($function, $library, $args) {
  xu($library);
  if (function_exists($function)) {
    return call_user_func($function, $args);
  }
  else {
    return FALSE;
  }
}

/**
 * Shorthand for xu_debug
 *
 * @param $input
 *  The variable to dump
 * @param null $name
 *  An optional name for the variable
 */
function xud($input, $name = NULL) {
  xu('debug');
  xu_debug($input, $name);
}

/**
 * Shorthand for xu_set_message
 *
 * @param $message
 * @param $status
 */
function xum($message, $status) {
  xu('error');
  xu_set_message($message, $status);
}
