<?php

/**
 * @file
 * Hooks documentation for the services_token_access module.
 */

/**
 * Invoked when a user's token is deleted.
 * 
 * @param int $uid
 *   User ID
 */
function hook_services_token_access_delete($uid) {
  dpm('deleted token for ' . $uid);
}

/**
 * Invoked when all tokens on the site are removed.
 */
function hook_services_token_access_truncate() {
  dpm('all tokens have been removed');
}

/**
 * Invoked when a user's token is (re)generated.
 * 
 * @param int $uid
 *   User ID
 * @param string $token
 *   Token string
 */
function hook_services_token_access_update($uid, $token) {
  dpm('updated token for ' . $uid);
  dpm($token);
}
