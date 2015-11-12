<?php
/**
 * @file
 * Database cache implementation for test purposes.
 */

/**
 * Base class for web test cases.
 */
class ConsistentCacheTestCaseBase extends DrupalWebTestCase {
  /**
   * Database cache backend.
   *
   * @var DrupalCacheInterface
   */
  public $cache_default;

  /**
   * Non-database cache backend.
   *
   * @var DrupalCacheInterface
   */
  public $cache_test;

  /**
   * Non-database cache backend through Cache Consistent.
   *
   * @var DrupalCacheInterface
   */
  public $cache_consistent;

  /**
   * The bin to test.
   *
   * @var string
   */
  public $bin = 'cache_consistent_test';

  /**
   * Setup function.
   */
  protected function setUp() {
    global $conf;

    $module_path = drupal_get_path('module', 'cache_consistent');
    parent::setUp();

    require_once DRUPAL_ROOT . '/' . $module_path . '/cache_consistent.inc';
    require_once DRUPAL_ROOT . '/' . $module_path . '/tests/cache_consistent.test.inc';
    require_once DRUPAL_ROOT . '/' . $module_path . '/tests/cache_consistent.tests.inc';

    db_create_table($this->bin, drupal_get_schema_unprocessed('system', 'cache'));

    $this->variables = new VariableCoreHandler();

    // God damn m*****f*****.
    $GLOBALS['cache_consistent_variable_storage_handler'] = $this->variables;

    $this->cache_default = new DrupalDatabaseCache($this->bin);
    $this->cache_default->testGroup = 'Database cache';

    $this->cache_test = new DrupalDatabaseCacheTest($this->bin);
    $this->cache_test->testGroup = 'Non-database cache';

    $conf['consistent_cache_default_class'] = 'DrupalDatabaseCacheTest';
    $conf['consistent_cache_default_safe'] = FALSE;

    $this->cache_memory = new DrupalMemoryCacheTest($this->bin);
    $this->cache_memory->testGroup = 'Memory cache';

    $this->cache_consistent = new ConsistentCache($this->bin);
    $this->cache_consistent->testGroup = 'Consistent cache';

    $conf['consistent_cache_default_class'] = 'DrupalMemoryCacheTest';
    $conf['consistent_cache_default_safe'] = FALSE;

    $this->cache_consistent_memory = new ConsistentCache($this->bin);
    $this->cache_consistent_memory->testGroup = 'Consistent cache memory';

    $this->max = 1000;
  }

  /**
   * Create a new connection for testing transactional caching.
   */
  protected function changeDatabasePrefix() {
    parent::changeDatabasePrefix();
    $connection_info = Database::getConnectionInfo();
    Database::addConnectionInfo(
      'default',
      'test',
      $connection_info['default']
    );
  }

  /**
   * Tear down.
   */
  protected function tearDown() {
    parent::tearDown();
  }

}

/**
 * Test class for transactionality.
 */
class ConsistentCacheTestTransaction {
  public $cache_object;
  public $tx;

  /**
   * Setup transaction object.
   */
  public function __construct($cache_object) {
    $this->cache_object = $cache_object;
    $this->tx = db_transaction();
  }

  /**
   * Rollback.
   */
  public function rollback() {
    if ($this->tx) {
      $this->tx->rollback();
      $this->tx = NULL;
      if ($this->cache_object instanceOf DrupalTransactionalCacheInterface) {
        $this->cache_object->rollback(Database::getConnection()->transactionDepth());
      }
    }
  }

  /**
   * Commit.
   */
  public function __destruct() {
    if ($this->tx) {
      $this->tx = NULL;
      if ($this->cache_object instanceOf DrupalTransactionalCacheInterface) {
        $this->cache_object->commit(Database::getConnection()->transactionDepth());
      }
    }
  }

}

/**
 * Defines a default cache implementation.
 *
 * This is Drupal's default cache implementation. It uses the database to store
 * cached data. Each cache bin corresponds to a database table by the same name.
 */
class DrupalDatabaseCacheTest implements DrupalCacheInterface {
  protected $bin;

  protected $db_options = array('target' => 'test');

  /**
   * Constructs a DrupalDatabaseCache object.
   *
   * @param string $bin
   *   The cache bin for which the object is created.
   */
  public function __construct($bin, $target = NULL) {
    $this->bin = $bin;
    if ($target) {
      $this->db_options['target'] = $target;
    }
  }

  /**
   * Implements DrupalCacheInterface::get().
   */
  public function get($cid) {
    $cids = array($cid);
    $cache = $this->getMultiple($cids);
    return reset($cache);
  }

  /**
   * Implements DrupalCacheInterface::getMultiple().
   */
  public function getMultiple(&$cids) {
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
      $result = db_query('SELECT cid, data, created, expire, serialized FROM {' . db_escape_table($this->bin) . '} WHERE cid IN (:cids)', array(':cids' => $cids), $this->db_options);
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
   * Garbage collection for get() and getMultiple().
   */
  protected function garbageCollection() {
    $cache_lifetime = variable_get('cache_lifetime', 0);

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
      db_delete($this->bin, $this->db_options)
        ->condition('expire', CACHE_PERMANENT, '<>')
        ->condition('expire', $cache_flush, '<=')
        ->execute();
    }
  }

  /**
   * Prepares a cached item.
   *
   * Checks that items are either permanent or did not expire, and unserializes
   * data as appropriate.
   *
   * @param object $cache
   *   An item loaded from cache_get() or cache_get_multiple().
   *
   * @return mixed
   *   The item with data unserialized as appropriate or FALSE if there is no
   *   valid item to load.
   */
  protected function prepareItem($cache) {
    global $user;

    if (!isset($cache->data)) {
      return FALSE;
    }
    // If the cached data is temporary and subject to a per-user minimum
    // lifetime, compare the cache entry timestamp with the user session
    // cache_expiration timestamp. If the cache entry is too old, ignore it.
    if ($cache->expire != CACHE_PERMANENT && variable_get('cache_lifetime', 0) && isset($_SESSION['cache_expiration'][$this->bin]) && $_SESSION['cache_expiration'][$this->bin] > $cache->created) {
      // Ignore cache data that is too old and thus not valid for this user.
      return FALSE;
    }

    // If the data is permanent or not subject to a minimum cache lifetime,
    // unserialize and return the cached data.
    if ($cache->serialized) {
      $cache->data = unserialize($cache->data);
    }

    return $cache;
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    $fields = array(
      'serialized' => 0,
      'created' => REQUEST_TIME,
      'expire' => $expire,
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
      db_merge($this->bin, $this->db_options)
        ->key(array('cid' => $cid))
        ->fields($fields)
        ->execute();
    }
    catch (Exception $e) {
      // The database may not be available, so we'll ignore cache_set requests.
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
          db_delete($this->bin, $this->db_options)
            ->condition('expire', CACHE_PERMANENT, '<>')
            ->condition('expire', REQUEST_TIME, '<')
            ->execute();
          variable_set('cache_flush_' . $this->bin, 0);
        }
      }
      else {
        // No minimum cache lifetime, flush all temporary cache entries now.
        db_delete($this->bin, $this->db_options)
          ->condition('expire', CACHE_PERMANENT, '<>')
          ->condition('expire', REQUEST_TIME, '<')
          ->execute();
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
            db_truncate($this->bin, $this->db_options)->execute();
          }
          else {
            throw new Exception(t('Invalid or missing cache bin specified: %bin', array('%bin' => $this->bin)));
          }
        }
        else {
          db_delete($this->bin, $this->db_options)
            ->condition('cid', db_like($cid) . '%', 'LIKE')
            ->execute();
        }
      }
      elseif (is_array($cid)) {
        // Delete in chunks when a large array is passed.
        do {
          db_delete($this->bin, $this->db_options)
            ->condition('cid', array_splice($cid, 0, 1000), 'IN')
            ->execute();
        } while (count($cid));
      }
      else {
        db_delete($this->bin, $this->db_options)
          ->condition('cid', $cid)
          ->execute();
      }
    }
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  public function isEmpty() {
    $this->garbageCollection();
    $query = db_select($this->bin, $this->db_options);
    $query->addExpression('1');
    $result = $query->range(0, 1)
      ->execute()
      ->fetchField();
    return empty($result);
  }

  /**
   * Checks if $this->bin represents a valid cache table.
   *
   * This check is required to ensure that non-cache tables are not truncated
   * accidentally when calling cache_clear_all().
   *
   * @return bool
   *   TRUE if the bin is valid.
   */
  public function isValidBin() {
    if ($this->bin == 'cache' || substr($this->bin, 0, 6) == 'cache_') {
      // Skip schema check for bins with standard table names.
      return TRUE;
    }
    // These fields are required for any cache table.
    $fields = array('cid', 'data', 'expire', 'created', 'serialized');
    // Load the table schema.
    $schema = drupal_get_schema($this->bin);
    // Confirm that all fields are present.
    return isset($schema['fields']) && !array_diff($fields, array_keys($schema['fields']));
  }

}

/**
 * Variable handler for tests.
 */
class VariableTestHandler extends ArrayObject implements CacheVariableHandlerInterface {
  /**
   * Set variable.
   */
  public function setVariable($name, $value) {
    $this[$name] = $value;
  }

  /**
   * Get variable.
   */
  public function getVariable($name, $default = NULL) {
    return isset($this[$name]) ? $this[$name] : $default;
  }

}

/**
 * Variable handler for tests.
 */
class VariableCoreHandler extends ArrayObject implements CacheVariableHandlerInterface {
  /**
   * Set variable.
   */
  public function setVariable($name, $value) {
    variable_set($name, $value);
  }

  /**
   * Get variable.
   */
  public function getVariable($name, $default = NULL) {
    return variable_get($name, $default);
  }

}

/**
 * Transaction handler for tests.
 */
class TransactionTestHandler implements CacheTransactionHandlerInterface {
  public $depth = 0;
  public $cache;

  /**
   * Set current cache object.
   *
   * @param DrupalTransactionalCacheInterface $cache
   *   The cache object to use.
   */
  public function setCacheObject(DrupalTransactionalCacheInterface $cache) {
    $this->cache = $cache;
  }

  /**
   * Get transaction depth.
   */
  public function getTransactionDepth() {
    return $this->depth;
  }

  /**
   * Start a transaction.
   */
  public function startTransaction() {
    $this->depth++;
    return $this->depth;
  }

  /**
   * End transaction.
   */
  public function endTransaction($depth = NULL) {
    $depth = isset($depth) ? $depth - 1 : $this->depth - 1;
    $depth = max($depth, 0);
    $this->depth = min($this->depth, $depth);
  }

  /**
   * Commit transaction.
   */
  public function commit($depth = NULL) {
    $this->endTransaction($depth);
    $this->cache->commit($this->depth);
  }

  /**
   * Rollback transaction.
   */
  public function rollback($depth = NULL) {
    $this->endTransaction($depth);
    $this->cache->rollback($this->depth);
  }

}

/**
 * Transaction scope handler for tests.
 */
class TransactionTestHandlerScope {
  public $depth;

  /**
   * Constructor.
   */
  public function __construct($handler) {
    $this->handler = $handler;
    $this->depth = $handler->startTransaction();
  }

  /**
   * Rollback transaction.
   */
  public function rollback() {
    $this->handler->rollback($this->depth);
  }

  /**
   * Commit transaction.
   */
  public function commit() {
    $this->handler->commit($this->depth);
  }

  /**
   * Commit transaction on scope end.
   */
  public function __destruct() {
    $this->commit($this->depth);
  }

}

/**
 * Defines a default cache implementation.
 *
 * This is Drupal's default cache implementation. It uses the database to store
 * cached data. Each cache bin corresponds to a database table by the same name.
 */
class DrupalMemoryCacheTest implements DrupalCacheInterface {
  protected $bin;

  protected $storage = array();
  static protected $storages = array();

  /**
   * Get variable.
   */
  public function getVariable($name, $default = NULL) {
    return $this->variables->getVariable($name, $default);
  }

  /**
   * Set variable.
   */
  public function setVariable($name, $value) {
    return $this->variables->setVariable($name, $value);
  }

  /**
   * Constructs a DrupalDatabaseCache object.
   *
   * @param string $bin
   *   The cache bin for which the object is created.
   */
  public function __construct($bin) {
    $this->bin = $bin;
    self::$storages[$bin] = array();
    $this->storage = &self::$storages[$bin];

    // God damn m*****f*****.
    $this->variables = $GLOBALS['cache_consistent_variable_storage_handler'];
  }

  /**
   * Implements DrupalCacheInterface::get().
   */
  public function get($cid) {
    return isset($this->storage[$cid]) ? $this->storage[$cid] : NULL;
  }

  /**
   * Implements DrupalCacheInterface::getMultiple().
   */
  public function getMultiple(&$cids) {
    $keys = array_combine($cids, $cids);
    $result = array_intersect_key($this->storage, $keys);
    $cids = array_keys(array_diff_key($keys, $result));
    return $result;
  }
  /**
   * Implements DrupalCacheInterface::set().
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    $this->storage[$cid] = (object) array(
      'cid' => $cid,
      'serialized' => 0,
      'created' => REQUEST_TIME,
      'expire' => $expire,
      'data' => $this->deepClone($data),
    );
  }

  /**
   * Implements DrupalCacheInterface::clear().
   */
  public function clear($cid = NULL, $wildcard = FALSE) {
    global $user;

    if (empty($cid)) {
      if ($this->getVariable('cache_lifetime', 0)) {
        $cache_flush = $this->getVariable('cache_flush_' . $this->bin, 0);
        if ($cache_flush == 0) {
          // This is the first request to clear the cache, start a timer.
          $this->setVariable('cache_flush_' . $this->bin, REQUEST_TIME);
        }
        elseif (REQUEST_TIME > ($cache_flush + $this->getVariable('cache_lifetime', 0))) {
          foreach ($this->storage as $key => $cache) {
            if ($cache->expire != CACHE_PERMANENT && $cache->expire < REQUEST_TIME) {
              unset($this->storage[$key]);
            }
          }
          $this->setVariable('cache_flush_' . $this->bin, 0);
        }
      }
      else {
        foreach ($this->storage as $key => $cache) {
          if ($cache->expire != CACHE_PERMANENT && $cache->expire < REQUEST_TIME) {
            unset($this->storage[$key]);
          }
        }
      }
    }
    else {
      if ($wildcard) {
        if ($cid == '*') {
          $this->storage = array();
        }
        else {
          $prefix = (string) $cid;
          foreach ($this->storage as $key => $cache) {
            $key = (string) $key;
            if ($key == $prefix || strpos($key, $prefix) === 0) {
              unset($this->storage[$key]);
            }
          }
        }
      }
      elseif (is_array($cid)) {
        $keys = array_combine($cid, $cid);
        $this->storage = array_diff_key($this->storage, $keys);
      }
      else {
        unset($this->storage[$cid]);
      }
    }
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  public function isEmpty() {
    return empty($this->storage);
  }

  /**
   * Deep clone data structure.
   *
   * @param mixed $data
   *   The data structure to clone.
   *
   * @return mixed
   *   Cloned data structure.
   */
  protected function deepClone($data) {
    if (is_array($data)) {
      $result = array();
      foreach ($data as $key => $value) {
        $result[$key] = $this->deepClone($value);
      }
      return $result;
    }
    elseif (is_object($data)) {
      return clone $data;
    }
    else {
      return $data;
    }
  }

  /**
   * Get the internal storage.
   */
  public function getStorage() {
    return $this->storage;
  }

}
