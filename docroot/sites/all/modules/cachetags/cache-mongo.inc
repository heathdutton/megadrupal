<?php

require_once dirname(__FILE__) . '/DrupalCacheTags.inc';
require_once DRUPAL_ROOT . '/sites/all/modules/mongodb/mongodb.module';
require_once DRUPAL_ROOT . '/sites/all/modules/mongodb/mongodb_cache/mongodb_cache.inc';

/**
 * Overrides to the Mongo cache, to add cache tag support.
 */
class DrupalMongoCacheTagsPlugin extends DrupalMongoDBCache {
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

    if (!$cache || !isset($cache['data'])) {
      return FALSE;
    }
    unset($cache['_id']);
    $cache = (object)$cache;
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
    if ($cache->tags && !cache_tags()->isValid($cache->checksum, $cache->tags)) {
      return FALSE;
    }

    if ($cache->data instanceof MongoBinData) {
      $cache->data = $cache->data->bin;
    }

    if ($cache->serialized) {
      $cache->data = unserialize($cache->data);
    }

    return $cache;
  }

  function set($cid, $data, $expire = CACHE_PERMANENT, $tags = array()) {
    $scalar = is_scalar($data);
    $entry = array(
      '_id' => (string)$cid,
      'cid' => (string)$cid,
      'created' => REQUEST_TIME,
      'expire' => $expire,
      'serialized' => !$scalar,
      'data' => $scalar ? $data : serialize($data),
      'tags' => $tags,
      'checksum' => cache_tags()->checksum($tags),
    );

    try {
      // Use MongoBinData for non-UTF8 strings.
      if (is_string($entry['data']) && !drupal_validate_utf8($entry['data'])) {
        $entry['data'] = new MongoBinData($entry['data']);
      }
      $collection = mongodb_collection($this->bin);
      $collection->save($entry);
    }
    catch (Exception $e) {
      // The database may not be available, so we'll ignore cache_set requests.
    }
  }
}

class DrupalMongoCacheTags extends DrupalCacheTags {
  protected $tag_cache = array();

  function invalidate($tags) {
    try {
      foreach (DrupalCacheTags::flatten($tags) as $tag) {
        unset($this->tag_cache[$tag]);
        mongodb_collection('cache_tags')->update(array('_id' => $tag), array('$inc' => array('invalidations' => 1)), array('upsert' => TRUE));
      }
    }
    catch (Exception $e) {

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
      try {
        foreach (mongodb_collection('cache_tags')->find(array('_id' => array('$in' => $query_tags))) as $db_tag) {
          $this->tag_cache[$db_tag['_id']] = $db_tag['invalidations'];
          $checksum += $db_tag['invalidations'];
        }
      }
      catch (Exception $e) {

      }
    }

    return $checksum;
  }
}
