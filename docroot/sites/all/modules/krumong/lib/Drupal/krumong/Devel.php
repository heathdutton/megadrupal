<?php

namespace Drupal\krumong;


class Devel {

  protected $main;
  protected $mainAC;

  function __construct($main, $mainAccessChecked) {
    $this->main = $main;
    $this->mainAC = $mainAccessChecked;
  }

  protected $dargs_printed;

  function dargs($always = TRUE) {
    if ($always || !$this->dargs_printed) {
      $call = $this->main->calledFromCall();
      if (isset($call['args'])) {
        $this->main->kPrint($call['args']);
        $this->dargs_printed = TRUE;
      }
    }
  }

  function ddebug_backtrace($return = FALSE, $pop = 0) {

    if (!user_access('access devel information')) {
      return;
    }

    // Show message if error_level is ERROR_REPORTING_DISPLAY_SOME or higher.
    // (This is Drupal's error_level, which is different from $error_level,
    // and we purposely ignore the difference between _SOME and _ALL,
    // see #970688!)
    if (variable_get('error_level', 1) < 1) {
      return;
    }

    $nicetrace = $this->niceTrace(debug_backtrace(), $pop);

    if ($return) {
      return $nicetrace;
    }
    $this->main->kPrint($nicetrace);
  }

  function dpm($input, $name = NULL, $type = 'status') {
    $this->mainAC->kMessage($input, $name, $type);
  }

  function dpq($query, $return = FALSE, $name = NULL) {

    if (!user_access('access devel information')) {
      return;
    }

    $sql = $this->queryToString($query);

    if ($return) {
      return $this->main->dump($sql, $name);
    }
    else {
      $this->main->kMessage($sql, $name);
    }
  }

  function kprint_r($input, $return = FALSE, $name = NULL, $function = 'print_r') {
    if ($return) {
      return $this->mainAC->dump($input, $name);
    }
    else {
      $this->mainAC->kPrint($input, $name);
    }
  }

  function kdevel_print_object($object, $prefix = NULL) {
    return $this->main->dump($object);
  }

  /**
   * TODO: We could move this to a different class.
   */
  protected function queryToString($query) {

    if (method_exists($query, 'preExecute')) {
      $query->preExecute();
    }

    $sql = (string) $query;
    $quoted = array();
    $connection = Database::getConnection();
    foreach ((array) $query->arguments() as $key => $val) {
      $quoted[$key] = $connection->quote($val);
    }

    return strtr($sql, $quoted);
  }

  /**
   * TODO: We could move this to a different class.
   */
  protected function niceTrace($backtrace, $pop = 0) {

    while ($pop-- > 0) {
      array_shift($backtrace);
    }

    $counter = count($backtrace);
    $path = $backtrace[$counter - 1]['file'];
    $path = substr($path, 0, strlen($path) - 10);
    $paths[$path] = strlen($path) + 1;
    $paths[DRUPAL_ROOT] = strlen(DRUPAL_ROOT) + 1;
    $nbsp = "\xC2\xA0";

    while (!empty($backtrace)) {
      $call = array();
      if (isset($backtrace[0]['file'])) {
        $call['file'] = $backtrace[0]['file'];
        foreach ($paths as $path => $len) {
          if (strpos($backtrace[0]['file'], $path) === 0) {
            $call['file'] = substr($backtrace[0]['file'], $len);
          }
        }
        $call['file'] .= ':' . $backtrace[0]['line'];
      }
      if (isset($backtrace[1])) {
        if (isset($backtrace[1]['class'])) {
          $function = $backtrace[1]['class'] . $backtrace[1]['type'] . $backtrace[1]['function'] . '()';
        }
        else {
          $function = $backtrace[1]['function'] . '()';
        }
        $backtrace[1] += array('args' => array());
        $call['args'] = $backtrace[1]['args'];
      }
      else {
        $function = 'main()';
        $call['args'] = $_GET;
      }
      $nicetrace[($counter <= 10 ? $nbsp : '') . --$counter . ': ' . $function] = $call;
      array_shift($backtrace);
    }

    return $nicetrace;
  }
}
