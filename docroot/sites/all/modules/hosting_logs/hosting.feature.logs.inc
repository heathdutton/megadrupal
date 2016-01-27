<?php

/**
 * @file
 *   The hosting feature definition for hosting_logs.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return
 *  associative array indexed by feature key.
 */
function hosting_logs_hosting_feature() {
  $features['logs'] = array(
    // title to display in form
    'title' => t('Webserver logs integration'),
    // description
    'description' => t('Provides a sites error and watchdog logs to hostmaster front end.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_logs',
    // associate with a specific node type.
    //  'node' => 'nodetype',
    // which group to display in ( null , experimental )
    'group' => 'experimental',
    'role_permissions' => array(
      'aegir platform manager' => array(
        'access hosting logs',
      ),
      'aegir account manager' => array(
        'access hosting logs',
      ),
    )
  );

  return $features;
}
