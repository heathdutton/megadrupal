<?php
/**
 * @file
 * Cache class wrapper for non-transactional backend.
 *
 * The class works in two modes: safe or non-safe.
 *
 * Both modes buffers cache operations during transactions, in order to keep
 * cache in sync with database contents.
 *
 * In safe-mode however, cache_set() is silenty ignored/discarded during
 * transactions.
 *
 * Safe mode is needed for fully eliminating the race conditions that cause
 * the cache to come out of sync with the DB, if the REPEATABLE-READ isolation
 * level is used.
 *
 * If using the READ-COMMITTED isolation level, safe mode should not be enabled.
 *
 * @code
 *  $conf['consistent_cache_default_safe'] =  FALSE;
 * @endcode
 */

require_once dirname(__FILE__) . '/cache_consistent.buffer.inc';

/**
 * Interface for cache backends that acts as a bufferservice.
 */
interface CacheBufferServiceInterface {
  /**
   * Get current bin of the buffer service.
   *
   * @return string
   *   Name of the cache bin.
   */
  public function getBin();

  /**
   * Get the current cache backend of the buffer service.
   *
   * @return DrupalTransactionalCacheInterface
   *   The cache backend.
   */
  public function getBackend();

  /**
   * Get the current buffer mechanism instance of the buffer service.
   *
   * @return ConsistentCacheBufferInterface
   *   The buffer mechanism.
   */
  public function getBuffer();

}

/**
 * Interface for variable storage handler.
 */
interface CacheVariableHandlerInterface {
  /**
   * Get variable from variable store.
   *
   * @param string $name
   *   The name of variable.
   * @param mixed $default
   *   Default value if variable not set.
   *
   * @return mixed
   *   Value of variable.
   */
  public function getVariable($name, $default = NULL);

}

/**
 * Interface for transaction handler.
 */
interface CacheTransactionHandlerInterface {
  /**
   * Return current transaction depth.
   *
   * @return int
   *   Depth of transaction, 0 = no transaction.
   */
  public function getTransactionDepth();

}

if (interface_exists('DrupalTransactionalCacheInterface')) {
  /**
   * Base class implementing DrupalTransactionalCacheInterface.
   */
  abstract class ConsistentCacheBaseFailSafe implements DrupalCacheInterface, DrupalTransactionalCacheInterface, CacheBufferServiceInterface, CacheVariableHandlerInterface, CacheTransactionHandlerInterface {

    protected $enabled = TRUE;

  }
}
else {
  /**
   * Base class not implementing DrupalTransactionalCacheInterface.
   */
  abstract class ConsistentCacheBaseFailSafe implements DrupalCacheInterface, CacheBufferServiceInterface, CacheVariableHandlerInterface, CacheTransactionHandlerInterface {

    protected $enabled = FALSE;

  }
}

/**
 * Defines a transactional aware cache implementation.
 *
 * Buffer/discard cache operations during transactions.
 */
class ConsistentCacheBase extends ConsistentCacheBaseFailSafe {
  /**
   * The cache bin for this instance.
   *
   * @var string
   */
  protected $bin;

  /**
   * The real cache backend to use for cache operations.
   *
   * @var DrupalCacheInterface
   */
  protected $backend = NULL;

  /**
   * The cache operations buffer.
   *
   * @var CacheBufferServiceInterface
   */
  protected $buffer = NULL;

  /**
   * Safe mode. If TRUE, cache set operations are skipped during transactions.
   *
   * @var boolean
   */
  protected $safe = TRUE;

  /**
   * Strict mode. If TRUE, cache expiration is checked during fetch.
   *
   * @var boolean
   */
  protected $strict = FALSE;

  /**
   * Backend singletons per cache bin.
   *
   * @var array
   */
  static protected $backends = array();

  /**
   * Buffer singletons per cache bin.
   *
   * @var array
   */
  static protected $buffers = array();

  public $variableHandler;
  public $transactionHandler;

  /**
   * The default cache backend to use.
   *
   * @var string
   */
  protected $defaultClass = 'DrupalDatabaseCache';

  /**
   * The default buffer mechanism to use.
   *
   * @var string
   */
  protected $defaultBufferClass = 'ConsistentCacheBuffer';

  /**
   * Helper function for getting bin specific variables with fallback.
   *
   * @param string $prefix
   *   The variable prefix, e.g. 'consistent_cache'.
   * @param string $bin
   *   The name of the bin, e.g. 'cache'.
   * @param string $name
   *   The name of the variable, e.g. 'class'.
   * @param string $default
   *   The default value if none found, e.g. 'DrupalDatabaseCache'.
   *
   * @return mixed
   *   The value of the variable.
   */
  public function getVariableFallback($prefix, $bin, $name, $default = NULL) {
    $data = $this->variableHandler->getVariable($prefix . '_' . $name . '_' . $bin);
    if ($bin && !isset($data)) {
      $data = $this->variableHandler->getVariable($prefix . '_default_' . $name, $default);
    }
    return isset($data) ? $data : $default;
  }

  /**
   * Implements CacheVariableHanderInterface::getVariable().
   */
  public function getVariable($name, $default = NULL) {
    return variable_get($name, $default);
  }

  /**
   * Constructor.
   *
   * Initialize bin.
   */
  public function __construct($bin, $options = array()) {
    $this->bin = $bin;

    $options += array(
      'variable_handler' => $this,
      'transaction_handler' => $this,
    );

    $this->variableHandler = $options['variable_handler'];
    $this->transactionHandler = $options['transaction_handler'];

    // Let our parent do its work.
    $class = $this->getVariableFallback('consistent_cache', $this->bin, 'class', $this->defaultClass);

    // Get buffer mechanism.
    $buffer_class = $this->getVariableFallback('consistent_cache', $this->bin, 'buffer_mechanism', $this->defaultBufferClass);

    // Check for safe mode.
    $this->safe = $this->getVariableFallback('consistent_cache', $this->bin, 'safe', TRUE);

    // Check for strict mode.
    $this->strict = $this->getVariableFallback('consistent_cache', $this->bin, 'strict', FALSE);

    $this->backend = new $class($bin);
    $this->buffer = new $buffer_class($this);
  }

  /**
   * Implements CacheBufferServiceInterface::getBin().
   */
  public function getBin() {
    return $this->bin;
  }

  /**
   * Implements CacheBufferServiceInterface::getBackend().
   */
  public function getBackend() {
    return $this->backend;
  }

  /**
   * Implements CacheBufferServiceInterface::getBuffer().
   */
  public function getBuffer() {
    return $this->buffer;
  }

  /**
   * Implements CacheTransactionHandlerInterface::getTransactionDepth().
   */
  public function getTransactionDepth() {
    return $this->enabled && drupal_bootstrap(NULL, FALSE) >= DRUPAL_BOOTSTRAP_DATABASE ? Database::getConnection()->transactionDepth() : 0;
  }

  /**
   * Destructor.
   *
   * Flush remaining buffers, if any.
   */
  public function __destruct() {
    $this->commit(0);
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
   *
   * Get multiple cache entries taking buffer into account.
   */
  public function getMultiple(&$cids) {
    $return = array();

    $fetch_cids = $cids ? array_combine($cids, $cids) : array();

    // If buffer is not empty, then check for changes in $cids requested.
    if (!$this->buffer->isEmpty()) {
      foreach ($cids as $cid) {
        if ($operation = $this->buffer->lookup($cid)) {
          switch ($operation[ConsistentCacheBufferInterface::CC_OPERATION]) {
            case 'set':
              $return[$cid] = (object) array(
                'cid' => $cid,
                'data' => $operation[ConsistentCacheBufferInterface::CC_DATA_WILDCARD],
                'created' => time(),
                'headers' => NULL,
                'flushes' => 0,
                'expire' => $operation[ConsistentCacheBufferInterface::CC_EXPIRE],
              );
              unset($fetch_cids[$cid]);
              break;

            case 'clear':
              unset($fetch_cids[$cid]);
              break;

          }
        }
      }
    }

    // Re-assign cache IDs found in buffer to $cids, to inform the consumer.
    $cids = $return ? array_combine(array_keys($return), array_keys($return)) : array();
    if ($fetch_cids) {
      $fetch_cids = array_values($fetch_cids);
      // Fetch relevant cache entries from the backend, and add to $cids, while
      // maintaining unique cache IDs.
      $return += $this->backend->getMultiple($fetch_cids);
      $cids += $fetch_cids ? array_combine($fetch_cids, $fetch_cids) : array();
      // Some cache entries may have been flushed. We don't know until we check
      // their timestamp.
      if ($removed = $this->buffer->removeFlushed($return)) {
        $cids = array_values(array_diff($cids, $removed));
      }
      else {
        $cids = array_values($cids);
      }
    }

    // Weed out expired cache entries.
    if ($this->strict) {
      foreach ($return as $cid => $cache) {
        if ($cache->expire > 0 && $cache->expire < time()) {
          unset($return[$cid]);
          $cids[] = $cid;
        }
      }
    }

    return $return;
  }

  /**
   * Implements DrupalCacheInterface::set().
   *
   * Buffer cache operation if inside transaction.
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    $depth = $this->transactionHandler->getTransactionDepth();
    if ($depth == 0) {
      // Don't buffer untransactionalized cache operations.
      return $this->backend->set($cid, $data, $expire);
    }
    elseif (!$this->safe) {
      // Only buffer during transactions when safe mode is off.
      // Otherwise, discard cache set.
      $this->buffer->set($cid, $this->deepClone($data), $expire, $depth);
    }
  }

  /**
   * Implements DrupalCacheInterface::clear().
   *
   * Buffer cache operation if inside transaction.
   */
  public function clear($cid = NULL, $wildcard = FALSE) {
    // Explicit memcache support :-(
    $wildcard = $wildcard ? TRUE : FALSE;
    if ($this->backend instanceOf MemCacheDrupal) {
      $backtrace = debug_backtrace();
      if ($cid == MEMCACHE_CONTENT_CLEAR || (isset($backtrace[2]) && $backtrace[2]['function'] == 'cache_clear_all' && empty($backtrace[2]['args']))) {
        $cid = MEMCACHE_CONTENT_CLEAR;
        $wildcard = NULL;
      }
    }

    $depth = $this->transactionHandler->getTransactionDepth();
    if ($depth == 0) {
      // Don't buffer untransactionalized cache operations.
      return $this->backend->clear($cid, $wildcard);
    }
    else {
      if ($wildcard) {
        if (!isset($cid)) {
          $this->buffer->flushAll($depth);
        }
        elseif ($cid == '*') {
          $this->buffer->deleteAll($depth);
        }
        else {
          $this->buffer->deletePrefix($cid, $depth);
        }
      }
      elseif (is_array($cid)) {
        $this->buffer->deleteMultiple($cid, $depth);
      }
      else {
        $this->buffer->delete($cid, $depth);
      }
    }
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  public function isEmpty() {
    // If anything is present in the buffer, then we cannot determine if the
    // bin is empty.
    if (!$this->buffer->isEmpty()) {
      return FALSE;
    }

    // If inside a transaction in safe mode, then nothing is certain.
    if ($this->safe) {
      if ($this->transactionHandler->getTransactionDepth()) {
        return FALSE;
      }
    }

    // Nothing in buffer nor safe+transaction; ask the backend.
    return $this->backend->isEmpty();
  }

  /**
   * Implements DrupalTransactionalCacheInterface::rollback().
   *
   * Clear cache operation buffer.
   *
   * Clear all affected cids from cache.
   * This is called only during rollback scenarios.
   */
  public function rollback($depth) {
    $this->buffer->rollback($depth);
  }

  /**
   * Implements DrupalTransactionalCacheInterface::commit().
   *
   * Flush cache operation buffer.
   */
  public function commit($depth) {
    $this->buffer->commit($depth);
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

}

if (interface_exists('DrupalCacheDeferredInterface', FALSE)) {
  /**
   * Cache Consistent class with Cache Heuristic support.
   */
  class ConsistentCache extends ConsistentCacheBase implements DrupalCacheDeferredInterface {
    protected $defaultClass = 'DrupalDatabaseCacheDeferred';

    /**
     * Implements DrupalCacheDeferredInterface::getMultipleDeferred().
     */
    public function getMultipleDeferred($cids) {
      if ($this->backend instanceof DrupalCacheDeferredInterface) {
        return $this->backend->getMultipleDeferred($cids);
      }
      else {
        return array();
      }
    }

    /**
     * Implements DrupalCacheDeferredInterface::prepareDeferredItem().
     */
    public function prepareDeferredItem($cid, $item) {
      $result = $this->backend->prepareDeferredItem($cid, $item);
      if (!$this->buffer->isEmpty()) {
        if ($operation = $this->buffer->lookup($cid)) {
          switch ($operation[ConsistentCacheBufferInterface::CC_OPERATION]) {
            case 'set':
              $result = (object) array(
                'cid' => $cid,
                'data' => $operation[ConsistentCacheBufferInterface::CC_DATA_WILDCARD],
                'created' => time(),
                'headers' => NULL,
                'flushes' => 0,
                'expire' => $operation[ConsistentCacheBufferInterface::CC_EXPIRE],
              );
              break;

            case 'clear':
              $result = NULL;
              break;
          }
        }
      }

      if ($result && $result->expire > 0 && $result->expire < time()) {
        $result = NULL;
      }

      return $result;
    }

    /**
     * Implements DrupalCacheDeferredInterface::fetchDeferredItems().
     */
    public function fetchDeferredItems($cids, $result) {
      if ($this->backend instanceof DrupalCacheDeferredInterface) {
        return $this->backend->fetchDeferredItems($cids, $result);
      }
      else {
        return array();
      }
    }

  }
}
else {
  /**
   * Cache Consistent class without Cache Heuristic support.
   */
  class ConsistentCache extends ConsistentCacheBase {}
}
