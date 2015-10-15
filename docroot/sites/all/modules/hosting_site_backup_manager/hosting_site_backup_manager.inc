<?php

/**
 * @file
 * Helper functions.
 */

/**
 * Helper function to get the root directory where backups are exported.
 */
function _hosting_site_backup_manager_get_backup_export_root() {
  $root = variable_get('hosting_site_backup_manager_export_root', '/var/aegir/backup-exports');
  return $root;
}

