<?php

/**
 * @file
 * Provides MaPS Suite integration.
 */

/**
 * Implements hook_maps_suite_admin_overview().
 */
function maps_suite_maps_suite_admin_overview() {
  $info = system_get_info('module', 'maps_suite');
  // @todo use the project version when released on d.org
  return array(
    t('Version') => $info['maps_version'],
  );
}
