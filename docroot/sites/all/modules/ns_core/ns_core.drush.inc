<?php

/**
 * Implements hook_drush_command().
 */
function ns_core_drush_command() {
  $items = array();
  // Enable all modules.
  $items['ns-core-enable-all'] = array(
    'description' => "Enable all NodeStream modules.",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL
  );
  $items['ns-core-devel-enable'] = array(
    'description' => "Enable development modules.",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL
  );
  $items['ns-core-devel-disable'] = array(
    'description' => "Disable development modules.",
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function ns_core_drush_help($section) {
  switch ($section) {
    case 'drush:ns-core-enable-all':
      return dt("Enables all NodeStream modules provided by ns_core.");
    case 'drush:ns-core-devel-enable':
      return dt("Enables helpful development modules used during development of NodeStream and it's products.");
    case 'drush:ns-core-enable-all':
      return dt("Disables helpful development modules used during development of NodeStream and it's products.");
  }
}

/**
 * Enable all NodeStream modules.
 */
function drush_ns_core_enable_all() {
  drush_include_engine('drupal', 'environment');
  $modules = array(
    'ns_ad',
    'ns_article',
    'ns_blog',
    'ns_prod_blog',
    'ns_prod_rss',
    'ns_prod_news',
    'ns_prod_search',
    'ns_contributor',
    'ns_fact',
    'ns_footer_content',
    'ns_form',
    'ns_media',
    'ns_page',
    'ns_poll',
    'ns_promo',
    'ns_text',
    'ns_workbench',
    'ns_field_translation',
    'ns_i18n',
  );
  module_enable($modules);
  drupal_flush_all_caches();
  drush_log(dt('All NodeStream modules was enabled'), 'ok');
}

/**
 * Returns array of development modules.
 */
function ns_core_devel_modules() {
  return array(
    'devel',
    'devel_generate',
    'diff',
    'field_ui',
    'views_ui',
    'simpletest',
    'selenium',
    'simpletest_clone',
    'locale',
    'rules_admin',
  );
}

/**
 * Enable development modules.
 */
function drush_ns_core_devel_enable() {
  drush_include_engine('drupal', 'environment');
  module_enable(ns_core_devel_modules());
  drupal_flush_all_caches();
  drush_log(dt('A set of handy development tools was enabled'), 'ok');
}

/**
 * Disable development modules.
 */
function drush_ns_core_devel_disable() {
  drush_include_engine('drupal', 'environment');
  module_disable(ns_core_devel_modules());
  drupal_flush_all_caches();
  drush_log(dt('A set of handy development tools was disabled'), 'ok');
}
