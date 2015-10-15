<?php

/**
 * @file
 * Convenient wrapper include for sites.php in one line,
 * Add the following directly there if you want to save a file operation
 * (mind the path changes, then!).
 */


require_once dirname( __FILE__ ) . '/sitematcher.class.inc';

$sitename = DrupalSiteMatcher::getSiteName();
$sitematcher = new DrupalSiteMatcher($sitename);
$sites[$sitename] = $sitematcher->internals['site'];
