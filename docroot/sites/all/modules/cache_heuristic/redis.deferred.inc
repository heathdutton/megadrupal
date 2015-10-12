<?php

/**
 * Cache backend for Redis module supporting deferred requests.
 */
class Redis_Cache_Deferred extends Redis_Cache implements DrupalCacheDeferredInterface {
  /**
   * @var DrupalCacheInterface
   */
  protected $deferredBackend;

  /**
   * Constructor.
   *
   * Setup client class.
   */
  function __construct($bin) {
    $className = Redis_Client::getClass(Redis_Client::REDIS_IMPL_CACHE);
    $this->backend = new $className($bin);
    $className .= '_Deferred';
    $this->deferredBackend = new $className($this->backend);
  }

  /**
   * Implements DrupalCacheDeferredInterface::getMultipleDeferred().
   */
  public function getMultipleDeferred($cids) {
    if (count($cids) == 1) {
      $cid = reset($cids);
      $result = $this->deferredBackend->get($cid);
      return $result ? array($cid => $result) : FALSE;
    }
    return $this->deferredBackend->getMultiple($cids);
  }

  /**
   * Implements DrupalCacheDeferredInterface::prepareDeferredItem().
   */
  public function prepareDeferredItem($cid, $item) {
    if ($item->serialized) {
      $item->data = unserialize($item->data);
    }
    return $item;
  }

  /**
   * Implements DrupalCacheDeferredInterface::fetchDeferredItems().
   */
  public function fetchDeferredItems($cids, $results) {
    $results = $results ? $results : array();
    foreach ($results as $cid => &$result) {
      if (empty($result)) {
        unset($results[$cid]);
      }
    }
    return $results;
  }

}

/**
 * PhpRedis wrapper class for getting unserialized items from Redis.
 */
class Redis_Cache_PhpRedis_Deferred {
  protected $backend;

  public function __construct($backend) {
    $this->backend = $backend;
  }

  function get($cid) {
    $client = Redis_Client::getClient();
    $key    = $this->backend->getKey($cid);

    $cached = $client->hgetall($key);

    // Recent versions of PhpRedis will return the Redis instance
    // instead of an empty array when the HGETALL target key does
    // not exists. I see what you did there.
    if (empty($cached) || !is_array($cached)) {
      return FALSE;
    }

    $cached = (object)$cached;
    return $cached;
  }

  function getMultiple(&$cids) {

    $client = Redis_Client::getClient();

    $ret = array();
    $keys = array_map(array($this->backend, 'getKey'), $cids);

    $pipe = $client->multi(Redis::PIPELINE);
    foreach ($keys as $key) {
      $pipe->hgetall($key);
    }
    $replies = $pipe->exec();

    foreach ($replies as $reply) {
      if (!empty($reply)) {
        $cached = (object)$reply;

        $ret[$cached->cid] = $cached;
      }
    }

    foreach ($cids as $index => $cid) {
      if (isset($ret[$cid])) {
        unset($cids[$index]);
      }
    }

    return $ret;
  }

}

/**
 * Predis wrapper class for getting unserialized items from Redis.
 */
class Redis_Cache_Predis_Deferred {
  protected $backend;

  public function __construct($backend) {
    $this->backend = $backend;
  }

  function get($cid) {

    $client = Redis_Client::getClient();
    $key    = $this->backend->getKey($cid);

    $cached = $client->hgetall($key);

    if (empty($cached)) {
      return FALSE;
    }

    $cached = (object)$cached;

    return $cached;
  }

  function getMultiple(&$cids) {

    $client = Redis_Client::getClient();
    $ret    = $keys = array();
    $keys   = array_map(array($this->backend, 'getKey'), $cids);

    $replies = $client->pipeline(function($pipe) use ($keys) {
      foreach ($keys as $key) {
        $pipe->hgetall($key);
      }
    });

    foreach ($replies as $reply) {
      if (!empty($reply)) {

        // HGETALL signature seems to differ depending on Predis versions.
        // This was found just after Predis update. Even though I'm not sure
        // this comes from Predis or just because we're misusing it.
        // FIXME: Needs some investigation.
        if (!isset($reply['cid'])) {
          $cache = new stdClass();
          $size = count($reply);
          for ($i = 0; $i < $size; ++$i) {
            $cache->{$reply[$i]} = $reply[++$i];
          }
        } else {
          $cache = (object)$reply;
        }

        $ret[$cache->cid] = $cache;
      }
    }

    foreach ($cids as $index => $cid) {
      if (isset($ret[$cid])) {
        unset($cids[$index]);
      }
    }

    return $ret;
  }

}
