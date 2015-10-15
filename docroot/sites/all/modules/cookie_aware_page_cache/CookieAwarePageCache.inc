<?php
/**
 * Extends the core database cache to factor in the specified cookie values.
 */
class CookieAwarePageCache extends DrupalDatabaseCache implements DrupalCacheInterface {
  /**
   * Alter the CID to include the cookie values we need.
   */
  function prepare_cid($cid) {
    // Make sure we only alter the CID for the page cache.
    if ($this->bin == 'cache_page') {
      $cookies = variable_get('cookie_aware_page_cache_cookies', array());
      foreach ($cookies as $cookie_name) {
        if (isset($_COOKIE[$cookie_name])) {
          $cid = $cookie_name . ':' . $_COOKIE[$cookie_name] . '|' . $cid;
        }
      }
    }
    return $cid;
  }

  function get($cid) {
    $cid = $this->prepare_cid($cid);
    return parent::get($cid);
  }

  function set($cid, $data, $expire = CACHE_PERMANENT) {
    $cid = $this->prepare_cid($cid);
    parent::set($cid, $data, $expire);
  }

  function clear($cid = NULL, $wildcard = FALSE) {
    $cid = $this->prepare_cid($cid);
    parent::clear($cid, $wildcard);
  }
}
