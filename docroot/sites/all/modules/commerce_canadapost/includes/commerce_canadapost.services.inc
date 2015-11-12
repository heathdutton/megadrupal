<?php

/**
 * @file
 * Shipping Service functions for the Commerce Canada Post module.
 */


/**
 * Saves an enabled service.
 *
 * @param array $service
 *   An array containing 'name', 'title', 'weight', and 'status' of the service.
 *
 * @return int
 *   The return value of the call to drupal_write_record(), FALSE on failure or
 *   SAVED_NEW / SAVED_UPDATED on success.
 */
function commerce_canadapost_service_save($service) {
  // Provide 'name' key and update record if already exists.
  $op = drupal_write_record('commerce_canadapost_service', $service, in_array($service['name'], array_keys(commerce_canadapost_load_disabled_services())) ? array('name') : array());

  // If insert was successful, notify other modules that a new service was
  // created.
  if ($op !== FALSE) {
    module_invoke_all('commerce_canadapost_service_insert', $service);
  }

  return $op;
}

/**
 * Disables a service by deleting its database row and related rules.
 *
 * @param string $name
 *   The machine name of the package service to delete.
 */
function commerce_canadapost_service_delete($name) {
  $shipping_service = commerce_shipping_service_load($name);

  // Check if any line items depend on service.
  if (commerce_canadapost_service_line_item_count($name) > 0) {
    // Change status to 'disabled' instead of deleting row.
    $data = array(
      'name' => $name,
      'status' => 'disabled',
    );
    drupal_write_record('commerce_canadapost_service', $data, 'name');
  }
  else {
    // Delete row.
    db_delete('commerce_canadapost_service')
      ->condition('name', $name)
      ->execute();
  }

  rules_config_delete(array('commerce_shipping_service_' . $name));

  watchdog('commerce_canadapost', 'Removed shipping service: @title.', array('@title' => $shipping_service['title']), WATCHDOG_NOTICE);

  // Notify other modules that this service was deleted.
  module_invoke_all('commerce_canadapost_service_delete', $shipping_service);
}

/**
 * Counts and returns the number of line items for a specified service.
 */
function commerce_canadapost_service_line_item_count($service) {
  $query = new EntityFieldQuery();

  $query
    ->entityCondition('entity_type', 'commerce_line_item', '=')
    ->fieldCondition('commerce_shipping_service', 'value', $service, '=')
    ->count();

  return $query->execute();
}

/**
 * Generates a machine name from a Canada Post service code.
 *
 * @param string $service_code
 *   A Canada Post service code (e.g. "USA.SP.AIR").
 *
 * @return string
 *   The generated machine name.
 */
function commerce_canadapost_generate_service_machine_name($service_code) {
  // Replace . with _ and convert to lower case.
  $service_code = strtolower(strtr($service_code, '.', '_'));

  // Strip any disallowed characters.
  $service_code = preg_replace('/[^a-z0-9_]+/', '', $service_code);

  return 'canadapost_' . $service_code;
}

/**
 * Loads all services with 'enabled' status from the database.
 */
function commerce_canadapost_load_enabled_services() {
  // Attempt to retrieve list from cache.
  $services = &drupal_static(__FUNCTION__);
  if (!isset($services)) {
    $services = db_query("SELECT * FROM {commerce_canadapost_service} WHERE status = 'enabled'")
      ->fetchAllAssoc('name', PDO::FETCH_ASSOC);
  }

  return $services;
}

/**
 * Loads all services with 'disabled' status from the database.
 */
function commerce_canadapost_load_disabled_services() {
  // Attempt to retrieve list from cache.
  $services = &drupal_static(__FUNCTION__);
  if (!isset($services)) {
    $services = db_query("SELECT * FROM {commerce_canadapost_service} WHERE status = 'disabled'")
      ->fetchAllAssoc('name', PDO::FETCH_ASSOC);
  }

  return $services;
}

/**
 * Resets shipping service list, rebuilds default rules & menu items.
 */
function commerce_canadapost_rebuild_enabled_services() {
  drupal_static_reset('commerce_canadapost_load_enabled_services');
  drupal_static_reset('commerce_canadapost_load_disabled_services');
  commerce_shipping_services_reset();
  entity_defaults_rebuild();
  rules_clear_cache(TRUE);
  menu_rebuild();
}

/**
 * Fetches list of all available Shipping Services from Canada Post.
 *
 * @return array
 *   An associative array keyed by the Canada Post service code.
 */
function commerce_canadapost_services_list() {
  $services = &drupal_static(__FUNCTION__);

  // If the services haven't been loaded yet, do so now.
  if (!isset($services)) {
    // Try to retrieve service list from cache.
    $cached_services = cache_get('commerce_canadapost_services_list:' . commerce_canadapost_get_language());
    if ($cached_services && (!empty($cached_services->data))) {
      return $cached_services->data;
    }

    $response = commerce_canadapost_api_request('https://soa-gw.canadapost.ca/rs/ship/service', NULL, t('Requesting shipping service list from Canada Post'));

    $services = array();
    if ($response) {
      foreach ($response as $service) {
        $services[(string) $service->{'service-code'}] = array(
          'name' => (string) $service->{'service-name'},
          'url'  => (string) $service->{'link'}->attributes()->{'href'},
        );
      }
    }

    // Cache service list (append language).
    cache_set('commerce_canadapost_services_list:' . commerce_canadapost_get_language(), $services, 'cache', CACHE_TEMPORARY);
  }

  return $services;
}
