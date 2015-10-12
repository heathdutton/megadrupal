<?php
/**
 * @file
 * Utility script to export the glacial cache elements used to initiate the
 * cache on module install.
 *
 * Called using drush, this tool is primarily for module developers and not
 * required for the normal function of the module.
 */

$cids = array(
  'country' => 'appac:country',
  'domestic_letter_services' => 'appac:auspost_domestic_letter',
  'domestic_parcel_services' => 'appac:auspost_domestic_parcel',
  'international_letter_services' => 'appac:auspost_intnl_letter',
  'international_parcel_services' => 'appac:auspost_intnl_parcel',
);

// For drupal_var_export which is a modified version of var_export which
// adheres to the Drupal coding standards.
require_once 'includes/utility.inc';

printf("function _uc_auspost_pac_install_prebuilt_cache() {\n\n");

$last_timestamp = 0;
foreach ($cids as $variable => $cid) {
  if ($cache = cache_get($cid, 'cache_uc_auspost_pac_glacial')) {
    $value = drupal_var_export($cache->data, '  ');

    $last_timestamp = max($last_timestamp, $cache->created);

    printf("  \$%s = %s;\n\n", $variable, $value);
  }
  else {
    drupal_set_message(t('Cache item %item was missing!', array('%item' => $cid)), 'error');
  }
}

printf("  // Add each item to the cache now.\n");
foreach ($cids as $variable => $cid) {
  printf("  cache_set('%s', \$%s, 'cache_uc_auspost_pac_glacial', CACHE_PERMANENT);\n", $cid, $variable);
}
printf("  cache_set('appac:built-marker', array('source' => 'install', 'created' => %s), 'cache_uc_auspost_pac_glacial', CACHE_PERMANENT);\n", $last_timestamp);
printf("}\n");
