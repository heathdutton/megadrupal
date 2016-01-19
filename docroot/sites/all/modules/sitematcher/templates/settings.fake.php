<?php

/**
 * @file
 * Fake settings.php to work around conf_path() issues.
 *
 * Save a copy of this file as "settings.php" in any "sites/*"
 * site folder, EXCEPT IN "default" AND "all".
 *
 * @see https://www.drupal.org/node/2605632
 */

include dirname( __FILE__ ) . '/../default/settings.php';
