<?php

/**
 * @file
 * Contains all local-compile checking functions.
 */

/**
 * Returns whether java is installed and executable on the machine.
 */
function closure_compiler_java_installed() {
  $output = shell_exec('java 2>&1');
  return FALSE !== stripos($output, 'Usage');
}
