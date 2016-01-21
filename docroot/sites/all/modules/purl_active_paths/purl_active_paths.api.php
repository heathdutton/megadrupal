<?php
/**
 * @file
 * purl_active_paths.api.php
 * Hooks provided by the Purl Active Paths module.
 */

/**
 * Hook to add paths to be included/excluded from url outbound rewrites.
 * Should return a keyed array where each key is either 'global' or a provider
 * identifier and each value is a sub-array of paths for inclusion/exclusion.
 */
function hook_purl_active_paths() {
  return array(
    'global' => array(
      'ahah*',
    ),
  );
}
