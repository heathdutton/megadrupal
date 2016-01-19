<?php

require_once dirname(__FILE__) . '/go1_base.module';
require_once dirname(__FILE__) . '/drush/go1_cache.inc';
require_once dirname(__FILE__) . '/drush/go1_fontello.inc';

/**
 * Implements hook_drush_command()
 */
function go1_base_drush_command() {
  $cmds = array();

  $cmds['go1_require'] = array(
    'callback' => 'drush_go1_require',
    'description' => 'Help download all missing dependencies.',
    'arguments' => array(
      'module' => 'Download dependencies for specific module',
    ),
    'examples' => array(
      'drush go1_require => Download dependencies for all modules',
      'drush go1_require module_name => Download dependencies for specific module'
    ),
    'aliases' => array('go1-require', 'go1r'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'options' => array(
      'working-copy' => 'Preserves VCS directories, like .git, for projects downloaded using such methods.',
    )
  );

  $cmds['go1_cache_clear'] = array(
    'callback' => 'drush_go1_cache_clear',
    'description' => 'Flush cache by tags',
    'examples' => array(
      'drush go1cc go1-config' => 'Flush cached content tagged with go1-config',
      'drush go1cc node user' => 'Flush cached content tagged with node or user',
    ),
    'aliases' => array('cache:clear', 'go1cc', 'go1_cache:clear'),
  );

  $cmds['go1_fontello'] = array(
    'callback' => 'drush_fontello',
    'description' => 'Build Fontello library.',
    'examples' => array(
      'drush fontello => Build Fontello library',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('go1ft'),
  );

  return $cmds;
}

/**
 * Implements drush_hook_pre_COMMAND()
 *
 * Make sure cached-config is flushed before other config data rebuilt.
 */
function drush_go1_base_pre_cache_clear() {
  cache_clear_all('*', 'cache');
}

/**
 * Drush make is not inluded in version 4, which leading to fatal errors.
 */
if (class_exists('DrushMakeProject_Library')) {
  require_once dirname(__FILE__) . '/drush/go1_require.inc';
}
else {
  function drush_go1_require() {
    $msg = 'go1_require depends on make feature of Drush, which introduced in 5.x';
    drush_print_r($msg);
  }
}
