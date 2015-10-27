<?php

/**
 * @file
 * Cache handling specific functions.
 */

/**
 * Implements hook_cache_clear_info().
 */
function devcontrol_cache_clear_info() {
  $ret = array();

  // Add hardcoded known caches.
  $ret['menu'] = array(
    'title'    => t("Rebuild menu"),
    'callback' => 'devcontrol_cache_clear_menu',
  );
  $ret['theme'] = array(
    'title'    => t("Empty theme registry"),
    'callback' => 'devcontrol_cache_clear_theme',
  );
  $ret['hook'] = array(
    'title'    => t("Clear hook cache"),
    'callback' => 'devcontrol_cache_clear_hook',
  );
  $ret['form'] = array(
    'title'    => t("Clear form state"),
    'callback' => 'devcontrol_cache_clear_form',
  );
  if (module_exists('simpletest')) {
    $ret['simpletest'] = array(
      'title'    => t("Clean test environment"),
      'callback' => 'devcontrol_cache_clear_simpletest',
    );
  }
  $ret['locale'] = array(
    'title'    => t("Clear locale cache"),
    'callback' => 'devcontrol_cache_clear_locale',
  );
  $ret['registry'] = array(
    'title'    => t("Rebuild autoload registry"),
    'callback' => 'devcontrol_cache_clear_registry',
  );
  if (module_exists('views')) {
    $ret['views'] = array(
      'title'    => t("Clear views cache"),
      'callback' => 'devcontrol_cache_clear_views',
    );
  }
  if (module_exists('entitycache')) {
    $ret['entitycache'] = array(
      'title'    => t("Clear entity cache"),
      'callback' => 'devcontrol_cache_clear_entitycache',
    );
  }
  $ret['entity'] = array(
    'title'    => t("Clear entity info cache"),
    'callback' => 'devcontrol_cache_clear_entityinfo',
  );
  $ret['field'] = array(
    'title'    => t("Clear field cache"),
    'callback' => 'devcontrol_cache_clear_field',
  );
  $ret['all'] = array(
    'title'    => t("Flush all"),
    'callback' => 'devcontrol_cache_clear_all',
  );

  return $ret;
}

/**
 * Clear hook cache.
 */
function devcontrol_cache_clear_hook() {
  cache_clear_all('module_implements', 'cache_bootstrap');
  cache_clear_all('hook_info', 'cache_bootstrap');
  cache_clear_all('boostrap_modules', 'cache_bootstrap');
  drupal_theme_rebuild();
  drupal_static_reset('module_implements');

  return t("Cached hook information and theme registry have been destroyed");
}

/**
 * Clear locale cache.
 */
function devcontrol_cache_clear_locale() {
  cache_clear_all('locale', 'cache', true);
  drupal_static_reset('locale');

  return t("Locale cache has been destroyed");
}

/**
 * Clear form cache.
 */
function devcontrol_cache_clear_form() {
  cache_clear_all('*', 'cache_form', true);

  return t("Form cache and state have been dropped");
}

/**
 * Clean simpletest environment.
 */
function devcontrol_cache_clear_simpletest() {
  simpletest_clean_environment();

  return t("Simpletest environment has been cleaned up");
}

/**
 * Clear theme registry and cache.
 */
function devcontrol_cache_clear_theme() {
  drupal_theme_rebuild();

  return t("Theme registry cache has been destroyed");
}

function devcontrol_cache_clear_views() {
  cache_clear_all('*', 'cache_views', true);
  cache_clear_all('*', 'cache_views_data', true);

  return t("Views cache has been destroyed");
}

/**
 * Rebuild menu.
 */
function devcontrol_cache_clear_menu() {
  menu_rebuild();

  return t("Menu has been rebuilt");
}

/**
 * Rebuild file registry and autoload cache.
 */
function devcontrol_cache_clear_registry() {
  registry_rebuild();

  return t("Registry has been rebuilt");
}

/**
 * Clear field cache.
 */
function devcontrol_cache_clear_field() {
  field_cache_clear();

  return t("Field cache has been cleared");
}

/**
 * Clear entity info cache.
 */
function devcontrol_cache_clear_entityinfo() {
  entity_info_cache_clear();

  return t("Entity info has been cleared");
}

/**
 * Clear entity cache.
 */
function devcontrol_cache_clear_entitycache() {
  foreach (entitycache_flush_caches() as $bin) {
    cache_clear_all('*', $bin, true);
  }

  return t("Entity cache has been cleared");
}

/**
 * Flush all caches.
 */
function devcontrol_cache_clear_all() {
  drupal_flush_all_caches();

  return t('Caches cleared.');
}
