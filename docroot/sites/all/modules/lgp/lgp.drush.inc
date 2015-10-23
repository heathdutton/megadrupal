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