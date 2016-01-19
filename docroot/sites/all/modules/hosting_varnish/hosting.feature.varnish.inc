<?php
/**
 * @file
 * Hosting feature definition.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_varnish_hosting_feature() {
  $features['varnish'] = array(
    'title' => t('Varnish integration'),
    'description' => t('Manage Varnish cache for individual sites.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'group' => 'experimental',
    'module' => 'hosting_varnish',
  );
  return $features;
}

