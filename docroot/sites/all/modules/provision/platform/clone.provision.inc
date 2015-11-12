<?php

/**
 * Clone command implementation
 *
 * This command when called will 
 *   1. Make a backup of the current site, before modifications are made.
 *   2. Execute 'provision deploy' to build the new site using the backup of the old site.
 *
 */

/**
 * Make sure we have a valid site being cloned, and that the file being cloned from exists
 */
function drush_provision_drupal_provision_clone_validate($new_name = null, $platform = null) {
  drush_bootstrap(DRUSH_BOOTSTRAP_DRUPAL_SITE);
}

/**
 * Make a backup before making any changes, and add extract the file we are restoring from
 */
function drush_provision_drupal_pre_provision_clone($new_name, $platform = null) {
  drush_invoke('provision-backup');
}

/**
 * Remove the extracted site directory
 */
function drush_provision_drupal_pre_provision_clone_rollback($new_name, $platform = null) {
  $success =  provision_file()->unlink(drush_get_option('backup_file'))
    ->succeed('Removed unused clone site package')
    ->fail('Could not remove unused clone site package');
}

/**
 * Switch the clone directories around now that we have the new db installed
 */
function drush_provision_drupal_provision_clone($new_name, $platform = null) {
  drush_set_option('old_platform', d()->platform->name);

  $options = d()->options;
  $options['uri'] = ltrim($new_name, '@');
  $hash_name = drush_get_option('#name') ? '#name' : 'name';
  $options[$hash_name] = $new_name;
  $options['platform'] = (isset($platform)) ? $platform : $options['platform'];
  $options['root'] = d($options['platform'])->root;
  $options['aliases'] = array();
  $options['redirection'] = 0;

  // XXX: right now we just drop SSL configuration when cloning, because
  // we do not check if the site name change is compatible with the
  // certificate. This can be removed when we a) check for wildcard
  // certs and b) allow the user to change the cert from the UI.
  $options['ssl_enabled'] = 0;
  unset($options['ssl_key']);

  // Do not copy CDN aliases to the cloned site --CDN vhost
  $options['cdn'] = 0;
  unset($options['cdn']);

  if ($profile = drush_get_option('profile', FALSE)) {
    $options['profile'] = $profile;
  }

  if ($db_server = drush_get_option('new_db_server', FALSE)) {
    $options['db_server'] = $db_server;
  }

  drush_invoke_process('@none', 'provision-save', array($new_name), $options);
  # note that we reset the aliases so they don't conflict with the original site

  // Build an options array to pass to the deploy task. This allows injection
  // of data into the deploy task. See: https://www.drupal.org/node/2471805
  $deploy_options = array(
    'old_uri' => d()->uri,
    'strict' => 0,
  );
  drush_command_invoke_all_ref('provision_deploy_options_alter', $deploy_options, 'clone');

  provision_backend_invoke($new_name, 'provision-deploy', array(drush_get_option('backup_file')), $deploy_options);

  if (!drush_get_error()) {
    provision_backend_invoke($new_name, 'provision-verify');
  }

  provision_reload_config('site', d()->site_path . '/drushrc.php');

}