<?php

/**
 * @file
 * Chatblock cache functions for APC.
 */

/**
 * Remember whether APC is enabled.
 */
define('CHATBLOCK_IS_APC', function_exists('apc_fetch'));

/**
 * Standard timeout for user auth token
 */
define('CHATBLOCK_CACHE_TOKEN_TTL', 3600);

/**
 * If "variable_get" function exists, we are within drupal. If not, we
 * are being called from chatblock.php which will define the constant
 * with the corresponding sanitized $_POST value.
 */
if (function_exists('variable_get')) {
  define('CHATBLOCK_CACHE_ID', variable_get('chatblock_cache_id', 'ErrorNoCacheIdWasFound'));
}

/**
 * Delete a value from the cache.
 *
 * @param $keydata
 *   The key to use for APC. Will be prefixed with 'chatblock_'.
 *   Must consist of only [a-zA-Z0-9-_] characters, otherwise this
 *   will lead to errors (as it is not being cahecked for performance's sake.
 *   May also be an array of the form (key => value) if multiple values
 *   are to be set at once.
 * @param $delete_fallback_file
 *   Delete the fallback file as well.
 *
 * @see fallback_file.inc
 * @see chatblock_filecache_delete()
 */
function chatblock_cache_delete($keydata, $delete_fallback_file = TRUE) {
  if (CHATBLOCK_IS_APC) {
    if (!is_array($keydata)) {
      $keydata = array($keydata);
    }
    foreach ($keydata as $key) {
      $apc_key = 'chatblock_' . CHATBLOCK_CACHE_ID . "_$key";
      apc_delete($apc_key);
    }
  }
  if ($delete_fallback_file) {
    require_once(dirname(__FILE__) . '/fallback_file.inc');
    chatblock_filecache_delete($keydata);
  }
}

/**
 * Read a value from the cache.
 *
 * @param $key
 *   The key to use for APC. Will be prefixed with 'chatblock_'.
 *   Must consist of only [a-zA-Z0-9-_] characters, otherwise this
 *   will lead to errors (as it is not being checked for performance's sake.
 * @param $use_fallback_file
 *   Whether to write the value to a file if APC is not present.
 *
 * @return mixed
 *   The unserialized value.
 */
function chatblock_cache_get($key, $use_fallback_file = FALSE) {

  // Add namespace prefix.
  $apc_key = 'chatblock_' . CHATBLOCK_CACHE_ID . "_$key";

  if (CHATBLOCK_IS_APC) {
    // Always delete the existing value. Otherwise some APC versions will fail.
    $ret = apc_fetch($apc_key);
    if ($ret !== FALSE) {
      $ret = unserialize($ret);
    }
  }
  elseif ($use_fallback_file) {
    require_once(dirname(__FILE__) . '/fallback_file.inc');
    $ret = chatblock_filecache_get($key);
    if ($ret !== FALSE) {
      $ret = unserialize($ret);
    }
  }
  else {
    $ret = FALSE;
  }
  return $ret;
}

/**
 * Write a value to the cache.
 *
 * @param $keydata
 *   The key to use for APC. Will be prefixed with 'chatblock_'.
 *   Must consist of only [a-zA-Z0-9-_] characters, otherwise this
 *   will lead to errors (as it is not being cahecked for performance's sake.
 *   May also be an array of the form (key => value) if multiple values
 *   are to be set at once.
 * @param $val
 *   The value to be stored (or empty, if $keydata is an array).
 * @param $use_fallback_file
 *   Whether to write the value to a file if APC is not present.
 * @param $ttl
 *   Optional TTL for APC entry.
 */
function chatblock_cache_set($keydata, $val = '', $use_fallback_file = FALSE, $ttl = 0) {

  if (!is_array($keydata)) {
    $keydata = array($keydata => $val);
  }

  foreach ($keydata as $key => $value) {

    // Add namespace prefix.
    $apc_key = 'chatblock_' . CHATBLOCK_CACHE_ID . "_$key";

    if (CHATBLOCK_IS_APC) {
      // Always delete the existing value. Otherwise some APC versions will fail.
      apc_delete($apc_key);
      // We will always serialize as apc does not properly story
      // multidimensional data types in some versions.
      apc_store($apc_key, serialize($value), $ttl);
    }
    elseif ($use_fallback_file) {
      require_once(dirname(__FILE__) . '/fallback_file.inc');
      chatblock_filecache_set($key, serialize($value));
    }
  }
}

/**
 * Write a value to the cache with TTL (without file fallback).
 *
 * @param $keydata
 *   The key to use for APC. Will be prefixed with 'chatblock_'.
 *   Must consist of only [a-zA-Z0-9-_] characters, otherwise this
 *   will lead to errors (as it is not being checked for performance's sake).
 *   May also be an array of the form (key => value) if multiple values
 *   are to be set at once.
 * @param $val
 *   The value to be stored (or empty, if $keydata is an array).
 * @param $ttl
 *   Optional TTL for APC entry.
 *
 * @see chatblock_cache_set()
 */
function chatblock_cache_set_ttl($keydata, $val, $ttl) {
  chatblock_cache_set($keydata, $val, FALSE, $ttl);
}
