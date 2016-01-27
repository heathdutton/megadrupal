<?php

require_once dirname(__FILE__) . '/DrupalCacheTags.inc';

/**
 * Overrides to the core database cache, to add cache tag support.
 */
class DrupalDatabaseCacheTagsPlugin extends DrupalDatabaseCache {
  function getMultiple(&$cids) {
    try {
      // Garbage collection necessary when enforcing a minimum cache lifetime.
      $this->garbageCollection($this->bin);

      // When serving cached pages, the overhead of using db_select() was found
      // to add around 30% overhead to the request. Since $this->bin is a
      // variable, this means the call to db_query() here uses a concatenated
      // string. This is highly discouraged under any other circumstances, and
      // is used here only due to the performance overhead we would incur
      // otherwise. When serving an uncached page, the overhead of using
      // db_select() is a much smaller proportion of the request.
      $result = db_query('SELECT cid, data, created, expire, serialized, tags, checksum FROM {' . db_escape_table($this->bin) . '} WHERE cid IN (:cids)', array(':cids' => $cids));
      $cache = array();
      foreach ($result as $item) {
        $item = $this->prepareItem($item);
        if ($item) {
          $cache[$item->cid] = $item;
        }
      }
      $cids = array_diff($cids, array_keys($cache));
      return $cache;
    }
    catch (Exception $e) {
      // If the database is never going to be available, cache requests should
      // return FALSE in order to allow exception handling to occur.
      return array();
    }
  }

  /**
   * Prepare a cached item.
   *
   * Checks that items are either permanent or did not expire, and unserializes
   * data as appropriate.
   *
   * @param $cache
   *   An item loaded from cache_get() or cache_get_multiple().
   * @return
   *   The item with data unserialized as appropriate or FALSE if there is no
   *   valid item to load.
   */
  protected function prepareItem($cache) {
    global $user;

    if (!isset($cache->data)) {
      return FALSE;
    }
    // If enforcing a minimum cache lifetime, validate that the data is
    // currently valid for this user before we return it by making sure the cache
    // entry was created before the timestamp in the current session's cache
    // timer. The cache variable is loaded into the $user object by _drupal_session_read()
    // in session.inc. If the data is permanent or we're not enforcing a minimum
    // cache lifetime always return the cached data.
    if ($cache->expire != CACHE_PERMANENT && variable_get('cache_lifetime', 0) && $user->cache > $cache->created) {
      // This cache data is too old and thus not valid for us, ignore it.
      return FALSE;
    }

    // The cache data is invalid if any of its tags have been cleared since.
    if ($cache->tags) {
      $cache->tags = explode(' ', $cache->tags);
      if (!cache_tags()->isValid($cache->checksum, $cache->tags)) {
        return FALSE;
      }
    }

    if ($cache->serialized) {
      $cache->data = unserialize($cache->data);
    }

    return $cache;
  }

  function set($cid, $data, $expire = CACHE_PERMANENT, $tags = array()) {
    $fields = array(
      'serialized' => 0,
      'created' => REQUEST_TIME,
      'expire' => $expire,
      'tags' => implode(' ', DrupalCacheTags::flatten($tags)),
      'checksum' => cache_tags()->checksum($tags),
    );
    if (!is_string($data)) {
      $fields['data'] = serialize($data);
      $fields['serialized'] = 1;
    }
    else {
      $fields['data'] = $data;
      $fields['serialized'] = 0;
    }

    try {
      db_merge($this->bin)
        ->key(array('cid' => $cid))
        ->fields($fields)
        ->execute();
    }
    catch (Exception $e) {
      // The database may not be available, so we'll ignore cache_set requests.
    }
  }
}

/**
 * Default cache tags implementation, using the database.
 */
class DrupalDatabaseCacheTags extends DrupalCacheTags {
  protected $tag_cache = array();

  function invalidate($tags) {
    foreach (DrupalCacheTags::flatten($tags) as $tag) {
      unset($this->tag_cache[$tag]);
      db_merge('cache_tags')
        ->key(array('tag' => $tag))
        ->fields(array('invalidations' => 1))
        ->expression('invalidations', 'invalidations + 1')
        ->execute();
    }
  }

  function checksum($tags) {
    $checksum = 0;
    $query_tags = array();

    foreach (DrupalCacheTags::flatten($tags) as $tag) {
      if (isset($this->tag_cache[$tag])) {
        $checksum += $this->tag_cache[$tag];
      }
      else {
        $query_tags[] = $tag;
      }
    }
    if ($query_tags) {
      if ($db_tags = db_query('SELECT tag, invalidations FROM {cache_tags} WHERE tag IN (:tags)', array(':tags' => $query_tags))->fetchAllKeyed()) {
        $this->tag_cache = array_merge($this->tag_cache, $db_tags);
        $checksum += array_sum($db_tags);
      }
    }

    return $checksum;
  }
}
