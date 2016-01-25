<?php

/**
 * Return an associative array of check names and callables.
 */
function hook_check_info() {
  return array(
    'some-thing' => array(
      // Whether to run this by default (without specifying --all at the command line).
      'run_by_default' => true,
      // The callable to run, this should accept no arguments and is invoked with 
      'callback' => '_example_check_some_thing',
      // The file to include to run this check.
      'file' => __FILE__,
    ),
  );
}

/**
 * Alter the set of checks made available.
 */
function hook_check_info_alter(&$checks) {
  unset($checks['some_check']);
}

/**
 * This example demonstrates a check callback.
 */
function _example_check_some_thing() {
  if (!example_verify_some_check()) {
    // If all is well you can return an array or just NULL
    return NULL;
  }
  else {
    return array(
      // The `status` should be one of CHECK_OK, CHECK_WARNING, CHECK_CRITICAL, or CHECK_UNKNOWN.
      'status' => CHECK_CRITICAL,
      // Message is a string to be run through `t()` and `dt()` with tokens for substitution.
      'message' => 'A message which can include a @substitution.',
      // An array of variables to do token replacement in `message` via the t() and dt() functions.
      'variables' => array(
        '@substitution' => 'translation token substitution',
      ),
    );
  }
}
