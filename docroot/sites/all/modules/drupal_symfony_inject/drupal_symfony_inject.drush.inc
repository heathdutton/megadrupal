<?php
/**
 * @file
 * Drush commands to clear off/rebuild the container.
 */

/**
 * Implements hook_drush_command().
 * @see drush_drupal_symfony_inject_rebuild_symfony_container()
 */
function drupal_symfony_inject_drush_command() {
  $items['rebuild_symfony_container'] = array(
    'description' => dt('Rebuild the Symfony container, compile and set cache.'),
    'examples' => array(
      'rebuild-symfony-container' => 'drush rebuild-symfony-container',
    ),
    'aliases' => array('rsc'),
  );

  $items['clear_symfony_proxies'] = array(
    'description' => dt('Clear the generated Symfony lazy load proxies.'),
    'examples' => array(
      'clear-symfony-proxies' => 'drush clear-symfony-proxies',
    ),
    'aliases' => array('csp'),
  );

  return $items;
}

/**
 * Implements hook_drush_cache_clear().
 */
function drupal_symfony_inject_drush_cache_clear(&$types) {
  // Ensure that the Symfony proxy caches are flushed on cache clear.
  $types['drupalsymfonyinject'] = 'drush_drupal_symfony_inject_cache_flush';
}

/**
 * Drush command - drush_drupal_symfony_inject_rebuild_symfony_container.
 */
function drush_drupal_symfony_inject_rebuild_symfony_container() {
  $module_path = drupal_get_path('module', 'drupal_symfony_inject');
  require_once $module_path . '/drupal_symfony_inject.module';
  require_once $module_path . '/helpers/drupal_symfony_inject.helpers.inc';

  drush_print('Clear the current container from cache.');
  drupal_symfony_inject_flush_container();

  // We need to see whether container compilation has any errors. Therefore
  // we are not wrapping drupal_symfony_inject_register_class_loader around
  // a try catch block.
  drush_print('Rebuild and Registering class loader.');
  drupal_symfony_inject_register_class_loader(TRUE);

  drush_print('Loading Yaml config with custom params.');
  $proxies_path = variable_get(SYMFONY_CLASS_LOADER_PROXIES_DIR, '/tmp/doctrine/proxies');
  $container_builder = drupal_symfony_inject_load_yaml_config($proxies_path);

  if (
    !empty($container_builder) &&
    $container_builder instanceof Symfony\Component\DependencyInjection\ContainerBuilder
  ) {
    cache_set(SYMFONY_DI_CONTAINER_CACHE_KEY, $container_builder);
    drush_print('Symfony DI container compiled and cached successfully !!!');
  }
  else {
    drush_print('Failed to compile and save the Symfont Container Builder.');
  }
}

/**
 * Clear the generated Symfony lazy load proxies.
 */
function drush_drupal_symfony_inject_cache_flush() {
  _drupal_symfony_inject_clear_symfony_proxies();
}
