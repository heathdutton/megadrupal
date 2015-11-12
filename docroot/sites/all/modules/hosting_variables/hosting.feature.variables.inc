<?php

/**
 * @file
 *   The hosting feature definition for hosting_saas_utils.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return
 *   An associative array indexed by feature key.
 */
function hosting_variables_hosting_feature() {
  $features['variables'] = array(
    // title to display in form
    'title' => t('Variables'),
    // description
    'description' => t('Allows you to force arbitrary Drupal variables through the site node.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_variables',
    // associate with a specific node type.
    //  'node' => 'nodetype',
    // which group to display in ( null , experimental )
    'group' => 'experimental',
  );

  return $features;
}
