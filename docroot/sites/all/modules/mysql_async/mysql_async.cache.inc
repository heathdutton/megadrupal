<?php
/**
 * @file
 * Cache class for asynchronous cache queries.
 */

/**
 * Deferred cache implementation using asynchronous database queries.
 */
class DrupalDatabaseCacheAsync extends DrupalDatabaseCacheDeferred {
  protected $asyncQuery;
  protected $results = array();

  /**
   * Constructor.
   */
  public function __construct($bin) {
    parent::__construct($bin);
    $this->asyncOptions = array('async' => TRUE) + $this->options;
    $this->asyncKeyedOptions = array('async_key' => $bin) + $this->asyncOptions;
  }

  /**
   * Garbage collection for get() and getMultiple().
   */
  protected function garbageCollection() {
    $cache_lifetime = variable_get('cache_lifetime', 0);

    // Reaping results is a part of garbage collection.
    $this->reapResults();

    // Clean-up the per-user cache expiration session data, so that the session
    // handler can properly clean-up the session data for anonymous users.
    if (isset($_SESSION['cache_expiration'])) {
      $expire = REQUEST_TIME - $cache_lifetime;
      foreach ($_SESSION['cache_expiration'] as $bin => $timestamp) {
        if ($timestamp < $expire) {
          unset($_SESSION['cache_expiration'][$bin]);
        }
      }
      if (!$_SESSION['cache_expiration']) {
        unset($_SESSION['cache_expiration']);
      }
    }

    // Garbage collection of temporary items is only necessary when enforcing
    // a minimum cache lifetime.
    if (!$cache_lifetime) {
      return;
    }
    // When cache lifetime is in force, avoid running garbage collection too
    // often since this will remove temporary cache items indiscriminately.
    $cache_flush = variable_get('cache_flush_' . $this->bin, 0);
    if ($cache_flush && ($cache_flush + $cache_lifetime <= REQUEST_TIME)) {
      // Reset the variable immediately to prevent a meltdown in heavy load
      // situations.
      variable_set('cache_flush_' . $this->bin, 0);
      // Time to flush old cache data.
      $conn = Database::getConnection($this->options['target']);
      $this->results[] = $conn->query('DELETE FROM {' . $conn->escapeTable($this->bin) . '} WHERE expire <> :permanent AND expire <= :flush', array(
          ':permanent' => CACHE_PERMANENT,
          ':flush' => $cache_flush,
      ), $this->asyncOptions);
    }
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    $fields = array(
      ':cid' => $cid,
      ':serialized' => 0,
      ':serialized_up' => 0,
      ':created' => REQUEST_TIME,
      ':created_up' => REQUEST_TIME,
      ':expire' => $expire,
      ':expire_up' => $expire,
    );
    if (!is_string($data)) {
      $fields[':data_up'] = $fields[':data'] = serialize($data);
      $fields[':serialized_up'] = $fields[':serialized'] = 1;
    }
    else {
      $fields[':data_up'] = $fields[':data'] = $data;
      $fields[':serialized_up'] = $fields[':serialized'] = 0;
    }

    try {
      $conn = Database::getConnection($this->options['target']);
      $this->results[] = $conn->query('INSERT INTO {' . $conn->escapeTable($this->bin) . '} (cid, data, serialized, created, expire) VALUES (:cid, :data, :serialized, :created, :expire) ON DUPLICATE KEY UPDATE data = :data_up, serialized = :serialized_up, created = :created_up, expire = :expire_up', $fields, $this->asyncOptions);
    }
    catch (Exception $e) {
      // The database may not be available, so we'll ignore cache_set requests.
      dpm((string) $e);
    }
  }

  /**
   * Implements DrupalCacheInterface::clear().
   */
  public function clear($cid = NULL, $wildcard = FALSE) {
    global $user;

    if (empty($cid)) {
      if (variable_get('cache_lifetime', 0)) {
        // We store the time in the current user's session. We then simulate
        // that the cache was flushed for this user by not returning cached
        // data that was cached before the timestamp.
        $_SESSION['cache_expiration'][$this->bin] = REQUEST_TIME;

        $cache_flush = variable_get('cache_flush_' . $this->bin, 0);
        if ($cache_flush == 0) {
          // This is the first request to clear the cache, start a timer.
          variable_set('cache_flush_' . $this->bin, REQUEST_TIME);
        }
        elseif (REQUEST_TIME > ($cache_flush + variable_get('cache_lifetime', 0))) {
          // Clear the cache for everyone, cache_lifetime seconds have
          // passed since the first request to clear the cache.
          $conn = Database::getConnection($this->options['target']);
          $this->results[] = $conn->query('DELETE FROM {' . $conn->escapeTable($this->bin) . '} WHERE expire <> :permanent AND expire < :flush', array(
              ':permanent' => CACHE_PERMANENT,
              ':flush' => REQUEST_TIME,
          ), $this->asyncOptions);
          variable_set('cache_flush_' . $this->bin, 0);
        }
      }
      else {
        // No minimum cache lifetime, flush all temporary cache entries now.
        $conn = Database::getConnection($this->options['target']);
        $this->results[] = $conn->query('DELETE FROM {' . $conn->escapeTable($this->bin) . '} WHERE expire <> :permanent AND expire < :flush', array(
            ':permanent' => CACHE_PERMANENT,
            ':flush' => REQUEST_TIME,
        ), $this->asyncOptions);
      }
    }
    else {
      if ($wildcard) {
        if ($cid == '*') {
          // Check if $this->bin is a cache table before truncating. Other
          // cache_clear_all() operations throw a PDO error in this situation,
          // so we don't need to verify them first. This ensures that non-cache
          // tables cannot be truncated accidentally.
          if ($this->isValidBin()) {
            $conn = Database::getConnection($this->options['target']);
            $this->results[] = $conn->query('TRUNCATE {' . $conn->escapeTable($this->bin) . '}', array(), $this->asyncOptions);
          }
          else {
            throw new Exception(t('Invalid or missing cache bin specified: %bin', array('%bin' => $this->bin)));
          }
        }
        else {
          $conn = Database::getConnection($this->options['target']);
          $this->results[] = $conn->query('DELETE FROM {' . $conn->escapeTable($this->bin) . '} WHERE cid LIKE :cid', array(
              ':cid' => db_like($cid) . '%',
          ), $this->asyncOptions);
        }
      }
      elseif (is_array($cid)) {
        // Delete in chunks when a large array is passed.
        do {
          db_delete($this->bin, $this->asyncOptions)
            ->condition('cid', array_splice($cid, 0, 1000), 'IN')
            ->execute();
        } while (count($cid));
      }
      else {
        $conn = Database::getConnection($this->options['target']);
        $this->results[] = $conn->query('DELETE FROM {' . $conn->escapeTable($this->bin) . '} WHERE cid = :cid', array(
            ':cid' => $cid,
        ), $this->asyncOptions);
      }
    }
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  public function isEmpty() {
    $this->garbageCollection();
    $query = db_select($this->bin, NULL, $this->options);
    $query->addExpression('1');
    $result = $query->range(0, 1)
      ->execute()
      ->fetchField();
    return empty($result);
  }

  /**
   * Reap results on destroy.
   */
  public function __destruct() {
    $this->reapResults();
  }

  /**
   * Reap all results waiting for this bin.
   */
  protected function reapResults() {
    foreach ($this->results as $result) {
      if ($result instanceof DatabaseStatementAsync) {
        $result->reapResult();
      }
    }
    $this->results = array();
  }

  /**
   * Implements DrupalCacheDeferredInterface::getMultipleDeferred().
   */
  public function getMultipleDeferred($cids) {
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
      $conn = Database::getConnection($this->options['target']);
      $result = $conn->query('SELECT cid, data, created, expire, serialized FROM {' . $conn->escapeTable($this->bin) . '} WHERE cid IN (:cids)', array(':cids' => $cids), $this->asyncKeyedOptions);
    }
    catch (Exception $e) {
      // If the database is never going to be available, return an empty result.
      $result = NULL;
    }
    return $result;
  }

  /**
   * Implements DrupalCachedDeferredInterface::fetchDeferredItems().
   */
  public function fetchDeferredItems($cids, $result) {
    $this->asyncQuery = $result;
    return $this->fetchDeferredItemsAsync();
  }

  /**
   * Wrapper for keeping Database::log() happy.
   *
   * Database::log() logs the caller's arguments, and PDO object cannot be
   * serialized.
   */
  public function fetchDeferredItemsAsync() {
    try {
      $result = $this->asyncQuery ? $this->asyncQuery->fetchAllAssoc('cid') : array();
    }
    catch (Exception $e) {
      // If the database is never going to be available, return an empty result.
      $result = array();
    }
    unset($this->asyncQuery);
    return $result;
  }

}
