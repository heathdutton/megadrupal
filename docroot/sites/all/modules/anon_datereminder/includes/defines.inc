<?php
/**
 * @file
 * Define common constants.
 */

/**
 * Permissions.
 */
define('ANON_DATEREMINDER_ADMINISTER_REMINDERS', 'Administer reminders');
define('ANON_DATEREMINDER_VIEW_OTHER_USER_REMINDERS', 'See visitor reminders');

/**
 * Enable states for a node type
 */
define('ANON_DATEREMINDER_TYPE_DISABLED', 0);
define('ANON_DATEREMINDER_TYPE_RETAIN', 1);
define('ANON_DATEREMINDER_TYPE_ALLOWED', 2);
define('ANON_DATEREMINDER_TYPE_ON', 3);

/**
 * Defaults
 */
define('ANON_DATEREMINDER_MAX_REMINDERS', 3);
define('ANON_DATEREMINDER_CRON_FREQUENCY', 60);

/**
 * Name of file with database apis.
 */
define('ANON_DATEREMINDER_DB', 'includes/db7');
