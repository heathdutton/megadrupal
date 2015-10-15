<?php
/**
 * @file
 * Buffer classes.
 */

/**
 * Cache buffer interface.
 */
interface ConsistentCacheBufferInterface {
  /**
   * Constants for buffer array indices.
   *
   * Let's use constants instead of strings, for reasons of compile-time
   * optimization.
   */
  const CC_DEPTH = 0;
  const CC_OPERATION = 1;
  const CC_CID = 2;
  const CC_DATA_WILDCARD = 3;
  const CC_EXPIRE = 4;

  /**
   * Constructor.
   *
   * @param DrupalCacheInterface $backend
   *   The cache object that uses the buffer.
   */
  public function __construct(DrupalCacheInterface $backend);

  /**
   * Get the current backend.
   *
   * @return DrupalCacheInterface
   *   The current cache backend.
   */
  public function getBackend();

  /**
   * Get the current buffer.
   *
   * @return array
   *   The current buffer.
   */
  public function getBuffer();

  /**
   * Buffer an operation.
   *
   * @param array $operation
   *   The operation to buffer.
   */
  public function buffer(array $operation);

  /**
   * Find an entry in the buffer that matches the $cid.
   *
   * With or without wildcard.
   *
   * @param string $cid
   *   The cache ID to look up.
   *
   * @return array
   *   Cache operation if found, NULL otherwise.
   */
  public function lookup($cid);

  /**
   * Remove flushed entries from cache fetch.
   *
   * @param array &$entries
   *   Array reference to cache entries.
   *
   * @return array
   *   Cache IDs removed, if any.
   */
  public function removeFlushed(array &$entries);

  /**
   * Is buffer empty.
   *
   * @return bool
   *   TRUE if empty, FALSE if not.
   */
  public function isEmpty();

  /**
   * Commit the buffer at the specified depth.
   *
   * @param int $depth
   *   The transaction depth to commit.
   */
  public function commit($depth);

  /**
   * Rollback the buffer at the specified depth.
   *
   * @param int $depth
   *   The transaction depth to rollback.
   */
  public function rollback($depth);

}


/**
 * Class for handling the cache buffer.
 */
class ConsistentCacheBuffer implements ConsistentCacheBufferInterface {
  /**
   * Index for the operation's group.
   */
  const CC_GROUP = 5;

  /**
   * The cache operations buffer.
   *
   * @var array
   *   Array operations, where each operation is an array containing:
   *     0: Transactional depth (0 - n).
   *     1: The operation to perform (set, clear).
   *     2: The Cache ID.
   *     3: The data if "set", or wildcard boolean if "clear".
   *     4: The expiration of the cache entry if "set".
   *     5: Group of cache operation. Used for merging clears when pruning.
   */
  protected $buffer = array();

  /**
   * The cache bin for this buffer.
   *
   * @var string
   */
  protected $bin;

  /**
   * The current buffer service.
   *
   * @var DrupalCacheInterface
   */
  protected $bufferService;

  /**
   * Lookup table for locating buffer entries.
   *
   * @var array
   */
  public $lookupTable = array();

  /**
   * Lookup table for locating wildcards in buffer.
   *
   * @var array
   */
  public $wildcardTable = array();

  /**
   * Lookup table for locating flushes in buffer.
   *
   * @var array
   */
  public $flushTable = array();

  /**
   * Internal buffer counter.
   *
   * @var integer
   */
  public $idx = 1;

  /**
   * Group of cache operations, Used for merging clears when pruning.
   *
   * @var integer
   */
  public $clearGroup = 1;

  /**
   * Implements ConsistentCacheBufferInterface::__construct().
   */
  public function __construct(DrupalCacheInterface $buffer_service) {
    $this->bufferService = $buffer_service;
  }

  /**
   * Implements ConsistentCacheBufferInterface::getBackend().
   */
  public function getBackend() {
    return $this->bufferService->getBackend();
  }

  /**
   * Get variable.
   *
   * @see variable_get()
   */
  public function getVariable($name, $default = NULL) {
    return $this->bufferService->variableHandler->getVariable($name, $default);
  }

  /**
   * Get bin.
   */
  public function getBin() {
    return $this->bufferService->getBin();
  }

  /**
   * Implements ConsistentCacheBufferInterface::getBuffer().
   */
  public function getBuffer() {
    return $this->buffer;
  }

  /**
   * Implements ConsistentCacheBufferInterface::isEmpty().
   */
  public function isEmpty() {
    return empty($this->buffer);
  }

  /**
   * Implements ConsistentCacheBufferInterface::buffer().
   */
  public function buffer(array $operation) {
    $this->buffer[$this->idx] = &$operation;
    if ($operation[self::CC_OPERATION] == 'clear' && empty($operation[self::CC_CID]) && empty($operation[self::CC_DATA_WILDCARD])) {
      // Flush expired.
      if ($this->getVariable('cache_lifetime', 0) && REQUEST_TIME <= ($this->getVariable('cache_flush_' . $this->getBin(), REQUEST_TIME) + $this->getVariable('cache_lifetime', 0))) {
        // Enforce minimum cache lifetime by preventing cache clear.
        unset($this->buffer[$this->idx]);
      }
      else {
        $this->flushTable[$this->idx] = REQUEST_TIME;
        $this->idx++;
        $this->clearGroup++;
      }
      return;
    }
    elseif ($operation[self::CC_OPERATION] == 'clear' && !empty($operation[self::CC_DATA_WILDCARD])) {
      // Wildcard clear.
      $this->wildcardTable[$this->idx] = $operation[self::CC_CID];
      $this->idx++;
      $this->clearGroup++;
      return;
    }
    elseif ($operation[self::CC_OPERATION] == 'clear' && isset($operation[self::CC_DATA_WILDCARD])) {
      // Singular clear.
      $operation[self::CC_GROUP] = $this->clearGroup;
    }
    else {
      // Other operations.
      $this->clearGroup++;
    }
    $this->lookupTable[$operation[self::CC_CID]][$this->idx] = $this->idx;
    $this->idx++;
  }

  /**
   * Set cache entries.
   */
  public function set($cid, $data, $expire, $depth) {
    $this->buffer(array(
      ConsistentCacheBufferInterface::CC_DEPTH         => $depth,
      ConsistentCacheBufferInterface::CC_OPERATION     => 'set',
      ConsistentCacheBufferInterface::CC_CID           => $cid,
      ConsistentCacheBufferInterface::CC_DATA_WILDCARD => $data,
      ConsistentCacheBufferInterface::CC_EXPIRE        => $expire,
    ));
  }

  /**
   * Flush cache entries (clear expired).
   */
  public function flushAll($depth) {
    $this->buffer(array(
      ConsistentCacheBufferInterface::CC_DEPTH         => $depth,
      ConsistentCacheBufferInterface::CC_OPERATION     => 'clear',
      ConsistentCacheBufferInterface::CC_CID           => NULL,
      ConsistentCacheBufferInterface::CC_DATA_WILDCARD => TRUE,
    ));
  }

  /**
   * Clear all cache entries.
   */
  public function deleteAll($depth) {
    $this->buffer(array(
      ConsistentCacheBufferInterface::CC_DEPTH         => $depth,
      ConsistentCacheBufferInterface::CC_OPERATION     => 'clear',
      ConsistentCacheBufferInterface::CC_CID           => '*',
      ConsistentCacheBufferInterface::CC_DATA_WILDCARD => TRUE,
    ));
  }

  /**
   * Clear cache entry.
   */
  public function delete($cid, $depth) {
    $this->buffer(array(
      ConsistentCacheBufferInterface::CC_DEPTH         => $depth,
      ConsistentCacheBufferInterface::CC_OPERATION     => 'clear',
      ConsistentCacheBufferInterface::CC_CID           => $cid,
      ConsistentCacheBufferInterface::CC_DATA_WILDCARD => FALSE,
    ));
  }

  /**
   * Clear cache entries starting with $prefix.
   */
  public function deletePrefix($prefix, $depth) {
    $this->buffer(array(
      ConsistentCacheBufferInterface::CC_DEPTH         => $depth,
      ConsistentCacheBufferInterface::CC_OPERATION     => 'clear',
      ConsistentCacheBufferInterface::CC_CID           => $prefix,
      ConsistentCacheBufferInterface::CC_DATA_WILDCARD => TRUE,
    ));
  }

  /**
   * Clear multiple cache entries.
   */
  public function deleteMultiple($cids, $depth) {
    foreach ($cids as $cid) {
      $this->delete($cid, $depth);
    }
  }

  /**
   * Implements ConsistentCacheBufferInterface::lookup().
   */
  public function lookup($cid) {
    $idx = !empty($this->lookupTable[$cid]) ? end($this->lookupTable[$cid]) : 0;

    // It's only necessary to check for flush and wildcard if we actually found
    // an entry in the buffer.
    if ($idx && !empty($this->flushTable) && $this->buffer[$idx][self::CC_EXPIRE] <> CACHE_PERMANENT) {
      $flush_timestamp = end($this->flushTable);
      do {
        $flush_idx = key($this->flushTable);
        if ($flush_idx <= $idx) {
          break;
        }
        if ($this->buffer[$idx][self::CC_EXPIRE] < $flush_timestamp) {
          $idx = $flush_idx;
          break;
        }
      } while ($flush_timestamp = prev($this->flushTable));
    }
    if (!empty($this->wildcardTable)) {
      $cid = (string) $cid;
      $prefix = (string) end($this->wildcardTable);
      do {
        $wildcard_idx = key($this->wildcardTable);
        if ($wildcard_idx <= $idx) {
          break;
        }
        if ($prefix === '*' || $prefix === '' || strpos($cid, $prefix) === 0) {
          $idx = $wildcard_idx;
          break;
        }
      } while (($prefix = (string) prev($this->wildcardTable)) != '');
    }

    return $idx ? $this->buffer[$idx] : NULL;
  }

  /**
   * Implements ConsistentCacheBufferInterface::removeFlushed().
   */
  public function removeFlushed(array &$entries) {
    $removed = array();
    if (!empty($this->flushTable)) {
      foreach ($entries as $cid => $entry) {
        $flush_timestamp = end($this->flushTable);
        do {
          if ($entry->expire <> CACHE_PERMANENT && $entry->expire < $flush_timestamp) {
            $removed[] = $cid;
            unset($entries[$cid]);
            break;
          }
        } while ($flush_timestamp = prev($this->flushTable));
      }
    }
    return $removed;
  }

  /**
   * Implements ConsistentCacheBufferInterface::rollback().
   */
  public function rollback($depth) {
    if ($this->isEmpty()) {
      return;
    }

    foreach ($this->buffer as $idx => $operation) {
      if ($operation[self::CC_DEPTH] > $depth) {
        $this->remove($idx, $operation[self::CC_CID]);
      }
    }
  }

  /**
   * Implements ConsistentCacheBufferInterface::commit().
   */
  public function commit($depth) {
    if ($this->isEmpty()) {
      return;
    }

    if ($depth > 0) {
      // Nested commit. Don't flush yet, but move all operations to
      // parent depth (pseudo-commit).
      $this->merge($depth);
    }
    else {
      // Only prune if enabled.
      if ($this->bufferService->getVariableFallback('consistent_cache', $this->getBin(), 'prune', TRUE)) {
        $this->prune();
      }

      $this->flush();
    }
  }

  /**
   * Merge all operations above a certain depth into the current depth.
   *
   * @param int $depth
   *   The depth to merge to.
   */
  public function merge($depth) {
    foreach ($this->buffer as $idx => &$operation) {
      if ($operation[self::CC_DEPTH] > $depth) {
        $operation[self::CC_DEPTH] = $depth;
      }
    }
  }

  /**
   * Remove and entry from the buffer.
   *
   * @param int $idx
   *   The buffer index to remove.
   */
  public function remove($idx, $cid) {
    unset($this->buffer[$idx]);
    unset($this->lookupTable[$cid][$idx]);
    if (empty($this->lookupTable[$cid])) {
      unset($this->lookupTable[$cid]);
    }
    unset($this->wildcardTable[$idx]);
    unset($this->flushTable[$idx]);
  }

  /**
   * Extract the first operation from buffer.
   *
   * @return array
   *   The operation.
   */
  public function shift() {
    // Start by getting and removing the very first entry from the buffer.
    reset($this->buffer);
    list($idx, $operation) = each($this->buffer);
    $this->remove($idx, $operation[self::CC_CID]);

    // Accumulate clear for multiclear purposes.
    if (!empty($operation[self::CC_GROUP])) {
      $operation[self::CC_CID] = array($operation[self::CC_CID]);
      do {
        list($idx, $next) = each($this->buffer);
        if (!empty($next[self::CC_GROUP]) && $next[self::CC_GROUP] == $operation[self::CC_GROUP]) {
          $operation[self::CC_CID][] = $next[self::CC_CID];
          $this->remove($idx, $next[self::CC_CID]);
          continue;
        }
        break;
      } while (TRUE);
      unset($operation[self::CC_GROUP]);
    }

    return $operation;
  }

  /**
   * Remove duplicate/superfluous operations from buffer.
   */
  public function prune() {
    if (!empty($this->flushTable)) {
      foreach ($this->buffer as $idx => $operation) {
        $cid = (string) $operation[self::CC_CID];
        if ($operation[self::CC_OPERATION] !== 'set' || $operation[self::CC_EXPIRE] == CACHE_PERMANENT) {
          continue;
        }
        $flush_timestamp = end($this->flushTable);
        do {
          $flush_idx = key($this->flushTable);
          if ($flush_idx <= $idx) {
            break;
          }
          if ($operation[self::CC_EXPIRE] < $flush_timestamp) {
            $this->remove($idx, $cid);
          }
        } while ($flush_timestamp = prev($this->flushTable));
      }
    }
    if (!empty($this->wildcardTable)) {
      foreach ($this->buffer as $idx => $operation) {
        $cid = (string) $operation[self::CC_CID];
        if ($cid === '*' || $cid === '') {
          continue;
        }
        $prefix = (string) end($this->wildcardTable);
        do {
          $wildcard_idx = key($this->wildcardTable);
          if ($wildcard_idx <= $idx) {
            break;
          }
          if ($prefix === '*' || $prefix === '' || strpos($cid, $prefix) === 0) {
            $this->remove($idx, $cid);
          }
        } while (($prefix = (string) prev($this->wildcardTable)) != '');
      }
    }
    foreach ($this->lookupTable as $cid => $data) {
      // Pop last operation. That's the one we want to keep.
      array_pop($data);

      // Remove the others from the buffer.
      foreach ($data as $remove_idx) {
        $this->remove($remove_idx, $cid);
      }
    }
  }

  /**
   * Perform all operations in buffer and clear it.
   */
  public function flush() {
    // Flush the buffer, FIFO-style.
    while (!$this->isEmpty()) {
      $operation = $this->shift();
      $depth = array_shift($operation);
      $method = array_shift($operation);
      call_user_func_array(array($this->getBackend(), $method), $operation);
    }
  }

}
