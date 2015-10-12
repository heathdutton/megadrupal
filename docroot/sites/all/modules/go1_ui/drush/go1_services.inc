<?php
/**
 * @file drush/go1_services.inc
 */

/**
 * Callback for 'go1_service' drush command.
 *
 * @param  string $service_name
 */
function drush_go1_services($service_name = 'all') {
  $config_fetcher = go1_container('helper.config_fetcher');

  if ($service_name === 'all') {
    $defs = $config_fetcher->getItems('go1_base', 'services', 'services', TRUE);
    drush_print_r($defs);
  }
  elseif (!empty($service_name)) {
    $def = $config_fetcher->getItem('go1_base', 'services', 'services', $service_name, TRUE);
    drush_print_r($def);
  }
  else {
    drush_set_error(DRUSH_APPLICATION_ERROR, 'Service not found: ' . $service_name);
  }
}
