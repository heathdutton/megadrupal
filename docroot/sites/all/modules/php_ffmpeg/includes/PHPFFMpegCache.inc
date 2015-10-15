<?php
/**
 * @file
 * Contains the PHPFFMpegCache class.
 */

/**
 * An DrupalCacheInterface adapter to Doctrine Cache.
 */
class PHPFFMpegCache implements \Doctrine\Common\Cache\Cache {

  /**
   * @var DrupalCacheInterface
   */
  private $backend;

  /**
   * @var string
   */
  private $prefix;

  /**
   * Create a new PHPFFMpegCache instance.
   *
   * @param DrupalCacheInterface $backend
   *   The DrupalCacheInterface to use as backend.
   * @param $prefix
   *   Prefix to append to cached items identifiers.
   */
  public function __construct(DrupalCacheInterface $backend, $prefix) {
    $this->backend = $backend;
    $this->prefix = (string) $prefix;
  }

  protected function expandId($id) {
    return "{$this->prefix}:{$id}";
  }

  /*
   * {@inheritdoc}
   */
  function fetch($id) {
    if ($cache = $this->backend->get($this->expandId($id))) {
      return $cache->data;
    }
    else {
      return FALSE;
    }
  }

  /*
   * {@inheritdoc}
   */
  function contains($id) {
    return (boolean) $this->backend->get($this->expandId($id));
  }

  /*
   * {@inheritdoc}
   */
  function save($id, $data, $lifeTime = 0) {
    $expire = ($lifeTime != 0 ) ? REQUEST_TIME + $lifeTime : CACHE_PERMANENT;
    $this->backend->set($this->expandId($id), $data, $expire);
    return TRUE;
  }

  /*
   * {@inheritdoc}
   */
  function delete($id) {
    $this->backend->clear($this->expandId($id));
    return TRUE;
  }

  /*
   * {@inheritdoc}
   */
  function getStats() {
    return NULL;
  }

} 