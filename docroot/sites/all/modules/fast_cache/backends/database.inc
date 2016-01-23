<?php

/**
 * @file
 * Contains FastCacheDatabase class.
 */

/**
 * Extends standard DrupalDatabaseCache class for db cache and
 * provides additional setMultiple() method for bulk cache inserts.
 */
class FastCacheDatabase extends DrupalDatabaseCache {

  /**
   * Execute bulk insert/update operation for all cache items.
   *
   * @param $caches
   *   Constains array with multiple cache items.
   *   Key of each item is cache id, and value is cache object.
   */
  function setMultiple($caches) {

    // Initialize variable for unique number for each db query placeholder.
    $max_placeholder = 0;

    // In this variable we will store array with database placeholder names and
    // their data representation.
    $arguments = array();

    // This variable will store array of inserted values for each cache item.
    $values_query = array();
    foreach ($caches as $cache_id => $cache) {

      $item = array(
        'cid' => $cache_id,
        'data' => $cache->data,
        'expire' => $cache->expire,
        'created' => isset($cache->created) ? $cache->created : REQUEST_TIME,
        'serialized' => 0,
      );

      // Serialize cache data, if needed.
      if (!is_string($item['data'])) {
        $item['data'] = serialize($item['data']);
        $item['serialized'] = 1;
      }

      // Prepare placeholders for the db bulk query.
      $replacements = array();
      foreach (array('cid', 'data', 'expire', 'created', 'serialized') as $key) {
        $replacement = ':placeholder_' . $max_placeholder++;
        $replacements[] = $replacement;
        $arguments[$replacement] = $item[$key];
      }

      // Prepare values for the cache item.
      $values_query[] = '(' . implode(', ', $replacements) . ')';
    }

    // Prepare mysql string for the bulk insert/update query. Unfortunately,
    // there is no other way to perform this query in a nicer way which will
    // have the same performance. And merge queries doesn't support bulk
    // operations. So yes, I also hate the general look of the three lines
    // below, but this is the only way to make fast multiple insert in the table.
    $query = 'INSERT INTO {' . db_escape_table($this->bin) . '} (cid, data, expire, created, serialized)';
    $query .= ' VALUES ' . implode(', ', $values_query);
    $query .= ' ON DUPLICATE KEY UPDATE data = VALUES(data), expire = VALUES(expire), created = VALUES(created), serialized = VALUES(serialized)';

    try {
      db_query($query, $arguments);
    }
    catch (Exception $e) {
      // The database may not be available, so we'll ignore cache_set requests.
    }
  }

}
