<?php

/**
 * Implements hook_drush_command().
 */
function lgp_drush_command() {
  $items = array();
  $items['lgp-console'] = array(
    'description' => 'Show log of functions: lfp(), lfv() and lfe().',
    'arguments' => array(),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(),
    'topics' => array('docs-policy'),
    'drupal dependencies' => array('lgp'),
    'aliases' => array('lgc'),
  );
  return $items;
}

function drush_lgp_console() {
  $filepath = lgp_get_temp_dir() . '/lgp.log';
  $exec = 'tail --retry -f ' . $filepath;
  drush_sql_bootstrap_further();
  while (TRUE) {
    if (file_exists($filepath) && is_readable($filepath)) {
      print "Press <Ctrl-C> to exit.\n";
      drush_shell_proc_open($exec);
      break;
    }
  }
}

/**
 * Print to stdout the output of lfp().
 */
function _lp() {
  $args = func_get_args();
  if (!array_key_exists(1, $args)) {
    $args[1] = NULL;
  }
  $args[2] = TRUE;
  call_user_func_array('lp', $args);
}

function _lfp() {
  $args = func_get_args();
  return call_user_func_array('_lp', $args);
}

/**
 * Print to stdout the output of lfd().
 */
function _ld() {
  $args = func_get_args();
  $args[1] = TRUE;
  call_user_func_array('ld', $args);
}

function _lfd() {
  $args = func_get_args();
  return call_user_func_array('_ld', $args);
}

/**
 * Print to stdout the output of lfp().
 */
function _lx() {
  $args = func_get_args();
  $args[1] = TRUE;
  call_user_func_array('lx', $args);
}

function _lfx() {
  $args = func_get_args();
  return call_user_func_array('_lx', $args);
}

/**
 * Print to stdout the output of lfp().
 */
function _lbt() {
  $args = func_get_args();
  if (!array_key_exists(0, $args)) {
    $args[0] = NULL;
  }
  $args[1] = TRUE;
  call_user_func_array('lbt', $args);
}
