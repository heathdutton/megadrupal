<?php

define('DRUPAL_ROOT', dirname( __FILE__ ) . '/../../../..');

include DRUPAL_ROOT . '/sites/all/libraries/sitematcher/sitematcher.class.inc';

$test = new DrupalSiteMatcher('www.example.com');
var_export($test->settings);
var_export($test->conf);
var_export($test->internals);
