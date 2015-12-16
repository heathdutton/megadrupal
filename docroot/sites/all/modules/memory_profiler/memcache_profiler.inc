<?php

class ProfileMemCacheDrupal extends MemCacheDrupal {
  function set($cid, $data, $expire = CACHE_PERMANENT, array $headers = NULL) {

    // Provide multiple versions of the set function for backwards compatibility with
    //  older versions of memcache.
    if (method_exists($this, 'wildcard_flushes')) {

      // set function based on memcache-7.x-1.0-beta2
      $created = time();

      // Create new cache object.
      $cache = new stdClass;
      $cache->cid = $cid;
      $cache->data = is_object($data) ? clone $data : $data;
      $cache->created = $created;
      $cache->headers = $headers;
      // Record the previous number of wildcard flushes affecting our cid.

      // N.B. wildcard_flushes may be a private method in some versions.
      if (is_callable('parent::wildcard_flushes')) {
        $wildcard_flushes = $this->wildcard_flushes($cid);
        $call_parent = FALSE;
      }
      else {
        // This cache object can't be used to do the actual set, so
        //  provide an arbitrary number to estimate the size of the data
        //  that will be sent to memcache when the parent method is called.
        $wildcard_flushes = 12345;
        // We'll have to actually call the parent set method.
        $call_parent = TRUE;
      }

      $cache->flushes = $wildcard_flushes;
      if ($expire == CACHE_TEMPORARY) {
        // Convert CACHE_TEMPORARY (-1) into something that will live in memcache
        // until the next flush.
        $cache->expire = REQUEST_TIME + 2591999;
      }
      // Expire time is in seconds if less than 30 days, otherwise is a timestamp.
      else if ($expire != CACHE_PERMANENT && $expire < 2592000) {
        // Expire is expressed in seconds, convert to the proper future timestamp
        // as expected in dmemcache_get().
        $cache->expire = REQUEST_TIME + $expire;
      }
      else {
        $cache->expire = $expire;
      }

      if (variable_get('profile_memcache_cache_set', TRUE)) {
        $this->profileCacheSet($cache);
      }

      if ($call_parent) {
        parent::set($cid, $data, $expire, $headers);
      }
      else {
        // We manually track the expire time in $cache->expire.  When the object
        // expires, we only allow one request to rebuild it to avoid cache
        // stampedes. Other requests for the expired object while it is still being
        // rebuilt get the expired object.
        dmemcache_set($cid, $cache, 0, $this->bin, $this->memcache);
      }
    }
    else {

      // set function from memcache-7.x-1.2
      $created = round(microtime(TRUE), 3);

      // Create new cache object.
      $cache = new stdClass();
      $cache->cid = $cid;
      $cache->data = is_object($data) ? clone $data : $data;
      $cache->created = $created;
      // Record the previous number of wildcard flushes affecting our cid.
      $cache->flushes = $this->wildcardFlushes($cid);
      if ($expire == CACHE_TEMPORARY) {
        // Convert CACHE_TEMPORARY (-1) into something that will live in memcache
        // until the next flush.
        $cache->expire = REQUEST_TIME + 2591999;
        // This is a temporary cache item.
        $cache->temporary = TRUE;
      }
      // Expire time is in seconds if less than 30 days, otherwise is a timestamp.
      elseif ($expire != CACHE_PERMANENT && $expire < 2592000) {
        // Expire is expressed in seconds, convert to the proper future timestamp
        // as expected in dmemcache_get().
        $cache->expire = REQUEST_TIME + $expire;
        $cache->temporary = FALSE;
      }
      else {
        $cache->expire = $expire;
        $cache->temporary = FALSE;
      }

      if (variable_get('profile_memcache_cache_set', TRUE)) {
        $this->profileCacheSet($cache);
      }

      // Manually track the expire time in $cache->expire.  When the object
      // expires, if stampede protection is enabled, it may be served while one
      // process rebuilds it. The ttl sent to memcache is set to the expire twice
      // as long into the future, this allows old items to be expired by memcache
      // rather than evicted along with a sufficient period for stampede
      // protection to continue to work.
      if ($cache->expire == CACHE_PERMANENT) {
        $memcache_expire = $cache->expire;
      }
      else {
        $memcache_expire = $cache->expire + (($cache->expire - REQUEST_TIME) * 2);
      }
      dmemcache_set($cid, $cache, $memcache_expire, $this->bin, $this->memcache);

      // Release lock if acquired in $this->valid().
      $lock = "memcache_$cid:$this->bin";
      if (variable_get('memcache_stampede_protection', FALSE) && isset($GLOBALS['locks'][$lock])) {
        lock_release("$lock");
      }
    }
  }

  function profileCacheSet($cache) {
    $cache_as_string = serialize($cache);
    $len_uncompressed = strlen($cache_as_string);
    if ($len_uncompressed > variable_get('profile_memcache_threshold_uncompressed', 1000000)) {
      $len_compressed = strlen(gzcompress($cache_as_string, -1));
      if ($len_compressed > variable_get('profile_memcache_threshold_compressed', 1000000)) {
        $log = array(
          '@bin' => $this->bin,
          '@key' => $cache->cid,
          '@len' => $len_uncompressed,
          '@comp' => $len_compressed,
          );
        watchdog('profile_memcache', 'bin: @bin | key: @key | len: @len | comp: @comp', $log, WATCHDOG_DEBUG);
      }
    }
  }

}

