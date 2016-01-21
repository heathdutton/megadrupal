<?php

/**
 * @file
 * Convenient wrapper include for sites.php in one line,
 * Add the following directly there if you want to save a file operation
 * (mind the path changes, then!).
 */


include dirname( __FILE__ ) . '/sitematcher.class.inc';

$sitematcher = new DrupalSiteMatcher();
$sites[$sitematcher->internals['sitename']] = $sitematcher->internals['site'];
