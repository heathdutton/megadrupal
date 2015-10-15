<?php

/**
 * @file
 * Convenient one-liner include for settings.php.
 * Add the following directly there if you want to save a file operation
 * (mind the path changes, then!).
 */

require_once dirname( __FILE__ ) . '/sitematcher.class.inc';

$sitematcher = new DrupalSiteMatcher();
extract($sitematcher->settings);
$conf = array_merge($conf, $sitematcher->conf);
