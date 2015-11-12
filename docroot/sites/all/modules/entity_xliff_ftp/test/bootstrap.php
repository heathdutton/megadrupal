<?php

/**
 * @file
 * Bootstrap file for PHPUnit tests.
 */

// Load in phpunit and related (dev) dependencies.
require_once('vendor/autoload.php');

// Set the default timezone. While this doesn't cause any tests to fail, PHP
// complains if 'date.timezone' is not set in php.ini.
date_default_timezone_set('UTC');

