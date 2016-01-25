<?php

/**
 * @file
 * Drush integration for Dcycle
 */

/**
 * Implements hook_drush_command().
 *
 * These are the Dcycle commands available when you type "drush" on the
 * terminal.
 */
function dcycle_drush_command() {
  $items['dcycle-test'] = array(
    'description' => dt('Runs tests as defined in module.drush.inc.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'module' => 'A Drupal module.',
    ),
  );
  $items['dcycle-check'] = array(
    'description' => dt('Run a Check, and keep metrics and artifacts.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'module' => 'A Dcycle Check class, for example DcycleCoder to run code review.',
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function dcycle_drush_help($section) {
  switch ($section) {
    case 'drush:dcycle-test':
      $help = dt('Runs tests as defined in module.drush.inc.');
      return $help;
    case 'drush:dcycle-check':
      $help = dt('Runs checks on your code and keep the artifacts and metrics. For example, you can run
      drush dcycle-check DcycleCoder or drush dcycle-check DcycleCoder "sites/all/modules/custom sites/all/modules/mymodule_not_in_custom_folder". For most metrics, if a list of modules is not provided, sites/all/modules/custom is assumed');
      return $help;
  }
}

// We must call interface_exists here because during the call
// to drush en dcycle, the interface is not present yet.
if (interface_exists('DcycleLog')) {
  class DcycleDrushLog implements DcycleLog {
    public function log($text, $vars = array()) {
      drush_log(dt($text, $vars), 'ok');
    }

    public function error($text, $vars = array()) {
      $this->log('DCYCLE_FAILURE');
      drush_set_error('DCYCLE_ERROR', dt($text, $vars));
      drush_set_context('DRUSH_ERROR_CODE', 1);
    }
  }
}

/**
 * Run 'dcycle-test'.
 */
function drush_dcycle_test() {
  $args = func_get_args();
  $module = $args[0];

  dcycle_test($module, new DcycleDrushLog);
}

/**
 * Run 'dcycle-test'.
 */
function drush_dcycle_check() {
  $args = func_get_args();
  $class = isset($args[0]) ? $args[0] : NULL;
  $log = new DcycleDrushLog;

  array_shift($args);
  return dcycle_check($class, $log, $args);
}
