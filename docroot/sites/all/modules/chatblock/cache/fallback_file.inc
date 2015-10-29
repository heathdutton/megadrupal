<?php

/**
 * @file
 * Chatblock fallback file cache functions.
 *
 * Will be used conditionally for some basic routines if APC is not present.
 */

/**
 * Remember directory path.
 */
define('CHATBLOCK_FILE_CACHE_PATH', dirname(__FILE__) . '/tmp');

/**
 * Delete a cache file.
 *
 * @param $keydata
 *   The key to use for APC. Will be prefixed with 'chatblock_'.
 *   Must consist of only [a-zA-Z0-9-_] characters, otherwise this
 *   will lead to errors (as it is not being cahecked for performance's sake.
 *   May also be an array of the form (key => value) if multiple values
 *   are to be set at once.
 */
function chatblock_filecache_delete($keydata) {
  if (!is_array($keydata)) {
    $keydata = array($keydata);
  }
  foreach ($keydata as $key) {
    if (file_exists(CHATBLOCK_FILE_CACHE_PATH . '/' . CHATBLOCK_CACHE_ID . ".$key.cache")) {
      unlink(CHATBLOCK_FILE_CACHE_PATH . '/' . CHATBLOCK_CACHE_ID . ".$key.cache");
    }
  }
}

/**
 * Reads from a primitive cache file.
 *
 * @param $key
 *   Cache file name. Will be prefixed with 'chatblock_'
 *   and suffixed with '.cache'.
 *   Must consist of only [a-zA-Z0-9-_] characters, otherwise this
 *   will lead to errors (as it is not being checked for performance's sake.
 *
 * @return string
 *   The serialized cache value.
 */
function chatblock_filecache_get($key) {

  return @file_get_contents(CHATBLOCK_FILE_CACHE_PATH . '/' . CHATBLOCK_CACHE_ID . ".$key.cache");
}

/**
 * Sets a primitive cache file.
 *
 * @param $keydata
 *   The key to use for APC. Will be prefixed with 'chatblock_'.
 *   Must consist of only [a-zA-Z0-9-_] characters, otherwise this
 *   will lead to errors (as it is not being cahecked for performance's sake.
 *   May also be an array of the form (key => value) if multiple values
 *   are to be set at once.
 * @param $val
 *   The value to be stored (or empty, if $keydata is an array).
 */
function chatblock_filecache_set($keydata, $val = '') {

  if (!is_array($keydata)) {
    $keydata = array($keydata => $val);
  }

  foreach ($keydata as $key => $value) {
    // @todo Use file_put_contents in D7 port as PHP5 is a requirement there.
    $h = @fopen(CHATBLOCK_FILE_CACHE_PATH . '/' . CHATBLOCK_CACHE_ID . ".$key.cache", 'w');
    if ($h) {
      flock($h, LOCK_EX);
      fwrite($h, $value);
      flock($h, LOCK_UN);
      fclose($h);
    }
  }
}
