<?php
/**
 * @file
 * Separates sites error logs into individual files, and makes an Apache Alias
 * from /error.log to the error log path.
 *
 * Once downloaded into a drush command folder, simply re-verify any site.
 * Once verified, check http://yoursite/error.log to see the raw apache error
 * logs.
 */

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * Creates the /var/log/aegir folder if it does not exist.
 */
function drush_provision_logs_pre_provision_verify() {
  $log_path = drush_get_option('provision_logs_path', '/var/log/aegir');
  if (!file_exists($log_path)) {
    mkdir($log_path, 0775);
  }
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * This runs for each tasks during the command
 *    drush @hostmaster hosting-tasks
 *
 * NOTE: This ONLY runs when being called from a hostmaster task.
 * This hook should ONLY be used to pass Options from a hostmaster task form to
 * the $task object, or if you don't need this functionality from the command
 * line.
 */
function drush_provision_logs_pre_hosting_task() {

  $task =& drush_get_context('HOSTING_TASK');

  // Set options for a Verify task a site.
  if ($task->ref->type == 'site' && $task->task_type == 'verify') {
    $task->options['logs_enabled'] = $task->ref->logs_enabled;
    $task->options['logs_available'] = $task->ref->logs_available;
  }
}

/**
 * Implements hook_provision_apache_vhost_config().
 *
 * Sets each site's error log to be at /var/aegir/logs/$uri.error.log
 *
 * Optionally makes http://$uri/errors available through the provisioned site.
 * WARNING: This currently has no form of authentication.
 */
function provision_logs_provision_apache_vhost_config($uri, $data) {
  $log_file_path = drush_get_option('provision_logs_file_path', '/var/log/aegir');
  $log_url_path = drush_get_option('provision_logs_url_path', 'error.log');

  $logs_enabled = drush_get_option('logs_enabled', TRUE);
  $logs_available = drush_get_option('logs_available', FALSE);

  $error_log_path = "$log_file_path/$uri.error.log";
  $vhost = array();

  $vhost[] = "";
  $vhost[] = "# PROVISION_LOGS";
  $vhost[] = "# ==============";
  if (!$logs_enabled) {
    $vhost[] = '# Site-specific logs have not been enabled.';
    return $vhost;
  }
  $vhost[] = "# Set error log to site-specific file.";
  $vhost[] = "ErrorLog $error_log_path";
  if ($logs_available) {
    $vhost[] = "";
    $vhost[] = "# Allow error log to be accessible from http://$uri/$log_url_path";
    $vhost[] = "Alias /$log_url_path $error_log_path";
  }
  $vhost[] = "# ==============";
  $vhost[] = "";
  return $vhost;
}
