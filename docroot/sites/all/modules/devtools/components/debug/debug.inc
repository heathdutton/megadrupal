<?php

/**
 * Alias for print_rr();
 */
function pr() {
  $args = func_get_args();
  call_user_func_array('print_rr', $args);
}

/**
 * Displays or returns formatted print_r() output;
 * @return - string
 * @param object $data - object to be ran through print_r();
 * @param object $return[optional] - boolean; FALSE to print output, or TRUE to return it;
 */
function perm_debug($data) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $debug = \devtools\components\Debug::instance();
    $debug->permOutput($data);
  }
}

/**
 * Displays or returns formatted print_r() output;
 * @return - string
 * @param object $data - object to be ran through print_r();
 * @param object $return[optional] - boolean; FALSE to print output, or TRUE to return it;
 */
function print_rr($data, $return = FALSE) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $debug = \devtools\components\Debug::instance();
    return $debug->output($data, $return);
  }
}

/**
 * Alias for dsmm();
 */
function dm() {
  $args = func_get_args();
  call_user_func_array('dsmm', $args);
}

/**
 * Wrapper for print_rr() which runs output through drupal_set_message();
 * @param  [type] $data [description]
 * @return [type]       [description]
 */
function dsmm($data) {
  $args = func_get_args();
  list($handler, $return) = devtools_func_handler(__FUNCTION__, $args);

  if (!$handler) {
    drupal_set_message(print_rr($data, 1));
  }
}

/**
 * Wrapper for debug_backtrace();
 * @param  boolean $remove_extra [description]
 * @return [type]                [description]
 */
function bt($remove_extra = FALSE) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $bt = debug_backtrace();

    if ($remove_extra) {
      foreach ($bt as &$item) {
        unset($item['args']);
        unset($item['object']);
      }
    }

    return $bt;
  }
}
