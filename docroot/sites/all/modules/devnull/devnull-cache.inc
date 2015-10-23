<?php
/**
 * @file
 * Provides a dev/null cache implementation to be used when you want cache sets to be really fast.
 */

class DevNuLLCache implements DrupalCacheInterface {
  function get($cid) {
    return FALSE;
  }

  function getMultiple(&$cids) {
    return array();
  }

  function set($cid, $data, $expire = CACHE_PERMANENT) {
  }

  function clear($cid = NULL, $wildcard = FALSE) {
  }

  function isEmpty() {
    return TRUE;
  }
}
