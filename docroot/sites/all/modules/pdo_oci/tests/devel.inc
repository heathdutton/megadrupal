<?php

openlog('pdo_oci', LOG_PID | LOG_PERROR, LOG_USER);

function ddebug_backtrace($return = FALSE, $pop = 0) {
  return dprint_r(debug_backtrace($return, $pop));
}

function dprint_r($input, $return = FALSE, $name = NULL, $function = 'print_r', $check= TRUE) {
  if ($name) {
    $name .= ' => ';
  }

  ob_start();
  $function($input);
  $output = ob_get_clean();

  syslog(LOG_ERR, $name . $output);
  return;

  $printed_value = '<pre>'. $name . $output .'</pre>';

  if ($return) {
    return $printed_value;
  }
  else {
    print $printed_value;
  }
}

