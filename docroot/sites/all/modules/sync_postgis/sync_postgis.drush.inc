<?php

/**
 * Implements hook_drush_command().
 */
function sync_postgis_drush_command() {
  $items = array();
  $items['sync_postgis_init'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'description' => dt('Initialize PostGIS tables.'),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function sync_postgis_drush_help($section) {
  switch ($section) {
    case 'drush:sync_postgis_init':
      return dt('The base tables in PostGIS will be created if they do not
exist yet, and any required bundle tables for extra field
data to be recreated. You will need to re-sync your data
after this.');
  }
}

/**
 * Create PostGIS base tables and indexes.
 */
function drush_sync_postgis_init() {
  module_load_include('inc', 'sync_postgis', 'sync_postgis.admin');
  sync_postgis_ensure_base_tables();
  sync_postgis_create_bundle_tables();
}
