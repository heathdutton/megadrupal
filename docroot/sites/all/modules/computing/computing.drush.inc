<?php

/**
 * Drupal Computing drush interface.
 * To use this, please install Drush at http://drupal.org/project/drush
 */

/**
 * Implements hook_drush_command().
 */
function computing_drush_command() {
  $items = array();

  $items['computing-call'] = array(
    'description' => 'Call any Drupal/PHP functions and print results as json for further process. All parameters must be encoded in Json.',
    'hidden' => TRUE,
    'examples' => array(
      'drush computing-call node_load 1' => 'Display node 1 data in json',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  // copied some code from php-eval in core.drush.inc.
  $items['computing-eval'] = array(
    'description' => 'Call any Drupal/PHP snippet and print results as json for further process.',
    'hidden' => TRUE,
    'arguments' => array(
      'code' => 'PHP code, or "-" to read code from STDIN',
    ),
    'required-arguments' => TRUE,
    'examples' => array(
      'drush computing-eval "$nid=1; return node_load($nid);"' => 'Display node 1 data in json',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/**
 * Call a Drupal function, where parameters are passed in as json string.
 * Print JSON results.
 */
function drush_computing_call() {
  //drush_print(func_num_args());
  // looks like it's fine to use drush_get_arguments(), but I'll use func_get_args() instead..
  $arguments = func_get_args();

  $func_name = array_shift($arguments);
  if (!function_exists($func_name)) {
    error_log("Function '$func_name' does not exist.");
    exit(-1);
  }

  // note: each argument is encoded in json, and need to decode before passing to a function.
  $result = @call_user_func_array($func_name, array_map('drush_json_decode', $arguments));
  drush_print_r($result);

  // $json = drush_format($result, NULL, 'json')
  // ATTENTION: this will call json_encode($param, TRUE), which basically return an array if it's json object.
  // to use object, the user function should manually convert array to object.
  $json = drush_json_encode($result);
  // see [#1697890], a simple work around is to add a whitespace.
  drush_print_pipe($json .' ');
  //return $result;
}


/**
 * Process code and print results in JSON
 * Difference to "eval": is that here we return results in JSON, instead of print out.
 */
function drush_computing_eval($code) {
  // read STDIN followed the example of drush_core_php_script()
  $php = ($code == '-') ? stream_get_contents(STDIN) : $code;

  // todo: might need to encode/decode here.
  $result = eval($php);
  drush_print_r($result);

  //drush_print(serialize($result));
  $json = drush_json_encode($result);
  drush_print_pipe($json .' ');

  // the returned value would be saved in $results['object']
  //return $result;
}
