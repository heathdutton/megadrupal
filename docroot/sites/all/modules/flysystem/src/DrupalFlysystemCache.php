<?php

/**
 * @file
 * Contains DrupalFlysystemCache.
 */

namespace Drupal\flysystem;

use League\Flysystem\Cached\Storage\AbstractCache;

/**
 * An adapter that allows Flysystem to use Drupal's cache system.
 */
class DrupalFlysystemCache extends AbstractCache {

  /**
   * The cache key.
   *
   * @var string
   */
  protected $key;

  /**
   * Constructs a DrupalFlysystemCache object.
   *
   * @param string $key
   *   The cache key to use.
   */
  public function __construct($key) {
    $this->key = $key;
  }

  /**
   * {@inheritdoc}
   */
  public function load() {
    if ($cache = cache_get($this->key)) {
      $this->cache = $cache->data[0];
      $this->complete = $cache->data[1];
    }
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    $cleaned = $this->cleanContents($this->cache);
    cache_set($this->key, array($cleaned, $this->complete));
  }

}
