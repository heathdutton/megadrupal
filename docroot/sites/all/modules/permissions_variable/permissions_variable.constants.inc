<?php
/**
 * @file
 * Define constants used by permissions_variable.
 */

// Paths.
define('PERMISSIONS_VARIABLE_CONFIG_PATH', 'admin/config/development/permissions_variable');
define('PERMISSIONS_VARIABLE_EXPORT_PATH', 'admin/people/permissions/export');

// Permissions.
define('PERMISSIONS_VARIABLE_PERMISSION_ADMINISTER', 'administer permissions variable');
define('PERMISSIONS_VARIABLE_PERMISSION_ADMINISTER_PERMISSIONS', 'bypass permissions access restriction');
define('PERMISSIONS_VARIABLE_PERMISSION_ADMINISTER_ROLES', 'bypass roles access restriction');

// Variable defaults.
define('PERMISSIONS_VARIABLE_ENABLED', 1);
define('PERMISSIONS_VARIABLE_CREATE_ROLES', 1);
define('PERMISSIONS_VARIABLE_LOCK_ROLES', 1);
define('PERMISSIONS_VARIABLE_CACHE', 1);
define('PERMISSIONS_VARIABLE_VERBOSE', 1);
define('PERMISSIONS_VARIABLE_DENY_PERMISSIONS', 1);
define('PERMISSIONS_VARIABLE_DENY_ROLES', 1);

// JavaScript settings.
define('PERMISSIONS_VARIABLE_EXPORT_TOGGLE_TEXT_CODE_HIDDEN', 'Show export');
define('PERMISSIONS_VARIABLE_EXPORT_TOGGLE_TEXT_CODE_VISIBLE', 'Hide export');
