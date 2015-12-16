<?php
/**
 * @file
 *
 * Example common include file, used to set the server token, include
 * required classes and define auxillary functions.
 *
 * NOTE: The examples only work on localhost and you must specify the correct
 * token (the same you configured using the module configuration page).
 */

// Set this token to your configured token
$TOKEN = 'change-this-to-the-token-in-your-module-config';

// Check if localhost
if($_SERVER['SERVER_NAME'] != 'localhost') {
  header('Unauthorized', TRUE, 401);
  die("<h1>Not Authorized</h1>");
}

// Include client classes
require_once('../client/DrupalUserAuth.class.inc');
require_once('../client/DrupalUserInfo.class.inc');
require_once('../client/DrupalUserList.class.inc');

// Set token
DrupalUserBase::setToken($TOKEN);
unset($TOKEN);

// Timing functions
static $_timer = 0;
function tic() { global $_timer; $_timer = microtime(true); }
function toc() { global $_timer; return (microtime(true) - $_timer) * 1000.0; @flush(); }
