<?php

/**
 * @file
 * The hosting feature definition for hosting_drulenium.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return
 *  associative array indexed by feature key.
 */
function hosting_drulenium_hosting_feature() {
  $features['drulenium'] = array(
    // title to display in form
    'title' => t('Drulenium integration'),
    // description
    'description' => t('Enables Visual Regression testin.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_drulenium',
    // associate with a specific node type.
    //  'node' => 'nodetype',
    // which group to display in ( null , experimental )
    'group' => 'experimental',
  );


  return $features;
}

